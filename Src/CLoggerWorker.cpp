/*
Copyright 2015 Alex Frappier Lachapelle

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "CLoggerWorker.hpp"

CLoggerWorker::CLoggerWorker()
    : logQueue()
    , threadMutex()
    , threadCondVar()
    , isLoggerRunning(false)
    , isThreadRunning(false)
    , isFlushing(false){

}

CLoggerWorker::~CLoggerWorker(){
    flush();
}


void CLoggerWorker::start(bool waitOnFlush){
    //NOTE: Possible ABA issue here?
    bool expected = false;
    if(isLoggerRunning.compare_exchange_strong(expected, true)){
        startThread(waitOnFlush);
    }
}

void CLoggerWorker::startThread(bool waitOnFlush){

    //If we're flushing, wait before starting up again.
    if(waitOnFlush){
        std::unique_lock<std::mutex> lock(startupMutex);
        startupCondVar.wait(lock, [this](){ return !isFlushing.load();});
    }

    //NOTE: Possible ABA issue here?
    bool expected = false;
    if(isThreadRunning.compare_exchange_strong(expected, true)){
        workerThread = std::thread(run, std::ref(logQueue), std::ref(sinkMap), std::ref(isThreadRunning), std::ref(threadCondVar), std::ref(threadMutex), std::ref(sinkMapMutex));
    }

}

void CLoggerWorker::stop(bool skipOnFlush){

    //If flushing, skip this call
    if(isFlushing.load() && skipOnFlush)
        return;

    //NOTE: Possible ABA issue here?
    bool expected = true;
    if(isLoggerRunning.compare_exchange_strong(expected, false)){
        stopThread(skipOnFlush);
    }
}

void CLoggerWorker::stopThread(bool skipOnFlush){

    //If flushing, skip this call
    if(isFlushing.load() && skipOnFlush)
        return;

    //NOTE: Possible ABA issue here?
    bool expected = true;
    if(isThreadRunning.compare_exchange_strong(expected, false)){
        threadCondVar.notify_one();
        workerThread.join();
    }
}


void CLoggerWorker::flush(){

    //NOTE: Possible ABA issue here?
    bool expected = false;
    if(!isFlushing.compare_exchange_strong(expected, true)){

        CLoggerLogStruct msg = {};
        stop(false);

        while(logQueue.try_dequeue(msg)){
            if(sinkMap.count(msg.channelID) > 0){
                //NOTE: is locking the map really necessary here?
                sinkMapMutex.lock();
                std::shared_ptr<CLoggerSinkBase> sink = sinkMap.at(msg.channelID);
                sinkMapMutex.unlock();
                sink.get()->writeMessage(msg);
            }
        }

        isFlushing.store(false);
    }
}


bool CLoggerWorker::addSink(uint32 sinkID, std::shared_ptr<CLoggerSinkBase> sink){

    stopThread();
    sinkMapMutex.lock();

    if(sink->onInit()){
        sinkMap.insert(std::pair<uint32, std::shared_ptr<CLoggerSinkBase>>(sinkID, sink));
        sinkMapMutex.unlock();
        startThread();
        return true;
    }else{
        sinkMapMutex.unlock();
        startThread();
        return false;
    }
}

bool CLoggerWorker::removeSink(uint32 channelID){

    stopThread();
    sinkMapMutex.lock();

    std::map<uint32, std::shared_ptr<CLoggerSinkBase>>::iterator it;
    it = sinkMap.find(channelID);
    if(it == sinkMap.end()){
        sinkMapMutex.unlock();
        startThread();
        return false;
    }else{
        it->second->onExit();
        sinkMap.erase(it);
        sinkMapMutex.unlock();
        startThread();
        return true;
    }
}


bool CLoggerWorker::addMessageToQueue(CLoggerLogStruct message){
    if(isLoggerRunning.load() && !isFlushing.load()){
        logQueue.enqueue(message);
        threadCondVar.notify_one();
        return true;
    }
    return false;
}


void CLoggerWorker::run(ConcurrentQueue<CLoggerLogStruct>                  &logQueue,
                        std::map<uint32, std::shared_ptr<CLoggerSinkBase>> &sinkMap,
                        std::atomic<bool>                                  &isThreadRunning,
                        std::condition_variable                            &threadCondVar,
                        std::mutex                                         &threadMutex,
                        std::mutex                                         &sinkMapMutex){

    while(isThreadRunning.load()){

        CLoggerLogStruct             msg = {};
        bool                         isDequeueSuccess;
        std::unique_lock<std::mutex> lock(threadMutex);

        threadCondVar.wait(lock, [&logQueue, &msg, &isDequeueSuccess, &isThreadRunning](){
            isDequeueSuccess = logQueue.try_dequeue(msg);
            return !isThreadRunning.load() || isDequeueSuccess;
        });

        //If we reach here and that isDequeueSuccess == false
        //Then notify_one was called while the queue was empty
        //If so, don't write an empty message.
        if(isDequeueSuccess){
            if(sinkMap.count(msg.channelID) > 0){
                sinkMapMutex.lock();
                std::shared_ptr<CLoggerSinkBase> sink = sinkMap.at(msg.channelID);
                sinkMapMutex.unlock();
                sink.get()->writeMessage(msg);
            }
        }
    }
}

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
    :  logQueue(std::make_shared<ConcurrentQueue<CLoggerLogStruct>>()),
       threadMutex(std::make_shared<std::mutex>()),
       conditionVar(std::make_shared<std::condition_variable>()),
       isRunning(std::make_shared<std::atomic<bool>>(false)),
       isThreadRunning(std::make_shared<std::atomic<bool>>(false)),
       isFlushing(std::make_shared<std::atomic<bool>>(false)){

}

CLoggerWorker::~CLoggerWorker(){
    flush();
}


void CLoggerWorker::start(){
    if(!isRunning.get()->load()){
        isRunning.get()->store(true);
        startThread();
    }
}

void CLoggerWorker::stop(){
    if(isRunning.get()->load()){
        isRunning.get()->store(false);
        stopThread();
    }
}

void CLoggerWorker::startThread(){
    if(!isThreadRunning.get()->load()){
        isThreadRunning.get()->store(true);
        workerThread = std::thread(run, logQueue, sinkMap, isThreadRunning, conditionVar, threadMutex, sinkMapMutex);
    }
}

void CLoggerWorker::stopThread(){
    if(isThreadRunning.get()->load()){
        isThreadRunning.get()->store(false);
        conditionVar.get()->notify_one();
        workerThread.join();
    }
}


void CLoggerWorker::flush(){

    if(!isFlushing.get()->load()){
        isFlushing.get()->store(true);

        CLoggerLogStruct msg = {};
        stop();

        while(logQueue.get()->try_dequeue(msg)){
            if(sinkMap.get()->count(msg.channelID) > 0){
                std::lock_guard<std::mutex> sinkMapGuard (*sinkMapMutex);
                std::shared_ptr<CLoggerSinkBase> sink = sinkMap.get()->at(msg.channelID);
                sink.get()->writeMessage(msg);
            }
        }

        isFlushing.get()->store(false);
    }
}


void CLoggerWorker::addSink(uint32 sinkID, std::shared_ptr<CLoggerSinkBase> sink){

    std::lock_guard<std::mutex> sinkMapGuard (*sinkMapMutex);

    stopThread();
    sinkMap.get()->insert(std::pair<uint32, std::shared_ptr<CLoggerSinkBase>>(sinkID, sink));
    startThread();
}

bool CLoggerWorker::removeSink(uint32 channelID){

    std::lock_guard<std::mutex> sinkMapGuard (*sinkMapMutex);

    stopThread();
    if(sinkMap.get()->erase(channelID) == 0){
        startThread();
        return false;
    }else{
        startThread();
        return true;
    }
}


bool CLoggerWorker::addMessageToQueue(CLoggerLogStruct message){
    if(isRunning.get()->load()){
        logQueue.get()->enqueue(message);
        conditionVar.get()->notify_one();
        return true;
    }
    return false;
}


void CLoggerWorker::run(std::shared_ptr<ConcurrentQueue<CLoggerLogStruct>> logQueue,
                        std::shared_ptr<std::map<uint32, std::shared_ptr<CLoggerSinkBase>>> sinkMap,
                        std::shared_ptr<std::atomic<bool>> isThreadRunning,
                        std::shared_ptr<std::condition_variable> conditionVar,
                        std::shared_ptr<std::mutex> threadMutex,
                        std::shared_ptr<std::mutex> sinkMapMutex){

    while(isThreadRunning.get()->load()){

        CLoggerLogStruct             msg = {};
        bool                         isDequeueSuccess;
        std::unique_lock<std::mutex> lock(*threadMutex);

        conditionVar.get()->wait(lock, [&logQueue, &msg, &isDequeueSuccess, &isThreadRunning](){
            isDequeueSuccess = logQueue.get()->try_dequeue(msg);
            return !isThreadRunning.get()->load() || isDequeueSuccess;
        });

        //If we reach here and that isDequeueSuccess == false
        //Then notify_one was called while the queue was empty
        //If so, don't write an empty message.
        if(isDequeueSuccess){
            if(sinkMap.get()->count(msg.channelID) > 0){
                std::lock_guard<std::mutex> sinkMapGuard(*sinkMapMutex);
                std::shared_ptr<CLoggerSinkBase> sink = sinkMap.get()->at(msg.channelID);
                sink.get()->writeMessage(msg);
            }
        }
    }
}

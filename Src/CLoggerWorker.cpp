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
       mutex(std::make_shared<std::mutex>()),
       conditionVar(std::make_shared<std::condition_variable>()),
       isRunning(std::make_shared<std::atomic<bool>>()),
       isSuspended(std::make_shared<std::atomic<bool>>()),
       isFlushing(std::make_shared<std::atomic<bool>>()){

}

CLoggerWorker::~CLoggerWorker(){
    //Check if OK
    INCOMPLETE_FUNC(__FILE__, __LINE__)
    stop();
    flush();
}


void CLoggerWorker::start(){
    INCOMPLETE_FUNC(__FILE__, __LINE__)
    isRunning.get()->store(true);
    isSuspended.get()->store(false);
    //workerThread = std::thread(run, backend, isSuspended, logQueue, conditionVar, mutex);
}

void CLoggerWorker::stop(){
    INCOMPLETE_FUNC(__FILE__, __LINE__)
    //Stop receiving messages
    isRunning.get()->store(false);
    //Stop the thread
    pause();

}

void CLoggerWorker::pause(){
    INCOMPLETE_FUNC(__FILE__, __LINE__)
    isSuspended.get()->store(true);
    conditionVar.get()->notify_one();
    workerThread.join();
}

void CLoggerWorker::resume(){
    STUB_FUNC(__FILE__, __LINE__)
}



void CLoggerWorker::flush(){

    //CLoggerLogStruct msg = {};

    ////Stop consuming.
    //stop();

    ////Write what's left in the queue;
    //while(messageQueue.get()->try_dequeue(msg)){
    //    backend.get()->writeMessage(msg);
    //    msg = {};
    //}

    //backend.get()->onExit();

    INCOMPLETE_FUNC(__FILE__, __LINE__)

}


void CLoggerWorker::addSink(std::shared_ptr<CLoggerSinkBase> sink, uint32 sinkID){
    INCOMPLETE_FUNC(__FILE__, __LINE__)
    pause();
    sinkMap.get()->insert(std::pair<uint32, std::shared_ptr<CLoggerSinkBase>>(sinkID, sink));
    start();
}

bool CLoggerWorker::removeSink(uint32 channelID){
    INCOMPLETE_FUNC(__FILE__, __LINE__)
    pause();
    if(sinkMap.get()->erase(channelID) == 0){
        start();
        return false;
    }else{
        start();
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
                        std::shared_ptr<std::atomic<bool>> isSuspended,
                        std::shared_ptr<std::condition_variable> conditionVar,
                        std::shared_ptr<std::mutex> mutex){

    while(!isSuspended.get()->load()){

        CLoggerLogStruct msg = {};
        std::unique_lock<std::mutex> lock(*mutex);

        bool isDequeueSuccess;
        conditionVar.get()->wait(lock, [&logQueue, &msg, &isDequeueSuccess, &isSuspended](){
            isDequeueSuccess = logQueue.get()->try_dequeue(msg);
            return isSuspended.get()->load() || isDequeueSuccess;
        });

        //If we reach here and that isDequeueSuccess == false
        //Then notify_one was called while the queue was empty
        //If so, don't write an empty message.
        if(isDequeueSuccess){
            std::shared_ptr<CLoggerSinkBase> sink = sinkMap.get()->at(msg.channelID);
            sink.get()->writeMessage(msg);
        }
    }
}
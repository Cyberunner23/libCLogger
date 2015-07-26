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

CLoggerWorker::CLoggerWorker(std::shared_ptr<CLoggerSinkBase> backend)
        : backend(backend),
          messageQueue(std::make_shared<ConcurrentQueue<CLoggerMessageStruct>>()),
          mutex(std::make_shared<std::mutex>()),
          conditionVar(std::make_shared<std::condition_variable>()),
          isConsuming(std::make_shared<bool>()){

    this->backend.get()->onInit();
    start();

}

CLoggerWorker::~CLoggerWorker(){
    flush();
}


void CLoggerWorker::start(){
    *isConsuming   = true;
    consumerThread = std::thread(run, backend, isConsuming, messageQueue, conditionVar, mutex);
}

void CLoggerWorker::stop(){
    *isConsuming = false;
    conditionVar.get()->notify_one();
    consumerThread.join();
}

void CLoggerWorker::flush(){

    CLoggerMessageStruct msg = {};

    //Stop consuming.
    stop();

    //Write what's left in the queue;
    while(messageQueue.get()->try_dequeue(msg)){
        backend.get()->writeMessage(msg);
        msg = {};
    }

    backend.get()->onExit();

}

void CLoggerWorker::setBackend(std::shared_ptr<CLoggerSinkBase> backend, bool flushQueue){

    if(flushQueue)
        flush();
    else
        stop();
    this->backend.get()->onExit();
    this->backend = backend;
    this->backend.get()->onInit();
    start();

}

std::shared_ptr<CLoggerSinkBase> CLoggerWorker::getBackend(){
    return backend;
}


bool CLoggerWorker::addMessageToQueue(CLoggerMessageStruct message){
    if(*isConsuming){
        messageQueue.get()->enqueue(message);
        conditionVar.get()->notify_one();
        return true;
    }
    return false;
}


void CLoggerWorker::run(std::shared_ptr<CLoggerSinkBase> backend, std::shared_ptr<bool> isConsuming, std::shared_ptr<ConcurrentQueue<CLoggerMessageStruct>> messageQueue, std::shared_ptr<std::condition_variable> conditionVar, std::shared_ptr<std::mutex> mutex){

    while(*isConsuming){

        CLoggerMessageStruct msg = {};
        std::unique_lock<std::mutex> lock(*mutex);

        bool isDequeueSuccess;
        conditionVar.get()->wait(lock, [&messageQueue, &msg, &isDequeueSuccess, &isConsuming](){
            isDequeueSuccess = messageQueue.get()->try_dequeue(msg);
            return !*isConsuming || isDequeueSuccess;
        });

        //If we reach here and that isDequeueSuccess == false
        //Then notify_one was called while the queue was empty
        //If so, don't write an empty message.
        if(isDequeueSuccess){
            backend.get()->writeMessage(msg);
        }
    }
}
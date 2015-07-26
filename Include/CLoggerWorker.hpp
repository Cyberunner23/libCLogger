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

#ifndef LIBCLOGGER_CLOGGERCORE_H
#define LIBCLOGGER_CLOGGERCORE_H

#include <mutex>
#include <condition_variable>

#include "ConcurrentQueue.h"

#include "CloggerSinkBase.hpp"
#include "CLoggerDefaultBackEnd.hpp"
#include "CLoggerMessage.hpp"

//TODO?: use something else than a shared_ptr for the global instance?
//TODO?: change start/stop/flush behavior?

using namespace moodycamel;

class CLoggerWorker{

public:

    //Vars

    //Funcs
    CLoggerWorker(std::shared_ptr<CloggerSinkBase> backend);
    ~CLoggerWorker();

    void                                start();
    void                                stop();
    void                                flush();
    void                                setBackend(std::shared_ptr<CloggerSinkBase> backend, bool flushQueue);
    std::shared_ptr<CloggerSinkBase> getBackend();

    bool addMessageToQueue(CLoggerMessageStruct message);


private:

    //Vars
    std::shared_ptr<ConcurrentQueue<CLoggerMessageStruct>> messageQueue;
    std::shared_ptr<CloggerSinkBase>                    backend;

    std::shared_ptr<std::mutex>              mutex;
    std::shared_ptr<std::condition_variable> conditionVar;
    std::thread                              consumerThread;
    std::shared_ptr<bool>                    isConsuming;


    //Funcs
    static void run(std::shared_ptr<CloggerSinkBase> backend, std::shared_ptr<bool> isConsuming, std::shared_ptr<ConcurrentQueue<CLoggerMessageStruct>> messageQueue, std::shared_ptr<std::condition_variable> conditionVar, std::shared_ptr<std::mutex> mutex);


};

extern std::shared_ptr<CLoggerWorker> cloggerCoreInstance;


#endif //LIBCLOGGER_CLOGGERCORE_H

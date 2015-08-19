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

#include <condition_variable>

#include <map>
#include <mutex>

#include "ConcurrentQueue.h"

#include "CLoggerSinkBase.hpp"
#include "CLoggerLog.hpp"
#include "DevMacros.hpp"

using namespace moodycamel;

class CLoggerWorker{

public:

    //Vars

    //Funcs
    CLoggerWorker();
    ~CLoggerWorker();

    void start(bool       waitOnFlush = true);
    void startThread(bool waitOnFlush = true);
    void stop(bool        skipOnFlush = true);
    void stopThread(bool  skipOnFlush = true);
    void flush();

    bool addSink(uint32 sinkID, std::shared_ptr<CLoggerSinkBase> sink);
    bool removeSink(uint32 channelID);

    bool addMessageToQueue(CLoggerLogStruct message);


private:

    //Vars

    ConcurrentQueue<CLoggerLogStruct>                  logQueue;
    std::map<uint32, std::shared_ptr<CLoggerSinkBase>> sinkMap;
    std::mutex                                         sinkMapMutex;

    std::thread             workerThread;
    std::condition_variable threadCondVar;
    std::mutex              threadMutex;

    std::condition_variable startupCondVar;
    std::mutex              startupMutex;

    std::atomic<bool> isLoggerRunning;
    std::atomic<bool> isThreadRunning;
    std::atomic<bool> isFlushing;

    //Funcs
    static void run(ConcurrentQueue<CLoggerLogStruct>                  &logQueue,
                    std::map<uint32, std::shared_ptr<CLoggerSinkBase>> &sinkMap,
                    std::atomic<bool>                                  &isThreadRunning,
                    std::condition_variable                            &threadCondVar,
                    std::mutex                                         &threadMutex,
                    std::mutex                                         &sinkMapMutex);


};

#endif //LIBCLOGGER_CLOGGERCORE_H

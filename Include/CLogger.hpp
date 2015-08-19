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

#ifndef LIBCLOGGER_CLOGGER_H
#define LIBCLOGGER_CLOGGER_H

#include <thread>

#include "ConcurrentQueue.h"

#include "DevMacros.hpp"
#include "CLoggerSinkBase.hpp"
#include "CLoggerWorker.hpp"
#include "CLoggerLog.hpp"

//TODO!!:  DOCUMENTATION

//TODO: create a stream and printf like front end.
//TODO: Implement cross platform crash handler
//TODO: Add multiple sink support with channels
//          to send the log to a certain sink
//          ?? or a group of sinks
//TODO: Change the Worker according to the other changes made.

using namespace moodycamel;

class CLogger{

public:

    //Vars

    //Funcs
    ~CLogger();

    /*  //?(ChannelID -> 0 = all channels,)? 1 = default channel, x = customChannel
     *  CLogger::getInstance().addSink(customSinkClass, sinkID);
     *  ...
     *  CLogger::getInstance().init(bool initWithDefaultSink = true);
     *
     * */


    static CLogger* getInstance();

    //TODO: MAKE THESE THREAD SAFE (Gonna have to be locking...)
    void addSink(std::shared_ptr<CLoggerSinkBase> sink, uint32 sinkID);
    bool removeSink(uint32 channelID, bool flush = true);


    //TODO: MAKE THESE THREAD SAFE
    void init();
    //stops the thread and stops accepting logs.
    void stop(bool flush = true);
    //stops the thread but the queue keeps filling up.
    void pause(bool flush = false);
    void resume();


private:

    //Vars
    static std::unique_ptr<CLogger> instance;

    CLoggerWorker workerThread;

    //Funcs
    CLogger();
    CLogger(CLogger const&){};
    CLogger& operator=(CLogger const&){};


};

#endif //LIBCLOGGER_CLOGGER_H

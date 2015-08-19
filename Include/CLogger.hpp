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
#include "CLoggerCapture.hpp"
#include "CLoggerSinkBase.hpp"
#include "CLoggerWorker.hpp"
#include "CLoggerLog.hpp"

//TODO!!:  DOCUMENTATION

//TODO: Implement cross platform crash handler
//TODO: Add multiple sink support with channels
//          [DONE] to send the log to a certain sink
//          [ ?? ] or a group of sinks

#define DEFAULT_CHANNEL 1

//Usage: These functions use a stringstream
//       LOG_X() << "log message" << some variable << "some more text";
//       LOGC_X(channelID) << "log message" << some variable << "some more text";

#define LOG_DEBUG()           CLoggerCapture(DEFAULT_CHANNEL, __LINE__, __FILE__, std::time(nullptr), "DEBUG",   false, std::this_thread::get_id()).stream()
#define LOGC_DEBUG(channel)   CLoggerCapture(channel,         __LINE__, __FILE__, std::time(nullptr), "DEBUG",   false, std::this_thread::get_id()).stream()
#define LOG_INFO()            CLoggerCapture(DEFAULT_CHANNEL, __LINE__, __FILE__, std::time(nullptr), "INFO",    false, std::this_thread::get_id()).stream()
#define LOGC_INFO(channel)    CLoggerCapture(channel,         __LINE__, __FILE__, std::time(nullptr), "INFO",    false, std::this_thread::get_id()).stream()
#define LOG_WARNING()         CLoggerCapture(DEFAULT_CHANNEL, __LINE__, __FILE__, std::time(nullptr), "WARNING", false, std::this_thread::get_id()).stream()
#define LOGC_WARNING(channel) CLoggerCapture(channel,         __LINE__, __FILE__, std::time(nullptr), "WARNING", false, std::this_thread::get_id()).stream()
#define LOG_FATAL()           CLoggerCapture(DEFAULT_CHANNEL, __LINE__, __FILE__, std::time(nullptr), "FATAL",   true,  std::this_thread::get_id()).stream()
#define LOGC_FATAL(channel)   CLoggerCapture(channel,         __LINE__, __FILE__, std::time(nullptr), "FATAL",   true,  std::this_thread::get_id()).stream()

//Usage: These functions are much like printf
//       LOGF_X("printf like text", variable1, variable2, ...)
//       LOGFC_X(channelID, "printf like text", variable1, variable2, ...)

#define LOGF_DEBUG(msg, ...)             CLoggerCapture(1,       __LINE__, __FILE__, std::time(nullptr), "DEBUG",   false, std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGFC_DEBUG(channel, msg, ...)   CLoggerCapture(channel, __LINE__, __FILE__, std::time(nullptr), "DEBUG",   false, std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGF_INFO(msg, ...)              CLoggerCapture(1,       __LINE__, __FILE__, std::time(nullptr), "INFO",    false, std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGFC_INFO(channel, msg, ...)    CLoggerCapture(channel, __LINE__, __FILE__, std::time(nullptr), "INFO",    false, std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGF_WARNING(msg, ...)           CLoggerCapture(0,       __LINE__, __FILE__, std::time(nullptr), "WARNING", false, std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGFC_WARNING(channel, msg, ...) CLoggerCapture(channel, __LINE__, __FILE__, std::time(nullptr), "WARNING", false, std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGF_FATAL(msg, ...)             CLoggerCapture(channel, __LINE__, __FILE__, std::time(nullptr), "FATAL",   true,  std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)
#define LOGFC_FATAL(channel, msg, ...)   CLoggerCapture(0,       __LINE__, __FILE__, std::time(nullptr), "FATAL",   true,  std::this_thread::get_id()).capturef(msg, ##__VA_ARGS__)

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


    static CLogger& getInstance();

    bool addSink(uint32 sinkID, std::shared_ptr<CLoggerSinkBase> sink);
    bool removeSink(uint32 channelID, bool flush = true);

    void init(bool waitOnFlush = true);
    void resume(bool waitOnFlush = true);
    //stops the thread and stops accepting logs.
    void stop(bool skipOnFlush = false);
    //stops the thread but the queue keeps filling up.
    void pause(bool skipOnFlush = false);

    void logMsg(CLoggerLogStruct message);


private:

    //Vars

    CLoggerWorker workerThread;

    //Funcs
    CLogger();
    CLogger(CLogger const&)            = delete;
    CLogger& operator=(CLogger const&) = delete;


};

#endif //LIBCLOGGER_CLOGGER_H

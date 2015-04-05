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

#ifndef LIBCLOGGER_CLOGGERDEFAULTFRONTEND_HPP
#define LIBCLOGGER_CLOGGERDEFAULTFRONTEND_HPP

#include <sstream>
#include "CLoggerCore.hpp"

#define INFO    "INFO"
#define WARNING "WARNING"
#define FATAL   "FATAL"
#define DEBUG   "DEBUG"

#define LOG(LEVEL, MSG) \
    CLoggerMessageStruct msg; \
    msg.fileName       = __FILE__; \
    msg.fileLineNumber = __LINE__; \
    msg.logMessage     = MSG; \
    msg.logLevel       = LEVEL; \
    msg.isLogFatal     = (msg.logLevel == "FATAL"); \
    msg.timeAtLog      = time(0); \
    std::stringstream  ss; \
    ss << std::this_thread::get_id(); \
    msg.threadName     = ss.str(); \
    cloggerCoreInstance.get()->addMessageToQueue(msg);



#endif //LIBCLOGGER_CLOGGERDEFAULTFRONTEND_HPP

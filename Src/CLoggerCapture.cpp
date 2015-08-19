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

#include "CLoggerCapture.hpp"

CLoggerCapture::CLoggerCapture(uint32 channelID, uint64 lineNumber, std::string fileName, std::time_t timeAtLog, std::string logLevelString, bool isLogFatal, std::thread::id threadID){
    log.channelID      = channelID;
    log.lineNumber     = lineNumber;
    log.fileName       = fileName;
    log.timeAtLog      = timeAtLog;
    log.logLevelString = logLevelString;
    log.isLogFatal     = isLogFatal;
    log.threadID       = threadID;
}

CLoggerCapture::~CLoggerCapture(){
    log.logMessage = osstream.str();
    CLogger::getInstance().logMsg(log);
}

std::ostringstream& CLoggerCapture::stream(){
    return osstream;
}

void CLoggerCapture::capturef(const char *printfLikeMsg, ...){

    static const int         maxMsgSize = 4096;
    static const std::string truncMsg   = " [truncated]";
    char                     finalizedMsg[maxMsgSize];
    va_list                  arglist;

    va_start(arglist, printfLikeMsg);

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__GNUC__))
    const int numChar = vsnprintf_s(finished_message, _countof(finished_message), _TRUNCATE, printf_like_message, arglist);
#else
    const int numChar = vsnprintf(finalizedMsg, sizeof (finalizedMsg), printfLikeMsg, arglist);
#endif

    va_end(arglist);

    if(numChar <= 0){
        stream() << "\n\tLOGGER ERROR: Failed to successfully parse the message:";
        stream() << "\"" << printfLikeMsg << "\"";
    }else if(numChar > maxMsgSize){
        stream() << finalizedMsg << truncMsg;
    }else{
        stream() << finalizedMsg;
    }
}





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

#include <fstream>
#include <sstream>
#include <string.h>
#include <thread>

#include "DevMacros.hpp"
#include "MPSCWorker.hpp"
#include "Typedefs.hpp"

//TODO!!:  DOCUMENTATION

//----------------------------------------------------------
//######################### Macros #########################
//----------------------------------------------------------

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif


//----------------------------------------------------------
//####################### Data Types #######################
//----------------------------------------------------------

//NOTE: Feel free to modify the CLoggerLogStruct but
// remember to edit your sinks as necessary!
struct CLoggerLogStruct{
    std::string     logMessage;
    uint64          lineNumber;
    std::string     fileName;
    std::time_t     timeAtLog;
    unsigned int    threadID;
    struct CLoggerLogLevel{
        std::string logLevelString;
        bool        isLogFatal;
    }logLevel;
};


//----------------------------------------------------------
//####################### Main Class #######################
//----------------------------------------------------------

template<const unsigned int NumOfSinks, class LogType = CLoggerLogStruct>
using CLoggerSingleton = MPSCWorkerSingleton<LogType, NumOfSinks>;



//----------------------------------------------------------
//##################### Helper Classes #####################
//----------------------------------------------------------

//This class assumes DefaultCLoggerLogStruct is used.
//example CLoggerSingleton<>
template<const unsigned int NumOfSinks, class LogType = CLoggerLogStruct>
class CLoggerCapture{

public:

    //Funcs
    CLoggerCapture(uint32 sinkID, uint64 lineNumber, std::string fileName, std::time_t timeAtLog, std::string logLevelString, bool isLogFatal){
        this->sinkID                = sinkID;
        log.lineNumber              = lineNumber;
        log.fileName                = fileName;
        log.timeAtLog               = timeAtLog;
        log.threadID                = Utils::getThreadID();
        log.logLevel.logLevelString = logLevelString;
        log.logLevel.isLogFatal     = isLogFatal;
    }

    ~CLoggerCapture(){
        log.logMessage = osstream.str();
        CLoggerSingleton<NumOfSinks, LogType>::getInstance()->send(log, sinkID);
    }

    std::ostringstream& stream(){return osstream;}

/*
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

*/


private:

    //Vars
    LogType            log;
    unsigned int       sinkID;
    std::ostringstream osstream;

};


template <class Type = CLoggerLogStruct>
class CLoggerSink : public SinkBase<Type>{
public:
    virtual bool onInit(){

        std::string tmpFilePath;

        //tmpFilePath =  "Logs/";
        tmpFilePath = getDate();

        for(int counter = 1; true; counter++){

            std::string logFilePathCheck;

            logFilePathCheck =  tmpFilePath;
            logFilePathCheck += "_";
            logFilePathCheck += std::to_string(counter);
            logFilePathCheck += ".log";

            bool doesFileExistCheck = doesFileExist(logFilePathCheck);
            bool isFileEmptyCheck   = isFileEmpty(logFilePathCheck);

            if((doesFileExistCheck && isFileEmptyCheck) || !doesFileExistCheck){
                logFilePath = logFilePathCheck;
                break;
            }
        }

        outStream.open(logFilePath, std::ios_base::out | std::ios_base::app);

        if(outStream.is_open()){

            outStream << "CLogger with default sink initialized on " << getDate() << " at " << getTime(time(0)) << std::endl << std::endl;
            outStream << "[Time Written][Time Logged][Thread ID][File Name][File Line Number][Level]: Message" << std::endl;

            return true;
        }else{
            return false;
        }

    };
    virtual void onProcess(Type data){
        outStream << "[" << getTime(time(0))                << "]";
        outStream << "[" << getTime(data.timeAtLog)         << "]";
        outStream << "[" << data.threadID                   << "]";
        outStream << "[" << data.fileName                   << "]";
        outStream << "[" << data.lineNumber                 << "]";
        outStream << "["  << data.logLevel.logLevelString   << "]";
        outStream << ": " << data.logMessage                << std::endl;
        if(data.logLevel.isLogFatal) std::exit(EXIT_FAILURE);
    };
private:

    //Vars
    std::ofstream outStream;
    std::string   logFilePath;

    //Funcs
    std::string getDate(){

        time_t      currentTime = time(0);
        tm          *date       = localtime(&currentTime);
        std::string dateString  = "";

        dateString =  std::to_string(date->tm_mday);
        dateString += "-";
        dateString += std::to_string(date->tm_mon + 1);
        dateString += "-";
        dateString += std::to_string(date->tm_year - 100);

        return dateString;
    }

    std::string getTime(time_t time){

        tm          *date       = localtime(&time);
        std::string timeString  = "";

        timeString =  std::to_string(date->tm_hour);
        timeString += ":";
        timeString += std::to_string(date->tm_min);
        timeString += ":";
        timeString += std::to_string(date->tm_sec);

        return timeString;
    }

    bool doesFileExist(std::string logFile){

        std::ifstream inStream(logFile);

        if(!inStream.is_open()){
            return false;
        }

        inStream.close();

        return true;
    }

    bool isFileEmpty(std::string logFile){

        std::ifstream inStream(logFile);
        uint64 fileLength;

        inStream.seekg(0, std::ios_base::end);
        fileLength = (uint64)inStream.tellg();

        if(fileLength != 0){
            return false;
        }

        inStream.close();

        return true;
    }


};

#endif //LIBCLOGGER_CLOGGER_H
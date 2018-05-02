/*
Copyright 2018 Alex Frappier Lachapelle

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

#pragma once

#include "CLoggerImpl.hpp"
#include "LogCapture.hpp"
#include "LogMessage.hpp"
#include "Singleton.hpp"


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define LOG_SINK(sinkid, lvl) LogCapture(sinkid, lvl, __LINE__, __FILE__, time(nullptr)).stream()

#define LOG(level) LOG_SINK(0, level)









/*

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

/*private:

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

#endif //LIBCLOGGER_CLOGGER_H*/

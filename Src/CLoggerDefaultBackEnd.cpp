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

#include "CLoggerDefaultBackEnd.hpp"

CLoggerDefaultBackEnd::CLoggerDefaultBackEnd(){

    std::string tmpFilePath;

    tmpFilePath =  "Logs/";
    tmpFilePath += getDate();

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
}

CLoggerDefaultBackEnd::~CLoggerDefaultBackEnd(){

}


bool CLoggerDefaultBackEnd::onInit(){

    outStream.open(logFilePath, std::ios_base::out | std::ios_base::app);

    if(outStream.is_open()){

        outStream << "CLogger with default backend initialized on " << getDate() << " at " << getTime(time(0)) << std::endl << std::endl;
#ifdef ENABLE_DEBUG_INFO
        outStream << "[Time Written][Time Logged][Thread Name][File Name][File Line Number][Level]: Message" << std::endl;
#else
        outStream << "[Time Written][Time Logged][Thread Name][Level]: Message" << std::endl;
#endif
        return true;
    }else{
        return false;
    }
}

void CLoggerDefaultBackEnd::onExit(){
    outStream << "CLogger shutting down." << std::endl;
    outStream.close();
}


void CLoggerDefaultBackEnd::writeMessageToBackend(CLoggerMessageStruct message){
    outStream << "[" << getTime(time(0))           << "]";
    outStream << "[" << getTime(message.timeAtLog) << "]";
    outStream << "[" << message.threadName         << "]";
#ifdef ENABLE_DEBUG_INFO
    outStream << "[" << message.fileName << "]";
    outStream << "[" << message.fileLineNumber << "]";
#endif
    outStream << "[" << message.logLevel           << "]";
    outStream << ": " << message.logMessage << std::endl;

}


std::string CLoggerDefaultBackEnd::getDate(){

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

std::string CLoggerDefaultBackEnd::getTime(time_t time){

    tm          *date       = localtime(&time);
    std::string timeString  = "";

    timeString =  std::to_string(date->tm_hour);
    timeString += ":";
    timeString += std::to_string(date->tm_min);
    timeString += ":";
    timeString += std::to_string(date->tm_sec);

    return timeString;
}


bool CLoggerDefaultBackEnd::doesFileExist(std::string logFile){

    std::ifstream inStream(logFile);

    if(!inStream.is_open()){
        return false;
    }

    inStream.close();

    return true;
}

bool CLoggerDefaultBackEnd::isFileEmpty(std::string logFile){

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
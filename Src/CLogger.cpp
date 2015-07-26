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

#include "CLogger.hpp"

std::unique_ptr<CLogger> CLogger::instance = nullptr;

CLogger* CLogger::getInstance(){

    if(instance.get() == nullptr){
       instance = std::unique_ptr<CLogger>(new CLogger());
    }

    return instance.get();
}


bool CLogger::addSink(std::shared_ptr<CLoggerSinkBase> sink, uint32 channelID){
    STUB_FUNC(__LINE__, __FILE__)
}

bool CLogger::removeSink(uint32 channelID){
    STUB_FUNC(__LINE__, __FILE__)
}

void CLogger::addLogLevel(uint32 logLevelID, std::string logLevelString, bool isFatal){
    STUB_FUNC(__LINE__, __FILE__)
}

void CLogger::addLogLevel(uint32 logLevelID, CLoggerLogLevel logLevel){
    STUB_FUNC(__LINE__, __FILE__)
}

bool CLogger::removeLogLevel(uint32 logLevelID){
    STUB_FUNC(__LINE__, __FILE__)
}

bool CLogger::removeLogLevel(CLoggerLogLevel logLevel){
    STUB_FUNC(__LINE__, __FILE__)
}


void CLogger::start(){
    //cloggerCoreInstance.get()->start();
    STUB_FUNC(__LINE__, __FILE__)
}

void CLogger::stop(bool flush){
    //if(flush)
        //cloggerCoreInstance.get()->flush();
    //else
        //cloggerCoreInstance.get()->stop();
    STUB_FUNC(__LINE__, __FILE__)
}



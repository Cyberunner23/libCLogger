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


CLogger::CLogger(){
    //TODO: initialize crash handler.
}

CLogger::~CLogger(){

}


CLogger* CLogger::getInstance(){

    if(instance.get() == nullptr){
       instance = std::unique_ptr<CLogger>(new CLogger());
    }

    return instance.get();
}


void CLogger::addSink(uint32 sinkID, std::shared_ptr<CLoggerSinkBase> sink){
    workerThread.addSink(sinkID, sink);
}

bool CLogger::removeSink(uint32 channelID, bool flush){
    workerThread.removeSink(channelID);
}


void CLogger::init(bool waitOnFlush){
    workerThread.start(waitOnFlush);
}

void CLogger::resume(bool waitOnFlush){
    workerThread.startThread(waitOnFlush);
}

void CLogger::stop(bool skipOnFlush){
    workerThread.stop(skipOnFlush);
}

void CLogger::pause(bool skipOnFlush){
    workerThread.stopThread(skipOnFlush);
}



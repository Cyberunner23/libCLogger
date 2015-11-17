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

static void run(CLogger<1> &clogger){
    for(int i = 0;  i != 5; i++){
        CLogger<1>::type msg;
        msg.logMessage              =  "Thread " + std::to_string(Utils::getThreadID())+ ": Test message #" + std::to_string(i) + ";";
        msg.lineNumber              = __LINE__;
        msg.fileName                = __FILENAME__;
        msg.timeAtLog               = std::time(0);
        msg.threadID                = Utils::getThreadID();
        msg.logLevel.logLevelString = "INFO";
        msg.logLevel.isLogFatal     = false;
        clogger.send(msg, 0);
    }
}

int main(){

    CLogger<1> clogger;

    std::unique_ptr<CLoggerSink<DefaultCloggerLogStruct>> sink(new CLoggerSink<DefaultCloggerLogStruct>);
    if(!clogger.addSink(std::move(sink), 0)){return 11;}
    clogger.start();

    std::thread threads[4];
    for(int i = 0; i < 4; i++){
        threads[i] = std::thread(run, std::ref(clogger));
    }

    for(int i = 0; i < 4; i++){
        threads[i].join();
    }

    return 0;
}

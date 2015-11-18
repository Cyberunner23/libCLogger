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

static void run(int val){
    for(int i = 0;  i != 5; i++){
        CLoggerCapture<1>(0, __LINE__, __FILENAME__, std::time(0), "BLAHG", false).stream() << "Thread " + std::to_string(Utils::getThreadID())+ ": Test message #" + std::to_string(i) + ";";
    }
}

int main(){


    std::unique_ptr<CLoggerSink<>> sink(new CLoggerSink<>);
    if(!CLoggerSingleton<1>::getInstance()->addSink(std::move(sink), 0)){return 11;}
    CLoggerSingleton<1>::getInstance()->start();

    std::thread threads[4];
    for(int i = 0; i < 4; i++){
        threads[i] = std::thread(run, 0);
    }

    for(int i = 0; i < 4; i++){
        threads[i].join();
    }

    return 0;
}

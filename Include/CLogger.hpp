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

#include "CloggerBackendBase.hpp"
#include "CLoggerWorker.hpp"
#include "CLoggerDefaultFrontEnd.hpp"
#include "CLoggerMessage.hpp"

//TODO:  DOCUMENTATION

//TODO: Make main class a singleton
//TODO: Backend -> sink
//TODO: Put default sink into the base class.
//TODO: create a stream and printf like front end.
//TODO: clean up the message struct
//TODO: Implement cross platform crash handler
//TODO: Add multiple sink support with channels
//          to send the log to a certain sink
//          or a group of sinks

using namespace moodycamel;

class CLogger{

public:

    //Vars

    //Funcs

    CLogger(std::shared_ptr<CloggerBackendBase> backend = std::make_shared<CLoggerDefaultBackEnd>());
    ~CLogger();

    void start();
    void stop(bool flush = false);

    void setBackEnd(std::shared_ptr<CloggerBackendBase> backend, bool flushQueue);


private:

    //Vars

    //Funcs

};

std::shared_ptr<CLoggerWorker> cloggerCoreInstance = nullptr;

#endif //LIBCLOGGER_CLOGGER_H

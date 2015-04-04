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

#ifndef LIBCLOGGER_CLOGGERBACKENDBASE_H
#define LIBCLOGGER_CLOGGERBACKENDBASE_H

#include "CLoggerMessage.hpp"

class CloggerBackendBase{

public:

    //Vars

    //Funcs

    CloggerBackendBase();
    ~CloggerBackendBase();

    virtual bool onInit() = 0;
    virtual void onExit() = 0;

    virtual void writeMessageToBackend(CLoggerMessageStruct message) = 0;


private:

    //Vars

    //Funcs

};

#endif //LIBCLOGGER_CLOGGERBACKENDBASE_H

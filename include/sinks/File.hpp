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

#include <ctime>
#include <string>
#include <fstream>

#include "../Sink.hpp"


class File : public Sink
{

public:

    explicit File(std::string fileName)
    {
        _fileStream.open(fileName);
    }

    ~File()
    {
        _fileStream.close();
    }

    void append(LogMessage& message) final
    {
        std::strftime(_timefmt, sizeof(_timefmt), "%T", std::localtime(&message.logTime));
        std::string fileName = message.fileName.substr(message.fileName.find_last_of("\\/") + 1, message.fileName.size());
        _fileStream << "[" << _timefmt << "]";
        _fileStream << "[" << fileName << ":" << message.lineNumber << "]";
        _fileStream << "[" << logLevelAsString(message.level) << "]: ";
        _fileStream << message.message;
    }

private:

    char _timefmt[100];
    std::ofstream _fileStream;

};


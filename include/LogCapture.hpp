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

#include <cassert>
#include <sstream>

#include "CLoggerImpl.hpp"
#include "LogMessage.hpp"

class LogCapture
{

public:

    explicit LogCapture(LogMessage&& message) noexcept : _message(std::move(message)) {}
    ~LogCapture() noexcept(false)
    {
        _stream << std::endl;
        _message.message = _stream.str();
        CLogger::getInstance()->log(std::move(_message));
    }

    std::ostringstream& stream() noexcept
    {
        return _stream;
    }

private:

    LogMessage _message;
    std::ostringstream _stream;

};

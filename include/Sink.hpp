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

#include "LogMessage.hpp"

/// Base class for the sinks, they take log messages and sends them to their destination.
/// example: A file, stdout, over the network, etc.

class Sink
{
public:
    /// Appends the log message to the stream associated to the child object.
    /// \param message Message to be appended.
    virtual void append(LogMessage& message) = 0;
};

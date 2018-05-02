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

 ///This enum represents the various log levels that the logger can use.
 ///This can be edited to suit users needs.

 enum LogLevel
 {
     TRACE,
     DEBUG,
     INFO,
     WARNING,
     ERROR,
     FATAL
 };

 /// Checks if the log level is a fatal level, used to handle cases
 /// where fatal levels are changed, for examlpe, a CRASH level can
 /// can be considered fatal.
 /// Should be edited along side LogLevel
 ///
 /// \param Level Level to be checked
 /// \return Whether it is fatal or not
 static bool isLevelFatal(LogLevel level)
 {
     return level == FATAL;
 }

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

#include <cstdlib>
#include <iostream>


#ifndef NDEBUG
#define assert_msg(condition, message) \
[](bool cond) \
{ \
    if (!(cond)) \
    { \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << ":" << __LINE__ << ": " << (message) << std::endl; \
        std::exit(EXIT_FAILURE); \
    } \
}((condition)) \

#else
#define assert_msg(condition, message) [](){}()
#endif

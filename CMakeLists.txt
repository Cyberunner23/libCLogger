#Copyright 2015 Alex Frappier Lachapelle
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

cmake_minimum_required(VERSION 3.2)
project(libCLogger CXX)

#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
SET(CMAKE_CXX_STANDARD 11)

if(UNIX OR CYGWIN OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
endif()


#----------------------------------------------------------
#################### SOURCE / HEADERS #####################
#----------------------------------------------------------
set(CLOGGER_SRC_FILES
    Tests/main.cpp
)

set(CLOGGER_HDR_FILES
    Include/CLogger.hpp
    Include/CLoggerVer.hpp
    Include/Utils/DevMacros.hpp
    Include/Utils/Typedefs.hpp

)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include/Utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/MPSCWorker)


#----------------------------------------------------------
######################## EXECUTABLE #######################
#----------------------------------------------------------
add_executable(CLogger ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
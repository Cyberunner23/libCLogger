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

cmake_minimum_required(VERSION 2.8.7)
project(libCLogger CXX)


#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------
option(LIBCLOGGER_BUILD_SHARED_LIBRARIES "Build libCLogger as a shared Lib (setting this to OFF will compile statically)" ON)


#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(UNIX OR CYGWIN OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
endif()

#----------------------------------------------------------
#################### SOURCE / HEADERS #####################
#----------------------------------------------------------
set(CLOGGER_SRC_FILES
    Src/CLogger.cpp
    Src/CLoggerSinkBase.cpp
    Src/CLoggerWorker.cpp
)

set(CLOGGER_HDR_FILES
    Include/CLogger.hpp
    Include/CLoggerSinkBase.hpp
    Include/CLoggerWorker.hpp
    Include/CLoggerLog.hpp
    Include/CLoggerVer.hpp
    Include/Utils/ConcurrentQueue.h
    Include/Utils/Typedefs.hpp

)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include/Utils)


#----------------------------------------------------------
######################### LIBRARY #########################
#----------------------------------------------------------

IF(LIBCLOGGER_BUILD_SHARED_LIBRARIES)
    add_library(libCLogger SHARED ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
ELSE(LIBCLOGGER_BUILD_SHARED_LIBRARIES)
    add_library(libCLogger STATIC ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
ENDIF(LIBCLOGGER_BUILD_SHARED_LIBRARIES)

set_target_properties(libCLogger PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    OUTPUT_NAME "CLogger"
)



#TEMPORARY TEST STUFF

#ENABLE_TESTING()
ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/Tests")




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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)
PROJECT(libCLogger CXX)


#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------
OPTION(LIBCLOGGER_BUILD_SHARED_LIBRARIES "Build libCLogger as a shared lib (setting this to OFF will compile statically)" ON)


#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#----------------------------------------------------------
#################### SOURCE / HEADERS #####################
#----------------------------------------------------------
SET(CLOGGER_SRC_FILES
    src/CLogger.cpp
    src/CloggerBackendBase.cpp
    src/CLoggerCore.cpp
    src/CLoggerDefaultBackEnd.cpp
)

SET(CLOGGER_HDR_FILES
    include/CLogger.hpp
    include/CloggerBackendBase.hpp
    include/CLoggerCore.hpp
    include/CLoggerDefaultBackEnd.hpp
    include/CLoggerDefaultFrontEnd.hpp
    include/CLoggerMessage.hpp
    include/CLoggerVer.hpp
    include/Tools/ConcurrentQueue.h
    include/Tools/Typedefs.hpp
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/Tools)


#----------------------------------------------------------
######################### LIBRARY #########################
#----------------------------------------------------------

IF(LIBCLOGGER_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libCLogger SHARED ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
ELSE(LIBCLOGGER_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libCLogger STATIC ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
ENDIF(LIBCLOGGER_BUILD_SHARED_LIBRARIES)

SET_TARGET_PROPERTIES(libCLogger PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    OUTPUT_NAME "CLogger"
)




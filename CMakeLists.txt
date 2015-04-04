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
OPTION(LIBCLOGGER_BUILD_SHARED_LIBRARIES "Build libCLogger as a shared Lib (setting this to OFF will compile statically)" ON)


#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#----------------------------------------------------------
#################### SOURCE / HEADERS #####################
#----------------------------------------------------------
SET(CLOGGER_SRC_FILES
    Src/CLogger.cpp
    Src/CloggerBackendBase.cpp
    Src/CLoggerCore.cpp
    Src/CLoggerDefaultBackEnd.cpp
)

SET(CLOGGER_HDR_FILES
    Include/CLogger.hpp
    Include/CloggerBackendBase.hpp
    Include/CLoggerCore.hpp
    Include/CLoggerDefaultBackEnd.hpp
    Include/CLoggerDefaultFrontEnd.hpp
    Include/CLoggerMessage.hpp
    Include/CLoggerVer.hpp
    Include/Tools/ConcurrentQueue.h
    Include/Tools/Typedefs.hpp

)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include/Tools)


#----------------------------------------------------------
######################### LIBRARY #########################
#----------------------------------------------------------

IF(LIBCLOGGER_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libCLogger SHARED ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
ELSE(LIBCLOGGER_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libCLogger STATIC ${CLOGGER_SRC_FILES} ${CLOGGER_HDR_FILES})
ENDIF(LIBCLOGGER_BUILD_SHARED_LIBRARIES)

SET_TARGET_PROPERTIES(libCLogger PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    OUTPUT_NAME "CLogger"
)




cmake_minimum_required(VERSION 3.0)

#------------------------------------------------------------------------------
# Project
#------------------------------------------------------------------------------

project(libCLogger VERSION 0.3.0)


#------------------------------------------------------------------------------
# Config
#------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)


#------------------------------------------------------------------------------
# Deps
#------------------------------------------------------------------------------

find_package(Threads REQUIRED)


#------------------------------------------------------------------------------
# Library
#------------------------------------------------------------------------------

add_library(libCLogger INTERFACE)
target_include_directories(libCLogger INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(libCLogger INTERFACE
    ${CMAKE_THREAD_LIBS_INIT}
)


#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------

set(TEST_SRC
    test/test.cpp
    include/Assert.hpp
    include/CLogger.hpp
    include/CLoggerImpl.hpp
    include/LogCapture.hpp
    include/LogLevel.hpp
    include/LogMessage.hpp
    include/Singleton.hpp
    include/Sink.hpp
    include/3rdParty/blockingconcurrentqueue.h
    include/3rdParty/concurrentqueue.h
)
add_executable(Test ${TEST_SRC})
target_link_libraries(Test ${CMAKE_THREAD_LIBS_INIT})

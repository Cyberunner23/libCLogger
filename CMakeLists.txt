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
# Library
#------------------------------------------------------------------------------

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(eventstore CXX C)

file(GLOB C_SOURCES "src/*.c")
file(GLOB CXX_SOURCES "src/*.cpp")

add_library(eventstore SHARED ${CXX_SOURCES} ${C_SOURCES})

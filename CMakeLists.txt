cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(eventstore CXX C)

INCLUDE_DIRECTORIES("src")

file(GLOB C_SOURCES "src/*.c")
file(GLOB CXX_SOURCES "src/*.cpp")
file(GLOB CXX_SOURCES "src/Allocators/*.cpp")
file(GLOB CXX_SOURCES "src/Segments/*.cpp")

add_library(eventstore SHARED ${CXX_SOURCES} ${C_SOURCES})

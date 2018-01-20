cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(eventstore CXX C)
project(program CXX C)

INCLUDE_DIRECTORIES("src")

#file(GLOB C_SOURCES "src/*.c")
file(GLOB CXX_SOURCES "src/Indexes/*.cpp" "src/Segments/*.cpp" "src/Allocators/*.cpp" "src/*.cpp" "src/Crypto/*.cxx")

add_library(eventstore SHARED ${CXX_SOURCES} ${C_SOURCES})

add_executable(program ${CXX_SOURCES} ${C_SOURCES})
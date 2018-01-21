cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

project(eventstore CXX C)
project(program CXX C)

INCLUDE_DIRECTORIES("src")

#file(GLOB C_SOURCES "src/*.c")
file(GLOB CXX_SOURCES "src/Indexes/*.cpp" "src/Segments/*.cpp" "src/Allocators/*.cpp" "src/*.cpp" "src/Crypto/*.cxx")

add_library(eventstore SHARED ${CXX_SOURCES} ${C_SOURCES})

add_executable(program ${CXX_SOURCES} ${C_SOURCES})

#sudo ln -s /usr/bin/llvm-symbolizer-3.8 /usr/bin/llvm-symbolizer
#make . && make -j16 && time ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer ./program
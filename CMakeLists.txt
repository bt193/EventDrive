cmake_minimum_required(VERSION 2.8)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(es C)

file(GLOB SOURCES "*.c")

add_library(es SHARED ${SOURCES})

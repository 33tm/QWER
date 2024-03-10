cmake_minimum_required(VERSION 3.5)

project(QWER)

find_package(CURL REQUIRED)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_executable(QWER ${SOURCES})

add_subdirectory(lib/simdjson)

target_link_libraries(QWER CURL::libcurl simdjson)
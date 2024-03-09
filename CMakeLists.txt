cmake_minimum_required(VERSION 3.5)

project(QWER)

find_package(CURL REQUIRED)

add_executable(QWER src/main.cpp)

add_subdirectory(lib/simdjson)

target_link_libraries(QWER CURL::libcurl simdjson)
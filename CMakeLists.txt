cmake_minimum_required(VERSION 3.5)

project(QWER)

set(CMAKE_CXX_STANDARD 23)

# hate this!! :D
if(WIN32)
    set(ZLIB_LIBRARY "-lz")
    set(ZLIB_INCLUDE_DIR C:/zlib/include)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_definitions(-DCURL_STATICLIB)
endif(WIN32)

find_package(CURL REQUIRED)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_executable(QWER ${SOURCES})

add_subdirectory(lib/simdjson)

target_link_libraries(QWER CURL::libcurl simdjson)
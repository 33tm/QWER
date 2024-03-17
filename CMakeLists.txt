cmake_minimum_required(VERSION 3.5)

project(QWER)

set(CMAKE_CXX_STANDARD 23)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_executable(QWER ${SOURCES})

find_package(CURL REQUIRED)

include(FetchContent)

FetchContent_Declare(glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)

target_link_libraries(QWER CURL::libcurl glaze::glaze)
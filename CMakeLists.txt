cmake_minimum_required(VERSION 3.26)
project(quicksort)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(parlaylib
        GIT_REPOSITORY  https://github.com/cmuparlay/parlaylib.git
        GIT_TAG         master
)
FetchContent_GetProperties(parlaylib)
if(NOT parlaylib_POPULATED)
    FetchContent_Populate(parlaylib)
    add_subdirectory(${parlaylib_SOURCE_DIR} EXCLUDE_FROM_ALL)
endif()

set(PARLAY_NUM_THREADS 8)

add_executable(quicksort main.cpp)

target_link_libraries(quicksort PRIVATE parlay)
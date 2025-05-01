cmake_minimum_required(VERSION 3.26)


if(MSVC)
    if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
        message(STATUS "Building for x64 architecture.")
    else()
        message(FATAL_ERROR "This project requires x64 architecture.")
    endif()
endif()

project(Mirage VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Mirage)
add_subdirectory(Sample)
add_subdirectory(Test)
add_subdirectory(SampleExemple)
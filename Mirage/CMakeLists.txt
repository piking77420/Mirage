cmake_minimum_required(VERSION 3.26)

project(Mirage VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCE_FILES src/source/*.c src/source/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/include/*.h src/include/*.hpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES}) 

target_include_directories(${PROJECT_NAME} PUBLIC src/include)

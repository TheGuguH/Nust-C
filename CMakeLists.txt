# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# C project version
set(CMAKE_C_STANDARD 11)

# GCC (i don't know how is in clang, sorry ;-;) options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Project name
project(nustc)

# Add include directorys (for headers files)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Search files in recursive way
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

# Add files to executable
add_executable(${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${SRC_FILES})

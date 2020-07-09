cmake_minimum_required(VERSION 3.16)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES "${PROJECT_NAME}/*.cpp" "${PROJECT_NAME}/*.h" "${PROJECT_NAME}/OpenGL.c")

set(dir ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_NAME})
include_directories(Dependencies)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
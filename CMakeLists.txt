cmake_minimum_required(VERSION 3.16)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE HEADER_FILES "${PROJECT_NAME}/*.h")
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_NAME}/*.cpp" "${PROJECT_NAME}/*.c")

include_directories(${PROJECT_NAME})
include_directories(Dependencies)

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
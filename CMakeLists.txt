cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ParaCompiler)

add_subdirectory(protobuf)
add_subdirectory(frontend)


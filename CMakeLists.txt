cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ParaCompiler)

option(SANITIZE "Enable compiler sanitizers" OFF)

add_subdirectory(protobuf)
add_subdirectory(frontend)
add_subdirectory(simulator)
add_subdirectory(graphviz)


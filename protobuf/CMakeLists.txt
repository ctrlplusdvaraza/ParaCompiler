cmake_minimum_required(VERSION 3.20)
project(serializer LANGUAGES CXX)

add_library(ast_serialization
  ${CMAKE_CURRENT_SOURCE_DIR}/serialization.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ast.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/nodes.proto
)

find_package(Protobuf REQUIRED)

if(DEFINED Protobuf_PROTOC_EXECUTABLE AND NOT EXISTS "${Protobuf_PROTOC_EXECUTABLE}")
  message(FATAL_ERROR "Protobuf_PROTOC_EXECUTABLE is set but does not exist: ${Protobuf_PROTOC_EXECUTABLE}")
endif()

set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(MAKE_DIRECTORY "${PROTO_GEN_DIR}")

target_include_directories(ast_serialization
  PUBLIC
    ${CMAKE_SOURCE_DIR}/ast
    ${PROTO_GEN_DIR}
)

target_link_libraries(ast_serialization
  PUBLIC
    protobuf::libprotobuf
)

protobuf_generate(
  TARGET ast_serialization
  LANGUAGE cpp
  PROTOC_OUT_DIR "${PROTO_GEN_DIR}"
  IMPORT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  PROTOS
    "${CMAKE_CURRENT_SOURCE_DIR}/ast.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/nodes.proto"
)


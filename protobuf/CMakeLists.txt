project(serializer LANGUAGES CXX)

find_package(Protobuf REQUIRED)  
find_package(absl CONFIG REQUIRED)

add_library(ast_serialization
    ${CMAKE_CURRENT_SOURCE_DIR}/ast.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/serialization.cpp
)

include_directories(ast_serialization
${CMAKE_SOURCE_DIR}/include
${CMAKE_CURRENT_BINARY_DIR}/.
)

protobuf_generate(
    TARGET ast_serialization
)

target_link_libraries(ast_serialization PUBLIC protobuf::libprotobuf)


project(serializer LANGUAGES CXX)

find_package(Protobuf REQUIRED)  
find_package(absl CONFIG REQUIRED)

add_library(ast_serialization
    ${CMAKE_CURRENT_SOURCE_DIR}/ast.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/tokens.proto
)

protobuf_generate(
    TARGET ast_serialization
)

target_link_libraries(ast_serialization PUBLIC protobuf::libprotobuf absl::check absl::log)
target_include_directories(ast_serialization PUBLIC ${CMAKE_CURRENT_BINARY_DIR})


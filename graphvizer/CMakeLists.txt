project(graphvizer LANGUAGES CXX)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
add_executable(graphvizer 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/dot_common.cpp
    ${SOURCE_DIR}/dot_io.cpp
)
    
include_directories(
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_SOURCE_DIR}/ast
		    ${CMAKE_SOURCE_DIR}/protobuf
		    ${CMAKE_BINARY_DIR}/protobuf
            ${LEXER_CODEGEN_DIR}
            ${PARSER_CODEGEN_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ast_serialization)

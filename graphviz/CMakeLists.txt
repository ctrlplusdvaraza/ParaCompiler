project(graphviz LANGUAGES CXX)

set(SRC_FILE 
    ${CMAKE_CURRENT_SOURCE_DIR}/graphviz.cpp
)

include_directories(
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_SOURCE_DIR}/ast
		    ${CMAKE_SOURCE_DIR}/protobuf
		    ${CMAKE_BINARY_DIR}/protobuf
            ${LEXER_CODEGEN_DIR}
            ${PARSER_CODEGEN_DIR}
)

add_library(${PROJECT_NAME}_lib ${SRC_FILE})
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}_lib PRIVATE ast_serialization)
target_link_libraries(${PROJECT_NAME} PRIVATE ast_serialization ${PROJECT_NAME}_lib)

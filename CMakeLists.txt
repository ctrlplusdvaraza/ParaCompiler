cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LEXER_CODEGEN_DIR "${CMAKE_BINARY_DIR}/codegen/lexer")
set(PARSER_CODEGEN_DIR "${CMAKE_BINARY_DIR}/codegen/parser")

project(paracl)

option(SANITIZE "Enable compiler sanitizers" OFF)

include_directories(
		    ${CMAKE_SOURCE_DIR}/ast
		    ${CMAKE_SOURCE_DIR}/frontend
		    ${CMAKE_SOURCE_DIR}/simulator
            ${CMAKE_SOURCE_DIR}/graphviz
            ${CMAKE_SOURCE_DIR}/protobuf
		    ${CMAKE_BINARY_DIR}/protobuf
            ${LEXER_CODEGEN_DIR}
            ${PARSER_CODEGEN_DIR}

)

add_compile_options(
    $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
)
add_link_options(
    $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
)

add_subdirectory(protobuf)
add_subdirectory(frontend)
add_subdirectory(simulator)
add_subdirectory(graphviz)

add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE ast_serialization frontend_lib simulator_lib graphviz_lib)

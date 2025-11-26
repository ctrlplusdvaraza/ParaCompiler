project(frontend LANGUAGES CXX)

make_directory(${LEXER_CODEGEN_DIR})
make_directory(${PARSER_CODEGEN_DIR})

set(FLEX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lexer.flex")
set(BISON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/parser.yy")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)


include_directories(
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_SOURCE_DIR}/ast
		    ${CMAKE_SOURCE_DIR}/protobuf
		    ${CMAKE_BINARY_DIR}/protobuf
            ${LEXER_CODEGEN_DIR}
            ${PARSER_CODEGEN_DIR}
)

flex_target(LEXER ${FLEX_SRC} ${LEXER_CODEGEN_DIR}/lexer.cpp
            DEFINES_FILE ${LEXER_CODEGEN_DIR}/lexer.hpp)

bison_target(PARSER ${BISON_SRC} ${PARSER_CODEGEN_DIR}/parser.cpp
             DEFINES_FILE ${PARSER_CODEGEN_DIR}/parser.hpp
             COMPILE_FLAGS -Wcounterexamples)

add_flex_bison_dependency(LEXER PARSER)

set(SRC_FILES
    ${FLEX_LEXER_OUTPUTS}
    ${BISON_PARSER_OUTPUTS}
    driver.cpp
)

add_library(${PROJECT_NAME}_lib ${SRC_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} main.cpp) 

target_link_libraries(${PROJECT_NAME} PRIVATE ast_serialization)


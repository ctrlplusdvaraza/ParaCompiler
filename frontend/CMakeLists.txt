project(frontend LANGUAGES CXX)

set(LEXER_CODEGEN_DIR "${CMAKE_BINARY_DIR}/codegen/lexer")
set(PARSER_CODEGEN_DIR "${CMAKE_BINARY_DIR}/codegen/parser")

make_directory(${LEXER_CODEGEN_DIR})
make_directory(${PARSER_CODEGEN_DIR})

set(FLEX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lexer/lexer.flex")
set(BISON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.yy")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

option(SANITIZE "Enable compiler sanitizers" OFF)

add_compile_options(
    -Wall
    -Wextra
    -Werror
    $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
)
add_link_options(
    $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
)

include_directories(
		    ${CMAKE_SOURCE_DIR}/include
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_BINARY_DIR}/protobuf
                    ${LEXER_CODEGEN_DIR}
                    ${PARSER_CODEGEN_DIR})

FLEX_TARGET(LEXER ${FLEX_SRC} ${LEXER_CODEGEN_DIR}/lexer.cpp
            DEFINES_FILE ${LEXER_CODEGEN_DIR}/scanner.hpp)

BISON_TARGET(PARSER ${BISON_SRC} ${PARSER_CODEGEN_DIR}/parser.cpp
             DEFINES_FILE ${PARSER_CODEGEN_DIR}/parser.hpp)

ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

set(SRC_FILES
    ${FLEX_LEXER_OUTPUTS}
    ${BISON_PARSER_OUTPUTS}
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC ast_serialization)

# target_link_libraries(compiler fl)

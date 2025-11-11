cmake_minimum_required(VERSION 3.20)
project(Compiler LANGUAGES C)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

option(SANITIZE "Enable compiler sanitizers" OFF)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
         $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
    )
    add_link_options(
         $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
    )
endif()
    

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Flex and Bison

# Lexer
FLEX_TARGET(MyLexer ${CMAKE_SOURCE_DIR}/lexer.l ${CMAKE_BINARY_DIR}/lexer.c)
# Parser
BISON_TARGET(MyParser ${CMAKE_SOURCE_DIR}/parser.y ${CMAKE_BINARY_DIR}/parser.c
             DEFINES_FILE ${CMAKE_BINARY_DIR}/parser.h)

# Ensure lexer depends on parser (if needed)
ADD_FLEX_BISON_DEPENDENCY(MyLexer MyParser)

# Source files
set(SRC_FILES
    ${FLEX_MyLexer_OUTPUTS}
    ${BISON_MyParser_OUTPUTS}
    ${CMAKE_SOURCE_DIR}/src/ast_node.c
)

# Build executable
add_executable(compiler ${SRC_FILES})

# Link Flex library
target_link_libraries(compiler fl)

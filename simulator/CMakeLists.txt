project(simulator LANGUAGES CXX)

include_directories(
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_SOURCE_DIR}/ast
		    ${CMAKE_SOURCE_DIR}/protobuf
		    ${CMAKE_BINARY_DIR}/protobuf
)

set(SRC_FILES
    simulator.cpp
)

add_library(${PROJECT_NAME}_lib ${SRC_FILES})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}_lib PRIVATE ast_serialization)
target_link_libraries(${PROJECT_NAME} PRIVATE ast_serialization ${PROJECT_NAME}_lib)

project(graphvizer LANGUAGES CXX)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
add_executable(graphvizer 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/dot_common.cpp
    ${SOURCE_DIR}/dot_io.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../protobuf
)

target_link_libraries(${PROJECT_NAME} PUBLIC ast_serialization)
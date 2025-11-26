project(simulator LANGUAGES CXX)

include_directories(
		    ${CMAKE_SOURCE_DIR}/include
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_BINARY_DIR}/protobuf
                )

set(SRC_FILES
    simulator.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC ast_serialization)

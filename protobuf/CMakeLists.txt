project(serializer LANGUAGES CXX)

find_package(Protobuf REQUIRED)

find_package(absl QUIET)

add_library(ast_serialization
    ${CMAKE_CURRENT_SOURCE_DIR}/ast.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/nodes.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/serialization.cpp
)

include_directories(ast_serialization
    ${CMAKE_SOURCE_DIR}/ast
    ${CMAKE_CURRENT_BINARY_DIR}/.
)

protobuf_generate(
    TARGET ast_serialization
)

target_link_libraries(ast_serialization PUBLIC protobuf::libprotobuf)

if(absl_FOUND)
    set(ABSL_TARGETS
        absl::log_internal_check_op
        absl::log_internal_message
        absl::status
        absl::statusor
        absl::strings
        absl::cord
        absl::absl_check
        absl::absl_log
    )
    
    foreach(ABSL_TARGET ${ABSL_TARGETS})
        if(TARGET ${ABSL_TARGET})
            target_link_libraries(ast_serialization PUBLIC ${ABSL_TARGET})
        endif()
    endforeach()
elseif(TARGET absl::abseil)
    # Older abseil versions might use a single target
    target_link_libraries(ast_serialization PUBLIC absl::abseil)
endif()

if(NOT absl_FOUND AND NOT TARGET absl::abseil)
    find_library(ABSL_BASE_LIB absl_base)
    if(ABSL_BASE_LIB)
        find_path(ABSL_INCLUDE_DIR absl/base/config.h)
        if(ABSL_INCLUDE_DIR)
            target_include_directories(ast_serialization PUBLIC ${ABSL_INCLUDE_DIR})
        endif()
    endif()
endif()


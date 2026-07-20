cmake_minimum_required(VERSION 3.28)
project(SorcererShowdown LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/source")
set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/header")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SOURCE_DIR}/*.cpp"
)

add_executable(SorcererShowdown ${SOURCES})

target_include_directories(SorcererShowdown PRIVATE
    "${HEADER_DIR}"
)

target_link_libraries(SorcererShowdown PRIVATE
    nlohmann_json::nlohmann_json
)

target_precompile_headers(SorcererShowdown PRIVATE
    "${HEADER_DIR}/std.h"
)

if(MSVC)
    target_compile_options(SorcererShowdown PRIVATE
        /EHsc /utf-8 /W4 /permissive-
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(SorcererShowdown PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -Wno-unused-parameter
    )
endif()

set(DATA_FILES
    characters.json
    domains.json
    cursedtools.json
)

foreach(data_file IN LISTS DATA_FILES)
    set(data_path "${CMAKE_CURRENT_SOURCE_DIR}/${data_file}")

    if(EXISTS "${data_path}")
        add_custom_command(
            TARGET SorcererShowdown
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${data_path}"
                "$<TARGET_FILE_DIR:SorcererShowdown>/${data_file}"
            VERBATIM
        )
    endif()
endforeach()

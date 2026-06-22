cmake_minimum_required(VERSION 3.28)
project(SorcererShowdown LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TLS_VERIFY ON)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/json.hpp")
    file(DOWNLOAD
        https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp
        STATUS download_status
    )
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "! Failed to download json.hpp")
    endif()
endif()


file(GLOB_RECURSE SOURCES "code/source/*.cpp" "code/source/**/*.cpp")
add_executable(SorcererShowdown ${SOURCES})

target_include_directories(SorcererShowdown PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_precompile_headers(SorcererShowdown PRIVATE "code/header/std.h")

if(MSVC)
    target_compile_options(SorcererShowdown PRIVATE /EHsc /utf-8 /W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND WIN32)
    target_compile_options(SorcererShowdown PRIVATE -Wall -Wextra -Wno-unused-parameter)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(SorcererShowdown PRIVATE -Wall -Wextra -Wno-unused-parameter -fPIC)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/characters.json")
    configure_file(characters.json ${CMAKE_CURRENT_BINARY_DIR}/characters.json COPYONLY)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/domains.json")
    configure_file(domains.json ${CMAKE_CURRENT_BINARY_DIR}/domains.json COPYONLY)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cursedtools.json")
    configure_file(cursedtools.json ${CMAKE_CURRENT_BINARY_DIR}/cursedtools.json COPYONLY)
endif()
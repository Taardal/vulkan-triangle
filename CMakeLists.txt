cmake_minimum_required(VERSION 3.20)
project(2d-game)

# Set C++ version
set(CMAKE_CXX_STANDARD 23)

# Generates a `compile_commands.json` file that contains compilation information for every source file in the project,
# facilitating better integration with various development tools and ensuring consistent and accurate builds across different environments.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure build type is always defined
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif ()
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_DIR_NAME)

# Project directory paths
set(bin_dir ${PROJECT_SOURCE_DIR}/bin)
set(src_dir ${PROJECT_SOURCE_DIR}/src)

add_executable(
    ${PROJECT_NAME}
    ${src_dir}/main.cpp
    ${src_dir}/app.cpp
    ${src_dir}/app.h
    ${src_dir}/system/assert.h
    ${src_dir}/system/environment.h
    ${src_dir}/system/error.cpp
    ${src_dir}/system/error.h
    ${src_dir}/system/error_signal.cpp
    ${src_dir}/system/error_signal.h
    ${src_dir}/system/clock.cpp
    ${src_dir}/system/clock.h
    ${src_dir}/system/log.cpp
    ${src_dir}/system/log.h
    ${src_dir}/system/numbers.h
    ${src_dir}/system/ref.h
    ${src_dir}/window/event.cpp
    ${src_dir}/window/event.h
    ${src_dir}/window/keyboard.cpp
    ${src_dir}/window/keyboard.h
    ${src_dir}/window/key_event.cpp
    ${src_dir}/window/key_event.h
    ${src_dir}/window/window.cpp
    ${src_dir}/window/window.h
    ${src_dir}/window/window_event.cpp
    ${src_dir}/window/window_event.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${src_dir})
target_precompile_headers(${PROJECT_NAME} PUBLIC ${src_dir}/pch.h)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${bin_dir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${bin_dir}/debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${bin_dir}/release
)

# --------------------------------------------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------------------------------------------

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)
message("-- Fetching dependencies")
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.9.2
)
message("-- Fetching dependencies - done")
message("-- Compiling dependencies")
FetchContent_MakeAvailable(
        glfw
        spdlog
)
message("-- Compiling dependencies - done")

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

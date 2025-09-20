cmake_minimum_required(VERSION 4.0.2)
project(game)

# Set C++ version
set(CMAKE_CXX_STANDARD 23)

# Generates a `compile_commands.json` file that contains compilation information for every source file in the project,
# facilitating better integration with various development tools and ensuring consistent and accurate builds across different environments.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure build type is always defined
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif ()
string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type_dir_name)

# Project directory paths
set(bin_dir ${PROJECT_SOURCE_DIR}/bin)
set(src_dir ${PROJECT_SOURCE_DIR}/src)
set(cmake_dir ${PROJECT_SOURCE_DIR}/cmake)

set(
    sources
    ${src_dir}/main.cpp
    ${src_dir}/app.cpp
    ${src_dir}/app.h
    ${src_dir}/game_loop.cpp
    ${src_dir}/game_loop.h
    ${src_dir}/run.cpp
    ${src_dir}/run.h
    ${src_dir}/graphics/vulkan.cpp
    ${src_dir}/graphics/vulkan.h
    ${src_dir}/graphics/vulkan_allocator.h
    ${src_dir}/graphics/vulkan_assert.h
    ${src_dir}/graphics/vulkan_device.cpp
    ${src_dir}/graphics/vulkan_device.h
    ${src_dir}/graphics/vulkan_instance.cpp
    ${src_dir}/graphics/vulkan_instance.h
    ${src_dir}/graphics/vulkan_physical_device.cpp
    ${src_dir}/graphics/vulkan_physical_device.h
    ${src_dir}/graphics/vulkan_pipeline.cpp
    ${src_dir}/graphics/vulkan_pipeline.h
    ${src_dir}/graphics/vulkan_render_pass.cpp
    ${src_dir}/graphics/vulkan_render_pass.h
    ${src_dir}/graphics/vulkan_surface.cpp
    ${src_dir}/graphics/vulkan_surface.h
    ${src_dir}/graphics/vulkan_swap_chain.cpp
    ${src_dir}/graphics/vulkan_swap_chain.h
    ${src_dir}/system/assert.cpp
    ${src_dir}/system/assert.h
    ${src_dir}/system/clock.cpp
    ${src_dir}/system/clock.h
    ${src_dir}/system/environment.h
    ${src_dir}/system/error.cpp
    ${src_dir}/system/error.h
    ${src_dir}/system/error_signal.cpp
    ${src_dir}/system/error_signal.h
    ${src_dir}/system/log.cpp
    ${src_dir}/system/log.h
    ${src_dir}/system/file.cpp
    ${src_dir}/system/file.h
    ${src_dir}/system/numbers.h
    ${src_dir}/system/time.cpp
    ${src_dir}/system/time.h
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

set(exe_target "${PROJECT_NAME}")
add_executable(${exe_target} ${sources})
target_include_directories(${exe_target} PUBLIC ${src_dir})
target_precompile_headers(${exe_target} PUBLIC ${src_dir}/pch.h)

set_target_properties(
        ${exe_target}
        PROPERTIES
        RUNTIME_OUTPUT_NAME ${exe_target}
        RUNTIME_OUTPUT_DIRECTORY ${bin_dir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${bin_dir}/debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${bin_dir}/release
)

# --------------------------------------------------------------------------------------------------------------
# Custom targets
# --------------------------------------------------------------------------------------------------------------

set(compile_shaders_target CompileShaders)
set(shaders_source_dir ${PROJECT_SOURCE_DIR}/res/shaders)
set(shaders_output_dir ${PROJECT_SOURCE_DIR}/bin/${build_type_dir_name}/res/shaders)
add_custom_target(
    ${compile_shaders_target}
    COMMAND ${CMAKE_COMMAND} -D SHADERS_SOURCE_DIR=${shaders_source_dir} -D SHADERS_OUTPUT_DIR=${shaders_output_dir} -P ${cmake_dir}/compile_shaders.cmake
    COMMENT "Compiling shaders"
)
add_dependencies(${exe_target} ${compile_shaders_target})

# --------------------------------------------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------------------------------------------

include(FetchContent)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(${exe_target} glfw)

# SPDLOG
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(${exe_target} spdlog::spdlog)

find_package(Vulkan REQUIRED)
target_include_directories(${exe_target} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${exe_target} ${Vulkan_LIBRARIES})

cmake_minimum_required(VERSION 3.19)
project(algo CXX C ASM)

set(CMAKE_CXX_STANDARD 14)

set(ASSIMP_BUILD_TESTS OFF)

find_package(glfw3 3.3 REQUIRED)

find_package(glm QUIET)
if (NOT yaml-cpp_FOUND)
    message("glm not found, initializing git submodule")
    execute_process(
            COMMAND git submodule update --init -- 3rdparty/glm
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_subdirectory(3rdparty/glm)
endif()

find_package(assimp QUIET)
if (NOT assimp_FOUND)
    message("glm not found, initializing git submodule")
    execute_process(
            COMMAND git submodule update --init -- 3rdparty/assimp
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_subdirectory(3rdparty/assimp)
endif()

add_subdirectory(3rdparty/glad)

add_subdirectory(res)

add_subdirectory(src)
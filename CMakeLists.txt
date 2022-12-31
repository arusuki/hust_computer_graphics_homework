cmake_minimum_required(VERSION 3.19)
project(algo CXX C)

set(CMAKE_CXX_STANDARD 14)

set(ASSIMP_BUILD_TESTS OFF)

message("assimp not found, initializing git submodule")
execute_process(
        COMMAND git submodule update --init -- 3rdparty/stb
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(STB INTERFACE IMPORTED)

set_target_properties(STB PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/stb
)

find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
    message("glfw3 not found, initializing git submodule")
    execute_process(
            COMMAND git submodule update --init -- 3rdparty/glfw
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_subdirectory(3rdparty/glfw)
endif()

find_package(glm QUIET)
if (NOT glm_FOUND)
    message("glm not found, initializing git submodule")
    execute_process(
            COMMAND git submodule update --init -- 3rdparty/glm
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_subdirectory(3rdparty/glm)
endif()

find_package(assimp QUIET)
if (NOT assimp_FOUND)
    message("assimp not found, initializing git submodule")
    execute_process(
            COMMAND git submodule update --init -- 3rdparty/assimp
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_subdirectory(3rdparty/assimp)
endif()

add_subdirectory(3rdparty/glad)

add_subdirectory(res)

add_subdirectory(src)
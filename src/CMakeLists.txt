add_executable(algo main.cpp shader.cpp camera.cpp)

add_library(utils shader.cpp camera.cpp stb_image.cpp cube.cpp)
target_include_directories(utils PUBLIC ./include)
target_link_libraries(utils PUBLIC glfw glad glm::glm assimp::assimp)

configure_file(shaders/colors.vs colors.vs COPYONLY)
configure_file(shaders/colors.fs colors.fs COPYONLY)
configure_file(shaders/light_cube.vs light_cube.vs COPYONLY)
configure_file(shaders/light_cube.fs light_cube.fs COPYONLY)

target_link_libraries(algo utils)

add_custom_target(
        run  $<TARGET_FILE:algo>
        DEPENDS algo
)

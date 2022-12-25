add_executable(algo main.cpp shader.cpp camera.cpp stb_image.cpp cube.cpp)


target_include_directories(algo PUBLIC ./include)
target_link_libraries(algo PUBLIC glfw glad glm::glm assimp::assimp)

configure_file(shaders/colors.vs colors.vs COPYONLY)
configure_file(shaders/colors.fs colors.fs COPYONLY)
configure_file(shaders/light_cube.vs light_cube.vs COPYONLY)
configure_file(shaders/light_cube.fs light_cube.fs COPYONLY)

install(TARGETS algo RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES shaders/colors.vs shaders/colors.fs shaders/light_cube.vs shaders/light_cube.fs
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)
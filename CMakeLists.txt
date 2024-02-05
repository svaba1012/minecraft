cmake_minimum_required(VERSION 3.10)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)

include(./FindGlew.cmake)

find_package(glfw3 3.3 REQUIRED)
find_package(glm CONFIG REQUIRED)
# target_link_libraries(main PRIVATE glm::glm)
# find_package(GLEW REQUIRED)

project(minecraft)

add_executable(minecraft ./src/main.cpp ./src/Window.cpp ./src/graphics/Shader.cpp ./src/Camera.cpp ./src/graphics/Mesh.cpp ./src/graphics/Texture.cpp ./src/Light.cpp ./src/game/GameObject.cpp  ./src/game/Scene.cpp  ./src/game/Block.cpp ./src/game/Character.cpp ./src/game/BlockType.cpp ./includes/stb_image_resize.h)

include_directories( ${GLEW_INCLUDE_DIRS} ./includes  )

target_link_libraries(minecraft glfw ${GLEW_LIBRARIES} glm::glm)

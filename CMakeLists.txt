cmake_minimum_required(VERSION 3.10)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)

# include(./FindGlew.cmake)
# include(./FindOpenGL.cmake)


find_package(glfw3 3.3 REQUIRED)
find_package(glm CONFIG REQUIRED)
# find_package(OpenGL REQUIRED)

# set(GLEW_ROOT_DIR "/home/svaba/Downloads/glew-2.1.0")
# set(GLEW_INCLUDE_DIR "/home/svaba/Downloads/glew-2.1.0/include/")
# set(GLEW_LIBRARY, "/home/svaba/Downloads/glew-2.1.0/lib")


find_package(GLEW REQUIRED)

project(minecraft)

add_executable(minecraft ./src/main.cpp ./src/Window.cpp ./src/graphics/Shader.cpp ./src/Camera.cpp ./src/graphics/Mesh.cpp ./src/graphics/Texture.cpp ./src/Light.cpp ./src/game/GameObject.cpp  ./src/game/OpenWorldScene.cpp  ./src/game/Block.cpp ./src/game/Character.cpp ./src/game/BlockType.cpp ./includes/stb_image_resize.h ./includes/stb_image.h ./src/gameLib/ui/GameUI2D.cpp ./includes/gameLib/extern/nuklear.h ./src/minecraft/Item.cpp ./src/minecraft/ItemType.cpp ./src/gameLib/inventory/InventoryUI.cpp ./src/gameLib/inventory/Inventory.cpp  ./src/gameLib/inventory/InventoryItem.cpp ./src/minecraft/MinecraftInventoryItem.cpp ./src/gameLib/SceneControls.cpp )

include_directories( ${GLEW_INCLUDE_DIRS} ./includes  )

target_link_libraries(minecraft glfw glm::glm  GLEW::glew)

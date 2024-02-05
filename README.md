# Minecraft 
I started this project to learn more about graphics programming, game development and OpenGL.

Idea of this project is to build simple minecraft copy.
## Building from source
> Current version only works on Ubuntu Linux
### Build steps
1. Setup Glut and Glew libraries
2. Clone this repo
3. Download minecraft assets(all data necessery by app such as textures, item receptes, item models...) from https://mcasset.cloud/1.20.4/
4. Extract downloaded assets to project in **./assets/** and rename it to **extern_minecraft_assets**
5. Run cmake --build ./build to build project
6. Run ./minecraft to start minecraft

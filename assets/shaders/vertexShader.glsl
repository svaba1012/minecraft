#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;


void main(){
    gl_Position = perspective * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
    texCoords = tex;
}

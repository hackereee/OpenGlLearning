#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
uniform mat4 revert;
out vec2 myTexcoord;
void main(){
    gl_Position = revert * vec4(position, 1.0);
    myTexcoord = texcoord;
}
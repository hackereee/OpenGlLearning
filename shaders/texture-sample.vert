#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textureCood;

out vec3 outColor;
out vec2 outTexcood;
void main(){
    gl_Position = vec4(pos, 1.0);
    outColor = color;
    outTexcood = textureCood;
}

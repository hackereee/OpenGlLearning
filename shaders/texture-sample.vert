#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 textureCood;

uniform mat4 transform;

out vec3 outColor;
out vec2 outTexcood;
void main(){
    gl_Position = transform * vec4(pos, 1.0);
    outTexcood = vec2(textureCood.x,  textureCood.y);
    outColor = inColor;
}

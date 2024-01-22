#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 textureCood;

//模型矩阵
uniform mat4 model;
//观察矩阵
uniform mat4 view;
//透视矩阵
uniform mat4 projection;

out vec3 outColor;
out vec2 outTexcood;
void main(){
    gl_Position =  projection * view * model * vec4(pos, 1.0);
    outTexcood = vec2(textureCood.x,  textureCood.y);
    outColor = inColor;
}

#version 330 core

//xy为顶点坐标
layout(location=0) in vec3 pos;
//纹理坐标
layout(location=1) in vec2 texCoord;

uniform mat4 transform;

out vec2 TexCoord;

void main(){
    gl_Position =  transform * vec4(pos.x, pos.y, pos.z, 1.0);
    TexCoord = texCoord;
}
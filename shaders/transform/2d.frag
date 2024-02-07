#version 330 core
in vec2 TexCoord;

//纹理数据
uniform sampler2D texData;

out vec4 FragColor;

void main(){
    //纹理采样
    FragColor = texture(texData, TexCoord);
}
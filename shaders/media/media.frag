#version 330 core

in vec2 myTexcoord;
uniform bool useTexture;
uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;

out vec4 FragColor;

void main() {
    if(!useTexture) {
        FragColor = vec4(0.2, 0.4, 0.8, 1.0);
    } else {
    //YUV to RGB
        vec3 yuv;
        yuv.x = texture(textureY, myTexcoord).r;
        yuv.y = texture(textureU, myTexcoord).r - 0.5;
        yuv.z = texture(textureV, myTexcoord).r - 0.5;


        vec3 rgb = mat3(1.0, 1.0, 1.0, 0.0, -0.39465, 2.03211, 1.13983, -0.58060, 0.0) * yuv;
        FragColor = vec4(rgb, 1.0);
    }
}
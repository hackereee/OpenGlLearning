#define STB_IMAGE_IMPLEMENTATION
#include<common/TextureSample.h>
#include <stb_image.h>
#include<program/shader.h>


unsigned char *loadContainerTexture() {
	//¿í¡¢¸ß¡¢Í¨µÀ
	int width, height, nrChannels;
	return stbi_load("resource/container.jpg", &width, &height, &nrChannels, 0);
}

void renderTextrue() {

	const char *vert = "shader/ texture - sample.vert";
	const char *frag = "shader/texture-sample.frag";
	Shader shader(vert, frag);
	shader.use();
	int programId = shader.ProgramId;
}


void renderCore() {

}


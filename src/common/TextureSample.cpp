#define STB_IMAGE_IMPLEMENTATION
#include<common/TextureSample.h>
#include <stb_image.h>


void loadContainerTexture() {
	//���ߡ�ͨ��
	int width, height, nrChannels;
	unsigned char *imageData = stbi_load("resource/container.jpg", &width, &height, &nrChannels, 0);

}



const float vvertices[] = {
	0.5f, 0.5f, 0,
	0.5f, -0.5f, 0,
	-0.5f, -0.5f, 0,
	-0.5f, 0.5f, 0
};

const float textureVertices[] = {
	1.0f, 1.0f,
	1.0f, 0,
	0,0,
	0, 1.0f
};


void renderTexture();

unsigned char* loadContainerTexture(int* width, int* height);

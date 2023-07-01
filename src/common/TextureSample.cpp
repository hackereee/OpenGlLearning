#define STB_IMAGE_IMPLEMENTATION
#include<common/TextureSample.h>
#include <stb_image.h>
#include<program/shader.h>
#include<common/gl_common.h>



unsigned char* loadContainerTexture(int* width, int* height) {
	//宽、高、通道
	int nrChannels;
	return stbi_load("resource/container.jpg", width, height, &nrChannels, 0);
}

void renderCore(Shader& shader, GLFWwindow* window) {
	unsigned int VAO;
	unsigned int VBO[2];
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertices), textureVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2, (void*)0);


	//纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//设置环绕和过滤方式
	//s、t轴都使用重复环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//都使用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* imgData = loadContainerTexture(&width, &height);
	//加载纹理数据
	if (imgData) {
		glTexImage2D(texture, 0, GL_RGB, width, height, 0/*历史问题，总是传0,*/, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed load texture: img is NULL!" << std::endl;
		return;
	}
	//释放图片
	stbi_image_free(imgData);
	//解绑VAO
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//交换缓冲区并查询IO事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


}


void renderTextrue() {
	if (!initGlEnv())
	{
		return;
	}
	GLFWwindow* window = initWindow(800, 600);
	if (window == NULL)
	{
		return;
	}
	const char* vert = "shader/ texture - sample.vert";
	const char* frag = "shader/texture-sample.frag";
	Shader shader(vert, frag);
	renderCore(shader, window);
}




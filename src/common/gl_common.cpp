#include<Common/gl_common.h>
#include <iostream>
#include <common/base.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>






GLFWwindow* initGlEnv(int width, int height, const char* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(onGlfwError);
	GLFWwindow* window = initWindow(width, height, title);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed init glad!" << std::endl;
		return  NULL;
	}
	return window;
}




void onFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


GLFWwindow* initWindow(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "init window error:" << "the window is NULL !" << std::endl;
		return  NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, onFrameBufferSizeCallback);
	return window;
}


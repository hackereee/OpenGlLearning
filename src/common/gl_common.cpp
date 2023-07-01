#include<Common/gl_common.h>
#include <iostream>


bool initGlEnv() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwSetErrorCallback(onGlfwError);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed init glad!" << std::endl;
		return  false;
	}
	return true;
}



void onGlfwError(int code, const  char *desc) {
	std::cout << "glfw error code :"  << code << "---desc:" << desc << std::endl;
}



void onFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


GLFWwindow *initWindow(int width, int height, char *title ) {
	GLFWwindow *window =  glfwCreateWindow(width, height, title, NULL, NULL);
	if(window == NULL)
	{
		std::cout << "init window error:" << "the window is NULL !" << std::endl;
		return  NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, onFrameBufferSizeCallback);
	return window;
}


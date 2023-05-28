#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* initWindow();
void render(GLFWwindow* window);
void onWindowSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void onGlfwError(int errorCode, const char* desc);

/// <summary>
/// ��������
/// </summary>
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f , 0.0f
};

/// <summary>
/// ������ɫ��Դ����
/// </summary>
const char *vertexShaderSource = "#version 330 core\n" 
"layout(location = 0) in vec3 aPos;\n"
"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

/// <summary>
/// Ƭ����ɫ��
/// </summary>
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(0.0, 0.3, 0.3, 1.0 );\n"
"}";

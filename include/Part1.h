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
	// ����             //RGB
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f, 0.5f , 0.0f,  0.0f, 0.0f, 1.0f
};

/// <summary>
/// ������ɫ��Դ����
/// vertexColor ���Զ������ɫ�����Ӷ�����ɫ�������룬��Ƭ����ɫ�������
/// aColor ��Cpu���루���룩
/// </summary>
const char *vertexShaderSource = "#version 330 core\n" 
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec4 vertexColor;\n"
"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"vertexColor = vec4(aColor, 1.0);\n"
"}\n";

/// <summary>
/// Ƭ����ɫ��
/// </summary>
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main(){\n"
"FragColor = vertexColor;\n"
"}";

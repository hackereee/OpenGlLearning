#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* initWindow();
void render(GLFWwindow* window);
void onWindowSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/// <summary>
/// 顶点坐标
/// </summary>
float vertices[] = {
	// 顶点             //RGB
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f, 0.5f , 0.0f,  0.0f, 0.0f, 1.0f
};

/// <summary>
/// 顶点着色器源代码
/// vertexColor 是自定义的颜色，它从顶点着色器中输入，在片段着色器中输出
/// aColor 由Cpu输入（代码）
/// </summary>
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec4 vertexColor;\n"
"void main() {\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"vertexColor = vec4(aColor, 1.0);\n"
"}\n";

/// <summary>
/// 片段着色器
/// </summary>
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main(){\n"
"FragColor = vertexColor;\n"
"}";


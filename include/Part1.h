#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* initWindow();
void render(GLFWwindow* window);
void onWindowSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void beginPart1();




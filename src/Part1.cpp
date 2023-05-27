#include<Part1.h>
#include<iostream>

using std::string;

int main() {
	//Opengl初始化
	glfwInit();
	
	//配置版本
	//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3 );
	//
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(onGlfwError);
	//如果是MACOS需要加上该行
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = initWindow();
	/*GLAD管理opengl函数指针，初始化GLAD*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "faild init glad!" << std::endl;
		return -1;
	}
	render(window);
	//渲染结束，释放所有资源
	glfwTerminate();
return 0;
}

/*初始化窗口*/
GLFWwindow * initWindow()   {
	GLFWwindow* window = glfwCreateWindow(800, 600, "Part1", NULL, NULL);
	if (window == NULL) {
		std::cout << "can not create the window, the window is null" << std::endl;
		glfwTerminate();
		return NULL;
	}
	
	//设置上下文
	glfwMakeContextCurrent(window);
	//加入窗口大小监听
	glfwSetFramebufferSizeCallback(window, onWindowSizeCallback);
	return window;
}

/*窗口大小改变时改变视口大小*/
void onWindowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width, height);
}

void onGlfwError(int code, const  char *desc) {
	std::cout << "error with" + string(desc) << std::endl;
}


/// <summary>
/// 渲染
/// </summary>
/// <param name="window">窗口</param>
void render(GLFWwindow *window) {
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//这里执行渲染
		glClearColor(1.0f,0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


/// <summary>
/// 按键输入空格，则停止渲染
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



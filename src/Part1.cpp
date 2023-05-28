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
	//创建顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//顶点着色器源码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译源码
	glCompileShader(vertexShader);
	char infoLog[1024];
	int success;
	//获取编译状态
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		std::cout << "error to create vertex sharder:" << infoLog << std::endl;

		return;
	}
	//创建片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//片段着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		std::cout << "error to create fragment shader" << infoLog << std::endl;
		return;
	}
	//创建着色器程序
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//链接着色器程序
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
		std::cout << "error to link program:" << infoLog << std::endl;
		return;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//定义VAO与VBO
	//顶点数组缓冲对象和顶点缓冲对象
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//首先绑定数组对象，再绑定顶点缓冲，这样数组对象的第0个元素就是VBO了
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//这里使用GL_STATIC_DRAW，因为渲染完成后几乎不会发生变化
	//与之相对的参考GL_DYNAMIC_DRAW 与 GL_STREAM_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//取顶点程序里的layout=0属性，有3个顶点，作为float类型，不进行坐标标准化（即归一化，将传入的顶点归一化到-1,1之间),步长为3,（因为我们设置了x,y,z），偏移量（offset）为0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//启用顶点
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//这一步取消对VAO的绑定，防止其他程序误用了这个缓冲
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//这里执行渲染
		glClearColor(1.0f,1.0f,1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//开始渲染三角形
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//使用三角形图元渲染这个三角形
		glDrawArrays(GL_TRIANGLES, 0, 3);
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



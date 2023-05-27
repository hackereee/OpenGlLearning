#include<Part1.h>
#include<iostream>

using std::string;

int main() {
	//Opengl��ʼ��
	glfwInit();
	
	//���ð汾
	//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//�ΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3 );
	//
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(onGlfwError);
	//�����MACOS��Ҫ���ϸ���
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = initWindow();
	/*GLAD����opengl����ָ�룬��ʼ��GLAD*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "faild init glad!" << std::endl;
		return -1;
	}
	render(window);
	//��Ⱦ�������ͷ�������Դ
	glfwTerminate();
return 0;
}

/*��ʼ������*/
GLFWwindow * initWindow()   {
	GLFWwindow* window = glfwCreateWindow(800, 600, "Part1", NULL, NULL);
	if (window == NULL) {
		std::cout << "can not create the window, the window is null" << std::endl;
		glfwTerminate();
		return NULL;
	}
	
	//����������
	glfwMakeContextCurrent(window);
	//���봰�ڴ�С����
	glfwSetFramebufferSizeCallback(window, onWindowSizeCallback);
	return window;
}

/*���ڴ�С�ı�ʱ�ı��ӿڴ�С*/
void onWindowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width, height);
}

void onGlfwError(int code, const  char *desc) {
	std::cout << "error with" + string(desc) << std::endl;
}


/// <summary>
/// ��Ⱦ
/// </summary>
/// <param name="window">����</param>
void render(GLFWwindow *window) {
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//����ִ����Ⱦ
		glClearColor(1.0f,0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


/// <summary>
/// ��������ո���ֹͣ��Ⱦ
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



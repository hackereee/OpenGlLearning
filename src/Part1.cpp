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
	//����������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//������ɫ��Դ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//����Դ��
	glCompileShader(vertexShader);
	char infoLog[1024];
	int success;
	//��ȡ����״̬
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		std::cout << "error to create vertex sharder:" << infoLog << std::endl;

		return;
	}
	//����Ƭ����ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Ƭ����ɫ��
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		std::cout << "error to create fragment shader" << infoLog << std::endl;
		return;
	}
	//������ɫ������
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//������ɫ������
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 1024, NULL, infoLog);
		std::cout << "error to link program:" << infoLog << std::endl;
		return;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//����VAO��VBO
	//�������黺�����Ͷ��㻺�����
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//���Ȱ���������ٰ󶨶��㻺�壬�����������ĵ�0��Ԫ�ؾ���VBO��
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����ʹ��GL_STATIC_DRAW����Ϊ��Ⱦ��ɺ󼸺����ᷢ���仯
	//��֮��ԵĲο�GL_DYNAMIC_DRAW �� GL_STREAM_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//ȡ����������layout=0���ԣ���3�����㣬��Ϊfloat���ͣ������������׼��������һ����������Ķ����һ����-1,1֮��),����Ϊ3,����Ϊ����������x,y,z����ƫ������offset��Ϊ0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//���ö���
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//��һ��ȡ����VAO�İ󶨣���ֹ���������������������
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//����ִ����Ⱦ
		glClearColor(1.0f,1.0f,1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//��ʼ��Ⱦ������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//ʹ��������ͼԪ��Ⱦ���������
		glDrawArrays(GL_TRIANGLES, 0, 3);
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



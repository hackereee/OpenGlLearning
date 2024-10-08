#include<common/TextureSample.h>
#include <stb_image.h>
#include<Program/shader.h>
#include<common/gl_common.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const int viewportWidth = 800;
const int viewportHeight = 600;

const float vertices[] = {
	// positions          // colors           // texture coords
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	   -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	   -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};


const int indices[] = {
	0,1,3,
	1,2,3
};


unsigned char* loadContainerTexture(int* width, int* height) {
	stbi_set_flip_vertically_on_load(true);
	//宽、高、通道
	int nrChannels;
	return stbi_load("resources/ee.jpg", width, height, &nrChannels, 0);
}

void renderCore(Shader& shader, GLFWwindow* window) {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	//模型矩阵，世界坐标系
	glm::mat4 model = glm::mat4(1.0f);
	//沿着x轴先旋转一定的角度
	model = glm::rotate<float>(model, (float)glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//观察矩阵,观察坐标系
	glm::mat4 view = glm::mat4(1.0f);
	//验证z轴将摄像机原理3倍， glm 为右手坐标系,所以以观察者为原点，越往后数字z轴应该越小（负数）
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	//透视投影矩阵，裁剪坐标系
	glm::mat4 projection = glm::mat4(1.0f);
	//透视方法，第一个参数指定fov角（filed of view），第二个参数指定透视变换，第三个为近平面，第四个为远平面
	projection = glm::perspective<float>((float)glm::radians(45.0f), viewportWidth / viewportHeight, 0.1f, 100.0f);


	shader.use();
	int modelLocation = glGetUniformLocation(shader.ProgramId, "model");
	int viewLocation = glGetUniformLocation(shader.ProgramId, "view");
	int projectionLocation = glGetUniformLocation(shader.ProgramId, "projection");
	//第三个参数代表是否需要对矩阵进行转置，由于glm与opengl都默认使用列主序排列，所以不需要转置，传false
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));





	//纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//设置环绕和过滤方式
	//s、t轴都使用重复环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* imgData = loadContainerTexture(&width, &height);
	//加载纹理数据
	if (imgData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed load texture: img is NULL!" << std::endl;
		return;
	}
	//释放图片
	stbi_image_free(imgData);

	std::string plane = "texture1";
	shader.setInt(plane, 0);
	//解绑VAO
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//交换缓冲区并查询IO事件
		glfwSwapBuffers(window);
		glfwPollEvents();
		// trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 2.0f));
	}


}


void renderTexture() {
	const char* title = "纹理示例";
	GLFWwindow* window = initGlEnv(viewportWidth, viewportHeight, title);
	if (window == NULL)
	{
		return;
	}
	const char* vert = "shaders/texture-sample.vert";
	const char* frag = "shaders/texture-sample.frag";
	Shader shader(vert, frag);
	renderCore(shader, window);
}



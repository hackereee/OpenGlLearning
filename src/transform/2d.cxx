// Purpose: 2D Transform
// Author:  ddy
// this is a simple demo for 2D transform

#include <common/gl_common.h>

#include <transform/2d.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>
#include <map>
#include <functional>

glm::mat4 rotate(glm::mat4 &input)
{
    return glm::rotate<float>(input, glm::radians(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 scale(glm::mat4 &input)
{
    return glm::scale(input, glm::vec3(0.5f, 0.5f, 0.5f));
}

glm::mat4 translate(glm::mat4 &input)
{
    return glm::translate(input, glm::vec3(0.5f, 0.5f, 0.5f));
}

glm::mat4 baseMat = glm::mat4(1.0f);

std::function<glm::mat4(glm::mat4 &)> transformFunction;

/// @brief 顶点及纹理坐标
const float vertices[] = {
    // 顶点坐标          // 纹理坐标
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

const int indices[] = {
    0, 1, 2,
    2, 3, 1};

Transform2D transform2D;

void trans2D()
{
    transform2D.start();
}

Transform2D::Transform2D()
{
    init_2d();
}

void Transform2D::init_2d()
{
    window = initGlEnv(800, 600, "2D Transform");
    shader = std::make_shared<Shader>("shaders/transform/2d.vert", "shaders/transform/2d.frag");

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    // 顶点坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    // 元素缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int width, height, nrChannels;
    // 加载图片是翻转
    stbi_set_flip_vertically_on_load(true);

    // 加载纹理图片
    unsigned char *data = stbi_load("resources/guoguo.jpg", &width, &height, &nrChannels, 0);

    if (data == NULL)
    {
        std::cout << "load texture failed: data is null!" << std::endl;
        return;
    }

    // 纹理
    // 环绕和过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // x轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // y轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大

    // 加载纹理
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Transform2D::start()
{
    baseMat = glm::rotate<float>(baseMat, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = &baseMat;
    transformInputAsync();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);         // 清屏
        shader->use();
        std::string name = "transform";
        shader->setMat4(name, *transform);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);
        // 绘制
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);
    realeased = true;
    delete transform;

    shader->release();
}

void transformInput()
{
    std::map<int, std::function<glm::mat4(glm::mat4 &)>> transformMap;
    transformMap[1] = rotate;
    transformMap[2] = scale;
    transformMap[3] = translate;
    std::cout << "您可以输入以下指令进行变换" << std::endl;
    std::cout << "1. 旋转" << std::endl;
    std::cout << "2. 缩放" << std::endl;
    std::cout << "3. 平移" << std::endl;
    int input = 0;
    std::cin >> input;
    if (!transform2D.realeased && transformMap.find(input) != transformMap.end())
    {
        transformFunction = transformMap[input];
    }
    else
    {
        std::cout << "输入错误" << std::endl;
    }
    transformInput();
}

void trans()
{
    while (!transform2D.realeased)
    {
        if (transformFunction)
        {
            glm::mat4 t = transformFunction(baseMat);
            transform2D.transform = &t;
        }
    }
}

void startTransform()
{

    std::thread t(trans);
    t.detach();
}

void Transform2D::transformInputAsync()
{
    std::thread t(transformInput);
    t.detach();
    startTransform();
}



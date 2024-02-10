#ifndef DD_H
#define DD_H


#include <GLFW/glfw3.h>
#include <Program/shader.h>
#include<memory>

void trans2D();


class Transform2D {

 private:
    GLFWwindow *window ;
    std::shared_ptr<Shader> shader ;
    unsigned int  texture1;
    unsigned int VAO;
    unsigned int EBO;
   

    public:
    Transform2D();
    void start();
     glm::mat4* transform;
      bool realeased;
   

   private:
    void init_2d();

    void transformInputAsync();
};
#endif
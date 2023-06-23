#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/// <summary>
/// 着色器类
/// </summary>
class Shader {
public :
	unsigned int ProgramId;
	/// <summary>
	/// </summary>
	/// <param name="vertexSource">顶点着色器源代码</param>
	/// <param name="fragmentSource">片段着色器源代码</param>
	Shader(char* vertexSource, char* fragmentSource) {

	}
	//使用
	void use();

	void setInt(std::string &name, int value);
	void setBool(std::string &name, bool value);
	void setFloat(std::string &name, float value);

private:
	void compileAndLink(const char* vertexCode, const char* fragmentCode);


};

#endif
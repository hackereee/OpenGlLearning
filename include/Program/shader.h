#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/// <summary>
/// ��ɫ����
/// </summary>
class Shader {
public :
	unsigned int ProgramId;
	/// <summary>
	/// </summary>
	/// <param name="vertexSource">������ɫ��Դ����</param>
	/// <param name="fragmentSource">Ƭ����ɫ��Դ����</param>
	Shader(char* vertexSource, char* fragmentSource) {

	}
	//ʹ��
	void use();
};

#endif
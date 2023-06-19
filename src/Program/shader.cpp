#include <Program/shader.h>

using namespace std;

Shader::Shader(char* vertexSourcePath, char* fragmentSourcePath) {
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		vShaderFile.open(vertexCode);
		fShaderFile.open(fragmentCode);
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "init shader source failed: " << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

}

void Shader::compileAndLink(char* vertexCode, char* fragmentCode) {
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

}

void Shader::use() {

}


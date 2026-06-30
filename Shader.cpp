#include "Shader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


cShader::cShader(const char* path) {
	std::string shaderCode;

	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	try {
		shaderFile.open(path);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
	}

	const char* constShaderCode = shaderCode.c_str();
	unsigned int shader;
	int success;
	char infoLog[512];

	//vertex
	shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &constShaderCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, shader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(shader);
}

void cShader::useAndDispatch(unsigned int x, unsigned int y, unsigned int z) {
	glUseProgram(ID);
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
void cShader::use() {
	glUseProgram(ID);
}
void cShader::dispatch(unsigned int x, unsigned int y, unsigned int z) {
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void cShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void cShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void cShader::setUint(const std::string& name, unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void cShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void cShader::setMat4(const std::string& name, float* values_ptr) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, values_ptr);
}




Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	try {
		//open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//read file's buffer content into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}

	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//vertex
	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//program
	ID = glCreateProgram();

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader::use() {
	glUseProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUint(const std::string& name, unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string& name, float *values_ptr) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, values_ptr);
}


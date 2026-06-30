#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>

class Shader
{
	public:
		unsigned int ID;

		Shader(const char *vertexPath, const char *fragmentPath);

		void use();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setUint(const std::string& name, unsigned int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, float* values_ptr) const;
};

class cShader
{
	public:
		unsigned int ID;

		cShader(const char* path);

		void useAndDispatch(unsigned int x, unsigned int y, unsigned int z);
		void use();
		void dispatch(unsigned int x, unsigned int y, unsigned int z);
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setUint(const std::string& name, unsigned int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, float* values_ptr) const;
};

#endif	

#pragma once 
#include "glad/glad.h"
#include <string>

class Shader
{
public:
	Shader(const char* vertexShaderPath,const char* fragShaderPath);
	virtual ~Shader();

	unsigned int ShaderProgramID;

	void UseShaderProgram();

	void SetUniformBool(const std::string& name, bool value);
	void SetUniformInt(const std::string& name, int value);
	void SetUniformFloat(const std::string& name, float value);

private:

};
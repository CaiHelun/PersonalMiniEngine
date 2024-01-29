#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexShaderPath, const char* fragShaderPath)
{
	std::string vertexPath = vertexShaderPath;
	std::string fragPath = fragShaderPath;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	std::string vertexCode;
	std::string fragCode;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragPath);

		std::stringstream vertexStream, fragStream;

		vertexStream << vShaderFile.rdbuf();
		fragStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vertexStream.str();
		fragCode = fragStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vertexShader = vertexCode.c_str();
	const char* fragShader = fragCode.c_str();

	unsigned int VS;
	VS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VS, 1, &vertexShader, nullptr);
	glCompileShader(VS);

	int shaderCompileSuccess;
	char infoLog[512];
	glGetShaderiv(VS, GL_COMPILE_STATUS, &shaderCompileSuccess);
	if (!shaderCompileSuccess)
	{
		glGetShaderInfoLog(VS, 512, nullptr, infoLog);
		std::cout << "Error::Shader::Vertex::Compilatioion_Failed\n" << infoLog << std::endl;
	}

	unsigned int FS;
	FS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FS, 1, &fragShader, nullptr);
	glCompileShader(FS);

	glGetShaderiv(FS, GL_COMPILE_STATUS, &shaderCompileSuccess);
	if (!shaderCompileSuccess)
	{
		glGetShaderInfoLog(FS, 512, nullptr, infoLog);
		std::cout << "Error::Shader::Vertex::Compilatioion_Failed\n" << infoLog << std::endl;
	}

	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, VS);
	glAttachShader(ShaderProgramID, FS);
	glLinkProgram(ShaderProgramID);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &shaderCompileSuccess);
	if (!shaderCompileSuccess)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, nullptr, infoLog);
	}

	glDeleteShader(VS);
	glDeleteShader(FS);
}

Shader::~Shader()
{
}

void Shader::UseShaderProgram()
{
	glUseProgram(ShaderProgramID);
}

void Shader::SetUniformBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3 vec)
{
	glUniform3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, false, glm::value_ptr(mat));
}

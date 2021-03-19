#include "Shader.h"
#include <fstream>
#include <sstream>

#include "../Logging/GlobalAppLog.h"



Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure exception)
	{
		GlobalAppLog.writeLog("Error reading shader files", LOGMODE::ERROR);
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	GLuint vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		GlobalAppLog.writeLog("Failed to compile vertex shader:", LOGMODE::ERROR);
		GlobalAppLog.writeLog(infoLog, LOGMODE::UNPREFIXED);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		GlobalAppLog.writeLog("Failed to compile fragment shader.", LOGMODE::ERROR);
		GlobalAppLog.writeLog(infoLog, LOGMODE::UNPREFIXED);
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		GlobalAppLog.writeLog("Failed to link shader program.", LOGMODE::ERROR);
		GlobalAppLog.writeLog(infoLog, LOGMODE::UNPREFIXED);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}



void Shader::useShader()
{
	glUseProgram(programID);
}



void Shader::setVec2(const std::string& name, const glm::vec2& vec) const
{
	const float xComponent = vec.x;
	const float yComponent = vec.y;
	glUniform2f(glGetUniformLocation(programID, name.c_str()), xComponent, yComponent);
}



void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

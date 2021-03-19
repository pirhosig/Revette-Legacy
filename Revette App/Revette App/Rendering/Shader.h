#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void useShader();

	void setVec2(const std::string& name, const glm::vec2& vec) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	GLuint programID;
private:
};


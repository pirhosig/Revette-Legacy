#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void useShader();

	void setVec2(const std::string& name, float x, float y) const;

	GLuint program_id;
private:
};


#pragma once
#include <memory>
#include <vector>

#include "../Rendering/Shader.h"
#include "Vertex.h"


class EntityMesh
{
public:

	void createMesh();
	bool draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, float entityX, float entityY);


	float sizeX;
	float sizeY;
	
	bool hasMesh;

private:

	bool bufferData();
	void clearMesh();
	bool setupBuffers();

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indicies;
	bool hasBuffers;
	bool hasMeshChanged;
	unsigned triangleCount;

	GLuint VAO{ NULL };
	GLuint EBO{ NULL };
	GLuint VBO{ NULL };
};
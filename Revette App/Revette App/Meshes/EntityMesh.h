#pragma once
#include <memory>
#include <vector>

#include "../Rendering/Shader.h"
#include "Vertex.h"


class EntityMesh
{
public:
	EntityMesh(const unsigned xsize, const unsigned ysize);

	void createMesh();
	bool draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, float entityX, float entityY);


	unsigned sizeX;
	unsigned sizeY;

private:

	bool bufferData();
	bool setupBuffers();

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indicies;
	bool hasBuffers;
	bool hasMesh;
	bool hasMeshChanged;
	unsigned triangleCount;

	GLuint VAO{ NULL };
	GLuint EBO{ NULL };
	GLuint VBO{ NULL };
};
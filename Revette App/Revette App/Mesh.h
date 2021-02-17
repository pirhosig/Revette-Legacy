#pragma once
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Rendering/Vertex.h"
#include "Rendering/Shader.h"

class Mesh
{
public:
	void addVertices(Vertex* vertexData, unsigned vertexCount, unsigned short* indexData, unsigned indexCount);
	void clearMesh();
	bool draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, const glm::vec2& chunkOffset);
	bool setupBuffers();
	bool bufferData();
	bool isBuffered {false};

private:

	std::vector<unsigned short> indices;
	std::vector<Vertex> vertices;
	unsigned triangleCount {0};
	GLuint VAO {NULL};
	GLuint VBO {NULL};
	GLuint EBO {NULL};
	
	bool meshHasChanged {false};
	bool meshExists {false};
};


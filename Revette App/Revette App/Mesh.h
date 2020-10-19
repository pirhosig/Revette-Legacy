#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>

#include "Vertex.h"
#include "Shader.h"

class Mesh
{
public:
	void addVertices(Vertex* vertexData, unsigned vertexCount, unsigned short* indexData, unsigned indexCount);
	void clearMesh();
	bool draw(std::unique_ptr<Shader>& shader);
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


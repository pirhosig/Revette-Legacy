#pragma once
#include <memory>
#include <vector>

#include "Rendering/Vertex.h"
#include "Rendering/Shader.h"
#include "Tile.h"

class ChunkMesh
{
public:
	void addSquare(unsigned short x, unsigned short y, unsigned short texture);
	bool bufferData();
	void clearMesh();
	bool createMesh(const std::unique_ptr<Tile[]>& tiles);
	bool drawMesh(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, float chunkXPos, float chunkYPos);
	bool setupBuffers();

	bool isBuffered{ false };

private:

	std::vector<unsigned short> indices;
	std::vector<Vertex> vertices;
	unsigned triangleCount{ 0 };
	GLuint VAO{ NULL };
	GLuint VBO{ NULL };
	GLuint EBO{ NULL };

	bool meshHasChanged{ false };
	bool meshExists{ false };
};


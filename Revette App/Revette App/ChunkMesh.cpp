#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ChunkMesh.h"


unsigned short test_tile_indices[6]{
	0, 1, 2, 2, 3, 0
};

constexpr unsigned CHUNK_KEY_SHIFT{ 5 };




// Adds a textured tile square to the mesh
void ChunkMesh::addSquare(unsigned short x, unsigned short y, unsigned short texture)
{
	unsigned short tileLowerX = 0 + x;
	unsigned short tileUpperX = 1 + x;
	unsigned short tileLowerY = 0 + y;
	unsigned short tileUpperY = 1 + y;

	unsigned char textureLowerX = 0;
	unsigned char textureUpperX = 128;
	unsigned char textureLowerY = 0;
	unsigned char textureUpperY = 128;
	
	ArrayVertex test_tile_vertices[4] {
		tileLowerX, tileLowerY, texture, textureLowerX, textureLowerY,
		tileUpperX, tileLowerY, texture, textureUpperX, textureLowerY,
		tileUpperX, tileUpperY, texture, textureUpperX, textureUpperY,
		tileLowerX, tileUpperY, texture, textureLowerX, textureUpperY
	};


	if (!meshExists) meshExists = true;

	// Add the data to the mesh
	unsigned current_index = static_cast<unsigned>(vertices.size());

	for (unsigned i = 0; i < 4; ++i)
	{
		vertices.push_back(test_tile_vertices[i]);
	}

	for (unsigned i = 0; i < 6; ++i)
	{
		indices.push_back(test_tile_indices[i] + current_index);
	}

	triangleCount += 2;
}


bool ChunkMesh::createMesh(const std::unique_ptr<Tile[]>& tiles)
{
	isBuffered = false;
	clearMesh();

	for (unsigned int x = 0; x < 32; ++x)
	{
		for (unsigned int y = 0; y < 32; ++y)
		{
			unsigned int tile_location = (x + (y << CHUNK_KEY_SHIFT));
			int type = tiles[tile_location].type;
			if (type > 0)
			{
				unsigned int textureType = static_cast<unsigned int>(type - 1);
				addSquare(x, y, textureType);
			}
		}
	}
	return true;
}


// Draws the buffered data
bool ChunkMesh::drawMesh(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, float chunkXPos, float chunkYPos)
{
	if (!meshExists) return true;
	if (!isBuffered)
	{
		if (!setupBuffers()) return false;
		if (!bufferData()) return false;
	}
	else
	{
		// Rebuffer the vertex and index data if the mesh has changed
		if (meshHasChanged)
		{
			if (!bufferData()) return false;
		}
	}
	// Set the shader to be used
	shader->useShader();

	//Calculate the transformation matrix
	glm::mat4 model = glm::mat4(1.0f);
	//Translate vertices to chunk position
	const glm::vec2 chunkOffset(chunkXPos, chunkYPos);
	model = glm::translate(model, glm::vec3(chunkOffset, 0.0f));
	//Translate vertices relative to the camera
	model = glm::translate(model, glm::vec3(cameraOffset, 0.0f));
	const glm::mat4 vertexTranformationMatrix = projection * model;

	shader->setMat4("vertexTransformationMatrix", vertexTranformationMatrix);

	// Load Vertex Array Object, which automatically loads configuration of buffers
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_SHORT, 0);

	// Unbind the VAO so future calls do not accidentally modify it
	glBindVertexArray(0);

	return true;
}



// Clears the underlying mesh containers
void ChunkMesh::clearMesh()
{
	vertices.clear();
	indices.clear();
	triangleCount = 0;
}



// Initially creates and configures the buffer objects on the GPU
bool ChunkMesh::setupBuffers()
{
	// Return if buffers already exist
	if (isBuffered) return false;

	// Generate and bind a Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate 2 buffers for vertices and indices
	GLuint buffers[2];
	glGenBuffers(2, buffers);
	VBO = buffers[0];
	EBO = buffers[1];
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Configure the attribute pointers and enable them
	glVertexAttribPointer(0, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(ArrayVertex), (void*)(0));
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_SHORT, sizeof(ArrayVertex), (void*)(4));
	glVertexAttribPointer(2, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(ArrayVertex), (void*)(6));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind the vertex array as to not interfere with anything else
	glBindVertexArray(0);

	isBuffered = true;

	return true;
}



// Uploads the mesh data to the buffers on the GPU
bool ChunkMesh::bufferData()
{
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ArrayVertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

	meshHasChanged = false;

	return true;
}


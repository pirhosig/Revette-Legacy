#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

// Adds a series of vertices and indices
void Mesh::addVertices(Vertex* vertexData, unsigned vertexCount, unsigned short* indexData, unsigned indexCount)
{
	meshExists = true;
	unsigned current_index = static_cast<unsigned>(vertices.size());
	for (unsigned i = 0; i < vertexCount; ++i)
	{
		vertices.push_back(vertexData[i]);
	}

	for (unsigned i = 0; i < indexCount; ++i)
	{
		indices.push_back(indexData[i] + current_index);
	}
	triangleCount = triangleCount + indexCount / 3;
}



void Mesh::clearMesh()
{
	vertices.clear();
	indices.clear();
	triangleCount = 0;
}



bool Mesh::draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, const glm::vec2& chunkOffset)
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
	shader.get()->useShader();

	//Calculate the transformation matrix
	glm::mat4 model = glm::mat4(1.0f);
	//Translate vertices to chunk position
	model = glm::translate(model, glm::vec3(chunkOffset, 0.0f));
	//Translate vertices relative to the camera
	model = glm::translate(model, glm::vec3(cameraOffset, 0.0f));
	const glm::mat4 vertexTranformationMatrix = projection * model;

	shader.get()->setMat4("vertexTransformationMatrix", vertexTranformationMatrix);

	// Load Vertex Array Object, which automatically loads configuration of buffers
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	return true;
}



// Initially creates and configures the buffer objects on the GPU
bool Mesh::setupBuffers()
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
	glVertexAttribPointer(0, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)(4));
	glVertexAttribPointer(2, 2, GL_UNSIGNED_BYTE,  GL_FALSE, sizeof(Vertex), (void*)(6));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Unbind the vertex array as to not interfere with anything else
	glBindVertexArray(0);

	isBuffered = true;

	return true;
}



// Uploads the mesh data to the buffers on the GPU
bool Mesh::bufferData()
{
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

	meshHasChanged = false;

	return true;
}

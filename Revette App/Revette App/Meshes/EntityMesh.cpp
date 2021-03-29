#include "EntityMesh.h"

#include <glm/gtc/matrix_transform.hpp>



// Creates the mesh for the entity
void EntityMesh::createMesh()
{
	// This entire function implementation is temporary
	// Everything inside here should change at some point
	// I cannot emphasise enough just how temporary this is

	clearMesh();
	hasMesh = true;
	hasMeshChanged = true;

	unsigned short TEMP_SQUARE[6]{
	0, 1, 2, 2, 3, 0
	};
	for (unsigned i = 0; i < 6; ++i)
	{
		indicies.push_back(TEMP_SQUARE[i]);
	}

	vertices.push_back({ -1, -1, 0,   0 });
	vertices.push_back({  1, -1, 255, 0 });
	vertices.push_back({  1,  0, 255, 255 });
	vertices.push_back({ -1,  0, 0,   255 });

	triangleCount += 2;
}



bool EntityMesh::draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, float entityX, float entityY)
{
	// Return failure if no mesh data exists
	if (!hasMesh) return false;
	// Buffer the data if it has not already been
	if (hasMeshChanged)
	{
		if (!bufferData()) return false;
	}

	//Calculate the transformation matrix
	glm::mat4 model = glm::mat4(1.0f);
	//Translate vertices to entity position
	const glm::vec2 position(entityX, entityY);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	//Translate vertices relative to the camera
	model = glm::translate(model, glm::vec3(cameraOffset, 0.0f));
	const glm::mat4 vertexTranformationMatrix = projection * model;

	// Set transformation matrix in the shader
	shader->setMat4("vertexTransformationMatrix", vertexTranformationMatrix);

	// Set entity size scaling vector
	glm::vec2 scaleVector(sizeX, sizeY);
	shader->setVec2("scaleVector", scaleVector);

	// Load Vertex Array Object, which automatically loads configuration of buffers
	glBindVertexArray(VAO);
	// Draw the data
	glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_SHORT, 0);

	return true;
}



// Upload the stored mesh data to the buffers located on the GPU
bool EntityMesh::bufferData()
{
	if (!hasMeshChanged) return true;
	if (!hasBuffers)
	{
		if (!setupBuffers()) return false;
	}

	// Bind VAO as the current
	glBindVertexArray(VAO);

	// Buffer data
	glBufferData(GL_ARRAY_BUFFER,         sizeof(Vertex)  * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int16_t) * indicies.size(), indicies.data(), GL_DYNAMIC_DRAW);

	//Unbind VAO
	glBindVertexArray(0);

	hasMeshChanged = false;
	if (!hasMesh) hasMesh = true;

	return true;
}



void EntityMesh::clearMesh()
{
	indicies.clear();
	vertices.clear();
	triangleCount = 0;
}



// Creates the GPU side buffer objects to store the mesh data, and also configures them to store the mesh
bool EntityMesh::setupBuffers()
{
	// Check if buffers already exist
	if (hasBuffers) return false;

	// Generate a vertex array object to store buffer objects and configurations
	glGenVertexArrays(1, &VAO);
	// Bind the VAO as the current one in order to save configurations and buffer objects
	glBindVertexArray(VAO);

	// Generate buffers for VBO and EBO
	GLuint buffers[2];
	glGenBuffers(2, buffers);
	// Assign the buffer locations
	EBO = buffers[0];
	VBO = buffers[1];
	// Set the buffers as the currently bound ones
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Configure the vertex attributes so that the buffer data is compatible with the shaders
	glVertexAttribPointer(0, 2, GL_SHORT        , GL_FALSE, sizeof(Vertex), (void*)(0));
	glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)(4));
	// Enable the vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Unbind the VAO so no accidental changes are made elsewhere
	glBindVertexArray(0);

	hasBuffers = true;

	return true;
}

#include "Player.h"



Player::Player()
{
	playerMesh.sizeX = 1.0f;
	playerMesh.sizeY = 2.0f;

	loadModel();
}



// Set the player entity's position to specified coordinates
void Player::setPosition(const float dX, const float dY)
{
	x = dX;
	y = dY;
}



bool Player::createMesh()
{
	playerMesh.createMesh();
	return true;
}



bool Player::draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset)
{
	if (!playerMesh.hasMesh)
	{
		if (!createMesh()) return false;
	}

	// Set the shader to be used as the current one
	shader->useShader();

	playerTexture.bindTexture();
	shader->setInt("entityTexture", 0);

	playerMesh.draw(shader, projection, cameraOffset, x, y);
	return true;
}



bool Player::loadModel()
{
	coordinate* modelVertices = new coordinate[4];

	modelVertices[0] = { -0.5f, -2.0f };
	modelVertices[1] = {  0.5f, -2.0f };
	modelVertices[2] = { -0.5f,  0.0f };
	modelVertices[3] = {  0.5f,  0.0f };

	setCollisionModel(modelVertices, 4);

	return true;
}



// Load the texture of the player
bool Player::loadTexture(const char* texturePath)
{
	return playerTexture.loadTexture(texturePath, GL_RGBA);
}

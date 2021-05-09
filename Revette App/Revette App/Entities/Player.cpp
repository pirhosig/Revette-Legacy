#include "Player.h"



Player::Player()
{
	playerMesh.sizeX = 0.49f;
	playerMesh.sizeY = 1.98f;

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

	modelVertices[0] = { -0.49f, -1.99f };
	modelVertices[1] = {  0.49f, -1.99f };
	modelVertices[2] = { -0.49f,  0.01f };
	modelVertices[3] = {  0.49f,  0.01f };

	setCollisionModel(modelVertices, 4);

	return true;
}



// Load the texture of the player
bool Player::loadTexture(const char* texturePath)
{
	return playerTexture.loadTexture(texturePath, GL_RGBA);
}

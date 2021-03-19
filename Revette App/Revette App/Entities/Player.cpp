#include "Player.h"



// Move the player entity about their current coordinates by the specified values
void Player::move(const float dX, const float dY)
{
	x += dX;
	y += dY;
}



// Set the player entity's position to specified coordinates
void Player::setPosition(const float dX, const float dY)
{
	x = dX;
	y = dY;
}



// Load the texture of the player
bool Player::loadTexture(const char* texturePath)
{
	return playerTexture.loadTexture(texturePath, GL_RGBA);
}

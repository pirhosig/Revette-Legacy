#include "Camera.h"



void Camera::setPosition(const float x, const float y)
{
	positionX = x;
	positionY = y;
}



glm::vec2 Camera::getPosition()
{
	glm::vec2 position(positionX, positionY);
	return position;
}



void Camera::translate(const float x, const float y)
{
	positionX += x;
	positionY += y;
}

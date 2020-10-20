#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	void setPosition(const float x, const float y);
	glm::vec2 getPosition();

	void translate(const float x, const float y);

	float positionX;
	float positionY;
};


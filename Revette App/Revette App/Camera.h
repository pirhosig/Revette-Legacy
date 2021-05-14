#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	void setPosition(const float x, const float y);
	glm::vec2 getPosition();

	void translate(const float x, const float y);

	float positionX {0.0f};
	float positionY {0.0f};
	float zoomFactor {40.0f};
};


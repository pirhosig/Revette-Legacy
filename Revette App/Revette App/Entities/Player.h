#pragma once
#include "Entity.h"
#include "../Rendering/Texture.h"


class Player : public Entity
{
public:
	void move(const float dX, const float dY);
	void setPosition(const float dX, const float dY);

	bool createMesh();
	bool draw();
	bool loadTexture(const char* texturePath);

private:
	Texture playerTexture;
};

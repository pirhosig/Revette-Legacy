#pragma once
#include "Entity.h"
#include "../Mesh.h"
#include "../Rendering/Texture.h"


class Player : public Entity
{
public:
	Player(const float xPos, const float yPos);

	void move(const float dX, const float dY);
	void setPosition(const float dX, const float dY);

	bool createMesh();
	bool draw();
	bool loadTexture(const char* texturePath);

private:
	Texture playerTexture;
	Mesh playerMesh;
};

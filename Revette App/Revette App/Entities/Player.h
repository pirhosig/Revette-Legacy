#pragma once
#include "Entity.h"

#include "../Meshes/EntityMesh.h"
#include "../Rendering/Texture.h"


class Player : public Entity
{
public:
	Player();

	void move(const float dX, const float dY);
	void setPosition(const float dX, const float dY);

	bool createMesh();
	bool draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset);
	bool loadTexture(const char* texturePath);

private:
	Texture playerTexture;
	EntityMesh playerMesh;
};

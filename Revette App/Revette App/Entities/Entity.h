#pragma once
#include <memory>
#include "../TileMap.h"



struct coordinate
{
	float x;
	float y;
};


class Entity
{
public:

	float x;
	float y;

	void move(TileMap& tileMap, double dX, double dY);

protected:

	Entity();

	void setCollisionModel(coordinate* model, unsigned int vertexCount);

	std::unique_ptr<coordinate[]> collisionModel;
	unsigned int collisionModelVertices;

private:
	bool tryStep(TileMap& tilemap, double newX, double newY);
};


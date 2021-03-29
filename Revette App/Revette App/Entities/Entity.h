#pragma once
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
	~Entity();

	void setCollisionModel(coordinate* model, unsigned int vertexCount);

	coordinate* collisionModel;
	unsigned int collisionModelVertices;

private:

	static unsigned int UEID;
	unsigned int getEUID();
};


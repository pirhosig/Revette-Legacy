#pragma once
#include "../Tile.h"



class WorldInterface
{
public:
	virtual ~WorldInterface() {};
	virtual Tile getTile(unsigned tileX, unsigned tileY) = 0;
	virtual void setTile(unsigned int tileX, unsigned int tileY, Tile tileType) = 0;
};
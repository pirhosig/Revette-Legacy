#pragma once
#include "../Tile.h"


enum class TilePlaceMode
{
	SET,
	REPLACE
};


class TilePlacementInfo
{
public:
	TilePlacementInfo(Tile tileData, TilePlaceMode tileMode, unsigned extraCount=0, Tile* extraData=nullptr);
	~TilePlacementInfo();

	Tile tile;
	TilePlaceMode mode;
	unsigned extraTileCount;
	Tile* extraTiles;
};

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
	TilePlacementInfo(Tile tileData, TilePlaceMode tileMode, int extraCount=0, Tile* extraData=nullptr);
	~TilePlacementInfo();

	Tile tile;
	TilePlaceMode mode;
	int extraTileCount;
	Tile* extraTiles;
};

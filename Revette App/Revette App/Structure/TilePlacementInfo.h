#pragma once
#include "../Tile.h"

#include <memory>



enum class TilePlaceMode
{
	SET,
	REPLACE
};


class TilePlacementInfo
{
public:
	TilePlacementInfo();
	TilePlacementInfo(Tile tileData, TilePlaceMode tileMode, unsigned extraCount=0, Tile* extraData=nullptr);

	Tile tile;
	TilePlaceMode mode;
	unsigned extraTileCount;
	std::unique_ptr<Tile[]> extraTiles;
};

#pragma once
#include "../Tile.h"

#include <memory>



enum class TilePlaceMode
{
	SET = 0,
	SKIP,
	CHANCE,
	REPLACE,
	CHANCE_REPLACE
};



class TilePlacementInfo
{
public:
	TilePlacementInfo();
	TilePlacementInfo(Tile tileData, TilePlaceMode tileMode, unsigned int chance, unsigned extraCount=0, Tile* extraData=nullptr);

	Tile tile;
	TilePlaceMode mode;
	unsigned int placementChance;
	unsigned int extraTileCount;
	std::unique_ptr<Tile[]> extraTiles;
};

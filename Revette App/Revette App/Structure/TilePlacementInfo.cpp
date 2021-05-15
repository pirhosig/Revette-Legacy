#include "TilePlacementInfo.h"



TilePlacementInfo::TilePlacementInfo()
{
	tile = { 0, 0 };
	mode = TilePlaceMode::SET;
	extraTileCount = 0;
}



TilePlacementInfo::TilePlacementInfo(Tile tileData, TilePlaceMode tileMode, unsigned int chance, unsigned extraCount, Tile* extraData)
{
	tile = tileData;
	mode = tileMode;
	placementChance = chance;
	extraTileCount = extraCount;
	extraTiles.reset(extraData);
}
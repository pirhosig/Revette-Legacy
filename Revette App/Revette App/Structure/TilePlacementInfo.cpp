#include "TilePlacementInfo.h"


TilePlacementInfo::TilePlacementInfo(Tile tileData, TilePlaceMode tileMode, int extraCount, Tile* extraData)
{
	tile = tileData;
	mode = tileMode;
	extraTileCount = extraCount;
	extraTiles = extraData;
}



TilePlacementInfo::~TilePlacementInfo()
{
	delete[] extraTiles;
}
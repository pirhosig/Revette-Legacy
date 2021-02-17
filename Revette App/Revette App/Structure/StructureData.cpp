#include "StructureData.h"



void StructureData::setData(unsigned x, unsigned y, int* data, TilePlacementInfo* tiles, int offsetX, int offsetY)
{
	// Set pallate data, which describes the types of tiles in "dataArray"
	tilePallate.reset(tiles);

	// Set data array to hold structure data
	dataArray.reset(data);

	xOffset = offsetX;
	yOffset = offsetY;
	sizeX = x;
	sizeY = y;

	hasData = true;
};



Tile StructureData::getTile(const int x, const int y, const Tile currentTile)
{
	// Return air if no data exists
	if (!hasData) return { 0, 0 };

	// Calculate the actual position of the data taking into account the offset values
	const unsigned locationX = x + xOffset;
	const unsigned locationY = y + yOffset;

	// Return air if the coordinates lie outside the data bounds
	if (locationX >= sizeX || locationY >= sizeY) return { 0, 0 };

	// Get the actual data location in the array
	unsigned tileLocation = locationY * sizeY + locationX;

	int tileIndex = dataArray[tileLocation];

	// Do stuff based on the placement mode
	TilePlacementInfo tileInfo = tilePallate[tileIndex];
	Tile returnTile;
	switch (tileInfo.mode)
	{
	case (TilePlaceMode::SET):
		returnTile = tileInfo.tile;
		break;
	default:
		returnTile = {0, 0};
		break;
	}

	return returnTile;
}


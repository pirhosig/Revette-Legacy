#pragma once
#include <memory>
#include "Tile.h"
#include "Structure/TilePlacementInfo.h"



class StructureData
{
public:
	void setData(unsigned x, unsigned y, int* data, TilePlacementInfo* tiles, int offsetX, int offsetY);
	Tile getTile(const int x, const int y, const Tile currentTile);

	unsigned sizeX;
	unsigned sizeY;
	int xOffset;
	int yOffset;
	
	bool hasData = false;

private:
	std::unique_ptr<TilePlacementInfo[]> tilePallate;
	std::unique_ptr<int[]> dataArray;
};


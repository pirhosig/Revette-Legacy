#pragma once
#include <memory>
#include "../Tile.h"
#include "../Structure/TilePlacementInfo.h"



class StructureData
{
public:
	bool loadDataFromFile(const char* filePath);
	void setData(unsigned x, unsigned y, unsigned* data, unsigned tileTypeCount, TilePlacementInfo* tiles, int offsetX, int offsetY);
	Tile getTile(const int x, const int y, const Tile currentTile);

	unsigned sizeX;
	unsigned sizeY;
	unsigned pallateSize;
	int xOffset;
	int yOffset;
	
	bool hasData = false;

private:
	std::unique_ptr<TilePlacementInfo[]> tilePallate;
	std::unique_ptr<unsigned[]> dataArray;
};


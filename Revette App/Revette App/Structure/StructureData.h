#pragma once
#include <memory>
#include "../Tile.h"
#include "../Structure/TilePlacementInfo.h"



class StructureData
{
public:
	StructureData();
	bool loadDataFromFile(const char* filePath);
	void setData(unsigned x, unsigned y, unsigned* data, unsigned tileTypeCount, TilePlacementInfo* tiles, int offsetX, int offsetY);
	Tile getTile(const int x, const int y, const Tile currentTile);

	unsigned sizeX;
	unsigned sizeY;
	int xOffset;
	int yOffset;
	unsigned pallateSize;
	
	bool hasData;

private:
	std::unique_ptr<TilePlacementInfo[]> tilePallate;
	std::unique_ptr<unsigned[]> dataArray;
};


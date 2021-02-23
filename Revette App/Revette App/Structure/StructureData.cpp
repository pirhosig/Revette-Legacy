#include "StructureData.h"

#include <fstream>

#include "../Logging/GlobalAppLog.h"



// Loads the data for a structure from a file. Currently loads from a plaintext file which could later be optimised.
bool StructureData::loadDataFromFile(const char* filePath)
{
	// Do not load any data if data already exists
	if (hasData)
	{
		GlobalAppLog.writeLog("Overwrote existing structure from file:", LOGMODE::INFO);
		GlobalAppLog.writeLog(filePath, LOGMODE::UNPREFIXED);
	}

	std::ifstream dataFile(filePath);
	
	// Load metadata
	unsigned x;
	unsigned y;
	int offsetX;
	int offsetY;
	unsigned sizePallate;
	
	// Read metadata from file
	dataFile
		>> x
		>> y
		>> offsetX
		>> offsetY
		>> sizePallate;

	// Set metadata
	sizeX = x;
	sizeY = y;
	xOffset = offsetX;
	yOffset = offsetY;
	pallateSize = sizePallate;

	// Create underlying data containers based on the size described in the metadata
	tilePallate = std::make_unique<TilePlacementInfo[]>(pallateSize);
	dataArray = std::make_unique<unsigned[]>(x * y);

	// Load tile pallate
	for (unsigned i = 0; i < pallateSize; ++i)
	{
		int tileType;
		int extraData;
		unsigned tilePlacementMode;
		unsigned extraTileCount;

		// Read from file
		dataFile
			>> tileType
			>> extraData
			>> tilePlacementMode
			>> extraTileCount;

		// Read in extra informational tiles if they exist
		Tile* extraTiles = nullptr;
		if (extraTileCount > 0)
		{
			extraTiles = new Tile[extraTileCount];
			for (unsigned j = 0; j < extraTileCount; ++j)
			{
				int eTileType;
				int eExtraData;

				dataFile
					>> eTileType
					>> eExtraData;

				extraTiles[j] = { eTileType, eExtraData };
			}
		}

		tilePallate[i] = TilePlacementInfo(
			Tile { tileType, extraData },
			static_cast<TilePlaceMode>(tilePlacementMode),
			extraTileCount,
			extraTiles
		);
	}

	// Load tile data
	for (unsigned i = 0; i < x * y; ++i)
	{
		unsigned tileIndex;
		// Read from file
		dataFile >> tileIndex;
		dataArray[i] = tileIndex;
	}

	hasData = true;

	return true;
};



void StructureData::setData(unsigned x, unsigned y, unsigned* data, unsigned tileTypeCount, TilePlacementInfo* tiles, int offsetX, int offsetY)
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
	const TilePlacementInfo& tileInfo = tilePallate[tileIndex];
	Tile returnTile;
	switch (tileInfo.mode)
	{
	case (TilePlaceMode::SET):
	{
		returnTile = tileInfo.tile;
		break;
	}
	case (TilePlaceMode::REPLACE):
	{
		bool replace = false;
		for (unsigned i = 0; i < tileInfo.extraTileCount; ++i)
		{
			if (tileInfo.extraTiles[i].type == currentTile.type)
			{
				replace = true;
				break;
			}
		}
		if (replace) returnTile = tileInfo.tile;
		else returnTile = currentTile;
		break;
	}
	default:
		returnTile = {0, 0};
		break;

	}

	return returnTile;
}


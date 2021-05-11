#include "Substructure.h"

using json = nlohmann::json;



Substructure::Substructure()
{
	sizeX = 0;
	sizeY = 0;
	xOffset = 0;
	yOffset = 0;
	pallateSize = 0;
}



void Substructure::loadSubstrucuture(nlohmann::json& SubstructureJSON)
{
	// Load metadata
	sizeX = SubstructureJSON.at("sizeX").get<unsigned int>();
	sizeY = SubstructureJSON.at("sizeY").get<unsigned int>();
	xOffset = SubstructureJSON.at("xOffset").get<int>();
	yOffset = SubstructureJSON.at("yOffset").get<int>();

	// Cache JSON object references to reduce lookups
	json& pallateTileJSON = SubstructureJSON.at("tilePallate");
	json& dataJSON = SubstructureJSON.at("data");

	// Load the tiles that make up the substructure
	pallateSize = static_cast<unsigned int>(pallateTileJSON.size());
	tilePallate = std::make_unique<TilePlacementInfo[]>(pallateSize);
	for (unsigned int i = 0; i < pallateSize; ++i)
	{
		// Keep JSON object references to avoid unecessary lookups
		json& currentTileJSON = pallateTileJSON.at(i);
		json& extraTilesJSON = currentTileJSON.at("extraTiles");

		// Read in tile info
		int tileType = currentTileJSON.at("type").get<int>();
		int extraData = currentTileJSON.at("extraData").get<int>();
		unsigned int tilePlacementMode = currentTileJSON.at("placementMode").get<unsigned int>();
		unsigned int extraTileCount = static_cast<unsigned int>(extraTilesJSON.size());

		// Read in extra tiles if they exist
		Tile* extraTiles = nullptr;
		if (extraTileCount > 0)
		{
			extraTiles = new Tile[extraTileCount];
			for (unsigned int j = 0; j < extraTileCount; ++j)
			{
				json& tileJSON = extraTilesJSON.at(j);
				int extraType = tileJSON.at("type").get<int>();
				int extraData = tileJSON.at("extraData").get<int>();

				extraTiles[i] = Tile(extraType, extraData);
			}
		}

		// Create a tile placement info object
		tilePallate[i] = TilePlacementInfo(
			Tile{ tileType, extraData },
			static_cast<TilePlaceMode>(tilePlacementMode),
			extraTileCount,
			extraTiles
		);
	}

	// Load the actual layout of the substructure
	unsigned int dataSize = static_cast<unsigned int>(dataJSON.size());
	dataArray = std::make_unique<unsigned int[]>(dataSize);
	for (unsigned int i = 0; i < dataSize; ++i)
	{
		dataArray[i] = dataJSON.at(i).get<unsigned int>();
	}
}



void Substructure::placeSubstructure(TileMap& tilemap, std::shared_ptr<WorldGenerator> worldGen, unsigned int& endX, unsigned int& endY, unsigned int x, unsigned int y)
{
	for (unsigned int i = 0; i < sizeX; ++i)
	{
		for (unsigned int j = 0; j < sizeY; ++j)
		{
			// Get the actual tile type at location
			const unsigned int tileIndex = j * sizeY + i;
			const TilePlacementInfo& tileInfo = tilePallate[dataArray[tileIndex]];

			unsigned int tileX = x + i + static_cast<unsigned int>(-xOffset);
			unsigned int tileY = y + j + static_cast<unsigned int>(-yOffset);

			// Execute tile placement dependant on placement type
			switch (tileInfo.mode)
			{
				case (TilePlaceMode::SET):
				{
					tilemap.setTile(tileX, tileY, tileInfo.tile);
					break;
				}
				case (TilePlaceMode::REPLACE):
				{
					Tile currentTile = tilemap.getTile(tileX, tileY);
					bool replace = false;
					for (unsigned int ii = 0; ii < tileInfo.extraTileCount; ++ii)
					{
						if (tileInfo.extraTiles[ii].type == currentTile.type)
						{
							replace = true;
							break;
						}
					}
					if (replace) tilemap.setTile(tileX, tileY, tileInfo.tile);
					break;
				}
				case (TilePlaceMode::SKIP):
					break;
				default:
					break;
			}
		}
	}

	endX = x + sizeX - 1;
	endY = y + sizeY - 1;
}

/*
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
	case (TilePlaceMode::SKIP):
	{
		returnTile = currentTile;
		break;
	}
	default:
		returnTile = {0, 0};
		break;
	}

	return returnTile;
}
*/
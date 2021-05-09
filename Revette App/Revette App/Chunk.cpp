#include "Chunk.h"
#include <algorithm>
#include "Logging/GlobalAppLog.h"


// Converts x and y coordinates within a chunk into tile location within the data array
unsigned int getTileArrayLocation(unsigned int xLoc, unsigned int yLoc)
{
	return xLoc + (yLoc << CHUNK_KEY_SHIFT);
}




Chunk::Chunk(unsigned x, unsigned y, std::shared_ptr<WorldGenerator>& generator)
	: terrainGenerator(generator),
	tileData(std::make_unique<Tile[]>(CHUNK_SIZE* CHUNK_SIZE)),
	tileMesh(std::make_unique<ChunkMesh>())
{
	chunkX = x;
	chunkY = y;
}



// Generates the basic ground shape in the chunk.
bool Chunk::generateChunk()
{
	unsigned chunkXOffset = chunkX * 32;
	unsigned chunkYOffset = chunkY * 32;

	// Loop through each column in the chunk and fill with blocks up to the height level
	for (unsigned x = 0; x < CHUNK_SIZE; ++x)
	{
		unsigned absoluteX = chunkXOffset + x;
		
		float heightOffsetNoiseValue = terrainGenerator->getHeightNoise(static_cast<float>(absoluteX));
		int heightOffsetValue = static_cast<int>(heightOffsetNoiseValue * 10.0f);
		unsigned groundHeight = GROUND_LEVEL - heightOffsetValue;

		// Proceed to next column if the entire column of the chunk is above the height level
		if ((chunkYOffset + CHUNK_SIZE) <= groundHeight) continue;
		
		unsigned terrainTopHeight = static_cast<unsigned>(std::max(0, (static_cast<int>(groundHeight) - static_cast<int>(chunkYOffset))));

		for (unsigned y = terrainTopHeight; y < CHUNK_SIZE; ++y)
		{
			unsigned absoluteY = chunkYOffset + y;
			float caveNoise = terrainGenerator->getCaveNoise(static_cast<float>(absoluteX), static_cast<float>(absoluteY));

			unsigned tileArrayLocation = getTileArrayLocation(x, y);

			if (absoluteY < groundHeight + 5)
			{
				if (caveNoise > 0.8f) tileData[tileArrayLocation] = { 8, 0 };
				else tileData[tileArrayLocation] = { 1, 0 };
			}
			else
			{
				if (caveNoise > 0.8f)
				{
					tileData[tileArrayLocation] = { 6, 0 };
				}
				else tileData[tileArrayLocation] = { 5, 0 };
			}
		}
	}

	dataHasChanged = true;
	return true;
}



// Gets the tile at location (tileX, tileY)
Tile Chunk::getChunkTile(unsigned tileX, unsigned tileY)
{
	unsigned tileLocation = getTileArrayLocation(tileX, tileY);
	return tileData[tileLocation];
}



// Sets the tile at location (tileX, tileY) to tile
void Chunk::setChunkTile(unsigned tileX, unsigned tileY, Tile tile)
{
	if ((tileX >= CHUNK_SIZE) || (tileY >= CHUNK_SIZE))
	{
		GlobalAppLog.writeLog("Invalid local tile placement.", LOGMODE::INFO);
		return;
	}

	unsigned tileLocation = getTileArrayLocation(tileX, tileY);
	tileData[tileLocation] = tile;

	dataHasChanged = true;
}



bool Chunk::draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset)
{
	if (dataHasChanged)
	{
		if (!generateMesh()) return false;
	}

	bool success = tileMesh->drawMesh(shader, projection, cameraOffset, static_cast<float>(chunkX * 32), static_cast<float>(chunkY * 32));

	return success;
}



bool Chunk::generateMesh()
{
	bool success = tileMesh->createMesh(tileData);

	dataHasChanged = false;

	return success;
}


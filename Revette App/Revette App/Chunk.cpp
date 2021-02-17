#include "Chunk.h"
#include <algorithm>
#include <iostream>


Chunk::Chunk(unsigned x, unsigned y, std::shared_ptr<WorldGenerator>& generator)
{
	chunkX = x;
	chunkY = y;
	tileData = std::make_unique<Tile[]>(CHUNK_SIZE * CHUNK_SIZE);
	tileMesh = std::make_unique<ChunkMesh>();

	terrainGenerator = generator;
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
		unsigned heightOffsetValue = static_cast<unsigned>(heightOffsetNoiseValue * 10.0f);
		unsigned groundHeight = GROUND_LEVEL - heightOffsetValue;

		// Proceed to next column if the entire column of the chunk is above the height level
		if ((chunkYOffset + CHUNK_SIZE - 1) <= groundHeight) continue;
		
		unsigned terrainTopHeight = static_cast<unsigned>(std::max(0, (static_cast<int>(groundHeight) - static_cast<int>(chunkYOffset))));

		for (unsigned y = terrainTopHeight; y < CHUNK_SIZE; ++y)
		{
			unsigned absoluteY = chunkYOffset + y;
			float caveNoise = terrainGenerator->getCaveNoise(absoluteX, absoluteY);

			unsigned tileArrayLocation = x + (y << CHUNK_KEY_SHIFT);

			if (absoluteY < groundHeight + 5) tileData[tileArrayLocation] = { 1, 0 };
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
Tile Chunk::getTile(unsigned tileX, unsigned tileY)
{
	unsigned tileLocation = tileX + (tileY << 5);
	return tileData[tileLocation];
}



// Sets the tile at location (tileX, tileY) to tile
bool Chunk::setTile(unsigned tileX, unsigned tileY, Tile tile)
{
	if (tileX >= CHUNK_SIZE || 0 > tileX) return false;
	if (tileY >= CHUNK_SIZE || 0 > tileY) return false;

	unsigned tileLocation = tileX + (tileY << 5);
	tileData[tileLocation] = tile;

	dataHasChanged = true;

	return true;
}



bool Chunk::draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset)
{
	if (dataHasChanged)
	{
		if (!generateMesh()) return false;
	}

	bool success = tileMesh->draw_mesh(shader, projection, cameraOffset, static_cast<float>(chunkX * 32), static_cast<float>(chunkY * 32));

	return success;
}



bool Chunk::generateMesh()
{
	bool success = tileMesh.get()->createMesh(tileData);

	dataHasChanged = false;

	return success;
}


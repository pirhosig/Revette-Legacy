#include "TileMap.h"

#include <algorithm>

#include "Logging/GlobalAppLog.h"
#include "Physics/Physics.h"



// Returns the key required to access the chunk with chunk coordinates (chunkX, chunkY) in the chunk map
unsigned int getChunkKey(unsigned int chunkX, unsigned int chunkY)
{
	return chunkX + (chunkY << TILEMAP_KEY_SHIFT);
}



// Checks if the chunk coordinates are within the bounds of the tilemap
inline bool chunkInTileMapBounds(unsigned int chunkX, unsigned int chunkY)
{
	return ((chunkX < TILEMAP_SIZE_X) && (chunkY < TILEMAP_SIZE_Y));
}




TileMap::TileMap()
	: terrainGenerator(std::make_shared<WorldGenerator>("./Assets/Generator.txt"))
{
	for (unsigned i = 0; i < TILEMAP_SIZE_X; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE_Y; ++j)
		{
			unsigned int chunk_key = getChunkKey(i, j);
			chunks.emplace( std::make_pair(chunk_key, std::make_unique<Chunk>(i, j, terrainGenerator)));
		}
	}
}



// Returns true if no collision would occur at the specified point, returns false if it collides
bool TileMap::collisionQuery(double x, double y)
{
	const auto tileX = static_cast<unsigned int>(x);
	const auto tileY = static_cast<unsigned int>(y);
	
	const Tile tile = getTile(tileX, tileY);

	if (TilePhysics[tile.type].isSolid) return true;
	else return false;
}



std::unique_ptr<Chunk>& TileMap::getChunk(unsigned x, unsigned y)
{
	unsigned int chunk_key = getChunkKey(x, y);
	std::unique_ptr<Chunk>& chunkReference = chunks[chunk_key];
	return chunkReference;
}



Tile TileMap::getTile(unsigned tileX, unsigned tileY)
{
	unsigned chunkX = tileX / CHUNK_SIZE;
	unsigned chunkY = tileY / CHUNK_SIZE;
	if (!chunkInTileMapBounds(chunkX, chunkY)) return { 0, 0 };
	chunkReference chunk = getChunk(chunkX, chunkY);

	unsigned localX = tileX - (chunkX * CHUNK_SIZE);
	unsigned localY = tileY - (chunkY * CHUNK_SIZE);
	Tile tileType = chunk->getChunkTile(localX, localY);
	return tileType;
}



void TileMap::setTile(unsigned int tileX, unsigned int tileY, Tile tileType)
{
	unsigned int chunkX = tileX / CHUNK_SIZE;
	unsigned int chunkY = tileY / CHUNK_SIZE;
	if (!chunkInTileMapBounds(chunkX, chunkY))
	{
		GlobalAppLog.writeLog("Invalid global tile placement.", LOGMODE::INFO);
		return;
	}
	chunkReference chunk = getChunk(chunkX, chunkY);

	unsigned int localX = tileX - (chunkX * CHUNK_SIZE);
	unsigned int localY = tileY - (chunkY * CHUNK_SIZE);

	chunk->setChunkTile(localX, localY, tileType);
}



bool TileMap::loadChunks()
{
	bool success = true;
	for (unsigned i = 0; i < TILEMAP_SIZE_X; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE_Y; ++j)
		{
			std::unique_ptr<Chunk>& chunk = getChunk(i, j);
			if (!chunk->generateChunk()) success = false;
		}
	}

	populateChunks();
	return success;
}



void TileMap::populateChunks()
{
	// Add structures and change tiles based on surrounding tiles
	for (unsigned x = 0; x < MAX_BLOCK_X; ++x)
	{
		const float xFloat = static_cast<float>(x);

		// Get height level
		const float heightOffsetNoiseValue = terrainGenerator->getHeightNoise(xFloat);
		const int heightOffsetValue = static_cast<int>(heightOffsetNoiseValue * 10.0f);
		const unsigned int groundHeight = GROUND_LEVEL - heightOffsetValue;
		const unsigned int plantBase = groundHeight - 1;

		// Make top block grass if it is a dirt block
		if (getTile(x, groundHeight).type == 1) setTile(x, groundHeight, { 2, 0 });

		// Add plants
		int foliageValue = terrainGenerator->getFoliageNoise(xFloat);
		std::map<int, unsigned int>::iterator it = terrainGenerator->plantNoiseThresholds.lower_bound(foliageValue);
		// Check if plant should be placed
		if (it != terrainGenerator->plantNoiseThresholds.end())
		{
			unsigned int plantIndex = it->second;
			terrainGenerator->plants[plantIndex].placeStructure((*this), terrainGenerator, x, plantBase);
		}
	}
}



bool TileMap::drawChunks(std::unique_ptr<Shader>& shader, TextureArray& tilemap, const glm::mat4& projection, const glm::vec2& cameraOffset)
{
	shader->useShader();
	tilemap.bindTexture();
	shader->setInt("tileAtlas", 0);

	bool success = true;
	for (unsigned i = 0; i < TILEMAP_SIZE_X; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE_Y; ++j)
		{
			std::unique_ptr<Chunk>& chunk = getChunk(i, j);
			if (!chunk->draw(shader, projection, cameraOffset)) success = false;
		}
	}
	return success;
}

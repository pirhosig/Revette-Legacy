#include "TileMap.h"

#include <algorithm>

#include "Physics/Physics.h"



const bool TREE_LEAVES[5][5] = {
	false, true,  true,  true,  false,
	true,  true,  true,  true,  true,
	true,  true,  true,  true,  true,
	true,  true,  true,  true,  true,
	false, true,  true,  true, false
};




TileMap::TileMap()
{
	for (unsigned i = 0; i < TILEMAP_SIZE; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE; ++j)
		{
			unsigned chunk_key = i + (j << TILEMAP_KEY_SHIFT);
			chunks.emplace( std::make_pair(chunk_key, std::make_unique<Chunk>(i, j, terrainGenerator)) );
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
	unsigned chunk_key = x + (y << TILEMAP_KEY_SHIFT);
	std::unique_ptr<Chunk>& chunkReference = chunks[chunk_key];
	return chunkReference;
}



Tile TileMap::getTile(unsigned tileX, unsigned tileY)
{
	unsigned chunkX = tileX / CHUNK_SIZE;
	unsigned chunkY = tileY / CHUNK_SIZE;
	unsigned localX = tileX - (chunkX * CHUNK_SIZE);
	unsigned localY = tileY - (chunkY * CHUNK_SIZE);
	chunkReference chunk = getChunk(chunkX, chunkY);
	Tile tileType = chunk->getTile(localX, localY);
	return tileType;
}



void TileMap::setTile(unsigned tileX, unsigned tileY, Tile tileType)
{
	unsigned chunkX = tileX / CHUNK_SIZE;
	unsigned chunkY = tileY / CHUNK_SIZE;
	if (chunkX < 0 || chunkY < 0 || chunkX >= TILEMAP_SIZE || chunkY >= TILEMAP_SIZE) return;
	unsigned localX = tileX - (chunkX * CHUNK_SIZE);
	unsigned localY = tileY - (chunkY * CHUNK_SIZE);

	chunkReference chunk = getChunk(chunkX, chunkY);
	chunk->setTile(localX, localY, tileType);
}



bool TileMap::loadChunks()
{
	bool success = true;
	for (unsigned i = 0; i < TILEMAP_SIZE; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE; ++j)
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
	// Add trees
	for (unsigned x = 0; x < (CHUNK_SIZE * TILEMAP_SIZE); ++x)
	{
		const float xFloat = static_cast<float>(x);
		// Get height level
		const float heightOffsetNoiseValue = terrainGenerator->getHeightNoise(xFloat);
		const unsigned heightOffsetValue = static_cast<unsigned>(heightOffsetNoiseValue * 10.0f);
		const unsigned groundHeight = GROUND_LEVEL - heightOffsetValue;

		// Make top block grass
		setTile(x, groundHeight, { 2, 0 });

		// Get foliage noise
		const float foliageNoise = terrainGenerator->getFoliageNoise(xFloat);
		const unsigned foliageValue = static_cast<unsigned>(foliageNoise * 100.0f);
		const float secondaryFoliageNoise = terrainGenerator->getSecondaryFoliageNoise(xFloat);
		const unsigned secondaryFoliageValue = static_cast<unsigned>(secondaryFoliageNoise * 100.0f);
		if (foliageValue > 95)
		{
			// Get tree height
			const unsigned treeHeight = static_cast<unsigned>(secondaryFoliageNoise * 10.0f) + 4;
			
			const unsigned treeBaseY = groundHeight - 1;
			const unsigned treeTopY  = groundHeight - treeHeight;
			
			// Make tree trunk
			for (unsigned y = 0; y < treeHeight; ++y)
			{
				setTile(x, treeBaseY - y, { 3, 0 });
			}
			unsigned leafGridX = x;
			unsigned leafGridY = treeTopY;

			for (int lX = -2; lX < 3; ++lX)
			{
				for (int lY = -2; lY < 3; ++lY)
				{
					Tile oldTile = getTile(leafGridX + lX, leafGridY + lY);
					Tile newTile = terrainGenerator->treeLeaves.getTile(lX, lY, oldTile);
					if (newTile.type != oldTile.type || newTile.extraValue != oldTile.extraValue)
					{
						setTile(leafGridX + lX, leafGridY + lY, newTile);
					}
				}
			}
		}
		else if (secondaryFoliageValue > 95)
		{
			setTile(x, groundHeight - 1, { 7, 0 });
		}
	}
}



bool TileMap::drawChunks(std::unique_ptr<Shader>& shader, TextureArray& tilemap, const glm::mat4& projection, const glm::vec2& cameraOffset)
{
	shader->useShader();
	tilemap.bindTexture();
	shader->setInt("tileAtlas", 0);

	bool success = true;
	for (unsigned i = 0; i < TILEMAP_SIZE; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE; ++j)
		{
			std::unique_ptr<Chunk>& chunk = getChunk(i, j);
			if (!chunk->draw(shader, projection, cameraOffset)) success = false;
		}
	}
	return success;
}

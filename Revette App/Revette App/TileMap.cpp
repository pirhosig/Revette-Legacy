#include "TileMap.h"

TileMap::TileMap()
{

	for (unsigned i = 0; i < TILEMAP_SIZE; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE; ++j)
		{
			unsigned chunk_key = i + (j << KEY_SHIFT);
			chunks.emplace( std::make_pair(chunk_key, std::make_unique<Chunk>(i, j)) );
		}
	}
}



std::unique_ptr<Chunk>& TileMap::getChunk(unsigned x, unsigned y)
{
	unsigned chunk_key = x + (y << KEY_SHIFT);
	return chunks[chunk_key];
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
	return success;
}



bool TileMap::drawChunks(std::unique_ptr<Shader>& shader)
{
	bool success = true;
	for (unsigned i = 0; i < TILEMAP_SIZE; ++i)
	{
		for (unsigned j = 0; j < TILEMAP_SIZE; ++j)
		{
			std::unique_ptr<Chunk>& chunk = getChunk(i, j);
			if (!chunk->draw(shader)) success = false;
		}
	}
	return success;
}

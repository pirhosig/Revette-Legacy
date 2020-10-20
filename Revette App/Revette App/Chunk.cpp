#include "Chunk.h"



Chunk::Chunk(unsigned x, unsigned y)
{
	chunkX = x;
	chunkY = y;
	tileData = std::make_unique<Tile[]>(CHUNK_SIZE * CHUNK_SIZE);
	tileMesh = std::make_unique<ChunkMesh>();
}



// Generates the chunk using the chunk generation algorithm
bool Chunk::generateChunk()
{
	tileData[0] = {1};
	tileData[33] = {1};
	tileData[64] = {1};
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

	bool success = tileMesh.get()->draw_mesh(shader, projection, cameraOffset, static_cast<float>(chunkX * 32), static_cast<float>(chunkY * 32));

	return success;
}



bool Chunk::generateMesh()
{
	bool success = tileMesh.get()->createMesh(tileData);

	dataHasChanged = false;

	return success;
}


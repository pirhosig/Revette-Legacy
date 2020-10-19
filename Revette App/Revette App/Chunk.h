#pragma once
#include <memory>
#include "Shader.h"
#include "ChunkMesh.h"
#include "Tile.h"


constexpr int CHUNK_SIZE { 32 };

class Chunk
{
public:
	Chunk(unsigned x, unsigned y);

	bool generateChunk();
	Tile getTile(unsigned tileX, unsigned tileY);
	bool setTile(unsigned tileX, unsigned tileY, Tile tile);

	bool generateMesh();
	bool draw(std::unique_ptr<Shader>& shader);

	unsigned chunkX;
	unsigned chunkY;

	std::unique_ptr<Tile[]> tileData;
	std::unique_ptr<ChunkMesh> tileMesh;
private:
	bool dataHasChanged = false;
};


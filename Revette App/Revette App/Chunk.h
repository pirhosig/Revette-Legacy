#pragma once
#include <memory>

#include "ChunkMesh.h"
#include "Shader.h"
#include "Tile.h"
#include "WorldGenerator.h"



constexpr int CHUNK_SIZE { 32 };

class Chunk
{
public:
	Chunk(unsigned x, unsigned y, std::shared_ptr<WorldGenerator>& generator);

	bool generateChunk();
	Tile getTile(unsigned tileX, unsigned tileY);
	bool setTile(unsigned tileX, unsigned tileY, Tile tile);

	bool generateMesh();
	bool draw(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset);

	unsigned chunkX;
	unsigned chunkY;

	std::unique_ptr<Tile[]> tileData;
	std::unique_ptr<ChunkMesh> tileMesh;
private:
	bool dataHasChanged = false;

	std::shared_ptr<WorldGenerator> terrainGenerator;
};


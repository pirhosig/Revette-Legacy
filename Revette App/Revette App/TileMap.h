#pragma once
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Chunk.h"
#include "WorldGenerator.h"

constexpr unsigned TILEMAP_SIZE = 16;
constexpr unsigned TILEMAP_KEY_SHIFT = 5;
constexpr unsigned MAX_BLOCK = TILEMAP_SIZE * CHUNK_SIZE;

typedef std::unique_ptr<Chunk>& chunkReference;



struct coordinates {
	unsigned x;
	unsigned y;
};

class TileMap
{
public:
	TileMap();

	std::unique_ptr<Chunk>& getChunk(unsigned x, unsigned y);
	Tile getTile(unsigned tileX, unsigned tileY);
	void setTile(unsigned tileX, unsigned tileY, Tile tileType);

	bool loadChunks();
	void populateChunks();

	bool drawChunks(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset);

	std::shared_ptr<WorldGenerator> terrainGenerator = std::make_shared<WorldGenerator>();
private:
	std::unordered_map<unsigned, std::unique_ptr<Chunk>> chunks;
};


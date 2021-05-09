#pragma once
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Chunk.h"
#include "Rendering/TextureArray.h"
#include "World Gen/WorldGenerator.h"

constexpr unsigned int TILEMAP_SIZE = 16U;
constexpr unsigned int TILEMAP_KEY_SHIFT = 5U;
constexpr unsigned int MAX_BLOCK = TILEMAP_SIZE * CHUNK_SIZE;

typedef std::unique_ptr<Chunk>& chunkReference;



struct coordinates {
	unsigned x;
	unsigned y;
};

class TileMap
{
public:
	TileMap();

	bool collisionQuery(double x, double y);
	std::unique_ptr<Chunk>& getChunk(unsigned x, unsigned y);
	Tile getTile(unsigned tileX, unsigned tileY);
	void setTile(unsigned int tileX, unsigned int tileY, Tile tileType);

	bool loadChunks();
	void populateChunks();

	bool drawChunks(std::unique_ptr<Shader>& shader, TextureArray& tilemap, const glm::mat4& projection, const glm::vec2& cameraOffset);

	std::shared_ptr<WorldGenerator> terrainGenerator = std::make_shared<WorldGenerator>();
private:
	std::unordered_map<unsigned, std::unique_ptr<Chunk>> chunks;
};


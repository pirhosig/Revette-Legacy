#pragma once
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

class Chunk;
#include "Constants.h"
#include "Chunk.h"
#include "Rendering/TextureArray.h"
#include "World Gen/WorldGenerator.h"

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

	std::shared_ptr<WorldGenerator> terrainGenerator;
private:
	std::unordered_map<unsigned int, std::unique_ptr<Chunk>> chunks;
};


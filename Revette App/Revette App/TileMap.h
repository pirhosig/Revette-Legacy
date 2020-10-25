#pragma once
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Chunk.h"
#include "WorldGenerator.h"

constexpr unsigned TILEMAP_SIZE = 4;
constexpr unsigned KEY_SHIFT = 4;

struct coordinates {
	unsigned x;
	unsigned y;
};

class TileMap
{
public:
	TileMap();
	std::unique_ptr<Chunk>& getChunk(unsigned x, unsigned y);
	bool loadChunks();
	bool drawChunks(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset);

	std::shared_ptr<WorldGenerator> terrainGenerator = std::make_shared<WorldGenerator>();
private:
	std::unordered_map<unsigned, std::unique_ptr<Chunk>> chunks;
};


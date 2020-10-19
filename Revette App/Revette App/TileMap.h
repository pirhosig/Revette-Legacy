#pragma once
#include <memory>
#include <unordered_map>
#include "Chunk.h"

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
	bool drawChunks(std::unique_ptr<Shader>& shader);

private:
	std::unordered_map<unsigned, std::unique_ptr<Chunk>> chunks;
};


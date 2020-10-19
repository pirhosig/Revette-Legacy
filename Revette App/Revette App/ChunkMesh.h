#pragma once
#include <memory>
#include "Mesh.h"
#include "Tile.h"

class ChunkMesh :
	public Mesh
{
public:
	void addSquare(unsigned short x, unsigned short y, unsigned texture);
	bool createMesh(const std::unique_ptr<Tile[]>& tiles);
	bool draw_mesh(std::unique_ptr<Shader>& shader);
};


#include "ChunkMesh.h"


unsigned short test_tile_indices[6]{
	0, 1, 2, 2, 3, 0
};



void ChunkMesh::addSquare(unsigned short x, unsigned short y, unsigned texture)
{
	Vertex test_tile_vertices[4]{
	0 + x, 0 + y, 0, 0, 0,
	1 + x, 0 + y, 0, 1, 0,
	1 + x, 1 + y, 0, 1, 1,
	0 + x, 1 + y, 0, 0, 1
	};

	addVertices(test_tile_vertices, 4, test_tile_indices, 6);
}



bool ChunkMesh::createMesh(const std::unique_ptr<Tile[]>& tiles)
{
	clearMesh();

	for (unsigned x = 0; x < 32; ++x)
	{
		for (unsigned y = 0; y < 32; ++y)
		{
			unsigned tile_location = (x + (y << 5));
			if (tiles[tile_location].type == 1)
			{
				addSquare(x, y, 1);
			}
		}
	}
	return true;
}



bool ChunkMesh::draw_mesh(std::unique_ptr<Shader>& shader)
{
	bool success = draw(shader);
	return success;
}


#include <glm/glm.hpp>

#include "ChunkMesh.h"


unsigned short test_tile_indices[6]{
	0, 1, 2, 2, 3, 0
};



// Adds a textured tile square to the mesh
void ChunkMesh::addSquare(unsigned short x, unsigned short y, unsigned short texture)
{
	unsigned short tileLowerX = 0 + x;
	unsigned short tileUpperX = 1 + x;
	unsigned short tileLowerY = 0 + y;
	unsigned short tileUpperY = 1 + y;

	unsigned char textureLowerX = 0;
	unsigned char textureUpperX = 128;
	unsigned char textureLowerY = 0;
	unsigned char textureUpperY = 128;
	
	Vertex test_tile_vertices[4] {
	tileLowerX, tileLowerY, texture, textureLowerX, textureLowerY,
	tileUpperX, tileLowerY, texture, textureUpperX, textureLowerY,
	tileUpperX, tileUpperY, texture, textureUpperX, textureUpperY,
	tileLowerX, tileUpperY, texture, textureLowerX, textureUpperY
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
			switch (tiles[tile_location].type)
			{
			case 1:
				addSquare(x, y, 0);
				break;
			case 2:
				addSquare(x, y, 1);
				break;
			case 3:
				addSquare(x, y, 2);
				break;
			default:
				break;
			}
			
		}
	}
	return true;
}


// Draws the buffered data
bool ChunkMesh::draw_mesh(std::unique_ptr<Shader>& shader, const glm::mat4& projection, const glm::vec2& cameraOffset, float chunkXPos, float chunkYPos)
{
	const glm::vec2 chunkOffset(chunkXPos, chunkYPos);
	bool success = draw(shader, projection, cameraOffset, chunkOffset);
	return success;
}


#pragma once
#include <memory>
#include "../Tile.h"
#include "StructurePlacementStep.h"
class Substructure;
#include "Substructure.h"
#include "TilePlacementInfo.h"



class StructureData
{
public:
	StructureData();
	void loadFromFile(const char* filePath);

	void placeStructure(TileMap& tilemap, std::shared_ptr<WorldGenerator> worldGen, unsigned int x, unsigned int y);

private:
	unsigned int substructureCount;
	std::unique_ptr<Substructure[]> substructures;

	unsigned int placementStepCount;
	std::unique_ptr<StructurePlacementStep[]> placementSteps;
};


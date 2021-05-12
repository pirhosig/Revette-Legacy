#pragma once
#include <memory>
#include "StructurePlacementInfo.h"
class Substructure;
#include "Substructure.h"
#include "TilePlacementInfo.h"
#include "../World Gen/WorldInterface.h"



class StructureData
{
public:
	StructureData();
	void loadFromFile(const char* filePath);

	void placeStructure(WorldInterface& world, std::shared_ptr<WorldGenerator> worldGen, unsigned int x, unsigned int y);

private:
	unsigned int substructureCount;
	std::unique_ptr<Substructure[]> substructures;

	unsigned int placementStepCount;
	std::unique_ptr<StructurePlacementStep[]> placementSteps;
};


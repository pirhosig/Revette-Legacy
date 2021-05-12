#include "StructureData.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../Logging/GlobalAppLog.h"


using json = nlohmann::json;


NLOHMANN_JSON_SERIALIZE_ENUM(
	StructureOffsetType,
	{
		{StructureOffsetType::NO_OFFSET,  "NO_OFFSET"},
		{StructureOffsetType::PLACE_LAST, "PLACE_LAST"},
		{StructureOffsetType::NO_OFFSET,  "OFFSET"},
		{StructureOffsetType::NO_OFFSET,  "LAST_OFFSET"}
	}
)
inline bool StructureHasLoopedOffset(StructurePlacementType offType)
{
	return static_cast<unsigned int>(offType) > 0;
}

NLOHMANN_JSON_SERIALIZE_ENUM(
	StructurePlacementType,
	{
		{StructurePlacementType::PLACE,          "PLACE"},
		{StructurePlacementType::PLACE_LOOPED_Y, "PLACE_LOOPED_Y"}
	}
)
inline bool StructureHasOffset(StructureOffsetType offType)
{
	return static_cast<unsigned int>(offType) > 1;
}




// Empty initialisation
StructureData::StructureData()
{
	substructureCount = 0;
	placementStepCount = 0;
};



// Load the appropriate data to build a structure from the JSON file that the path is provided to.
void StructureData::loadFromFile(const char* filePath)
{
	// Load and parse the JSON from the file
	json structureJSON;
	{
		std::ifstream structureFile(filePath);
		if (!structureFile) throw std::runtime_error("Error loading structure.");
		std::stringstream structureStream;
		structureStream << structureFile.rdbuf();
		structureStream >> structureJSON;
		structureFile.close();
	}

	json& substructureJSON = structureJSON.at("substructures");
	substructureCount = static_cast<unsigned int>(substructureJSON.size());
	substructures = std::make_unique<Substructure[]>(substructureCount);
	for (unsigned int i = 0; i < substructureCount; ++i)
	{
		substructures[i].loadSubstrucuture(substructureJSON.at(i));
	}

	json& placementJSON = structureJSON.at("placementSteps");
	placementStepCount = static_cast<unsigned int>(placementJSON.size());
	placementSteps = std::make_unique<StructurePlacementStep[]>(placementStepCount);
	for (unsigned int i = 0; i < placementStepCount; ++i)
	{
		json& stepJSON = placementJSON.at(i);

		placementSteps[i].offsetType = stepJSON.at(0).get<StructureOffsetType>();
		placementSteps[i].placementType = stepJSON.at(1).get<StructurePlacementType>();
		placementSteps[i].substructureIndex = stepJSON.at(2).get<unsigned int>();

		int stepArrayIndex = 3;
		if (StructureHasOffset(placementSteps[i].offsetType))
		{
			int xOffset = stepJSON.at(stepArrayIndex++).get<int>();
			int yOffset = stepJSON.at(stepArrayIndex++).get<int>();
			placementSteps[i].xOffset = xOffset;
			placementSteps[i].yOffset = yOffset;
		}
		if (StructureHasLoopedOffset(placementSteps[i].placementType))
		{
			int xLoopOffset = stepJSON.at(stepArrayIndex++).get<int>();
			int yLoopOffset = stepJSON.at(stepArrayIndex++).get<int>();
			placementSteps[i].loopXOffset = xLoopOffset;
			placementSteps[i].loopYOffset = yLoopOffset;
		}
	}
}



// Constructs the structure in the provided tilemap at the coordinates (x, y) using the loaded structure construction
// methods and the noise values provided in the worldGenerator object.
void StructureData::placeStructure(WorldInterface& world, std::shared_ptr<WorldGenerator> worldGen, unsigned int x, unsigned int y)
{
	// Store previous substructure end point for substructures that are placed relative to a common parent
	unsigned int lastX = x;
	unsigned int lastY = y;

	for (unsigned int i = 0; i < placementStepCount; ++i)
	{
		const unsigned int substructureIndex = placementSteps[i].substructureIndex;

		// Determine the coordinates to place the substructure at
		int placementX;
		int placementY;
		switch (placementSteps[i].offsetType)
		{
		case (StructureOffsetType::NO_OFFSET):
			placementX = x;
			placementY = y;
			break;
		case (StructureOffsetType::PLACE_LAST):
			placementX = lastX;
			placementY = lastY;
			break;
		case (StructureOffsetType::OFFSET):
			placementX = x + placementSteps[i].xOffset;
			placementY = y + placementSteps[i].yOffset;
			break;
		case (StructureOffsetType::LAST_OFFSET):
			placementX = lastX + placementSteps[i].xOffset;
			placementY = lastY + placementSteps[i].yOffset;
			break;
		default:
			placementX = x;
			placementY = y;
			break;
		}


		// Place the structure based on the placement method
		switch (placementSteps[i].placementType)
		{
		case (StructurePlacementType::PLACE):
			substructures[substructureIndex].placeSubstructure(
				world,
				worldGen,
				lastX,
				lastY,
				placementX,
				placementY
			);
			break;
		case (StructurePlacementType::PLACE_LOOPED_Y):
		{
			const float heightNoise = worldGen->getSecondaryFoliageNoise(static_cast<float>(x));
			const int height = static_cast<int>(heightNoise * 10.0f) + 4;
			for (int j = 0; j < height; ++j)
			{
				int newXOffset = placementSteps[i].loopXOffset * j;
				int newYOffset = placementSteps[i].loopYOffset * j;
				substructures[substructureIndex].placeSubstructure(
					world,
					worldGen,
					lastX,
					lastY,
					placementX + newXOffset,
					placementY + newYOffset
				);
			}
			break;
		}
		default:
			break;
		}
	}
}


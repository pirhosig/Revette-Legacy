#pragma once
#include <map>
#include "../Other/FastNoise.h"
class StructureData;
#include "../Structure/StructureData.h"


class WorldGenerator
{
public:
	WorldGenerator(const char* generatorFile);
	void setSeed(const int seedValue);

	float getCaveNoise(float xValue, float yValue);
	float getSecondaryFoliageNoise(float xValue);
	float getHeightNoise(float xValue);

	StructureData* const getPlant(unsigned int xValue);
	StructureData* const getBuildingType(unsigned int chunkX, unsigned int chunkY);

	void getBuildingOffset(unsigned int& xOffset, unsigned int& yOffset, unsigned int chunkX, unsigned int chunkY);

	unsigned int getTilePlacementNoise(unsigned int xValue, unsigned int yValue);

private:
	unsigned int plantCount;
	std::unique_ptr<StructureData[]> plants;
	std::map<int, unsigned int> plantNoiseThresholds;

	unsigned int buildingCount;
	std::unique_ptr<StructureData[]> buildings;
	std::map<int, unsigned int> buildingNoiseThresholds;

	FastNoise noiseBuilding;
	FastNoise noiseBuildingOffset;
	FastNoise noiseCave;
	FastNoise noiseFoliage;
	FastNoise noiseHeight;
	FastNoise noiseTilePlacement;

	int seed;
};


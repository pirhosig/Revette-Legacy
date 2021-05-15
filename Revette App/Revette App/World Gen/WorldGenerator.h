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
	unsigned int getTilePlacementNoise(unsigned int xValue, unsigned int yValue);

private:
	unsigned int plantCount;
	std::unique_ptr<StructureData[]> plants;
	std::map<int, unsigned int> plantNoiseThresholds;

	FastNoise noiseCave;
	FastNoise noiseFoliage;
	FastNoise noiseHeight;
	FastNoise noiseTilePlacement;

	int seed;
};


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
	int getFoliageNoise(float xValue);
	float getSecondaryFoliageNoise(float xValue);
	float getHeightNoise(float xValue);

	unsigned int plantCount;
	std::unique_ptr<StructureData[]> plants;
	std::map<int, unsigned int> plantNoiseThresholds;

private:
	FastNoise noiseCave;
	FastNoise noiseFoliage;
	FastNoise noiseHeight;

	int seed;
};


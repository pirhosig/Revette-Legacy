#pragma once
#include "../Other/FastNoise.h"
#include "../Structure/StructureData.h"


class WorldGenerator
{
public:
	WorldGenerator();
	void setSeed(const int seedValue);

	float getCaveNoise(float xValue, float yValue);
	float getFoliageNoise(float xValue);
	float getSecondaryFoliageNoise(float xValue);
	float getHeightNoise(float xValue);
	
	StructureData treeLeaves;

private:
	FastNoise noiseCave;
	FastNoise noiseFoliage;
	FastNoise noiseSecondaryFoliage;
	FastNoise noiseHeight;

	int seed;
};


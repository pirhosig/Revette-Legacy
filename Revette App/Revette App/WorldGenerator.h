#pragma once
#include "Other/FastNoise.h"



class WorldGenerator
{
public:
	WorldGenerator();
	float getCaveNoise(float xValue, float yValue);
	float getFoliageNoise(float xValue);
	float getSecondaryFoliageNoise(float xValue);
	float getHeightNoise(float xValue);
private:
	FastNoise noiseCave;
	FastNoise noiseFoliage;
	FastNoise noiseSecondaryFoliage;
	FastNoise noiseHeight;

	int seed = 421;
};


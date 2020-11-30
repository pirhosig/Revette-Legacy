#pragma once
#include "Other/FastNoise.h"



class WorldGenerator
{
public:
	WorldGenerator();
	float getCaveNoise(float xValue, float yValue);
	float getFoliageNoise(float xValue);
	float getHeightNoise(float xValue);
private:
	FastNoise noiseCave;
	FastNoise noiseFoliage;
	FastNoise noiseHeight;

	int seed = 420;
};


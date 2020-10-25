#pragma once
#include "Other/FastNoise.h"



class WorldGenerator
{
public:
	WorldGenerator();
	unsigned getNoiseHeight(float xValue);
private:
	FastNoise noiseHeight;

	float yScale = 5.0f;
	float xScale = 0.5f;

	int seed = 420;
};


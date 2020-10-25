#include "WorldGenerator.h"



WorldGenerator::WorldGenerator()
{
	// Set noise types
	noiseHeight.SetNoiseType(FastNoise::Perlin);
	
	// Set seed values
	noiseHeight.SetSeed(seed);
}



unsigned WorldGenerator::getNoiseHeight(float xValue)
{
	float scaledXValue = xValue / xScale;

	float rawNoise = noiseHeight.GetPerlin(scaledXValue, 0.0f);
	float normalisedNoise = (rawNoise + 1) / 2;
	unsigned processedNoise = static_cast<unsigned>(normalisedNoise * 8);
	return processedNoise;
}
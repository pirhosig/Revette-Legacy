#include "WorldGenerator.h"



WorldGenerator::WorldGenerator()
{
	// Set noise types
	noiseCave.SetNoiseType(FastNoise::Simplex);
	noiseFoliage.SetNoiseType(FastNoise::WhiteNoise);
	noiseHeight.SetNoiseType(FastNoise::SimplexFractal);

	// Set seed values
	noiseCave.SetSeed(seed);
	noiseFoliage.SetSeed(seed);
	noiseHeight.SetSeed(seed + 1);
}



float WorldGenerator::getCaveNoise(float xValue, float yValue)
{
	constexpr float caveNoiseScale = 0.5f;

	float scaledXValue = xValue / caveNoiseScale;
	float scaledYValue = yValue / caveNoiseScale;
	float rawNoise = noiseCave.GetSimplex(scaledXValue, scaledYValue);
	float normalisedNoise = (rawNoise + 1.0f) / 2.0f;
	return normalisedNoise;
}



float WorldGenerator::getFoliageNoise(float xValue)
{

	float rawNoise = noiseFoliage.GetWhiteNoise(xValue, 0.0f);
	float normalisedNoise = (rawNoise + 1.0f) / 2.0f;
	return normalisedNoise;
}



// Returns the height offset value for a given x value.
// Returns a value between -1.0f and 1.0f
float WorldGenerator::getHeightNoise(float xValue)
{
	constexpr float heightNoiseXScale = 0.75f;

	float scaledXValue = xValue / heightNoiseXScale;
	float rawNoise = noiseHeight.GetSimplexFractal(scaledXValue, 0.0f);
	return rawNoise;
}



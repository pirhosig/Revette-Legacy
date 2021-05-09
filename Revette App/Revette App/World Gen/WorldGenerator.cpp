#include "WorldGenerator.h"
#include "../Logging/GlobalAppLog.h"



WorldGenerator::WorldGenerator()
{
	// Set noise types
	noiseCave.SetNoiseType(FastNoise::SimplexFractal);
	noiseFoliage.SetNoiseType(FastNoise::WhiteNoise);
	noiseSecondaryFoliage.SetNoiseType(FastNoise::WhiteNoise);
	noiseHeight.SetNoiseType(FastNoise::SimplexFractal);

	// Set seed values
	setSeed(420);

	//Set noise octaves
	noiseCave.SetFractalOctaves(4);

	if (!treeLeaves.loadDataFromFile("./Assets/Structures/leaves.txt"))
	{
		GlobalAppLog.writeLog("Error loading trees", LOGMODE::ERROR);
	}
}



// Sets the seed values for all noise functions
void WorldGenerator::setSeed(const int seedValue)
{
	seed = seedValue;
	noiseCave.SetSeed(seedValue);
	noiseFoliage.SetSeed(seedValue);
	noiseSecondaryFoliage.SetSeed(seedValue + 0x12);
	noiseHeight.SetSeed(seedValue + 0xAF);
}



float WorldGenerator::getCaveNoise(float xValue, float yValue)
{
	constexpr float caveNoiseScale = 0.4f;

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



float WorldGenerator::getSecondaryFoliageNoise(float xValue)
{
	float rawNoise = noiseSecondaryFoliage.GetWhiteNoise(xValue, 0.0f);
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



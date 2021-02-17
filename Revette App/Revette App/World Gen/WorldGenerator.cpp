#include "WorldGenerator.h"



WorldGenerator::WorldGenerator()
{
	// Set noise types
	noiseCave.SetNoiseType(FastNoise::SimplexFractal);
	noiseFoliage.SetNoiseType(FastNoise::WhiteNoise);
	noiseSecondaryFoliage.SetNoiseType(FastNoise::WhiteNoise);
	noiseHeight.SetNoiseType(FastNoise::SimplexFractal);

	// Set seed values
	noiseCave.SetSeed(seed);
	noiseFoliage.SetSeed(seed);
	noiseSecondaryFoliage.SetSeed(seed + 0x12);
	noiseHeight.SetSeed(seed + 0xAF);

	// VERY HIGHLY EXTREMELY TEMPORARY
	int* LEAVES = new int[25]
	{
		0, 1, 1, 1, 0,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		0, 1, 1, 1, 0
	};

	Tile* LOG = new Tile[1];
	LOG[0].type = 0;
	LOG[0].extraValue = 0;

	TilePlacementInfo* LEAFPALLATE = new TilePlacementInfo[2]
	{
		TilePlacementInfo({0, 0}, TilePlaceMode::SET),
		TilePlacementInfo({4, 0}, TilePlaceMode::REPLACE, 1, LOG)
	};

	treeLeaves.setData(5, 5, LEAVES, LEAFPALLATE, 2, 2);
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



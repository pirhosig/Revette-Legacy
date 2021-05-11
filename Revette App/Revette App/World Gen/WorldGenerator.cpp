#include "WorldGenerator.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <json.hpp>

#include "../Logging/GlobalAppLog.h"



// Makes code shorter and more understandable
using json = nlohmann::json;



WorldGenerator::WorldGenerator(const char* generatorFile)
{
	try
	{
		// Load JSON file and parse it
		json settingsJSON;
		{
			std::ifstream settingsJSONFile(generatorFile);
			if (!settingsJSONFile) throw std::runtime_error("Error reading generator settings.");
			std::stringstream settingsJSONStream;
			settingsJSONStream << settingsJSONFile.rdbuf();
			settingsJSONStream >> settingsJSON;
			settingsJSONFile.close();
		}

		// Load settings

		const int seed = settingsJSON.at("seed").get<int>();
		setSeed(seed);

		// Set noise types
		noiseCave.SetNoiseType(FastNoise::SimplexFractal);
		noiseFoliage.SetNoiseType(FastNoise::WhiteNoise);
		noiseSecondaryFoliage.SetNoiseType(FastNoise::WhiteNoise);
		noiseHeight.SetNoiseType(FastNoise::SimplexFractal);


		//Set noise octaves
		noiseCave.SetFractalOctaves(4);

		// This is very temporary
		plantCount = static_cast<unsigned int>(settingsJSON.at("plants").size());
		plants = std::make_unique<StructureData[]>(plantCount);
		for (unsigned int i = 0; i < plantCount; ++i)
		{
			std::string structureFile = settingsJSON.at("plants").at(i).get<std::string>();
			plants[i].loadFromFile(structureFile.c_str());
		}
	}
	catch (std::exception)
	{
		GlobalAppLog.writeLog("Error loading and parsing generation settings JSON file", LOGMODE::ERROR);
		throw;
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



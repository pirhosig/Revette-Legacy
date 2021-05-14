#include "WorldGenerator.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <json.hpp>
#include "../Constants.h"
#include "../Logging/GlobalAppLog.h"



// Makes code shorter and more understandable
using json = nlohmann::json;



// Normalizes a float that has a value within the range -1.0f to 1.0f
inline float normalize(const float x)
{
	return (x + 1.0f) / 2.0f;
}



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
		noiseHeight.SetNoiseType(FastNoise::SimplexFractal);


		//Set noise octaves
		noiseCave.SetFractalOctaves(4);

		json& plantsJSON = settingsJSON.at("plants");
		plantCount = static_cast<unsigned int>(plantsJSON.size());
		plants = std::make_unique<StructureData[]>(plantCount);

		int currentThreshold = -1;

		for (unsigned int i = 0; i < plantCount; ++i)
		{
			// Get JSON elements
			json& plantJSON = plantsJSON.at(i);
			std::string structureFile = plantJSON.at("file").get<std::string>();
			float spawnChance = plantJSON.at("spawnChance").get<float>();

			// Load plant from file
			plants[i].loadFromFile(structureFile.c_str());

			// Add plant with spawn rate to the plant pool
			int spawnChanceRange = static_cast<int>(spawnChance * 10.0f);
			currentThreshold += spawnChanceRange;
			plantNoiseThresholds[currentThreshold] = i;
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
	// Simplex noise functions
	noiseCave.SetSeed(seedValue);
	noiseHeight.SetSeed(seedValue + 0xAF);
	// White noise functions
	noiseFoliage.SetSeed(seedValue);
}



float WorldGenerator::getCaveNoise(float xValue, float yValue)
{
	constexpr float caveNoiseScale = 0.4f;

	float scaledXValue = xValue / caveNoiseScale;
	float scaledYValue = yValue / caveNoiseScale;
	float rawNoise = noiseCave.GetSimplex(scaledXValue, scaledYValue);
	float normalised = normalize(rawNoise);
	return normalised;
}



int WorldGenerator::getFoliageNoise(float xValue)
{
	float rawNoise = noiseFoliage.GetWhiteNoise(xValue, 0.0f);
	float scaledNoise = normalize(rawNoise) * 1000.0f;
	return static_cast<int>(scaledNoise);
}



float WorldGenerator::getSecondaryFoliageNoise(float xValue)
{
	float rawNoise = noiseFoliage.GetWhiteNoise(xValue, 1.0f);
	return normalize(rawNoise);
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



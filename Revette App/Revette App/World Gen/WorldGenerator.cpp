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
		noiseTilePlacement.SetNoiseType(FastNoise::WhiteNoise);

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
	noiseHeight.SetSeed(seedValue + 0xAE);
	// White noise functions
	noiseFoliage.SetSeed(seedValue);
	noiseTilePlacement.SetSeed(seedValue + 0xAF);
}



float WorldGenerator::getCaveNoise(float xValue, float yValue)
{
	constexpr float caveNoiseScale = 0.4f;

	float scaledXValue = xValue / caveNoiseScale;
	float scaledYValue = yValue / caveNoiseScale;
	float rawNoise = noiseCave.GetSimplexFractal(scaledXValue, scaledYValue);
	float normalised = normalize(rawNoise);
	return normalised;
}



float WorldGenerator::getSecondaryFoliageNoise(float xValue)
{
	float rawNoise = noiseFoliage.GetWhiteNoise(xValue, 5.0f);
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



StructureData* const WorldGenerator::getPlant(unsigned int xValue)
{
	float rawNoise = noiseFoliage.GetWhiteNoiseInt(static_cast<int>(xValue), 0);
	int plantNoise = static_cast<int>(normalize(rawNoise) * 1000.0f);
	std::map<int, unsigned int>::iterator it = plantNoiseThresholds.lower_bound(plantNoise);
	// Return if no plant matches
	if (it == plantNoiseThresholds.end()) return nullptr;

	// Get index of plant
	unsigned int plantIndex = it->second;
	// Return a pointer to the plant structure object
	return &(plants[plantIndex]);
}



// A noise function for determining random factors during tile placement of structures
unsigned int WorldGenerator::getTilePlacementNoise(unsigned int xValue, unsigned int yValue)
{
	int xInt = static_cast<int>(xValue);
	int yInt = static_cast<int>(yValue);
	float rawNoise = noiseTilePlacement.GetWhiteNoiseInt(xInt, yInt);
	unsigned int intNoise = static_cast<unsigned int>(normalize(rawNoise) * 1000.0f);
	return intNoise;
}



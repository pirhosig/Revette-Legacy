#pragma once
#include "TilePlacementInfo.h"
#include <memory>
#include <json.hpp>
class WorldGenerator;
#include "../World Gen/WorldGenerator.h"
#include "../World Gen/WorldInterface.h"



class Substructure
{
public:
	Substructure();
	void loadSubstrucuture(nlohmann::json& SubstructureJSON);

	void placeSubstructure(WorldInterface& world, std::shared_ptr<WorldGenerator> worldGen, unsigned int& endX, unsigned int& endY, unsigned int x, unsigned int y);

private:
	unsigned int sizeX;
	unsigned int sizeY;
	int xOffset;
	int yOffset;
	unsigned pallateSize;

	std::unique_ptr<TilePlacementInfo[]> tilePallate;
	std::unique_ptr<unsigned int[]> dataArray;
};
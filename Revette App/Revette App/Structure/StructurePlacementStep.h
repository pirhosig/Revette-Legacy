#pragma once


enum class StructureOffsetType
{
	NO_OFFSET,
	PLACE_LAST,
	OFFSET,
	LAST_OFFSET
};


enum class StructurePlacementType
{
	PLACE,
	PLACE_LOOPED_Y
};



struct StructurePlacementStep
{
	StructureOffsetType offsetType;
	StructurePlacementType placementType;
	unsigned int substructureIndex = 0;
	int xOffset = 0;
	int yOffset = 0;
	int loopXOffset = 0;
	int loopYOffset = 0;
};
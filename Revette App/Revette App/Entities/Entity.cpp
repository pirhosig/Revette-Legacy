#include "Entity.h"



Entity::Entity(const float xPos, const float yPos)
{
	x = xPos;
	y = yPos;
};



unsigned int Entity::UEID = 0;
unsigned int Entity::getEUID()
{
	return UEID++;
};


#include "Entity.h"



Entity::Entity()
{
	x = 0.0f;
	y = 0.0f;
};



unsigned int Entity::UEID = 0;
unsigned int Entity::getEUID()
{
	return UEID++;
};


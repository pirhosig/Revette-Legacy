#include "Entity.h"

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>



Entity::Entity()
{
	x = 0.0f;
	y = 0.0f;

	collisionModel = nullptr;
	collisionModelVertices = 0;
};



Entity::~Entity()
{
	delete[] collisionModel;
}



// Move the entity with collision by the specified amounts
void Entity::move(TileMap& tileMap, double dX, double dY)
{
	// Temporary higher precision coordinates for collision
	glm::dvec2 position(x, y);

	glm::dvec2 movementVector(dX, dY);
	const double largestComponent = std::max(std::abs(dX), std::abs(dY));
	const glm::dvec2 loopVector = movementVector / largestComponent;

	bool collided = false;
	// Loop through collision moving by "loopVector" each time in x and y axis
	while (glm::length(movementVector) > glm::length(loopVector))
	{
		movementVector = movementVector - loopVector;
		glm::dvec2 newPosition = position + loopVector;
		bool canMove = true;
		for (unsigned int i = 0; i < collisionModelVertices; ++i)
		{
			coordinate modelVertex = collisionModel[i];
			double collideX = newPosition.x + modelVertex.x;
			double collideY = newPosition.y + modelVertex.y;
			bool wouldCollide = tileMap.collisionQuery(collideX, collideY);
			if (wouldCollide) canMove = false;
		}

		// Only move if no collisions occured
		if (canMove)
		{
			position = newPosition;
		}
		else
		{
			collided = true;
			break;
		}
	}

	// Try to move to final coordinates
	if (glm::length(movementVector) > 0)
	{
		glm::dvec2 newPosition = position + movementVector;
		bool canMove = true;
		for (unsigned int i = 0; i < collisionModelVertices; ++i)
		{
			coordinate modelVertex = collisionModel[i];
			double collideX = newPosition.x + modelVertex.x;
			double collideY = newPosition.y + modelVertex.y;
			bool wouldCollide = tileMap.collisionQuery(collideX, collideY);
			if (wouldCollide) canMove = false;
		}

		// Only move if no collisions occured
		if (canMove)
		{
			position = newPosition;
		}
	}

	x = static_cast<float>(position.x);
	y = static_cast<float>(position.y);
}



void Entity::setCollisionModel(coordinate* model, unsigned int vertexCount)
{
	// Free previous model data if not empty
	if (collisionModel != nullptr) delete[] collisionModel;

	collisionModel = model;
	collisionModelVertices = vertexCount;
}



unsigned int Entity::UEID = 0;
unsigned int Entity::getEUID()
{
	return UEID++;
};


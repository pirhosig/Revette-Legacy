#include "Entity.h"

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>

#include "../Logging/GlobalAppLog.h"



Entity::Entity()
{
	x = 0.0f;
	y = 0.0f;

	collisionModel = nullptr;
	collisionModelVertices = 0;
};



bool tryMove(TileMap& tileMap, glm::dvec2 newPosition)
{
	return false;
}



// Move the entity with collision by the specified amounts
void Entity::move(TileMap& tileMap, double dX, double dY)
{
	// Temporary higher precision coordinates for collision
	double currentX = x;
	double currentY = y;

	const double largestComponent = std::max(std::abs(dX), std::abs(dY));
	const double loopX = dX / largestComponent;
	const double loopY = dY / largestComponent;
	const double squaredLoopLength = (loopX * loopX) + (loopY * loopY);

	bool collidedX = false;
	bool collidedY = false;
	// Loop through collision moving by "loopVector" each time in x and y axis
	while (((dX * dX) + (dY * dY)) >= squaredLoopLength)
	{
		if (!collidedX && !collidedY)
		{
			double newX = currentX + loopX;
			double newY = currentY + loopY;
			if (!tryStep(tileMap, newX, newY))
			{
				currentX = newX;
				currentY = newY;
				dX -= loopX;
				dY -= loopY;
			}
			else
			{
				collidedX = tryStep(tileMap, newX, currentY);
				collidedY = tryStep(tileMap, currentX, newY);
			}
		}
		else
		{
			if (collidedX && collidedY) break;
			if (!collidedX)
			{
				double newX = currentX + loopX;
				if (!tryStep(tileMap, newX, currentY))
				{
					currentX = newX;
					dX -= loopX;
				}
				else collidedX = true;
			}
			if (!collidedY)
			{
				double newY = currentY + loopY;
				if (!tryStep(tileMap, currentX, newY))
				{
					currentY = newY;
					dY -= loopY;
				}
				else collidedY = true;
			}
		}
	}

	// Try to move to final coordinates
	if (!collidedX && !collidedY)
	{
		double newX = currentX + dX;
		double newY = currentY + dY;
		if (!tryStep(tileMap, newX, newY))
		{
			currentX = newX;
			currentY = newY;
			dX = 0;
			dY = 0;
		}
		else
		{
			// Try to move in either x or y direction
			if (!tryStep(tileMap, newX, currentY))
			{
				currentX = newX;
				dX = 0;
			}
			else if (!tryStep(tileMap, currentX, newY))
			{
				currentY = newY;
				dY = 0;
			}
		}
	}
	else
	{
		if (!collidedX)
		{
			double newX = currentX + dX;
			if (!tryStep(tileMap, newX, currentY))
			{
				currentX = newX;
				dX = 0;
			}
		}
		if (!collidedY)
		{
			double newY = currentY + dY;
			if (!tryStep(tileMap, currentX, newY))
			{
				currentY = newY;
				dY = 0;
			}
		}
	}

	// Check if any movement remains
	if ((std::abs(dX) > 0) || (std::abs(dY) > 0))
	{
		double axisX = 0.0f;
		if (dX < 0)
		{
			double minX = currentX + collisionModel[0].x;
			axisX = std::max(std::floor(minX) - minX, dX);
		}
		else if (dX > 0)
		{
			double maxX = currentX + collisionModel[1].x;
			axisX = std::min(std::ceil(maxX) - maxX, dX);
		}
		double axisY = 0.0f;
		if (dY < 0)
		{
			double minY = currentY + collisionModel[0].y;
			axisY = std::max(std::floor(minY) - minY, dY);
		}
		else if (dY > 0)
		{
			double maxY = currentY + collisionModel[1].y;
			axisY = std::min(std::ceil(maxY) - maxY, dY);
		}

		double newX = currentX + axisX;
		double newY = currentY + axisY;
		if (!tryStep(tileMap, newX, newY))
		{
			currentX = newX;
			currentY = newY;
		}
	}

	x = static_cast<float>(currentX);
	y = static_cast<float>(currentY);
}



void Entity::setCollisionModel(coordinate* model, unsigned int vertexCount)
{
	// Free previous model data if not empty
	collisionModel.reset(model);
	collisionModelVertices = vertexCount;
}



bool Entity::tryStep(TileMap& tilemap, double newX, double newY)
{
	double lowerX = newX + collisionModel[0].x;
	double lowerY = newY + collisionModel[0].y;
	double upperX = newX + collisionModel[1].x;
	double upperY = newY + collisionModel[1].y;
	return tilemap.collisionQuery(lowerX, lowerY, upperX, upperY);
}



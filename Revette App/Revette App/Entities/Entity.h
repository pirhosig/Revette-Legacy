#pragma once
class Entity
{
public:
	Entity(const float xPos, const float yPos);

	float x;
	float y;

private:
	static unsigned int UEID;
	unsigned int getEUID();
};


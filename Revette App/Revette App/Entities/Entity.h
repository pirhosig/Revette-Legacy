#pragma once
class Entity
{
public:
	Entity();

	float x;
	float y;

private:
	static unsigned int UEID;
	unsigned int getEUID();
};


#pragma once

struct Tile
{
	int type;
	int extraValue;

	Tile()
	{
		type = 0;
		extraValue = 0;
	}

	Tile(int tileType, int tileExtraValue)
	{
		type = tileType;
		extraValue = tileExtraValue;
	}

	friend bool operator==(const Tile& a, const Tile& b)
	{
		if (a.type == b.type && a.extraValue == b.extraValue) return true;
		else return false;
	}
};
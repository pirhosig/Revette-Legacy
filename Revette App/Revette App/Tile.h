#pragma once

struct Tile
{
	int type;
	int extraValue;

	friend bool operator==(const Tile& a, const Tile& b)
	{
		if (a.type == b.type && a.extraValue == b.extraValue) return true;
		else return false;
	}
};
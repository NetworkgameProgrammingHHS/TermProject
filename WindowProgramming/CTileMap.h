#pragma once
#include "CObject.h"

class CTile : public CObject
{

};

class CTileMap : public CTile
{
public:
	CTileMap(const string& Filename);
	virtual ~CTileMap();

private:
	vector<char> m_vTile;
};

// 0: Nothing, D: wall
// R: Red potion,   G: Green potion,   B: Blue potion,	 N: Black potion
// 1: Red Obstacle, 2: Green Obstacle, 3: Blue Obstacle, 4: Yellow Obs
// Z: Red Jump,     Y: Green Jump,     X: Blue Jump,     W: Yellow Jump,  V: Purple Jump
// !: Red lazor		@: Green lazor,    #: Blue lazor,	 $: Yellow lazor, %: Purple lazor 
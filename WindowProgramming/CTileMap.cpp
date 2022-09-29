#include "pch.h"
#include "CTileMap.h"

CTileMap::CTileMap(const string& Filename)
{
	int cnt = 0;
	m_vTile.reserve(TILE_NUM_H * TILE_NUM_W);
	ifstream in(Filename);
	while (!in.eof()) {
		char tile;
		in >> tile;
		if(m_vTile.size() < TILE_NUM_H * TILE_NUM_W)
			m_vTile.emplace_back(tile);	
	}
}

CTileMap::~CTileMap()
{
}

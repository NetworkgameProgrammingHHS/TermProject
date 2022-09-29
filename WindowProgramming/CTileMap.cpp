#include "pch.h"
#include "CObject.h"
#include "CTileMap.h"

CTileMap::CTileMap(const string& Filename)
{
	m_vTile.reserve(TILE_NUM_H * TILE_NUM_W);
	ifstream in(Filename);
	while (!in.eof()) {
		char tile;
		in >> tile;
		m_vTile.emplace_back(tile);
		cout << tile << endl;
	}

}

CTileMap::~CTileMap()
{
}

#include "pch.h"
#include "TileMap.h"

TileMap::TileMap(string filename)
{
	ifstream in(filename);

	int cnt = 0;
	char tile;
	while (!in.eof()) {
		in >> tile;
		if (cnt < TILE_NUM_W * TILE_NUM_H) {
			m_Tiles[cnt % TILE_NUM_W][cnt / TILE_NUM_W];
			++cnt;
		}
	}
}

TileMap::~TileMap()
{
}

#include "pch.h"
#include "TileMap.h"
#include <random>

TileMap::TileMap(string filename)
{
	ifstream in(filename);

	int cnt = 0;
	char tile;
	while (!in.eof()) {
		in >> tile;
		if (cnt < TILE_NUM_W * TILE_NUM_H) {
			m_Tiles[cnt % TILE_NUM_W][cnt / TILE_NUM_W] = tile;
			++cnt;
		}
	}
}

TileMap::~TileMap()
{
}

void TileMap::CreateGun()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution<int> uid(0, TILE_NUM_W * TILE_NUM_H); //625
	int iRand = uid(dre);
	while (m_Tiles[iRand / TILE_NUM_W][iRand % TILE_NUM_W] == '0')
	{
		iRand = uid(dre);
	}
	m_Tiles[iRand / TILE_NUM_W][iRand % TILE_NUM_W] == 'A';


}

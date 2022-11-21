#pragma once

class TileMap
{
public:
	TileMap(string filename);
	~TileMap();

	const int& GetPotionCnt() const { return m_PotionCnt; }

	int m_Tiles[TILE_NUM_W][TILE_NUM_H] = {0, };
	void CreateGun();

private:
	int m_PotionCnt = 0;
};


#pragma once
class Player;

class TileMap
{
public:
	TileMap(string filename);
	~TileMap();

	const int& GetPotionCnt() const { return m_PotionCnt; }
	const Vec2& GetGunPos() const { return m_GunPos; }

	int m_Tiles[TILE_NUM_W][TILE_NUM_H] = {0, };
	void CreateGun();

	void Collide_Wall(Player* in);
	bool Collide_Gun(Player* in);

	multimap<int, Vec2> m_mmObjPos;

private:
	int m_PotionCnt = 0;
	Vec2 m_GunPos;
};


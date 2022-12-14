#pragma once
class Player;

class TileMap
{
public:
	TileMap(string filename);
	~TileMap();

	const Vec2& GetGunPos() const { return m_GunPos; }

	int m_Tiles[TILE_NUM_H][TILE_NUM_W] = {0, };
	void CreateGun();

	void Collide_Wall(Player* in);
	void Collide_Gate(Player* in);
	void Collide_Jump(Player* in);
	bool Collide_Gun(Player* in);

	multimap<int, Vec2> m_mmObjPos;

private:
	Vec2 m_GunPos;
};


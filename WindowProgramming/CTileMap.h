#pragma once
#include "CObject.h"

constexpr int TILE_CNT = 20;	// 타일 종류

class CTile : public CObject
{
public:
	CTile() {};
	CTile(const string& Filename);
	~CTile();

	void SetTexture(const sf::Texture& tex) { m_sfTexture = tex; }
	void SetSpriteTex() { m_sfSprite.setTexture(m_sfTexture); }

	const sf::Texture GetTexture() const { return m_sfTexture; }
	const sf::Sprite GetSprite() const { return m_sfSprite; }
};

class CTileMap
{
public:
	CTileMap(const string& Filename);
	virtual ~CTileMap();

	vector<char> m_vMap;					// Map Info
	unordered_map<char, CTile> m_umTiles;	// Tile Info
	array<CTile, TILE_NUM_H* TILE_NUM_W> m_arrTiles;	// Tile Obj
};

// 0: Nothing,      D: wall
// R: Red potion    G: Green potion    B: Blue potion 	 N: Black potion
// 1: Red Turret	2: Green Turret    3: Blue Turret    4: Yellow 	     5: Purple Turret	6: GB Turret	7: Idle Turret
// Z: Red Jump,     Y: Green Jump,     X: Blue Jump,     W: Yellow Jump  V: Purple Jump		U: GB Jump
// !: Red Gate		@: Green Gate,     #: Blue Gate,	 $: Yellow Gate  %: Purple Gate		^: GB Gate
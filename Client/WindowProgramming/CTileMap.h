#pragma once
#include "CObject.h"

constexpr int TILE_CNT = 25;	// 타일 종류

class CTile : public CObject
{
public:
	CTile() {};
	CTile(const string& Filename, const TILE_TYPE& type);
	CTile(const sf::Texture& tex, const sf::Vector2f& pos);
	CTile(const sf::Vector2f& pos);
	~CTile();

	void SetTexture(const sf::Texture& tex) { m_sfTexture = tex; }
	void SetSpriteTex() { m_sfSprite.setTexture(m_sfTexture); }
	void SetType(const TILE_TYPE& type) { m_eType = type; }
	void SetEnable(const bool enable) { m_bEnable = enable; }

	const sf::Texture& GetTexture() const { return m_sfTexture; }
	const TILE_TYPE& GetType() const { return m_eType; }
	const bool GetEnable() const { return m_bEnable; }

private:
	TILE_TYPE m_eType = TILE_TYPE::NONE;
	bool m_bEnable = true;
};

class CTurret : public CTile
{
public:
	CTurret(const sf::Texture& idle, const sf::Texture& tex, const sf::Vector2f& pos, const TILE_TYPE& type);
	~CTurret() {};

	const sf::Rect<float>& GetTurretAABB(int index) { return m_rtTurretAABB[index]; }
	const sf::Texture& GetTurretTex() { return m_sfTurretTex; }
	const bool GetActivate() { return m_bActivate; }

	void SetTurretAABB(sf::Vector2f pos);
	void SetTurretTex(sf::Texture tex) { m_sfTurretTex = tex; }
	void SetActivate(bool activate) { m_bActivate = activate; }

private:
	sf::Texture m_sfTurretTex;
	sf::Rect<float> m_rtTurretAABB[2];
	PLAYER_COLOR m_eTurretColor;
	bool m_bActivate = false;
};

class CTileMap
{
public:
	CTileMap(const string& Filename);
	virtual ~CTileMap();

	void Initialize();
	void Render(sf::RenderWindow& RW);
	void Reset();
	void TileReset();

	const int GetPotionNum() const { return m_iPotionNum; }
	int SetPotionNum(int num) { return m_iPotionNum = num; }

	vector<char> m_vMap;					// Map Info
	unordered_map<char, CTile> m_umTileInfo;	// Tile Info
	unordered_map<TILE_TYPE, vector<CTile*>> m_umTiles; // Tile Obj

private:
	int m_iPotionNum = 0;
};

// 0: Nothing,      D: wall
// R: Red potion    G: Green potion    B: Blue potion 	 N: Black potion
// 1: Red Turret	2: Green Turret    3: Blue Turret    4: Yellow 	     5: Purple Turret	6: GB Turret	7: Idle Turret
// Z: Red Jump,     Y: Green Jump,     X: Blue Jump,     W: Yellow Jump  V: Purple Jump		U: GB Jump
// !: Red Gate		@: Green Gate,     #: Blue Gate,	 $: Yellow Gate  %: Purple Gate		^: GB Gate
// P: Spoid		    A: Gun
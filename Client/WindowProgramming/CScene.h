#pragma once
#include "CGun.h"
#include "CPlayer.h"
#include <array>

class CPlayer;
class CTileMap;
class CNetworkMgr;

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void KeyBoardInput(const sf::Keyboard::Key& key);

	virtual void MouseClickInput(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow) {};

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
	virtual void Next_Stage();
	

	const SCENE_NUM GetSceneNum() const { return m_eCurScene; }
	const bool GetNext() const { return m_bNext; }
	//const int GetPlayerIndex() const { return m_pNetworkMgr->GetPlayerIndex(); }

	void SetNext(bool next) { m_bNext = next; }
	//void SetPlayerIndex(int index) { m_pNetworkMgr->GetPlayerIndex() = index; }

	void SetGunState(int enable, int bulletx, int bullety);
	void Logout(int index);

	void TileReset();

protected:
	void ShowPlayerInfo();
	//void Collide_Wall();
	void Collide_OBJ();
	void Collide_Potion();
	//void Collide_Gate();
	void Collide_Turret();
	//void Collide_Jump();
	void Collide_Spoid();

	virtual void Reset();

protected:
	shared_ptr<CNetworkMgr> m_pNetworkMgr = nullptr;
	array<shared_ptr<CPlayer>, PLAYERNUM> m_ppPlayers;
	
	unique_ptr<CTileMap> m_pTileMap = nullptr;

	unique_ptr<CGun> m_pGun = nullptr;

	sf::Texture m_sfTexture;
	sf::Sprite m_sfBackground;


	SCENE_NUM m_eCurScene = SCENE_NUM::NONE;
	bool m_bNext = false;	// 다음 스테이지 확인

	//int m_pNetworkMgr->GetPlayerIndex() = 0;
};


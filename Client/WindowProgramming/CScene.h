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

	void SetNext(bool next) { Lock(); m_bNext = next; Unlock(); }

	void SetGunState(int enable, int bulletx, int bullety);
	void Logout(int index);

	void TileReset();

protected:
	void ShowPlayerInfo();
	void Collide_OBJ();
	void Collide_Potion();
	void Collide_Turret();
	void Collide_Spoid();

	virtual void Reset();

	void Lock() { EnterCriticalSection(&m_CS); };
	void Unlock() { LeaveCriticalSection(&m_CS); };

protected:
	shared_ptr<CNetworkMgr> m_pNetworkMgr = nullptr;
	array<shared_ptr<CPlayer>, PLAYERNUM> m_ppPlayers = {};
	
	unique_ptr<CTileMap> m_pTileMap = nullptr;

	sf::Texture m_sfTexture = {};
	sf::Sprite m_sfBackground = {};

	SCENE_NUM m_eCurScene = SCENE_NUM::NONE;
	bool m_bNext = false;	// 다음 스테이지 확인

	CRITICAL_SECTION m_CS = {};
};


#pragma once
#include "CGun.h"

class CPlayer;
class CTileMap;

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void KeyBoardInput(const sf::Keyboard::Key& key);

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
	virtual void Next_Stage();

	const SCENE_NUM GetSceneNum() const { return m_eCurScene; }
	const bool GetNext() const { return m_bNext; }

	void SetNext(bool next) { m_bNext = next; }

protected:
	void Collide_Wall();
	void Collide_OBJ();
	void Collide_Potion();
	void Collide_Gate();
	void Collide_Turret();
	void Collide_Jump();
	void Collide_Spoid();

	virtual void Reset();

protected:
	shared_ptr<CPlayer> m_pPlayer = nullptr;
	
	unique_ptr<CTileMap> m_pTileMap = nullptr;

	unique_ptr<CGun> m_pGun = nullptr;

	sf::Texture m_sfTexture;
	sf::Sprite m_sfBackground;

	SCENE_NUM m_eCurScene = SCENE_NUM::NONE;
	bool m_bNext = false;	// 다음 스테이지 확인
};


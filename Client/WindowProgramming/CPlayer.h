#pragma once
#include "CObject.h"

#define TIME_PER_ACTION 0.25f
#define ACTION_PER_TIME 1.0f / TIME_PER_ACTION
#define FRAMES_PER_ACTION 3

class CNetworkMgr;

class CPlayer : public CObject
{
public:
	CPlayer(shared_ptr<CNetworkMgr> networkmgr);
	virtual ~CPlayer();

	void Reset();
	void Update(const float ElapsedTime);
	void Render(sf::RenderWindow& RW);
	virtual void UpdateAABB();

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

	const PLAYER_COLOR& GetColor() const { return m_eColor; }
	const sf::Vector2f& GetPos() const { return m_vec2fPos; }
	const bool GetJump() const { return m_bJump; }
	const bool GetSpoid() const { return m_bSpoid; }
	const bool GetGun() const { return m_bGun; }
	const PLAYER_COLOR& GetSavedColor() const { return m_eSavedColor; }
	const bool GetOnline() const { return m_bOnline; }
	const bool GetReady() const { return m_bReady; }
	const SCENE_NUM GetStageNum() const { return m_eStageNum; }

	virtual void SetPosition(const sf::Vector2f& vec) { m_vec2fPos = vec; }
	void SetColor(const PLAYER_COLOR color) { m_eColor = color; m_sfSprite.setTexture(m_umTextures.find(color)->second); }
	void SetJump(const bool jump) { m_bJump = jump;}
	void SetSpoid(const bool spoid) { m_bSpoid = spoid; }
	void SetGun(const bool gun) { m_bGun = gun; }
	void SetSavedColor(PLAYER_COLOR color) { m_eSavedColor = color; }
	void SetDir(const int dir) { m_iDir = dir; }
	void SetOnline(const bool online) { m_bOnline = online; }
	void SetReady(const bool ready) { m_bReady = ready; }
	void SetStage(const SCENE_NUM sn) { m_eStageNum = sn; }

private:
	void Animation(const float ElapsedTime);

private:
	shared_ptr<CNetworkMgr> m_pNetworkMgr = nullptr;

	// Pos, Move
	int m_iDir = 0;
	int m_nKeyDir = 0;
	bool m_bJump = false;

	// State, Color
	float m_fSpriteLeft = 0;
	int m_iSpriteTop = 32;
	bool m_bOnline = false;
	bool m_bReady = false;
	PLAYER_STATE m_eState = PLAYER_STATE::IDLE;
	PLAYER_COLOR m_eColor = PLAYER_COLOR::NORMAL;
	PLAYER_COLOR m_eSavedColor = PLAYER_COLOR::NORMAL;
	unordered_map<PLAYER_COLOR, sf::Texture> m_umTextures = {};

	SCENE_NUM m_eStageNum = SCENE_NUM::STAGE1;

	bool m_bSpoid = false;

	// Gun
	bool m_bGun = false;
};


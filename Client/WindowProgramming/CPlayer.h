#pragma once
#include "CObject.h"

constexpr float PLAYER_SPEED = 150.0f;
constexpr float JUMP_SPEED = 300.0f;
constexpr float SUPERJUMP_SPEED = 600.0f;
constexpr float GRAVITY = 15.0f;

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
	const sf::Vector2f& GetPrevPos() const { return m_vec2fPrevPos; }
	const sf::Vector2f& GetPos() const { return m_vec2fPos; }
	const bool GetFall() const { return m_bFall; }
	const bool GetJump() const { return m_bJump; }
	const bool GetSuperJump() const { return m_bSuperJump; }
	const bool GetJumpDir() const { return m_iJumpCnt < m_iJumpChange; }	// true는 위로, false는 아래로
	const int GetJumpChange() const { return m_iJumpChange; }
	const sf::Rect<float> GetFallBB() const { return m_rtFallBB; }
	const bool GetSpoid() const { return m_bSpoid; }
	const bool GetGun() const { return m_bGun; }
	const PLAYER_COLOR& GetSavedColor() const { return m_eSavedColor; }
	const bool GetOnline() const { return m_bOnline; }

	virtual void SetPosition(const sf::Vector2f& vec) { m_vec2fPos = vec; }
	void SetColor(const PLAYER_COLOR color) { m_eColor = color; m_sfSprite.setTexture(m_umTextures.find(color)->second); }
	void SetFall(const bool fall) { m_bFall = fall; }
	void SetJump(const bool jump) { m_bJump = jump;}
	void SetSuperJump(const bool jump) { m_bSuperJump = jump; }
	void SetJumpCnt(const int cnt) { m_iJumpCnt = cnt; }
	void SetJumpVelocity(const float speed) { m_fJumpVelocity = speed; }
	void SetSpoid(const bool spoid) { m_bSpoid = spoid; }
	void SetGun(const bool gun) { m_bGun = gun; }
	void SetSavedColor(PLAYER_COLOR color) { m_eSavedColor = color; }
	void SetDir(const int dir) { m_iDir = dir; }
	void SetOnline(const bool online) { m_bOnline = online; }
private:
	void Animation(const float ElapsedTime);

private:
	shared_ptr<CNetworkMgr> m_pNetworkMgr = nullptr;

	// Pos, Move
	sf::Vector2f m_vec2fPrevPos = {};
	int m_iDir = 0;
	bool m_bFall = false;
	bool m_bJump = false;
	bool m_bSuperJump = false;
	int m_iJumpCnt = 0;
	int m_iJumpChange = 0;
	float m_fJumpVelocity = 0.0f;

	// State, Color
	float m_fSpriteLeft = 0;
	int m_iSpriteTop = 32;
	bool m_bOnline = false;
	PLAYER_STATE m_eState = PLAYER_STATE::IDLE;
	PLAYER_COLOR m_eColor = PLAYER_COLOR::NORMAL;
	PLAYER_COLOR m_eSavedColor = PLAYER_COLOR::NORMAL;
	unordered_map<PLAYER_COLOR, sf::Texture> m_umTextures = {};

	sf::Rect<float> m_rtFallBB = {};

	bool m_bSpoid = false;
	bool m_bDebug = false;

	// Gun
	bool m_bGun = false;
};


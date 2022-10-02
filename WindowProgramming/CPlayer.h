#pragma once
#include "CObject.h"

constexpr float PLAYER_SPEED = 150.0f;

#define TIME_PER_ACTION 0.25f
#define ACTION_PER_TIME 1.0f / TIME_PER_ACTION
#define FRAMES_PER_ACTION 3

class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	void Update(const float ElapsedTime);
	void Render(sf::RenderWindow& RW);

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

	virtual void SetPosition(const sf::Vector2f& vec) { m_vec2fPos = vec; }

private:
	void Animation(const float ElapsedTime);
	void Collide();

private:
	sf::Vector2f m_vec2fPrevPos = {};
	int m_iDir = 0;

	float m_fSpriteLeft = 0;
	int m_iSpriteTop = 32;
	PLAYER_STATE m_eState = PLAYER_STATE::IDLE;
};


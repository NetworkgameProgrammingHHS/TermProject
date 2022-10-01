#pragma once
#include "CObject.h"

constexpr float PLAYER_SPEED = 100.0f;

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
	int m_iDir = 0;
};


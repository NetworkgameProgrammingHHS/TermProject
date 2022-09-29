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

private:
	int m_iDir = 0;
};


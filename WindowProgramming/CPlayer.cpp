#include "pch.h"
#include "CObject.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	if (!m_sfTexture.loadFromFile("Resource\\Character\\Scientist.png")) {
		exit(1);
	}
	m_sfSprite.setTexture(m_sfTexture);
	m_sfSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

	m_vec2fPos = { 0.0f, 0.0f };

}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(const float ElapsedTime)
{
	m_vec2fPos.x += m_iDir * PLAYER_SPEED * ElapsedTime;

	m_sfSprite.setPosition(m_vec2fPos);
}

void CPlayer::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfSprite);
}

void CPlayer::KeyBoardInput(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Left:
		if (m_iDir > -1)
			m_iDir -= 1;
		break;
	case sf::Keyboard::Right:
		if (m_iDir < 1)
			m_iDir += 1;
		break;
	case sf::Keyboard::Space:
		cout << "space" << endl;
		break;
	case sf::Keyboard::Escape:
		exit(1);
		break;
	default:
		break;
	}
}

void CPlayer::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Left:
		if (m_iDir < 1)
			m_iDir += 1;
		break;
	case sf::Keyboard::Right:
		if (m_iDir > -1)
			m_iDir -= 1;
		break;
	default:
		break;
	}
}

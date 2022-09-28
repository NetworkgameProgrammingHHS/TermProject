#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	if (!m_sfTexture.loadFromFile("Resource\\Character\\Scientist.png")) {
		exit(1);
	}
	m_sfSprite.setTexture(m_sfTexture);
	m_sfSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{

}

void CPlayer::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfSprite);
}

void CPlayer::KeyBoardInput(sf::Keyboard::Key key)
{
}

#include "pch.h"
#include "CScene.h"
#include "CTitle.h"
#include "CStage1.h"

CTitle::CTitle()
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Title_1.png"))
		exit(1);
	if (!m_sfTexture2.loadFromFile("Resource\\BackGround\\Title_2.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	m_sfBackground2.setTexture(m_sfTexture2);
	m_sfBackground2.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_eCurScene = SCENE_NUM::TITLE;
}

CTitle::~CTitle()
{
}

void CTitle::KeyBoardInput(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Escape:
		exit(1);
		break;
	default:
		m_bNext = true;
		break;
	}
}

void CTitle::Update(const float ElapsedTime)
{
	m_fTime += ElapsedTime;
	if (m_fTime > 1.0f) {
		m_fTime = 0.0f;
	}
}

void CTitle::Render(sf::RenderWindow& RW)
{
	if (m_fTime < 0.5f) {
		RW.draw(m_sfBackground);
	}
	else {
		RW.draw(m_sfBackground2);

	}
}

#include "pch.h"
#include "CTitle.h"
#include "CScene.h"
#include "CStage1.h"

CTitle::CTitle()
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Information_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_eCurScene = SCENE_NUM::TITLE;
}

CTitle::~CTitle()
{
}

void CTitle::Update(const float ElpasedTime)
{
}

void CTitle::Render(sf::RenderWindow& RW)
{
}

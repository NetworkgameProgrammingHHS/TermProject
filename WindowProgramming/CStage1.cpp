#include "pch.h"
#include "CScene.h"
#include "CStage1.h"
#include "CTileMap.h"

CStage1::CStage1()
{
	if(!m_sfTexture.loadFromFile("Resource\\BackGround\\Information_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = new CTileMap("Resource\\File\\Stage1.txt");
}

CStage1::~CStage1()
{
}

void CStage1::Update(const float ElapsedTime)
{
}

void CStage1::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfBackground);
}
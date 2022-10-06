#include "pch.h"
#include "CScene.h"
#include "CStage1.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage1::CStage1(shared_ptr<CPlayer> player)
{
	if(!m_sfTexture.loadFromFile("Resource\\BackGround\\Information_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage1.txt");
	m_pTileMap->Initialize();

	m_pPlayer = player;
	m_pPlayer->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
}

CStage1::~CStage1()
{
}

void CStage1::Update(const float ElapsedTime)
{	
	CScene::Collide_OBJ();
}

void CStage1::Render(sf::RenderWindow& RW)
{
	// Background Render
	RW.draw(m_sfBackground);

	// Map Render
	m_pTileMap->Render(RW);
}
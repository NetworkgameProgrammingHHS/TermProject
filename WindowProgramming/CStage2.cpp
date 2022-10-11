#include "pch.h"
#include "CScene.h"
#include "CStage2.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage2::CStage2(shared_ptr<CPlayer> player)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Scientist_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage2.txt");
	m_pTileMap->Initialize();

	m_pPlayer = player;
	m_pPlayer->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
}

CStage2::~CStage2()
{
}

void CStage2::Reset()
{
	cout << "Reset" << endl;
}

void CStage2::Update(const float ElapsedTime)
{
	Next_Stage();

	CScene::Collide_OBJ();
}

void CStage2::Render(sf::RenderWindow& RW)
{
	// Background Render
	RW.draw(m_sfBackground);

	// Map Render
	m_pTileMap->Render(RW);
}
#include "pch.h"
#include "CScene.h"
#include "CStage4.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage4::CStage4(shared_ptr<CPlayer> player)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Control_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage4.txt");
	m_pTileMap->Initialize();

	m_pPlayer = player;
	m_pPlayer->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });

	m_eCurScene = SCENE_NUM::STAGE4;
}

CStage4::~CStage4()
{
}

void CStage4::Next_Stage()
{
	if (m_pPlayer->GetSprite().getPosition().x >= TILE_NUM_W * 32 && m_pPlayer->GetSprite().getPosition().y >= (TILE_NUM_H - 3) * 32) {
		if (m_pTileMap->GetPotionNum() == 0 && m_pPlayer->GetColor() == PLAYER_COLOR::NORMAL) {
			m_bNext = true;
		}
		else {
			Reset();
		}
	}
}

void CStage4::Reset()
{
	m_pTileMap->Reset();
	m_pTileMap->Initialize();
	m_pPlayer->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
	m_pPlayer->Reset();
}

void CStage4::Update(const float ElapsedTime)
{
	m_pPlayer->Update(ElapsedTime);

	CScene::Collide_OBJ();

	Next_Stage();
}

void CStage4::Render(sf::RenderWindow& RW)
{
	// Background Render
	RW.draw(m_sfBackground);

	// Map Render
	m_pTileMap->Render(RW);

	// Player Render
	m_pPlayer->Render(RW);
}
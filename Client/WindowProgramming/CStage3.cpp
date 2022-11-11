#include "pch.h"
#include "CScene.h"
#include "CStage3.h"
#include "CNetworkMgr.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage3::CStage3(shared_ptr<CNetworkMgr> networkmgr, shared_ptr<CPlayer> player)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Security_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage3.txt");
	m_pTileMap->Initialize();

	m_ppPlayers[m_nPlayerIndex] = player;
	m_ppPlayers[m_nPlayerIndex]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });

	m_pNetworkMgr = networkmgr;
	m_eCurScene = SCENE_NUM::STAGE3;
}

CStage3::~CStage3()
{
}

void CStage3::Next_Stage()
{
	if (m_ppPlayers[m_nPlayerIndex]->GetSprite().getPosition().x >= TILE_NUM_W * 32 && m_ppPlayers[m_nPlayerIndex]->GetSprite().getPosition().y >= (TILE_NUM_H - 3) * 32) {
		if (m_pTileMap->GetPotionNum() == 0 && m_ppPlayers[m_nPlayerIndex]->GetColor() == PLAYER_COLOR::NORMAL) {
			m_bNext = true;
		}
		else {
			Reset();
		}
	}
}

void CStage3::Reset()
{
	m_pTileMap->Reset();
	m_pTileMap->Initialize();
	m_ppPlayers[m_nPlayerIndex]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
	m_ppPlayers[m_nPlayerIndex]->Reset();
}

void CStage3::Update(const float ElapsedTime)
{
	m_ppPlayers[m_nPlayerIndex]->Update(ElapsedTime);
	if (m_pGun)m_pGun->Update(ElapsedTime);

	CScene::Collide_OBJ();

	Next_Stage();
}

void CStage3::Render(sf::RenderWindow& RW)
{
	// Background Render
	RW.draw(m_sfBackground);

	// Map Render
	m_pTileMap->Render(RW);

	// Player Render
	m_ppPlayers[m_nPlayerIndex]->Render(RW);

	// Gun, Bullet Render
	if (m_pGun)m_pGun->Render(RW);
}
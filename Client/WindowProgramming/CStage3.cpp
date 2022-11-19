#include "pch.h"
#include "CScene.h"
#include "CStage3.h"
#include "CNetworkMgr.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage3::CStage3(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM>  players)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Security_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage3.txt");
	m_pTileMap->Initialize();

	m_ppPlayers = players;
	m_ppPlayers[m_nPlayerIndex]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });

	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (m_ppPlayers[i])
		{
			m_ppPlayers[i] = make_shared<CPlayer>(m_pNetworkMgr);
			m_ppPlayers[i]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
		}
	}

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

	//Send Reset Packet to Server
	CS_PLAYER_RESET_PACKET* packet = new CS_PLAYER_RESET_PACKET;
	packet->type = CS_PLAYER_RESET;
	packet->reset = RESET_ON;
	m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_RESET_PACKET));

}

void CStage3::Update(const float ElapsedTime)
{
	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (m_ppPlayers[i])m_ppPlayers[i]->Update(ElapsedTime);
	}
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
	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (m_ppPlayers[i]) m_ppPlayers[i]->Render(RW);
	}

	// Gun, Bullet Render
	if (m_pGun)m_pGun->Render(RW);
}
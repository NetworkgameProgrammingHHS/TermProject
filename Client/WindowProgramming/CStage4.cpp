#include "pch.h"
#include "CScene.h"
#include "CStage4.h"
#include "CNetworkMgr.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage4::CStage4(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM>  players)
{
	m_pNetworkMgr = networkmgr;

	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Control_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage4.txt");
	m_pTileMap->Initialize();

	m_ppPlayers = players;
	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });

	m_eCurScene = SCENE_NUM::STAGE4;
}

CStage4::~CStage4()
{
}

void CStage4::Next_Stage()
{
	if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getPosition().x >= TILE_NUM_W * 32 && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getPosition().y >= (4) * 32) {
		if (m_pTileMap->GetPotionNum() == 0) {
			m_bNext = true;
			//Send Reset Packet to Server
			CS_NEXT_STAGE_PACKET* packet = new CS_NEXT_STAGE_PACKET;
			packet->type = CS_NEXTSTAGE;
			m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_NEXT_STAGE_PACKET));
		}
		else {
			Reset();
		}
	}
}

void CStage4::Reset()
{
	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->Reset();
	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
	m_pTileMap->Reset();
	m_pTileMap->Initialize();
	m_pNetworkMgr->SetPlayerInfo(PLAYER_COLOR::NORMAL, m_pNetworkMgr->GetPlayerIndex());

	//Send Reset Packet to Server
	CS_PLAYER_RESET_PACKET* packet = new CS_PLAYER_RESET_PACKET;
	packet->type = CS_PLAYER_RESET;
	packet->reset = RESET_ON;
	m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_RESET_PACKET));
}

void CStage4::Update(const float ElapsedTime)
{
	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (m_ppPlayers[i] && m_ppPlayers[i]->GetStageNum() == SCENE_NUM::STAGE4)
			m_ppPlayers[i]->Update(ElapsedTime);
	}

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
	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (m_ppPlayers[i] && m_ppPlayers[i]->GetStageNum() == SCENE_NUM::STAGE4)
			m_ppPlayers[i]->Render(RW);
	}

	// Gun, Bullet Render
	if (m_pGun)m_pGun->Render(RW);

	CScene::Render(RW);
}
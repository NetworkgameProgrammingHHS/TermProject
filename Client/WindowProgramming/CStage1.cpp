#include "pch.h"
#include "CScene.h"
#include "CStage1.h"
#include "CNetworkMgr.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CStage1::CStage1(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM>  players)
{
	Lock();
	m_pNetworkMgr = networkmgr;

	if(!m_sfTexture.loadFromFile("Resource\\BackGround\\Information_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pTileMap = make_unique<CTileMap>("Resource\\File\\Stage1.txt");
	m_pTileMap->Initialize();	
	m_ppPlayers = players;
	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f{static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE)});

		
	m_eCurScene = SCENE_NUM::STAGE1;
	Unlock();
}

CStage1::~CStage1()
{
}

void CStage1::Next_Stage()
{
	if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]) {
		if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getPosition().x >= TILE_NUM_W * 32 && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getPosition().y >= (TILE_NUM_H - 3) * 32) {
			if (m_pTileMap) {
				if (m_pTileMap->GetPotionNum() == 0 && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() == PLAYER_COLOR::NORMAL) {
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
	}
}

void CStage1::Reset()
{
	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->Reset();
	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) });
	m_pTileMap->TileReset();
	m_pNetworkMgr->SetPlayerInfo(PLAYER_COLOR::NORMAL, m_pNetworkMgr->GetPlayerIndex());

	//Send Reset Packet to Server
	CS_PLAYER_RESET_PACKET* packet = new CS_PLAYER_RESET_PACKET;
	packet->type = CS_PLAYER_RESET;
	packet->reset = RESET_ON;
	m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_RESET_PACKET));
	
}

void CStage1::Update(const float ElapsedTime)
{	
	for (int i = 0; i < PLAYERNUM; ++i) 
	{
		if(m_ppPlayers[i] && m_ppPlayers[i]->GetStageNum() == SCENE_NUM::STAGE1)
			m_ppPlayers[i]->Update(ElapsedTime);
	}

	CScene::Collide_OBJ();

	Next_Stage();
}

void CStage1::Render(sf::RenderWindow& RW)
{
	// Background Render
	RW.draw(m_sfBackground);

	// Map Render
	if (m_pTileMap)
	{
		m_pTileMap->Render(RW);

		// Player Render
		for (int i = 0; i < PLAYERNUM; ++i)
		{
			if (m_ppPlayers[i] && m_ppPlayers[i]->GetStageNum() == SCENE_NUM::STAGE1)
				m_ppPlayers[i]->Render(RW);
		}

		// Gun, Bullet Render
		if (m_pNetworkMgr->GetGun())if (m_pNetworkMgr->GetGun()->GetGunStage() == m_eCurScene) m_pNetworkMgr->GetGun()->Render(RW);

		CScene::Render(RW);
	}
}
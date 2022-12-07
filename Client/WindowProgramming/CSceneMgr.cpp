#include "pch.h"
#include "CSceneMgr.h"
#include "CNetworkMgr.h"
#include "CScene.h"
#include "CTitle.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CStage3.h"
#include "CStage4.h"
#include "CStage5.h"
#include "CRanking.h"
#include "CPlayer.h"
#include "CLobby.h"

CSceneMgr::CSceneMgr(shared_ptr<CNetworkMgr> networkmgr)
{
	m_pNetworkMgr = networkmgr;
	InitializeCriticalSection(&m_CS);
}

CSceneMgr::~CSceneMgr()
{
	DeleteCriticalSection(&m_CS);
}

void CSceneMgr::Initialize()
{
	

	for (int i = 0; i < PLAYERNUM; ++i)
	{
		m_ppPlayers[i] = make_shared<CPlayer>(m_pNetworkMgr);
	}

	m_pScene = dynamic_pointer_cast<CScene>(make_shared<CTitle>(m_pNetworkMgr));
	
	m_eCurScene = SCENE_NUM::TITLE;
}

void CSceneMgr::Update(const float ElpasedTime)
{
	if (m_pScene) {
		m_pScene->Update(ElpasedTime);

		if (m_pScene->GetNext()) {
			m_pScene->SetNext(false);
			Next_Stage();
		}
	}
}

void CSceneMgr::Render(sf::RenderWindow& RW)
{
	if (m_pNetworkMgr->GetWinnerName().getString().getSize() && m_eCurScene != SCENE_NUM::RANKING)
	{
		EnterCriticalSection(&m_CS);
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CRanking>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::RANKING;
		LeaveCriticalSection(&m_CS);
	}

	m_pScene->Render(RW);

	if (static_cast<int>(m_pScene->GetSceneNum()) >= static_cast<int>(SCENE_NUM::STAGE1) && 
		static_cast<int>(m_pScene->GetSceneNum()) != static_cast<int>(SCENE_NUM::RANKING)) {
		for (int i = 0; i < 3; ++i) {
			if (!m_pNetworkMgr->GetPlayerInfo().empty()) {
				RW.draw(m_pNetworkMgr->GetPlayerInfo()[i]);
			}

			for (int j = 0; j < 2; ++j) {
				if (!m_pNetworkMgr->GetPlayerInfoText().empty()) {
					RW.draw(m_pNetworkMgr->GetPlayerInfoText()[i][j]);

				}
			}
		}
	}
}

void CSceneMgr::KeyBoardInput(const sf::Keyboard::Key& key)
{
	if (key == sf::Keyboard::F1 && m_eCurScene < SCENE_NUM::RANKING) {
		if(m_eCurScene == SCENE_NUM::STAGE5)
			m_pNetworkMgr->SetWinnerName(m_pNetworkMgr->GetPlayerInfoText()[m_pNetworkMgr->GetPlayerIndex()][0].getString());
		Next_Stage();
		//Send Reset Packet to Server
		
	
		CS_NEXT_STAGE_PACKET* packet = new CS_NEXT_STAGE_PACKET;
		packet->type = CS_NEXTSTAGE;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_NEXT_STAGE_PACKET));

		m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::NORMAL);
		CS_PLAYER_COLOR_PACKET* cp = new CS_PLAYER_COLOR_PACKET;
		cp->type = CS_COLOR;
		cp->color = static_cast<short>(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor());
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(cp), sizeof(CS_PLAYER_COLOR_PACKET));
		m_pNetworkMgr->SetPlayerInfo(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor(), m_pNetworkMgr->GetPlayerIndex());
	}
	if (key == sf::Keyboard::R && m_eCurScene > SCENE_NUM::LOBBY && m_eCurScene < SCENE_NUM::RANKING) {
		m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->Reset();
		m_pScene->TileReset();
		m_pNetworkMgr->SetPlayerInfo(PLAYER_COLOR::NORMAL, m_pNetworkMgr->GetPlayerIndex());
		CS_PLAYER_RESET_PACKET* packet = new CS_PLAYER_RESET_PACKET;
		packet->type = CS_PLAYER_RESET;
		packet->reset = RESET_ON;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_RESET_PACKET));
	}

	if (m_eCurScene == SCENE_NUM::TITLE || m_eCurScene == SCENE_NUM::LOBBY)
		m_pScene->KeyBoardInput(key);
	else {
		m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->KeyBoardInput(key);
	}
}

void CSceneMgr::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	if (m_eCurScene != SCENE_NUM::TITLE || m_eCurScene == SCENE_NUM::LOBBY)
		m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->KeyBoardRelease(key);
}

void CSceneMgr::MouseClick(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow)
{
	if (m_eCurScene == SCENE_NUM::TITLE)
		m_pScene->MouseClickInput(btn, sfWindow);
}

void CSceneMgr::Next_Stage()
{
	if (m_pScene) {
		switch (m_pScene->GetSceneNum()) {
		case SCENE_NUM::TITLE:
		{
			////EnterCriticalSection(&m_CS);
			m_pScene.reset();
			m_pScene = dynamic_pointer_cast<CScene>(make_shared<CLobby>(m_pNetworkMgr, m_ppPlayers));
			m_eCurScene = SCENE_NUM::LOBBY;
			////LeaveCriticalSection(&m_CS);
			break;
		}
		case SCENE_NUM::LOBBY:
		{
			//EnterCriticalSection(&m_CS);
			m_pScene.reset();
			m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pNetworkMgr, m_ppPlayers));
			m_eCurScene = SCENE_NUM::STAGE1;
			//LeaveCriticalSection(&m_CS);
			break;
		}
		case SCENE_NUM::STAGE1:
			//EnterCriticalSection(&m_CS);
			m_pScene.reset();
			m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage2>(m_pNetworkMgr, m_ppPlayers));
			m_eCurScene = SCENE_NUM::STAGE2;
			//LeaveCriticalSection(&m_CS);
			break;
		case SCENE_NUM::STAGE2:
			//EnterCriticalSection(&m_CS);
			m_pScene.reset();
			m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage3>(m_pNetworkMgr, m_ppPlayers));
			m_eCurScene = SCENE_NUM::STAGE3;
			//LeaveCriticalSection(&m_CS);
			break;
		case SCENE_NUM::STAGE3:
			//EnterCriticalSection(&m_CS);
			m_pScene.reset();
			m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage4>(m_pNetworkMgr, m_ppPlayers));
			m_eCurScene = SCENE_NUM::STAGE4;
			//LeaveCriticalSection(&m_CS);
			break;
		case SCENE_NUM::STAGE4:
			//EnterCriticalSection(&m_CS);
			m_pScene.reset();
			m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage5>(m_pNetworkMgr, m_ppPlayers));
			m_eCurScene = SCENE_NUM::STAGE5;
			//LeaveCriticalSection(&m_CS);
			break;

		default:
			break;
		}
	}
}

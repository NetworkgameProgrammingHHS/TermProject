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
	InitializeCriticalSection(&g_CS);
	m_pNetworkMgr = networkmgr;
}

CSceneMgr::~CSceneMgr()
{
	DeleteCriticalSection(&g_CS);
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
	m_pScene->Update(ElpasedTime);

	if (m_pScene->GetNext()) {
		m_pScene->SetNext(false);
		Next_Stage();
	}
}

void CSceneMgr::Render(sf::RenderWindow& RW)
{
	if (m_pNetworkMgr->GetWinnerName().getString().getSize() && m_eCurScene != SCENE_NUM::RANKING)
	{
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CRanking>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::RANKING;
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
					if (!m_pNetworkMgr->GetPlayerInfoText().empty()) {
						RW.draw(m_pNetworkMgr->GetPlayerInfoText()[i][j]);
					}
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
	switch (m_pScene->GetSceneNum()) {
	case SCENE_NUM::TITLE: 
	{
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CLobby>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::LOBBY;
		break;
	}
	case SCENE_NUM::LOBBY:
	{
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::STAGE1;
		break;
	}
	case SCENE_NUM::STAGE1:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage2>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::STAGE2;
		break;
	case SCENE_NUM::STAGE2:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage3>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::STAGE3;
		break;
	case SCENE_NUM::STAGE3:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage4>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::STAGE4;
		break;
	case SCENE_NUM::STAGE4:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage5>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::STAGE5;
		break;
	case SCENE_NUM::STAGE5:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CRanking>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::RANKING;
		break;

	default:
		break;
	}
}

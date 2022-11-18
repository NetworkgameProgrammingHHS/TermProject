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

CSceneMgr::CSceneMgr(shared_ptr<CNetworkMgr> networkmgr)
{
	m_pNetworkMgr = networkmgr;
}

CSceneMgr::~CSceneMgr()
{

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
	m_pScene->Render(RW);
}

void CSceneMgr::KeyBoardInput(const sf::Keyboard::Key& key)
{
	if (m_eCurScene == SCENE_NUM::TITLE)
		m_pScene->KeyBoardInput(key);

	m_ppPlayers[m_pScene->GetPlayerIndex()]->KeyBoardInput(key);
}

void CSceneMgr::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_ppPlayers[m_pScene->GetPlayerIndex()]->KeyBoardRelease(key);
}

void CSceneMgr::Next_Stage()
{
	switch (m_pScene->GetSceneNum()) {
	case SCENE_NUM::TITLE:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pNetworkMgr, m_ppPlayers));
		m_eCurScene = SCENE_NUM::STAGE1;
		break;
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

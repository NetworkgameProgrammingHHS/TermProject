#include "pch.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CTitle.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CPlayer.h"

CSceneMgr::CSceneMgr()
{

}

CSceneMgr::~CSceneMgr()
{

}

void CSceneMgr::Initialize()
{
	m_pPlayer = make_shared<CPlayer>();

	m_pScene = dynamic_pointer_cast<CScene>(make_shared<CTitle>());

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

	m_pPlayer->KeyBoardInput(key);
}

void CSceneMgr::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardRelease(key);
}

void CSceneMgr::Next_Stage()
{
	switch (m_pScene->GetSceneNum()) {
	case SCENE_NUM::TITLE:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pPlayer));
		m_eCurScene = SCENE_NUM::STAGE1;
		break;
	case SCENE_NUM::STAGE1:
		m_pScene.reset();
		m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage2>(m_pPlayer));
		m_eCurScene = SCENE_NUM::STAGE2;
		break;
	case SCENE_NUM::STAGE2:
		break;
	case SCENE_NUM::STAGE3:
		break;
	default:
		break;
	}
}

#include "pch.h"
#include "CSceneMgr.h"
#include "CScene.h"
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

	m_pScene = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pPlayer));

	m_eCurScene = SCENE_NUM::STAGE1;
}

void CSceneMgr::Update(const float ElpasedTime)
{
	m_pScene->Update(ElpasedTime);

	if (m_pScene->Next_Stage()) {
		Next_Stage();
	}
}

void CSceneMgr::Render(sf::RenderWindow& RW)
{
	m_pScene->Render(RW);
}

void CSceneMgr::KeyBoardInput(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardInput(key);
}

void CSceneMgr::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardRelease(key);
}

void CSceneMgr::Next_Stage()
{
	
}

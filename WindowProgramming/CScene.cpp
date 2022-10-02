#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CStage1.h"
#include "CTileMap.h"

CScene::CScene()
{
	
}

CScene::~CScene()
{
}

void CScene::Initialize()
{
	m_pPlayer = make_shared<CPlayer>();

	m_pStage = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pPlayer));

	//m_pStage = dynamic_cast<CScene*>(new CStage1(m_pPlayer));
}

void CScene::Update(const float ElapsedTime)
{
	m_pStage->Update(ElapsedTime);

	m_pPlayer->Update(ElapsedTime);
}

void CScene::Render(sf::RenderWindow& RW)
{
	m_pStage->Render(RW);
	m_pPlayer->Render(RW);
}

void CScene::KeyBoardInput(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardInput(key);
}

void CScene::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardRelease(key);
}

#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"

CScene::CScene()
{
	m_pPlayer = new CPlayer;
}

CScene::~CScene()
{
	delete m_pPlayer;
}

void CScene::Update()
{
	m_pPlayer->Update();
}

void CScene::Render(sf::RenderWindow& RW)
{
	m_pPlayer->Render(RW);
}

void CScene::KeyBoardInput(sf::Keyboard::Key key)
{
	m_pPlayer->KeyBoardInput(key);
}

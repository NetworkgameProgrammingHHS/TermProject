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

void CScene::Update(const float ElpasedTime)
{
	m_pPlayer->Update();
}

void CScene::Render(sf::RenderWindow& RW)
{
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

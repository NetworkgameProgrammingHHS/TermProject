#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"

CScene::CScene()
{
	m_pPlayer = new CPlayer;
	if (!m_sfBackground.loadFromFile("Resource\\Character\\Scientist.png")) {
		exit(1);
	}
}

CScene::~CScene()
{
	delete m_pPlayer;
}

void CScene::Update(const float ElapsedTime)
{
	m_pPlayer->Update(ElapsedTime);
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

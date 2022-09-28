#include "pch.h"
#include "CFramework.h"
#include "CScene.h"

CFramework::CFramework()
{
	m_pScene = new CScene;
}

CFramework::~CFramework()
{
	delete m_pScene;
}

void CFramework::Process()
{
	m_pScene->Update();
}

void CFramework::KeyBoardInput(sf::Keyboard::Key key)
{
	m_pScene->KeyBoardInput(key);
}

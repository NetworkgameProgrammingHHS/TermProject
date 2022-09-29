#include "pch.h"
#include "CFramework.h"
#include "CScene.h"

CFramework::CFramework() : m_sfWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Client")
{
	m_pScene = new CScene;
	m_pScene->Initialize();

	m_sfWindow.setFramerateLimit(60);
}

CFramework::~CFramework()
{
	delete m_pScene;
}

void CFramework::Process()
{
	while (m_sfWindow.isOpen())
	{
		sf::Event event;
		while (m_sfWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_sfWindow.close();
			if (event.type == sf::Event::KeyPressed)
				KeyBoardInput(event.key.code);
			if (event.type == sf::Event::KeyReleased)
				KeyBoardRelease(event.key.code);
		}
		
		Update();

		m_sfWindow.clear();

		Render(m_sfWindow);

		m_sfWindow.display();
	}
}

void CFramework::KeyBoardInput(const sf::Keyboard::Key& key)
{
	m_pScene->KeyBoardInput(key);
}

void CFramework::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_pScene->KeyBoardRelease(key);
}

void CFramework::Update()
{
	float ElapsedTime = m_sfFrame.getElapsedTime().asSeconds();
	m_pScene->Update(ElapsedTime);

	m_sfFrame.restart();
}

void CFramework::Render(sf::RenderWindow& RW)
{
	m_pScene->Render(RW);
}

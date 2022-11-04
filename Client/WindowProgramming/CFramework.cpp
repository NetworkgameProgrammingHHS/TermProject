#include "pch.h"
#include "CFramework.h"
#include "CScene.h"
#include "CSceneMgr.h"

CFramework::CFramework() : m_sfWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Client")
{
	m_pSceneMgr = make_unique<CSceneMgr>();
	m_pSceneMgr->Initialize();

	m_sfWindow.setFramerateLimit(60);
}

CFramework::~CFramework()
{

}

void CFramework::Process()
{
	//Thread 생성
	//HANDLE hThread = CreateThread(NULL, 0, RecvProcess,
	//	(LPVOID)m_pNetWorkMgr->client_sock, 0, NULL);
	//if (hThread == NULL)
	//{
	//	closesocket(m_pNetWorkMgr->client_sock);
	//}
	//else
	//{
	//	CloseHandle(hThread);
	//}

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
		//수정
		Update();

		m_sfWindow.clear();

		Render(m_sfWindow);

		m_sfWindow.display();
	}
}

void CFramework::KeyBoardInput(const sf::Keyboard::Key& key)
{
	m_pSceneMgr->KeyBoardInput(key);
}

void CFramework::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_pSceneMgr->KeyBoardRelease(key);
}

void CFramework::Update()
{
	float ElapsedTime = m_sfFrame.getElapsedTime().asSeconds();
	m_pSceneMgr->Update(ElapsedTime);

	// FPS
	m_fTime += m_sfFrame.getElapsedTime().asMilliseconds();
	++m_iFrame;
	if (m_fTime - m_fBaseTime > 1000) {
		cout << "FPS : " << m_iFrame << endl;
		m_iFrame = static_cast<int>(m_iFrame * 1000.0f / (m_fTime - m_fBaseTime));
		m_fBaseTime = m_fTime;
		m_iFrame = 0;
	}

	m_sfFrame.restart();
}

void CFramework::Render(sf::RenderWindow& RW)
{
	m_pSceneMgr->Render(RW);
}

//DWORD __stdcall CFramework::RecvProcess(LPVOID arg)
//{
//	while (m_sfWindow.isOpen())
//	{
//		//packet Recv
//
//		//Update
//		Update();
//	}
//	return 0;
//}

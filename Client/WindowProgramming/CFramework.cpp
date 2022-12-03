#include "pch.h"
#include "CFramework.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CNetworkMgr.h"
#include "CTitle.h"

CFramework::CFramework() : m_sfWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Client")
{
	m_pNetworkMgr = make_shared<CNetworkMgr>();
	m_pNetworkMgr->InitializeSocket();

	m_pSceneMgr = make_shared<CSceneMgr>(m_pNetworkMgr);
	m_pSceneMgr->Initialize();

	m_pRTParameter = new RecVThreadParameter;
	m_pRTParameter->p = this;
	m_pRTParameter->pNetMgr = m_pNetworkMgr;
	m_pRTParameter->pSceneMgr = m_pSceneMgr;

	m_sfWindow.setFramerateLimit(60);
}

CFramework::~CFramework()
{

}

void CFramework::Process()
{	
	//Thread »ý¼º
	HANDLE hThread = CreateThread(NULL, 0, RecvProcess, reinterpret_cast<LPVOID>(m_pRTParameter), 0, NULL);
	if (hThread == NULL)
	{
		closesocket(m_pNetworkMgr->GetSocket());
	}
	else
	{
		CloseHandle(hThread);
	}

	while (m_sfWindow.isOpen())
	{
		sf::Event event;
		while (m_sfWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_sfWindow.close();
			if (event.type == sf::Event::KeyPressed)
			{
				KeyBoardInput(event.key.code);
				if (m_pSceneMgr->GetpScene()->GetSceneNum() == SCENE_NUM::TITLE &&
					dynamic_cast<CTitle*>(m_pSceneMgr->GetpScene().get())->GetRectangle().getFillColor() == sf::Color::Blue)
				{
					if (event.text.unicode == 59)
					{
						if(titleID.size() > 0)
							titleID.erase(titleID.size() - 1);
					}

				}
			}		
			if (event.type == sf::Event::KeyReleased)
				KeyBoardRelease(event.key.code);
			if (event.type == sf::Event::MouseButtonPressed)
				MouseClick(event.mouseButton.button, m_sfWindow);
			if (event.type == sf::Event::TextEntered)
			{
				if (m_pSceneMgr->GetpScene()->GetSceneNum() == SCENE_NUM::TITLE && 
					dynamic_cast<CTitle*>(m_pSceneMgr->GetpScene().get())->GetRectangle().getFillColor() == sf::Color::Blue && titleID.size() < NAME_SIZE - 1)
				{
					if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13)
					{
						titleID += static_cast<char>(event.text.unicode);
					}
				}
			}
		}
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

void CFramework::MouseClick(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow)
{
	m_pSceneMgr->MouseClick(btn, sfWindow);
}

void CFramework::Update()
{
	float ElapsedTime = m_sfFrame.getElapsedTime().asSeconds();
	m_pSceneMgr->Update(ElapsedTime);
	if (m_pSceneMgr->GetpScene()->GetSceneNum() == SCENE_NUM::TITLE)
	{
		dynamic_cast<CTitle*>(m_pSceneMgr->GetpScene().get())->SetPlayerID(titleID);
	}
	// FPS
	m_fTime += m_sfFrame.getElapsedTime().asMilliseconds();
	++m_iFrame;
	if (m_fTime - m_fBaseTime > 1000) {
		//cout << "FPS : " << m_iFrame << endl;
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

DWORD WINAPI CFramework::RecvProcess(LPVOID arg)
{
	RecVThreadParameter* pParameter = reinterpret_cast<RecVThreadParameter*> (arg);
	while (pParameter->p->GetRenderWindow()->isOpen())
	{
		//packet Recv		
		pParameter->pNetMgr->RecvPacket(pParameter->pSceneMgr->GetpScene().get(), pParameter->pSceneMgr->GetpPlayer());
	}

	return 0;
}

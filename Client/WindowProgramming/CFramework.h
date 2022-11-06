#pragma once

struct RecVThreadParameter
{
	CFramework* p;
	shared_ptr<CNetworkMgr> pNetMgr;
	shared_ptr<CSceneMgr> pSceneMgr;
};

class CSceneMgr;
class CNetworkMgr;

class CFramework
{
public:
	CFramework();
	virtual ~CFramework();

	void Process();
	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

	const sf::RenderWindow* GetRenderWindow() const{ return &m_sfWindow; };

private:
	void Update();
	void Render(sf::RenderWindow& RW);
	static DWORD WINAPI RecvProcess(LPVOID arg);

private:
	shared_ptr<CSceneMgr> m_pSceneMgr;
	shared_ptr<CNetworkMgr> m_pNetworkMgr;
	sf::RenderWindow m_sfWindow;
	sf::Clock m_sfFrame;

	int m_iFrame = 0;
	float m_fTime = 0.f;
	float m_fBaseTime = 0.f;
	
	RecVThreadParameter* m_pRTParameter;
};


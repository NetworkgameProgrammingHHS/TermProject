#pragma once

class CSceneMgr;
class CNetworkMgr;
struct RecVThreadParameter;

class CFramework
{
public:
	CFramework(string ip);
	virtual ~CFramework();

	void Process();
	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);
	void MouseClick(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow );

	const sf::RenderWindow* GetRenderWindow() const{ return &m_sfWindow; };

private:
	void Update();
	void Render(sf::RenderWindow& RW);
	static DWORD WINAPI RecvProcess(LPVOID arg);

private:
	shared_ptr<CSceneMgr> m_pSceneMgr = nullptr;
	shared_ptr<CNetworkMgr> m_pNetworkMgr = nullptr;
	sf::RenderWindow m_sfWindow = {};
	sf::Clock m_sfFrame = {};

	int m_iFrame = 0;
	float m_fTime = 0.f;
	float m_fBaseTime = 0.f;
	string titleID ="";
	
	RecVThreadParameter* m_pRTParameter = nullptr;
};

struct RecVThreadParameter
{
	CFramework* p = {};
	shared_ptr<CNetworkMgr> pNetMgr = {};
	shared_ptr<CSceneMgr> pSceneMgr = {};
};

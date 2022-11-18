#pragma once

class CScene;
class CPlayer;
class CNetworkMgr;

class CSceneMgr
{
public:
	CSceneMgr(shared_ptr<CNetworkMgr> networkmgr);
	virtual ~CSceneMgr();

	void Initialize();
	void Update(const float ElpasedTime);
	void Render(sf::RenderWindow& RW);

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

	const shared_ptr<CScene> GetpScene() const { return m_pScene; }
	const array<shared_ptr<CPlayer>, PLAYERNUM> GetpPlayer() const { return m_ppPlayers; }

private:
	void Next_Stage();

private:
	shared_ptr<CNetworkMgr> m_pNetworkMgr = nullptr;
	shared_ptr<CScene> m_pScene = nullptr;
	array<shared_ptr<CPlayer>, PLAYERNUM> m_ppPlayers;
	SCENE_NUM m_eCurScene;
};


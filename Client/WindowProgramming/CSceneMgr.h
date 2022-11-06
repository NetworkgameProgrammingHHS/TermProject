#pragma once

class CScene;
class CPlayer;

class CSceneMgr
{
public:
	CSceneMgr();
	virtual ~CSceneMgr();

	void Initialize();
	void Update(const float ElpasedTime);
	void Render(sf::RenderWindow& RW);

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

	const shared_ptr<CScene> GetpScene() const { return m_pScene; }
	const shared_ptr<CPlayer> GetpPlayer() const { return m_pPlayer; }

private:
	void Next_Stage();

private:
	shared_ptr<CScene> m_pScene = nullptr;
	shared_ptr<CPlayer> m_pPlayer = nullptr;
	SCENE_NUM m_eCurScene;
};


#pragma once

class CScene;

class CFramework
{
public:
	CFramework();
	virtual ~CFramework();

	void Process();
	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

private:
	void Update();
	void Render(sf::RenderWindow& RW);

private:
	unique_ptr<CScene> m_pScene;
	sf::RenderWindow m_sfWindow;
	sf::Clock m_sfFrame;

	int m_iFrame = 0;
	float m_fTime = 0.f;
	float m_fBaseTime = 0.f;
};


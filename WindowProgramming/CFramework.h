#pragma once

class CScene;

class CFramework
{
public:
	CFramework();
	virtual ~CFramework();

	void Process();
	void KeyBoardInput(sf::Keyboard::Key key);

private:
	void Update();
	void Render(sf::RenderWindow& RW);

private:
	CScene* m_pScene;
	sf::RenderWindow m_sfWindow;
};


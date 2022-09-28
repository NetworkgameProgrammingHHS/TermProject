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
	CScene* m_pScene;
};


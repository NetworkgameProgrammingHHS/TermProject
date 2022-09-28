#pragma once

class CPlayer;

class CScene
{
public:
	CScene();
	virtual ~CScene();

	void Update();
	void KeyBoardInput(sf::Keyboard::Key key);

private:
	CPlayer* m_pPlayer;
};


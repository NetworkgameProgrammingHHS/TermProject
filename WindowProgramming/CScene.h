#pragma once

class CPlayer;

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void Update();
	virtual void Render(sf::RenderWindow& RW);

	void KeyBoardInput(sf::Keyboard::Key key);

private:
	CPlayer* m_pPlayer;
};


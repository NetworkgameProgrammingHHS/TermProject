#pragma once

class CPlayer;

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

private:
	CPlayer* m_pPlayer;
};


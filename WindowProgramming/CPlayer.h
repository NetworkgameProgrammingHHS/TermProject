#pragma once
class CPlayer
{
public:
	CPlayer();
	virtual ~CPlayer();

	void Update();
	void Render(sf::RenderWindow& RW);

	void KeyBoardInput(sf::Keyboard::Key key);

private:
	sf::Texture m_sfTexture;
	sf::Sprite m_sfSprite;
};


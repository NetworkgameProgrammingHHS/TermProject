#pragma once

class CPlayer
{
public:
	CPlayer();
	virtual ~CPlayer();

	void Update(const float ElpasedTime);
	void Render(sf::RenderWindow& RW);

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

private:
	sf::Texture m_sfTexture;
	sf::Sprite m_sfSprite;

	sf::Vector2f m_vec2fPos;
	int m_iDir = 0;
};


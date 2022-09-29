#pragma once

class CObject
{
public:
	CObject();
	virtual ~CObject();

protected:
	sf::Texture m_sfTexture = {};
	sf::Sprite m_sfSprite = {};

	sf::Vector2f m_vec2fPos = {};
};


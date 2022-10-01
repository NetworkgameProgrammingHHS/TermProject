#pragma once

class CObject
{
public:
	CObject();
	virtual ~CObject();

	virtual void SetPosition(const sf::Vector2f& vec) { m_sfSprite.setPosition(vec); }

protected:
	sf::Texture m_sfTexture = {};
	sf::Sprite m_sfSprite = {};

	sf::Vector2f m_vec2fPos = {};
};


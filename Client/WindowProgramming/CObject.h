#pragma once

class CObject
{
public:
	CObject();
	virtual ~CObject();

	virtual void SetPosition(const sf::Vector2f& vec) { m_sfSprite.setPosition(vec); }
	void SetAABB(const sf::Vector2f& pos, const sf::Vector2f& size) { m_rtAABB.left = pos.x; m_rtAABB.top = pos.y; m_rtAABB.width = size.x; m_rtAABB.height = size.y; }

	const sf::Sprite GetSprite() const { return m_sfSprite; }
	const sf::Rect<float>& GetAABB() const { return m_rtAABB; }

	virtual void UpdateAABB();

protected:
	sf::Texture m_sfTexture = {};
	sf::Sprite m_sfSprite = {};

	sf::Rect<float> m_rtAABB = {};
	sf::Vector2f m_vec2fPos = {};
};


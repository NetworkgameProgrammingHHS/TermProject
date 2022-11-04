#pragma once

class CScene;
class CPlayer;

class CTitle : public CScene
{
public:
	CTitle();
	virtual ~CTitle();

	virtual void KeyBoardInput(const sf::Keyboard::Key& key);
	virtual void Update(const float ElapsedTime);
	virtual void Render(sf::RenderWindow& RW);

private:
	sf::Texture m_sfTexture2 = {};
	sf::Sprite m_sfBackground2 = {};
	float m_fTime = 0.f;
};

#pragma once

class CPlayer;
class CTileMap;

class CScene
{
public:
	CScene();
	virtual ~CScene();

	void Initialize();
	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);

	void KeyBoardInput(const sf::Keyboard::Key& key);
	void KeyBoardRelease(const sf::Keyboard::Key& key);

protected:
	CPlayer* m_pPlayer = nullptr;
	CScene* m_pStage = nullptr;
	CTileMap* m_pTileMap = nullptr;

	sf::Texture m_sfTexture;
	sf::Sprite m_sfBackground;
};


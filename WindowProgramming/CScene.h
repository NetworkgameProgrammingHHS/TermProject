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
	shared_ptr<CPlayer> m_pPlayer = nullptr;
	shared_ptr<CScene> m_pStage = nullptr;
	unique_ptr<CTileMap> m_pTileMap = nullptr;

	sf::Texture m_sfTexture;
	sf::Sprite m_sfBackground;
};


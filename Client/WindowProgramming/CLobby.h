#pragma once
class CScene;
class CPlayer;
class CNetworkMgr;


class CLobby : public CScene
{
public:
	CLobby(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM>  players);
	virtual ~CLobby();
	virtual void KeyBoardInput(const sf::Keyboard::Key& key);
	virtual void Update(const float ElapsedTime);
	virtual void Render(sf::RenderWindow& RW);
private:
	array<sf::Sprite,3> m_sfPlayer;
	sf::RectangleShape m_Rectangle;
	sf::Texture m_sfPlayerTexture;
	sf::Vertex Left_lines[2];
	sf::Vertex Right_lines[2];
	array<sf::Text, 3> m_sfReadyText;
	array<sf::Text, 3> m_sfPlayerText;
	array<sf::Text, 3> m_sfNameText;
	sf::Font m_sfFont;
};

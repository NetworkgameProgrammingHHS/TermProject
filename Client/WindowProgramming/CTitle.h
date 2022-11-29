#pragma once

class CScene;
class CPlayer;
class CNetworkMgr;

class CTitle : public CScene
{
public:
	CTitle(shared_ptr<CNetworkMgr> netwokrmgr);
	virtual ~CTitle();

	virtual void KeyBoardInput(const sf::Keyboard::Key& key);
	virtual void Update(const float ElapsedTime);
	virtual void Render(sf::RenderWindow& RW);
	virtual void MouseClickInput(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow);

	void SetPlayerID(string id) { m_strPlayerID = id; }

	sf::RectangleShape& GetRectangle() { return rectangle; }

private:
	sf::Texture m_sfTexture2 = {};
	sf::Sprite m_sfBackground2 = {};
	sf::RectangleShape rectangle;
	sf::Text text;
	string m_strPlayerID;
	float m_fTime = 0.f;
};

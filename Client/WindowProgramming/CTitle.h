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

	sf::RectangleShape& GetRectangle() { return m_OutRectangle; }

private:
	sf::RectangleShape m_Rectangle;
	sf::RectangleShape m_OutRectangle;
	sf::Text m_Text;
	sf::Text m_TextID;
	sf::Font m_sfTitleFont;
	sf::Vertex lines[2];
	string m_strPlayerID = "";
	float m_fTime = 0.f;
};

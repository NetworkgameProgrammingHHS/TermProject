#pragma once

class CScene;
class CPlayer;
class CNetworkMgr;

class CRanking :public CScene
{
public:
	CRanking(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM> players);
	virtual ~CRanking();

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);

private:

	sf::Text m_Text;
	sf::Font m_Font;
};
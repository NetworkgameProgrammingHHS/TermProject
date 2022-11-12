#pragma once

class CScene;
class CPlayer;
class CNetworkMgr;

class CRanking :public CScene
{
public:
	CRanking(shared_ptr<CNetworkMgr> networkmgr, shared_ptr<CPlayer> player);
	virtual ~CRanking();

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};
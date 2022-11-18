#pragma once

class CScene;
class CPlayer;
class CNetworkMgr;

class CStage3 : public CScene
{
public:
	CStage3(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM>  players);
	virtual ~CStage3();

	virtual void Next_Stage();
	virtual void Reset();
	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};


#pragma once

class CScene;
class CPlayer;
class CNetworkMgr;

class CStage4 : public CScene
{
public:
	CStage4(shared_ptr<CNetworkMgr> networkmgr, shared_ptr<CPlayer> player);
	virtual ~CStage4();

	virtual void Next_Stage();
	virtual void Reset();
	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};


#pragma once

class CScene;
class CPlayer;

class CStage1 : public CScene
{
public:
	CStage1(shared_ptr<CPlayer> player);
	virtual ~CStage1();

	virtual bool Next_Stage();
	virtual void Reset();
	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};


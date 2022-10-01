#pragma once

class CScene;
class CPlayer;

class CStage1 : public CScene
{
public:
	CStage1(CPlayer* player);
	virtual ~CStage1();

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};


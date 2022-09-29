#pragma once

class CScene;
class CStage1 : public CScene
{
public:
	CStage1();
	virtual ~CStage1();

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};


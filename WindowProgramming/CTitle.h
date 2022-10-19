#pragma once

class CScene;
class CPlayer;

class CTitle : public CScene
{
public:
	CTitle();
	virtual ~CTitle();

	virtual void Update(const float ElpasedTime);
	virtual void Render(sf::RenderWindow& RW);
};
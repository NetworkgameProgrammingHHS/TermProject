#include "pch.h"
#include "CGun.h"

CGun::CGun()
{
	m_sfTexture.loadFromFile("Resource\\Object\\Gun.png");
	m_sfSprite.setTexture(m_sfTexture);
}

CGun::~CGun()
{
	if (m_pBullet) delete m_pBullet;
}

void CGun::Render(sf::RenderWindow& RW)
{
	if (m_bEnable)
	{
		RW.draw(m_sfSprite);
	}
	else if (m_pBullet->GetEnable())
	{
		m_pBullet->Render(RW);
	}
}

void CGun::Update(const float ElapsedTime)
{
	m_pBullet->Update(ElapsedTime);
}
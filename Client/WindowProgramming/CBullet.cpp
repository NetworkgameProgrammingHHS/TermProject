#include "pch.h"
#include "CObject.h"
#include "CBullet.h"


CBullet::CBullet()
{
	m_sfTexture.loadFromFile("Resource\\Object\\Bullet.png");
	m_sfSprite.setTexture(m_sfTexture);
}

CBullet::~CBullet()
{
}

void CBullet::Update(const float ElapsedTime)
{
	m_vec2fPos.x += m_Dir * BULLET_SPEED * ElapsedTime;
}

void CBullet::Render(sf::RenderWindow& RW)
{
	if (m_bEnable)
	{
		RW.draw(m_sfSprite);
	}
}

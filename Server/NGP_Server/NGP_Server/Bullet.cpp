#include "pch.h"
#include "Bullet.h"

Bullet::Bullet()
{
	m_Direction = NULL;
}

void Bullet::Update(const float ElapsedTime)
{
	if (m_bShow)
	{
		switch (m_Direction) {
		case LEFT:
			m_Pos.x -= ElapsedTime * BULLET_SPEED;
			break;
		case RIGHT:
			m_Pos.x += ElapsedTime * BULLET_SPEED;
			break;
		}
	}
	
}

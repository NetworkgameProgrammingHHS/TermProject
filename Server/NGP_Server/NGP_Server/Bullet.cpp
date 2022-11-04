#include "pch.h"
#include "Bullet.h"

Bullet::Bullet()
{
	m_Direction = NULL;
}

void Bullet::Update(const float ElapsedTime)
{
	switch (m_Direction) {
	case DIR_LEFT:
		m_Pos.x -= ElapsedTime * BULLET_SPEED;
		break;
	case DIR_RIGHT:
		m_Pos.x += ElapsedTime * BULLET_SPEED;
		break;
	case DIR_UP:
		m_Pos.y -= ElapsedTime * BULLET_SPEED;
		break;
	case DIR_DOWN:
		m_Pos.y += ElapsedTime * BULLET_SPEED;
		break;
	}
}

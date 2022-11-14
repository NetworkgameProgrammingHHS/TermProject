#include "pch.h"
#include "Bullet.h"

Bullet::Bullet()
{
	m_Direction = NULL;
}

void Bullet::Update(const float ElapsedTime)
{
	switch (m_Direction) {
	case KEY_DIR_LEFT:
		m_Pos.x -= ElapsedTime * BULLET_SPEED;
		break;
	case KEY_DIR_RIGHT:
		m_Pos.x += ElapsedTime * BULLET_SPEED;
		break;
	case KEY_DIR_UP:
		m_Pos.y -= ElapsedTime * BULLET_SPEED;
		break;
	case KEY_DIR_DOWN:
		m_Pos.y += ElapsedTime * BULLET_SPEED;
		break;
	}
}

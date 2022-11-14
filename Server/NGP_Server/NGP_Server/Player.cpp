#include "pch.h"
#include "Player.h"

Player::Player()
{
	m_SockInfo = new SOCK_INFO;
}

Player::~Player()
{
	delete m_SockInfo;
}

void Player::Update(const float ElapsedTime)
{
	m_Pos.x += m_Velocity.x * ElapsedTime;
	m_Pos.y += m_Velocity.y * ElapsedTime;
}

void Player::CollideCheck(int x, int y, Vec2 bulletPos)
{
	//�޾ƿ� Ÿ�� �ε����� �浹 ���ڸ� �����.
	//�÷��̾��� �浹 ���ڸ� �����.

	if (/*Ÿ�ϰ� �浹���� ��*/0)
	{
		//�� �÷��̾ ���� ������ �ִ� �÷��̾�� �����, �� Ÿ���� �� Ÿ�Ϸ� �����.
		SetGun(true);
	}

	if (/*�Ѿ˰� �浹���� ��*/0)
	{
		//�� �÷��̾ �����Ѵٴ� ��Ŷ�� �����Ѵ�.
	}
}

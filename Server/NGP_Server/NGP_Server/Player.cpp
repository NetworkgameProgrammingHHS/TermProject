#include "pch.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(const float ElapsedTime)
{
	m_Pos.x += m_Velocity.x * ElapsedTime;
	m_Pos.y += m_Velocity.y * ElapsedTime;
}

void Player::CollideCheck(int tileW, int tileH)
{
	//받아온 타일 인덱스로 충돌 상자를 만든다.
	//플레이어의 충돌 상자를 만든다.

	if (/*타일과 충돌했을 때*/0)
	{
		//이 플레이어를 총을 가지고 있는 플레이어로 만들고, 그 타일을 빈 타일로 만든다.
		SetGun(true);
	}
}

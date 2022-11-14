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
	//받아온 타일 인덱스로 충돌 상자를 만든다.
	//플레이어의 충돌 상자를 만든다.

	if (/*타일과 충돌했을 때*/0)
	{
		//이 플레이어를 총을 가지고 있는 플레이어로 만들고, 그 타일을 빈 타일로 만든다.
		SetGun(true);
	}

	if (/*총알과 충돌했을 때*/0)
	{
		//이 플레이어를 리셋한다는 패킷을 전송한다.
	}
}

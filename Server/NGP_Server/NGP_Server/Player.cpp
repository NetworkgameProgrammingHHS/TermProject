#include "pch.h"
#include "Player.h"

Player::Player()
{
	m_SockInfo = new SOCK_INFO;
	m_Pos = { TILE_SIZE, WINDOW_HEIGHT - 2 * TILE_SIZE };
	m_Velocity = { PLAYER_SPEED, JUMP_SPEED };
	m_Jump = false;
	m_SuperJump = false;
	m_Fall = false;
	m_JumpChange = 10;
}

Player::~Player()
{
	delete m_SockInfo;
}

void Player::Update(const float ElapsedTime)
{
	m_Pos.x += PLAYER_SPEED * m_Direction * ElapsedTime;	

	if (m_Jump) {
		if (m_JumpCnt < m_JumpChange) {
			m_Pos.y -= m_Velocity.y * ElapsedTime;
		}
		else {
			m_Pos.y += m_Velocity.y * ElapsedTime;
		}
		++m_JumpCnt;
	}
	if (m_Fall)
		m_Pos.y += JUMP_SPEED * ElapsedTime;
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

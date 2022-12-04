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
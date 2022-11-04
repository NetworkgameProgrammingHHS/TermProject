#pragma once
#include "TileMap.h"

class Player
{
public:
	Player();
	virtual ~Player();

	const Vec2& GetPos() const { return m_Pos; }
	const Vec2& GetVelocity() const { return m_Velocity; }
	const char GetDirection() const { return m_Direction; }
	const PLAYER_COLOR& GetColor() const { return m_Color; }
	const bool GetOnline() const { return m_bOnline; }
	const bool GetGun() const { return m_bGun; }

	void SetPos(const Vec2& pos) { m_Pos = pos; }
	void SetVelocity(const Vec2& velocity) { m_Velocity = velocity; }
	void SetDirection(const char dir) { m_Direction = dir; }
	void SetColor(const PLAYER_COLOR& color) { m_Color = color; }
	void SetPos(const bool online) { m_bOnline = online; }
	void SetGun(const bool gun) { m_bGun = gun; }

	void Update(const float ElapsedTime);
	void CollideCheck(int tileW, int tileH);

private:
	Vec2 m_Pos;
	Vec2 m_Velocity;
	char m_Direction;
	PLAYER_COLOR m_Color;
	bool m_bOnline;
	bool m_bGun;
};


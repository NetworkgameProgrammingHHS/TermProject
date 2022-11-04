#pragma once

class Bullet
{
public:
	Bullet();
	~Bullet() {};

	const Vec2& GetPos() const { return m_Pos; }
	const char GetDirection() const { return m_Direction; }

	void SetPos(const Vec2& pos) { m_Pos = pos; }
	void SetDirection(const char dir) { m_Direction = dir; }

	void Update(const float ElapsedTime);

private:
	Vec2 m_Pos;
	char m_Direction;
};


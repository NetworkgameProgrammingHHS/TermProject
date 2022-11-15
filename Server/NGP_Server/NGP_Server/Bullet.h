#pragma once

class Bullet
{
public:
	Bullet();
	~Bullet() {};

	const Vec2& GetPos() const { return m_Pos; }
	const char GetDirection() const { return m_Direction; }
	const char GetStage() const { return m_Stage; }

	void SetPos(const Vec2& pos) { m_Pos = pos; }
	void SetDirection(const char dir) { m_Direction = dir; }
	void SetStage(const char& stage) { m_Stage = stage; }

	void Update(const float ElapsedTime);

private:
	Vec2 m_Pos;
	char m_Direction;
	char m_Stage;
};


#pragma once

class Bullet
{
public:
	Bullet();
	~Bullet() {};

	const Vec2& GetPos() const { return m_Pos; }
	const short GetDirection() const { return m_Direction; }
	const char GetStage() const { return m_Stage; }
	const bool GetbShow() const { return m_bShow; }


	void SetPos(const Vec2& pos) { m_Pos = pos; }
	void SetDirection(const char dir) { m_Direction = dir; }
	void SetStage(const char& stage) { m_Stage = stage; }
	void SetbShow(const bool show) { m_bShow = show; }
	

	void Update(const float ElapsedTime);

private:
	Vec2 m_Pos;
	short m_Direction;
	char m_Stage;
	bool m_bShow = false;
};


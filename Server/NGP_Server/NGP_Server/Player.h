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
	const char* GetName() const { return m_Client_Name; };
	const bool GetReady() const { return m_bReady; }
	const int GetStageNum() const { return m_iStageNum; }
	const bool GetKeyState() const { return m_bKeyState; }


	void SetPos(const Vec2& pos) { m_Pos = pos; }
	void SetVelocity(const Vec2& velocity) { m_Velocity = velocity; }
	void SetDirection(const char dir) { m_Direction = dir; }
	void SetColor(const PLAYER_COLOR& color) { m_Color = color; }
	void SetOnline(const bool online) { m_bOnline = online; }
	void SetGun(const bool gun) { m_bGun = gun; }
	void SetName(const char* name) { memcpy(m_Client_Name, name, NAME_SIZE); };
	void SetReady(const bool ready) { m_bReady = ready; }
	void SetStageNum(const bool stageNum) { m_iStageNum = stageNum; }
	void SetKeyState(const bool keyState) { m_bKeyState = keyState; }

	void Update(const float ElapsedTime);
	void CollideCheck(int x, int y, Vec2 bulletPos);

private:
	Vec2 m_Pos;
	Vec2 m_Velocity;
	char m_Direction;
	PLAYER_COLOR m_Color;
	bool m_bOnline;
	bool m_bGun;
	char m_Client_Name[NAME_SIZE];
	bool m_bReady; 
	bool m_bKeyState; // true = ket_press
	int  m_iStageNum;
};


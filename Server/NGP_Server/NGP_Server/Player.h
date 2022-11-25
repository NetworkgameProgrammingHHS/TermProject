#pragma once
#include "TileMap.h"

struct SOCK_INFO {
	SOCKET client_sock = NULL;
	int id = 0;
};

class Player
{
public:
	Player();
	virtual ~Player();

	const Vec2& GetPos() const { return m_Pos; }
	const Vec2& GetVelocity() const { return m_Velocity; }
	const short GetDirection() const { return m_Direction; }
	const PLAYER_COLOR& GetColor() const { return m_Color; }
	const bool GetOnline() const { return m_bOnline; }
	const bool GetGun() const { return m_bGun; }
	const char* GetName() const { return m_Client_Name; };
	const bool GetReady() const { return m_bReady; }
	const int GetStageNum() const { return m_iStageNum; }
	const bool GetKeyState() const { return m_bKeyState; }
	SOCK_INFO* GetSockInfo() const { return m_SockInfo; }
	SOCKET GetSocket() const { return m_SockInfo->client_sock; }
	const int GetID() const { return m_SockInfo->id; }
	const bool GetJump() const { return m_Jump; }
	const bool GetSuperJump() const { return m_SuperJump; }

	void SetPos(const Vec2& pos) { m_Pos = pos; }
	void SetVelocity(const Vec2& velocity) { m_Velocity = velocity; }
	void SetDirection(const short dir) { m_Direction = dir; }
	void SetColor(const PLAYER_COLOR& color) { m_Color = color; }
	void SetOnline(const bool online) { m_bOnline = online; }
	void SetGun(const bool gun) { m_bGun = gun; }
	void SetName(const char* name) { memcpy(m_Client_Name, name, NAME_SIZE); };
	void SetReady(const bool ready) { m_bReady = ready; }
	void SetStageNum(const bool stageNum) { m_iStageNum = stageNum; }
	void SetKeyState(const bool keyState) { m_bKeyState = keyState; }
	void SetSocket(const SOCKET& sock) { m_SockInfo->client_sock = sock; }
	void SetID(const int id) { m_SockInfo->id = id; }
	void SetJump(const bool jump) { m_Jump = jump; }
	void SetSuperJump(const bool Sjump) { m_SuperJump = Sjump; }

	void Update(const float ElapsedTime);
	void CollideCheck(int x, int y, Vec2 bulletPos);

private:
	SOCK_INFO* m_SockInfo;

	Vec2 m_Pos;
	Vec2 m_Velocity;
	short m_Direction;
	PLAYER_COLOR m_Color;
	bool m_bOnline;
	bool m_bGun;
	char m_Client_Name[NAME_SIZE];
	bool m_bReady;
	bool m_bKeyState; // true = ket_press
	int  m_iStageNum;

	bool m_Jump;
	bool m_SuperJump;
	int m_JumpCnt;
	int m_JumpChange; //NormalJump 10, SuperJump 30
	bool m_Fall;
};


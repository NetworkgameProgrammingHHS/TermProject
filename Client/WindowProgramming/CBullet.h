#pragma once
#include "CObject.h"

constexpr float BULLET_SPEED = 300.0F;

class CBullet : public CObject
{
public:
	CBullet();
	virtual ~CBullet();

	void Render(sf::RenderWindow& RW);
	void SetEnable(bool enable) { m_bEnable = enable; }
	void SetDir(int dir) { m_Dir = dir; }
	const bool GetEnable() const { return m_bEnable; }
	const int GetDir() const { return m_Dir; }

private:
	bool		m_bEnable = false;
	int			m_Dir;
};


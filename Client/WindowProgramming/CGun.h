#pragma once
#include "CObject.h"
#include "CBullet.h"

class CGun : public CObject
{
public:
	CGun();
	virtual ~CGun();



	void Render(sf::RenderWindow& RW);
	void SetEnable(int enable);
	void SetBulletPos(int x, int y);
	void SetGunStage(SCENE_NUM in) { m_eCurScene = in; }
	const bool GetEnable() const { return m_bEnable; }
	const SCENE_NUM GetGunStage() const { return m_eCurScene; }

private:
	CBullet*		m_pBullet = NULL;
	bool			m_bEnable = false;
	SCENE_NUM m_eCurScene = SCENE_NUM::NONE;
};


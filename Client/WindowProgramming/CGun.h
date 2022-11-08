#pragma once
#include "CBullet.h"
#include "CObject.h"

class CGun : public CObject
{
public:
	CGun();
	virtual ~CGun();

	void Render(sf::RenderWindow& RW);
	void Update(const float ElapsedTime);
	void SetEnable(bool enable) { m_bEnable = enable; }
	const bool GetEnable() const { return m_bEnable; }

private:
	CBullet*		m_pBullet = NULL;
	bool			m_bEnable;
};


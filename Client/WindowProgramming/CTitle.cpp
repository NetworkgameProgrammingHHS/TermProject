#include "pch.h"
#include "CScene.h"
#include "CTitle.h"
#include "CNetworkMgr.h"
#include "CStage1.h"

CTitle::CTitle(shared_ptr<CNetworkMgr> networkmgr)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Title_1.png"))
		exit(1);
	if (!m_sfTexture2.loadFromFile("Resource\\BackGround\\Title_2.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	m_sfBackground2.setTexture(m_sfTexture2);
	m_sfBackground2.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pNetworkMgr = networkmgr;
	m_eCurScene = SCENE_NUM::TITLE;
}

CTitle::~CTitle()
{
}

void CTitle::KeyBoardInput(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Escape:
		exit(1);
		break;
	case sf::Keyboard::R:
	{
		CS_PLAYER_READY_PACKET* packet = new CS_PLAYER_READY_PACKET;
		packet->ready = READY_ON;
		packet->type = CS_PLAYER_READY;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_READY_PACKET));
		break;
	}
	
	default:
	{
		CS_LOGIN_PACKET* packet = new CS_LOGIN_PACKET;
		memcpy(packet->name, "test", sizeof("test"));
		packet->type = CS_LOGIN;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_LOGIN_PACKET));
		break;
	}
	}
}

void CTitle::Update(const float ElapsedTime)
{
	m_fTime += ElapsedTime;
	if (m_fTime > 1.0f) {
		m_fTime = 0.0f;
	}
}

void CTitle::Render(sf::RenderWindow& RW)
{
	if (m_fTime < 0.5f) {
		RW.draw(m_sfBackground);
	}
	else {
		RW.draw(m_sfBackground2);
	}
}

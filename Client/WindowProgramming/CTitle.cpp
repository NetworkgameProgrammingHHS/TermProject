#include "pch.h"
#include "CScene.h"
#include "CTitle.h"
#include "CNetworkMgr.h"
#include "CPlayer.h"
#include "CStage1.h"

CTitle::CTitle(shared_ptr<CNetworkMgr> networkmgr, shared_ptr<CPlayer> player)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Title_1.png"))
		exit(1);
	if (!m_sfTexture2.loadFromFile("Resource\\BackGround\\Title_2.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	m_sfBackground2.setTexture(m_sfTexture2);
	m_sfBackground2.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_pPlayer = player;

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
	default:
		m_bNext = true;
		CS_LOGIN_PACKET* packet = new CS_LOGIN_PACKET;
		memcpy(packet->name, "test", sizeof("test"));
		packet->type = CS_LOGIN;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_LOGIN_PACKET));
		break;
	}

	// After Login Scene is Created Use this code for player ready
	bool ready = false;
	if (ready) {
		// Sending Ready Packet to Server
		CS_PLAYER_READY_PACKET* packet = new CS_PLAYER_READY_PACKET;
		if (m_pPlayer->GetReady()) {
			packet->type = CS_PLAYER_READY;
			packet->ready = READY_OFF;
		}
		else {
			packet->type = CS_PLAYER_READY;
			packet->ready = READY_ON;
		}
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_READY));
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

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

	rectangle.setSize(sf::Vector2f(400, 30));
	rectangle.setOutlineColor(sf::Color::Green);
	rectangle.setOutlineThickness(2);
	rectangle.setPosition(WINDOW_WIDTH/2-200, WINDOW_HEIGHT - 100);

	text.setFont(m_sfFont); 
	text.setString("");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Blue);
	text.setStyle(sf::Text::Bold);
	text.setPosition(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 100);

	m_strPlayerID.reserve(NAME_SIZE);

	m_pNetworkMgr = networkmgr;
	m_eCurScene = SCENE_NUM::TITLE;
}

CTitle::~CTitle()
{
}

void CTitle::KeyBoardInput(const sf::Keyboard::Key& key)
{
	if (rectangle.getOutlineColor() != sf::Color::Blue)
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
			m_strPlayerID += '\0';
			memcpy(packet->name, m_strPlayerID.c_str(), m_strPlayerID.size());
			packet->type = CS_LOGIN;
			m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_LOGIN_PACKET));
			break;
		}
		}
	}
}

void CTitle::Update(const float ElapsedTime)
{
	m_fTime += ElapsedTime;
	if (m_fTime > 1.0f) {
		m_fTime = 0.0f;
	}
	text.setString(m_strPlayerID);
}

void CTitle::Render(sf::RenderWindow& RW)
{
	if (m_fTime < 0.5f) {
		RW.draw(m_sfBackground);
	}
	else {
		RW.draw(m_sfBackground2);
	}
	RW.draw(rectangle);
	RW.draw(text);
}

void CTitle::MouseClickInput(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow)
{
	switch (btn)
	{
	case sf::Mouse::Left:
	{
		sf::Vector2i temp = sf::Mouse::getPosition(sfWindow);
		sf::Vector2f tempRect = rectangle.getPosition();
		if ((temp.x >= tempRect.x && temp.x <= tempRect.x + rectangle.getSize().x) &&
			(temp.y >= tempRect.y && temp.y <= tempRect.y + rectangle.getSize().y))
		{
			rectangle.setOutlineColor(sf::Color::Blue);
		}
		else
			rectangle.setOutlineColor(sf::Color::Green);
		//cout << m_strPlayerID << endl;
		break;
	}
	default:
		rectangle.setOutlineColor(sf::Color::Green);
		break;
	}
}



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

	m_Rectangle.setSize(sf::Vector2f(400, 30));
	//m_Rectangle.setOutlineColor(sf::Color::Green);
	m_Rectangle.setOutlineThickness(0);
	m_Rectangle.setPosition(WINDOW_WIDTH/2-170, WINDOW_HEIGHT - 100);

	m_OuRectangle.setSize(sf::Vector2f(480, 60));
	m_OuRectangle.setFillColor(sf::Color::Green);
	m_OuRectangle.setOutlineThickness(0);
	m_OuRectangle.setPosition(WINDOW_WIDTH / 2 - 230, WINDOW_HEIGHT - 115);

	m_sfTitleFont.loadFromFile("Resource\\Font\\koverwatch.ttf");

	m_Text.setFont(m_sfTitleFont);
	m_Text.setString("");
	m_Text.setCharacterSize(24);
	m_Text.setFillColor(sf::Color::Blue);
	m_Text.setStyle(sf::Text::Bold);
	m_Text.setPosition(WINDOW_WIDTH / 2 - 170 + 5, WINDOW_HEIGHT - 100);


	m_TextID.setFont(m_sfTitleFont);
	m_TextID.setString("ID:");
	m_TextID.setCharacterSize(50);
	m_TextID.setFillColor(sf::Color::White);
	m_TextID.setStyle(sf::Text::Bold);
	m_TextID.setPosition(WINDOW_WIDTH / 2 - 220, WINDOW_HEIGHT - 115);

	lines[0] = sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2 - 165, WINDOW_HEIGHT - 72));
	lines[0].color = sf::Color::Black;
	lines[1] = sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2 - 165, WINDOW_HEIGHT - 98));
	lines[1].color = sf::Color::Black;

	m_strPlayerID.reserve(NAME_SIZE);

	m_pNetworkMgr = networkmgr;
	m_eCurScene = SCENE_NUM::TITLE;
}

CTitle::~CTitle()
{
}

void CTitle::KeyBoardInput(const sf::Keyboard::Key& key)
{
	if (m_OuRectangle.getFillColor() != sf::Color::Blue)
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
	m_Text.setString(m_strPlayerID);
	for (int i = 0; i < 2; ++i)
	{
		lines[i].position.x = m_Text.getPosition().x + m_Text.getGlobalBounds().width + 5;
	}
}

void CTitle::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfBackground);
	//if (m_fTime < 0.5f) {
	//	RW.draw(m_sfBackground);
	//}
	//else {
	//	RW.draw(m_sfBackground2);
	//}
	RW.draw(m_OuRectangle);
	RW.draw(m_Rectangle);
	RW.draw(m_TextID);
	RW.draw(m_Text);
	if (m_fTime < 0.5f && m_OuRectangle.getFillColor() == sf::Color::Blue)
		RW.draw(lines, 2, sf::Lines);
}

void CTitle::MouseClickInput(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow)
{
	switch (btn)
	{
	case sf::Mouse::Left:
	{
		sf::Vector2i temp = sf::Mouse::getPosition(sfWindow);
		sf::Vector2f tempRect = m_OuRectangle.getPosition();
		if ((temp.x >= tempRect.x && temp.x <= tempRect.x + m_OuRectangle.getSize().x) &&
			(temp.y >= tempRect.y && temp.y <= tempRect.y + m_OuRectangle.getSize().y))
		{
			m_OuRectangle.setFillColor(sf::Color::Blue);
		}
		else
			m_OuRectangle.setFillColor(sf::Color::Green);
		//cout << m_strPlayerID << endl;
		break;
	}
	default:
		m_OuRectangle.setFillColor(sf::Color::Green);
		break;
	}
}



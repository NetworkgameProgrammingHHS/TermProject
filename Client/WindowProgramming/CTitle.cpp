#include "pch.h"
#include "CScene.h"
#include "CTitle.h"
#include "CNetworkMgr.h"

CTitle::CTitle(shared_ptr<CNetworkMgr> networkmgr)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Title_1.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_Rectangle.setSize(sf::Vector2f(400, 30));
	m_Rectangle.setOutlineThickness(0);
	m_Rectangle.setPosition(WINDOW_WIDTH/2-170, WINDOW_HEIGHT - 100);

	m_OutRectangle.setSize(sf::Vector2f(480, 60));
	m_OutRectangle.setFillColor(sf::Color::Green);
	m_OutRectangle.setOutlineThickness(0);
	m_OutRectangle.setPosition(WINDOW_WIDTH / 2 - 230, WINDOW_HEIGHT - 115);

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
	switch (key) {
	case sf::Keyboard::Escape:
		exit(1);
		break;
	case sf::Keyboard::Enter:
	{
		if (m_strPlayerID != "")
		{
			CS_LOGIN_PACKET* packet = new CS_LOGIN_PACKET;
			m_strPlayerID += '\0';
			memcpy(packet->name, m_strPlayerID.c_str(), m_strPlayerID.size());
			packet->type = CS_LOGIN;
			m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_LOGIN_PACKET));
		}
		break;
	}
	default:
		break;
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
	RW.draw(m_OutRectangle);
	RW.draw(m_Rectangle);
	RW.draw(m_TextID);
	RW.draw(m_Text);
	if (m_fTime < 0.5f && m_OutRectangle.getFillColor() == sf::Color::Blue)
		RW.draw(lines, 2, sf::Lines);
}

void CTitle::MouseClickInput(const sf::Mouse::Button& btn, sf::RenderWindow& sfWindow)
{
	switch (btn)
	{
	case sf::Mouse::Left:
	{
		sf::Vector2i temp = sf::Mouse::getPosition(sfWindow);
		sf::Vector2f tempRect = m_OutRectangle.getPosition();
		if ((temp.x >= tempRect.x && temp.x <= tempRect.x + m_OutRectangle.getSize().x) &&
			(temp.y >= tempRect.y && temp.y <= tempRect.y + m_OutRectangle.getSize().y))
		{
			m_OutRectangle.setFillColor(sf::Color::Blue);
		}
		else
			m_OutRectangle.setFillColor(sf::Color::Green);
		break;
	}
	default:
		m_OutRectangle.setFillColor(sf::Color::Green);
		break;
	}
}



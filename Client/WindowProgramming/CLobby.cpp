#include "pch.h"
#include "CScene.h"
#include "CLobby.h"
#include "CNetworkMgr.h"
#include "CStage1.h"

CLobby::CLobby(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM>  players)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\White_background.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	m_pNetworkMgr = networkmgr;
	m_eCurScene = SCENE_NUM::LOBBY;

	m_ppPlayers = players;
	m_sfPlayerTexture.loadFromFile("Resource\\Character\\Scientist.png");

	for (int i = 0; i < 3; ++i)
	{
		m_sfPlayer[i].setTexture(m_sfPlayerTexture);
		m_sfPlayer[i].setTextureRect(sf::IntRect(0, 32, 32, 32));
		m_sfPlayer[i].scale(3.f, 3.f);
		m_sfPlayer[i].setPosition(100 + i*250, WINDOW_HEIGHT / 2);
	}
	float Left_lineX = WINDOW_WIDTH / 3.f;
	float Right_lineX = WINDOW_WIDTH / 3.f * 2.f;
	Left_lines[0] = sf::Vertex(sf::Vector2f(Left_lineX, 0));
	Left_lines[0].color = sf::Color::Black;
	Left_lines[1] = sf::Vertex(sf::Vector2f(Left_lineX, WINDOW_HEIGHT));
	Left_lines[1].color = sf::Color::Black;
	Right_lines[0] = sf::Vertex(sf::Vector2f(Right_lineX, 0));
	Right_lines[0].color = sf::Color::Black;
	Right_lines[1] = sf::Vertex(sf::Vector2f(Right_lineX, WINDOW_HEIGHT));
	Right_lines[1].color = sf::Color::Black;

	m_sfFont.loadFromFile("Resource\\Font\\koverwatch.ttf");

	for (int i = 0; i < 3; ++i)
	{
		m_sfReadyText[i].setFont(m_sfFont);
		m_sfReadyText[i].setString("Ready");
		m_sfReadyText[i].setCharacterSize(72);
		m_sfReadyText[i].setFillColor(sf::Color::Red);
		m_sfReadyText[i].setStyle(sf::Text::Bold);
		m_sfReadyText[i].setPosition(m_sfPlayer[i].getPosition().x, WINDOW_HEIGHT / 2 + 170);

		m_sfPlayerText[i].setFont(m_sfFont);
		m_sfPlayerText[i].setCharacterSize(72);
		m_sfPlayerText[i].setFillColor(sf::Color::Red);
		m_sfPlayerText[i].setStyle(sf::Text::Bold);
		m_sfPlayerText[i].setPosition(m_sfPlayer[i].getPosition().x, 50);

		m_sfNameText[i].setFont(m_sfFont);
		m_sfNameText[i].setCharacterSize(36);
		m_sfNameText[i].setFillColor(sf::Color::Red);
		m_sfNameText[i].setStyle(sf::Text::Bold);
		m_sfNameText[i].setPosition(m_sfPlayer[i].getPosition().x + 15, 150);

	}
	m_sfReadyText[0].setPosition(m_sfReadyText[0].getPosition().x - 20, m_sfReadyText[0].getPosition().y);
	m_sfReadyText[1].setPosition(m_sfReadyText[1].getPosition().x - 20, m_sfReadyText[1].getPosition().y);
	m_sfPlayerText[0].setPosition(m_sfReadyText[0].getPosition().x - 20, m_sfPlayerText[0].getPosition().y);
	m_sfPlayerText[1].setPosition(m_sfReadyText[1].getPosition().x - 20, m_sfPlayerText[1].getPosition().y);
	m_sfPlayerText[2].setPosition(m_sfReadyText[2].getPosition().x - 20, m_sfPlayerText[2].getPosition().y);
	m_sfNameText[1].setPosition(m_sfNameText[1].getPosition().x + 10, m_sfNameText[1].getPosition().y);
	m_sfNameText[2].setPosition(m_sfNameText[2].getPosition().x + 20, m_sfNameText[2].getPosition().y);
	m_sfPlayerText[0].setString("Player1");
	m_sfPlayerText[1].setString("Player2");
	m_sfPlayerText[2].setString("Player3");


}

CLobby::~CLobby()
{
}

void CLobby::KeyBoardInput(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Escape:
		exit(1);
		break;
	case sf::Keyboard::R:
	{
		CS_PLAYER_READY_PACKET* packet = new CS_PLAYER_READY_PACKET;
		if (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetReady())
		{
			packet->ready = READY_ON;
			packet->type = CS_PLAYER_READY;
		}
		else
		{
			packet->ready = READY_OFF;
			packet->type = CS_PLAYER_READY;
		}
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_READY_PACKET));
		break;
	}
	default:
		break;
	}
}

void CLobby::Update(const float ElapsedTime)
{
	for (int i = 0; i < 3; ++i)
	{
		m_sfNameText[i].setString(m_pNetworkMgr->GetPlayerInfoText()[i][0].getString());
	}
}

void CLobby::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfBackground);
	for (int i = 0; i < 3; ++i)
	{
		if (m_ppPlayers[i]->GetOnline())
		{
			RW.draw(m_sfPlayer[i]);
			RW.draw(m_sfPlayerText[i]);
			RW.draw(m_sfNameText[i]);
		}
		if (m_ppPlayers[i]->GetReady())
			RW.draw(m_sfReadyText[i]);
	}
	RW.draw(Left_lines, 2, sf::Lines);
	RW.draw(Right_lines, 2, sf::Lines);
}

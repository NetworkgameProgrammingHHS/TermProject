#include "pch.h"
#include "CScene.h"
#include "CRanking.h"
#include "CNetworkMgr.h"
#include "CTileMap.h"
#include "CObject.h"
#include "CPlayer.h"

CRanking::CRanking(shared_ptr<CNetworkMgr> networkmgr, array<shared_ptr<CPlayer>, PLAYERNUM> players)
{
	if (!m_sfTexture.loadFromFile("Resource\\BackGround\\Central_Room.png"))
		exit(1);
	m_sfBackground.setTexture(m_sfTexture);
	m_sfBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	m_Font.loadFromFile("Resource\\Font\\koverwatch.ttf");

	m_Text = networkmgr->GetWinnerName();
	m_Text.setFont(m_Font);
	m_Text.setCharacterSize(50);
	m_Text.setFillColor(sf::Color::White);
	m_Text.setStyle(sf::Text::Bold);
	m_Text.setPosition(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 100);

	m_ppPlayers = players;

	m_pNetworkMgr = networkmgr;
	m_eCurScene = SCENE_NUM::RANKING;
}

CRanking::~CRanking()
{
}

void CRanking::Update(const float ElpasedTime)
{
}

void CRanking::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfBackground);
	
	m_Text.setFillColor(sf::Color::Black);
	m_Text.setCharacterSize(45);
	m_Text.setPosition(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 150);
	RW.draw(m_Text);
	m_Text.setFillColor(sf::Color::White);
	m_Text.setCharacterSize(40);
	m_Text.setPosition(WINDOW_WIDTH / 2 - 47.5, WINDOW_HEIGHT - 147.5);
	RW.draw(m_Text);

	sf::Text win;
	win.setFont(m_Font);
	win.setString("Win!!!");
	win.setCharacterSize(45);
	win.setFillColor(sf::Color::Black);
	win.setStyle(sf::Text::Bold);
	win.setPosition(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT - 150);

	RW.draw(win);
	win.setFillColor(sf::Color::White);
	win.setCharacterSize(40);
	win.setPosition(WINDOW_WIDTH / 2 + 52.5, WINDOW_HEIGHT - 147.5);
	RW.draw(win);

	sf::Sprite WinnerPlayer;

	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (m_Text.getString() == m_pNetworkMgr->GetPlayerInfoText()[i][0].getString())
		{
			WinnerPlayer = m_ppPlayers[i]->GetSprite();
			break;
		}
	}
	WinnerPlayer.setScale(sf::Vector2f(10.0f, 10.0f));
	WinnerPlayer.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - (TILE_SIZE*5), WINDOW_HEIGHT/2 - (TILE_SIZE * 5)));
	RW.draw(WinnerPlayer);
}

#include "pch.h"
#include "CFramework.h"

#define _WIN32_WINNT 0x0500

int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	sf::RenderWindow IPwindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Input IP Address");
	string titleID;

	sf::Font m_sfTitleFont;
	sf::Text m_Text;
	sf::Text m_TextID;
	sf::Vertex lines[2];
	sf::RectangleShape m_OutRectangle;
	float m_fTime =0.f;

	m_OutRectangle.setSize(sf::Vector2f(480, 60));
	m_OutRectangle.setFillColor(sf::Color::White);
	m_OutRectangle.setOutlineThickness(0);
	m_OutRectangle.setPosition(WINDOW_WIDTH / 2 - 230, WINDOW_HEIGHT / 2);

	m_sfTitleFont.loadFromFile("Resource\\Font\\koverwatch.ttf");
	m_Text.setFont(m_sfTitleFont);
	m_Text.setString("");
	m_Text.setCharacterSize(24);
	m_Text.setFillColor(sf::Color::Black);
	m_Text.setStyle(sf::Text::Bold);
	m_Text.setPosition(WINDOW_WIDTH / 2 - 170 + 5, WINDOW_HEIGHT / 2 + 15);

	m_TextID.setFont(m_sfTitleFont);
	m_TextID.setString("IP:");
	m_TextID.setCharacterSize(50);
	m_TextID.setFillColor(sf::Color::Black);
	m_TextID.setStyle(sf::Text::Bold);
	m_TextID.setPosition(WINDOW_WIDTH / 2 - 220, WINDOW_HEIGHT / 2);

	lines[0] = sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2 - 165, WINDOW_HEIGHT / 2 + 45));
	lines[0].color = sf::Color::Black;
	lines[1] = sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2 - 165, WINDOW_HEIGHT / 2 + 15));
	lines[1].color = sf::Color::Black;

	IPwindow.setFramerateLimit(60);
	sf::Clock clock;
	while (IPwindow.isOpen())
	{
		m_fTime += clock.getElapsedTime().asSeconds();
		sf::Event event;
		while (IPwindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				IPwindow.close();
			if (event.type == sf::Event::TextEntered)
			{
				if (titleID.size() < INET_ADDRSTRLEN)
				{
					if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13)
					{
						titleID += static_cast<char>(event.text.unicode);
						m_Text.setString(titleID);
						for (int i = 0; i < 2; ++i)
						{
							lines[i].position.x = m_Text.getPosition().x + m_Text.getGlobalBounds().width + 2;
						}
					}
					else if (event.text.unicode == 8 && titleID.size() > 0) {
						titleID.erase(titleID.size() - 1);
						m_Text.setString(titleID);
						for (int i = 0; i < 2; ++i)
						{
							lines[i].position.x = m_Text.getPosition().x + m_Text.getGlobalBounds().width + 2;
						}
					}
					else if (event.text.unicode == 13) {
						IPwindow.close();

					}
				}
			}
		}
		clock.restart();
		IPwindow.clear();

		IPwindow.draw(m_OutRectangle);
		IPwindow.draw(m_TextID);
		IPwindow.draw(m_Text);
		if (m_fTime < 0.5f) {
			IPwindow.draw(lines, 2, sf::Lines);
		}
		if (m_fTime > 1.f) {
			m_fTime = 0.f;
		}

		IPwindow.display();
	}

	CFramework* Framework = new CFramework(titleID);
	
	Framework->Process();

	delete Framework;

	return 0;
}
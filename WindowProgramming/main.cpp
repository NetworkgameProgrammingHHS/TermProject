#include "pch.h"
#include "CFramework.h"
sf::RenderWindow* g_window;
CFramework* g_pFramework;

int main()
{
	g_pFramework = new CFramework();

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Client");
	window.setFramerateLimit(60);

	g_window = &window;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				g_pFramework->KeyBoardInput(event.key.code);
		}

		g_pFramework->Process();
		
		window.clear();
		window.display();
	}
	
	delete g_pFramework;

	return 0;
}
#include "PCH.h"

#include "core/Scene.h"


#ifdef _DEBUG
	#define WINDOW_SIZE sf::VideoMode(1920, 980)
	#define WINDOW_MODE sf::Style::Default
#else
	#define WINDOW_SIZE sf::VideoMode(1920, 1080)
	#define WINDOW_MODE sf::Style::Fullscreen
#endif


int main()
{
	sf::RenderWindow window(WINDOW_SIZE, "Tree Game", WINDOW_MODE);
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;

	Scene level("res/scenes/scene.json");
	level.p_window = &window;

	sf::Event event;
	while (window.isOpen())
	{
		const float deltatime = clock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
			}

			level.handleEvent(event);
		}

		level.update(deltatime);

		window.clear();
		level.render(window);
		window.display();
	}
}
#include <SFML/Graphics.hpp>
#include <maze.h>
#include <botmaze.h>
#include <iostream>

using namespace botmaze;

int WinMain()
{
	sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight), "NEATmaze");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	int count = 0;
	Maze maze;
	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Up:
					maze.UpKeyDown();
					break;
				case sf::Keyboard::Down:
					maze.DownKeyDown();
					break;
				case sf::Keyboard::Left:
					maze.LeftKeyDown();
					break;
				case sf::Keyboard::Right:
					maze.RightKeyDown();
					break;
				case sf::Keyboard::Space:
					maze.NewRound();
					break;
				case sf::Keyboard::Delete:
					maze.Setup(kGrid, kBlockSize);
					break;
				case sf::Keyboard::Escape:
					window.close();
					return EXIT_SUCCESS;
					break;
				default:
					break;
				}
			}

			if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code) {
				case sf::Keyboard::Up:
					maze.UpKeyUp();
					break;
				case sf::Keyboard::Down:
					maze.DownKeyUp();
					break;
				case sf::Keyboard::Left:
					maze.LeftKeyUp();
					break;
				case sf::Keyboard::Right:
					maze.RightKeyUp();
					break;
				default:
					break;
				}
			}
		}
		maze.Update();
		maze.Draw(window);
	}
	return 0;
}
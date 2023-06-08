#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <math.h>
#include <plot.hpp>

const int winSize_x = 1920;
const int winSize_y = 1080;
sf::RenderWindow window(sf::VideoMode(winSize_x, winSize_y), "Neural Net");
float deltaTime;

int main()
{
	sf::Font font;
			font.loadFromFile("assets/ari.ttf");
	sf::Clock dt;
	bool dragging = false;

	float deltatime = 0.f;
	sf::Clock clock;
	sf::Vector2i graphSize(winSize_x / 2, winSize_y / 2);

	std::vector<point> points = getRandomPoints(graphSize);
	splitInTwoGroups(&points, sf::Color::Blue);

	plot scatter_plot(points, graphSize, 5, &window);

	while (window.isOpen())
	{

		double frameTime = dt.restart().asSeconds();
		deltaTime = (frameTime < 1 / 60.0) ? frameTime : 1 / 60.0;
		sf::Event event;

		sf::Vector2i point;

		while (window.pollEvent(event))
		{

			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed:

				if (event.mouseButton.button == sf::Mouse::Left)
				{

				}
				break;

			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					dragging = false;
				}
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}
		if (clock.getElapsedTime().asSeconds() >= 0)
		{
			scatter_plot.drawPlot();
			window.display();
			window.clear();
			clock.restart();
		}

	}
	return 0;
}

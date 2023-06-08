#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <math.h>
#include <neural/neural_network.hpp>

const int winSize_x = 800;
const int winSize_y = 600;
sf::RenderWindow window(sf::VideoMode(winSize_x, winSize_y), "Neural Net");
float deltaTime;

bool learn = false;
int pointCount = 100;
// HyperParameters (missing the layers, its down bellow)
double learnRate = 0.5;
int batchSize = 5;

int main()
{
	sf::Font font;
	font.loadFromFile("assets/ari.ttf");
	sf::Clock dt;
	bool dragging = false;

	float deltatime = 0.f;
	sf::Clock clock;
	sf::Vector2i graphSize(winSize_x / 2, winSize_y / 2);

	std::vector<Point> points = getRandomPoints(graphSize, 1000);
	splitInTwoGroups(&points, sf::Color::Blue);

	Plot scatter_plot(points, graphSize, 2, &window);
	std::vector<int> layerSizes;
	layerSizes.push_back(2);
	layerSizes.push_back(3);
	layerSizes.push_back(1);
	std::vector<std::vector<Point>> batches;
	NeuralNetwork neuralNetwork(layerSizes);

	while (window.isOpen())
	{

		double frameTime = dt.restart().asSeconds();
		deltaTime = (frameTime < 1 / 60.0) ? frameTime : 1 / 60.0;
		sf::Event event;

		sf::Vector2i Point;

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
				if (event.key.code == sf::Keyboard::Space)
					learn = !learn;
				if (event.key.code == sf::Keyboard::Right)
					learnRate *= 10;
				if (event.key.code == sf::Keyboard::Left)
					learnRate /= 10;
				if (event.key.code == sf::Keyboard::Q)
				{
					int seed = rand();
					std::mt19937 gen(seed);
					std::uniform_real_distribution<double> dis(-1, 1);
					
					for (size_t i = 0; i < neuralNetwork.layers.size(); i++)
					{
						for (int j = 0; j < neuralNetwork.layers[i].biases.size(); j++)
						{
							neuralNetwork.layers[i].biases[j] = dis(gen);
						}
					}
					
				}
			}
		}
		if (clock.getElapsedTime().asSeconds() >= 0)
		{
			sf::Uint8 *pixels = new sf::Uint8[graphSize.x * graphSize.y * 4];
			if (learn)
			{
				// split into batches
				points = shuffle(points);
				batches = getBatches(points, batchSize);

				for (int i = 0; i < batches.size(); i++)
				{
					neuralNetwork.learn(batches[i], learnRate);
				}
			}

			for (int x = 0; x < graphSize.x; x++)
			{
				for (int y = 0; y < graphSize.y; y++)
				{
					std::vector<double> inputs;
					inputs.push_back((((double)x) / graphSize.x) * 2 - 1);
					inputs.push_back(((graphSize.y-((double)y)) / graphSize.y) * 2 - 1);

					std::pair<int, std::vector<double>> result = neuralNetwork.classify(inputs);
					float value = result.second[0]; // assuming result.second[0] is a decimal value
					sf::Color c;


					c = (value > 0)?sf::Color::Blue:sf::Color::Red; 
					pixels[(x + y * graphSize.x) * 4] = c.r;	 // R?
					pixels[(x + y * graphSize.x) * 4 + 1] = c.g; // G?
					pixels[(x + y * graphSize.x) * 4 + 2] = c.b; // B?
					pixels[(x + y * graphSize.x) * 4 + 3] = 100; // A?
					
				}
			}
			int correct = 0;
			for (size_t i = 0; i < points.size(); i++)
			{
				double c = neuralNetwork.classify(points[i].inputs()).second[0];
				if ((points[i].getColor() == sf::Color::Blue && c > 0 )||
					(points[i].getColor() == sf::Color::Red && c <= 0))
					correct++;
			}

			scatter_plot.drawPlot(pixels);
			delete[] pixels;
			sf::Text text(std::to_string(((int)floor(1 / deltaTime))) + "\n" +
							  std::to_string(correct) + "/" + std::to_string(points.size()) + "\n" +
							  "Learn Rate: "+ std::to_string(learnRate),
						  font);
			text.setCharacterSize(10);
			text.setPosition(winSize_x * 0.05, winSize_y * 0.05);
			window.draw(text);
			window.display();
			window.clear();
			clock.restart();
		}
	}
	return 0;
}

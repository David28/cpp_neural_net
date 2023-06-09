#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <math.h>
#include <neural/neural_network.hpp>

const int winSize_x = 1920;
const int winSize_y = 1080;
sf::RenderWindow window(sf::VideoMode(winSize_x, winSize_y), "Neural Net");
float deltaTime;

bool learn = false;
int pointCount = 1000;
const int AA_SCALE = 5; // Anti-aliasing scaling factor
int epochCount = 0;
// HyperParameters (missing the layers, its down bellow)
double learnRate = 0.1;
int batchSize = 1;
ActivationType activation = SILU;

int main()
{
	sf::Font font;
	font.loadFromFile("assets/ari.ttf");
	sf::Clock dt;
	bool dragging = false;

	float deltatime = 0.f;
	sf::Clock clock;
	sf::Vector2i graphSize(winSize_x / 2, winSize_y / 2);
	sf::Vector2i costGraphSize(winSize_x / 8, winSize_y / 8);
	std::vector<Point> points = getRandomPoints(graphSize, pointCount);
	splitInTwoGroups(&points, sf::Color::Blue);

	ScatterPlot scatter_plot(points, graphSize, 2, &window);
	TimePlot costPlot(costGraphSize, &window);
	std::vector<int> layerSizes;
	layerSizes.push_back(2);
	layerSizes.push_back(5);
	layerSizes.push_back(1);
	std::vector<std::vector<Point>> batches;
	NeuralNetwork neuralNetwork(layerSizes, activation);
	// Create a render texture to hold the initial image at higher resolution
	sf::RenderTexture renderTexture;
	renderTexture.create(graphSize.x * AA_SCALE, graphSize.y * AA_SCALE);

	sf::VertexArray pixels(sf::PrimitiveType::Quads, graphSize.x * graphSize.y * 4);
	sf::Texture texture;
	int curBatch = 0;
	bool eachBatch = false;
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
				if (event.key.code == sf::Keyboard::C)
					eachBatch = !eachBatch;
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
			if (learn)
			{
				// split into batches
				if(curBatch == 0){
					points = shuffle(points);
					batches = getBatches(points, batchSize);
				}

				do
				{
					neuralNetwork.learn(batches[curBatch], learnRate);
					costPlot.addPoint(neuralNetwork.cost(points));

					curBatch = (curBatch+1)%batches.size();
				} while (eachBatch && curBatch != 0);
				
				
				if (curBatch == 0){
					//epoch ended
					epochCount++;
					renderTexture.clear(sf::Color(0, 0, 0, 0));
				// Draw each pixel to the render texture
				for (int x = 0; x < graphSize.x; x++)
				{
					for (int y = 0; y < graphSize.y; y++)
					{
						std::vector<double> inputs;
						inputs.push_back((((double)x) / graphSize.x) * 2 - 1);
						inputs.push_back((((double)y) / graphSize.y) * 2 - 1);

						std::pair<int, std::vector<double>> result = neuralNetwork.classify(inputs);
						float value = result.second[0]; // assuming result.second[0] is a decimal value

						sf::Color c;
						c = (value > 0) ? sf::Color::Blue : sf::Color::Red;
						c.a = 150;

						// Add the pixel to the vertex array, which is more of a rectangle but shaders are hard to make
						int index = (x + y * graphSize.x) * 4;
						pixels[index].position = sf::Vector2f(x * AA_SCALE, y * AA_SCALE);
						pixels[index + 1].position = sf::Vector2f(x * AA_SCALE + AA_SCALE, y * AA_SCALE);
						pixels[index + 2].position = sf::Vector2f(x * AA_SCALE + AA_SCALE, y * AA_SCALE + AA_SCALE);
						pixels[index + 3].position = sf::Vector2f(x * AA_SCALE, y * AA_SCALE);

						pixels[index].color = c;
						pixels[index + 1].color = c;
						pixels[index + 2].color = c;
						pixels[index + 3].color = c;
						// Draw the pixel at the higher resolution
						// sf::RectangleShape pixel(sf::Vector2f(AA_SCALE, AA_SCALE));
						// pixel.setFillColor(c);
						// pixel.setPosition(x * AA_SCALE, y * AA_SCALE);
						// renderTexture.draw(pixel);
					}
				}
				renderTexture.draw(pixels);
				// Get the final image from the render texture and scale it down for the anti-aliasing effect
				texture = renderTexture.getTexture();
				texture.setSmooth(true); // Enable SFML's built-in anti-aliasing
				} 

				
			}

			sf::Sprite sprite(texture);
			sprite.setScale(1.0f / AA_SCALE, 1.0f / AA_SCALE);
			int correct = 0;
			for (size_t i = 0; i < points.size(); i++)
			{
				double c = neuralNetwork.classify(points[i].inputs()).second[0];
				if ((points[i].getColor() == sf::Color::Blue && c > 0) ||
					(points[i].getColor() == sf::Color::Red && c < 0))
					correct++;
			}

			scatter_plot.drawPlot(sprite);
			costPlot.drawTimePlot();
			sf::Text text(std::to_string(((int)floor(1 / deltaTime))) + "\n" +
							  std::to_string(correct) + "/" + std::to_string(points.size()) + "\n" +
							  "Learn Rate: " + std::to_string(learnRate) + "\n" +
							  "Epoch: " + std::to_string(epochCount),
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

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <time.h>
#include <math.h>
#include <neural/neural_network.hpp>
#include <plot.hpp>
#include <menu.hpp>
const int winSize_x = 1920;
const int winSize_y = 1080;
sf::RenderWindow window(sf::VideoMode(winSize_x, winSize_y), "Neural Net");
float deltaTime;

bool learn = false;
int pointCount = 1000;
int epochCount = 0;
int dataset = 1; // (Temporary) but 0 for normal subgroups, 1 for radius
// HyperParameters (missing the layers, its down bellow)
double learnRate = 0.01;
int batchSize = 10;
Activation activation = Activation(RELU);

void updateUniforms(sf::Shader &shader, const std::vector<int> &layerSizes, const std::vector<float> &flatWeights, const std::vector<float> &flatBiases, sf::Vector2i graphSize)
{
	
	// Create the textures
	shader.setUniform("layerSizesL", (float) layerSizes.size());
	shader.setUniform("graphSize", sf::Glsl::Vec2(graphSize));
	shader.setUniform("actType", (int)activation.getType());
	std::vector<float> floatVec(layerSizes.begin(), layerSizes.end());
	// Bind the textures to the shader
	shader.setUniformArray("layerSizes", &floatVec[0],floatVec.size());
	shader.setUniformArray("flatWeights", &flatWeights[0],flatWeights.size());
	shader.setUniformArray("flatBiases", &flatBiases[0],flatBiases.size());
}




std::vector<float> getFlatWeights(NeuralNetwork network) {
	std::vector<float> flat;
	for (Layer l : network.layers) {
		flat.insert(flat.end(), l.weights.begin(), l.weights.end());
	}
	return flat;
}
std::vector<float> getFlatBiases(NeuralNetwork network) {
	std::vector<float> flat;
	for (Layer l : network.layers) {
		flat.insert(flat.end(), l.biases.begin(), l.biases.end());
	}
	return flat;
}

int main()
{
	sf::Shader shader;
	printf("Loading data...\n");
	// Load the shader from a file
	if (!shader.loadFromFile("assets/shaders/shader.vert", "assets/shaders/shader.frag"))
	{
		printf("Failed to load shaders...\n");

		return -1;
	}
	printf("Loaded shaders...\n");


	sf::Font font;
	if (!font.loadFromFile("assets/ari.ttf"))
	{
		printf("Failed to load font...\n");

		return -1;
	}
	printf("Loaded font...\n");
	
	sf::Clock dt;
	bool dragging = false;
	float deltatime = 0.f;
	sf::Clock clock;
	sf::Vector2i graphSize(winSize_x / 2, winSize_y / 2);
	sf::Vector2i costGraphSize(winSize_x / 8, winSize_y / 8);

	
		
	
	sf::Event event;

	Menu menu = Menu(&window, font);
	while (window.isOpen() && !menu.done)
	{
		while (window.pollEvent(event))
		{
			menu.updateMenu(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		menu.drawMenu();
		window.display();
	}
	pointCount = menu.options.pointCount;
	dataset = menu.options.dataset;
	learnRate = menu.options.learnRate;
	batchSize = menu.options.batchSize;
	activation = menu.options.activation;

	//EXpected Outputs
	std::vector<double> expectedRed = {activation.rescaleToActivation(-1, std::make_pair(-1.f,1.f))};
	std::vector<double> expectedBlue = {activation.rescaleToActivation(1, std::make_pair(-1.f,1.f))};


	std::vector<Point> points = getRandomPoints(graphSize, pointCount, expectedRed);
	if (dataset == 0)
		splitInGroups(&points, sf::Color::Blue, points.size()/2, 3, expectedBlue);
	else
		splitInRadius(&points, sf::Color::Blue, expectedBlue, 0.8,0.4);
	
	TimePlot costPlot(costGraphSize, &window);
	std::vector<int> layerSizes;
	layerSizes = (dataset == 0)? std::vector<int>{2,5,1}:std::vector<int>{2,10,5,1};
	std::vector<std::vector<Point>> batches;
	NeuralNetwork neuralNetwork(layerSizes, activation);
	updateUniforms(shader, layerSizes, getFlatWeights(neuralNetwork), getFlatBiases(neuralNetwork),graphSize);

	sf::Texture texture;
	texture.create(graphSize.x, graphSize.y);
			sf::Sprite sprite(texture);

	sf::VertexArray pixels(sf::PrimitiveType::Quads, graphSize.x * graphSize.y * 4);
	int curBatch = 0;
	bool eachBatch = false;
int correct = 0;
			double cost;
	ScatterPlot scatter_plot(points, graphSize, 2, &window);
	while (window.isOpen())
	{
		double frameTime = dt.restart().asSeconds();
		deltaTime = (frameTime < 1 / 60.0) ? frameTime : 1 / 60.0;


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
				
			}
		}
		if (clock.getElapsedTime().asSeconds() >= 0)
		{
			if (learn)
			{
				correct = 0;
				// split into batches
				if (curBatch == 0)
				{
					points = shuffle(points);
					batches = getBatches(points, batchSize);
				}

				do
				{
					neuralNetwork.learn(batches[curBatch], learnRate);
					

					curBatch = (curBatch + 1) % batches.size();
				} while (eachBatch && curBatch != 0);

				if (curBatch == 0)
				{
					epochCount++;
				}
				updateUniforms(shader, layerSizes, getFlatWeights(neuralNetwork), getFlatBiases(neuralNetwork),graphSize);
				for (size_t i = 0; i < points.size(); i++)
				{
					double c = neuralNetwork.classify(points[i].inputs()).second[0];
					double bound = (expectedBlue[0]-expectedRed[0])/2;
					if ((points[i].getColor() == sf::Color::Blue && c > bound) ||
						(points[i].getColor() == sf::Color::Red && c < bound))
						correct++;
				}
				cost = 1.0 - ((float) correct)/points.size();
				costPlot.addPoint(cost);
			}
			scatter_plot.drawPlot(sprite, shader);
			costPlot.drawTimePlot();
			sf::Text text(std::to_string(((int)floor(1 / deltaTime))) + "\n" +
							  std::to_string(correct) + "/" + std::to_string(points.size()) + "\n" +
							  "Learn Rate: " + std::to_string(learnRate) + "\n" +
							  "Epoch: " + std::to_string(epochCount)+"\n"+
							  "Cost: " +std::to_string(cost),
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

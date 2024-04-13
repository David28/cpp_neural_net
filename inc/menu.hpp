#include <SFML/Graphics.hpp>
#include <activation/activation.hpp>
#include <vector>

class RunOptions {
public:
    int pointCount = 1000;
    int dataset = 1; // (Temporary) but 0 for normal subgroups, 1 for radius
    // HyperParameters (missing the layers, its down bellow)
    double learnRate = 0.01;
    int batchSize = 10;
    Activation activation = Activation(RELU);
    std::vector<int> layerSizes = {2, 4, 4, 2};
    RunOptions();
};


//menu to control UI interacts with arrow keys
class Menu {
public:
    RunOptions options;
    sf::Font font;
    sf::RenderWindow *window;
    int selectedOption = 0;
    bool done = false;
    Menu(sf::RenderWindow *window, sf::Font font);
    void drawMenu();
    void updateMenu(sf::Event event);
};
#include <menu.hpp>

RunOptions::RunOptions() {
    pointCount = 1000;
    dataset = 1;
    learnRate = 0.01;
    batchSize = 10;
    activation = Activation(RELU);
}

Menu::Menu(sf::RenderWindow *window, sf::Font font) {
    this->window = window;
    this->font = font;
    this->options = RunOptions();
}

//simple menu navigated by arrow keys
//selected option is highlighted
void Menu::drawMenu() {
    sf::Text text;
    //get Window size
    sf::Vector2u windowSize = window->getSize();
    sf::Vector2f center = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);
    
    text.setFont(font);
    //title
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::White);
    text.setPosition(center.x - 100, center.y - 200);
    text.setString("Options");
    window->draw(text);


    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(center.x - 100, center.y - 100);
    std::string pointCountStr = "< Point Count: " + std::to_string(options.pointCount) + " >";
    text.setString(pointCountStr);
    window->draw(text);

    text.setPosition(center.x - 100, center.y - 70);
    std::string datasetStr = "< Dataset: " + std::to_string(options.dataset) + " >";
    text.setString(datasetStr);
    window->draw(text);

    text.setPosition(center.x - 100, center.y - 40);
    std::string learnRateStr = "< Learn Rate: " + std::to_string(options.learnRate) + " >";
    text.setString(learnRateStr);
    window->draw(text);

    text.setPosition(center.x - 100, center.y - 10);
    std::string batchSizeStr = "< Batch Size: " + std::to_string(options.batchSize) + " >";
    text.setString(batchSizeStr);
    window->draw(text);

    text.setPosition(center.x - 100, center.y + 20);
    std::string activationStr = "< Activation: " + activationToString(options.activation.getType()) + " >";
    text.setString(activationStr);
    window->draw(text);

    sf::RectangleShape highlight = sf::RectangleShape(sf::Vector2f(200, 30));
    highlight.setFillColor(sf::Color(0, 0, 255, 128));

    //draw the layers as balls
    for (int i = 0; i < options.layerSizes.size(); i++) {
        for (int j = 0; j < options.layerSizes[i]; j++) {
            sf::CircleShape circle = sf::CircleShape(10);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(center.x - 100 + i * 50, center.y + 100 + j * 30);
            window->draw(circle);

            //draw the lines to the next layer
            if (i < options.layerSizes.size() - 1) {
                for (int k = 0; k < options.layerSizes[i + 1]; k++) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(center.x - 100 + i * 50 + 10, center.y + 100 + j * 30 + 10)),
                        sf::Vertex(sf::Vector2f(center.x - 100 + (i + 1) * 50, center.y + 100 + k * 30 + 10))
                    };
                    window->draw(line, 2, sf::Lines);
                }
            }
        }
    }



    switch (selectedOption) {
        case 0:
            highlight.setPosition(center.x - 100, center.y - 100);
            highlight.setSize(sf::Vector2f(pointCountStr.size()*12, 30));
            break;
        case 1:
            highlight.setPosition(center.x - 100, center.y - 70);
            highlight.setSize(sf::Vector2f(datasetStr.size()*12, 30));
            break;
        case 2:
            highlight.setPosition(center.x - 100, center.y - 40);
            highlight.setSize(sf::Vector2f(learnRateStr.size()*12, 30));
            break;
        case 3:
            highlight.setPosition(center.x - 100, center.y - 10);
            highlight.setSize(sf::Vector2f(batchSizeStr.size()*12, 30));
            break;
        case 4:
            highlight.setPosition(center.x - 100, center.y + 20);
            highlight.setSize(sf::Vector2f(activationStr.size()*12, 30));
            break;
        case 5: 
            //select the layer
            highlight.setPosition(center.x - 100, center.y + 100);
            highlight.setSize(sf::Vector2f(options.layerSizes.size() * 44, 30));
            break;
        default:
            //select the layer
            int layer = selectedOption - 6 +1 -1;
            int layerSize = options.layerSizes[layer];
            int layerX = center.x - 100 + layer * 50;
            int layerY = center.y + 100;
            highlight.setPosition(layerX, layerY);
            highlight.setSize(sf::Vector2f(20, layerSize * 30));
            break;
    }

    highlight.setOutlineThickness(2);
    highlight.setOutlineColor(sf::Color::White);
    window->draw(highlight);

}

void Menu::updateMenu(sf::Event event) {
    int totalOptions = 6 + options.layerSizes.size();   
    if (event.type == sf::Event::KeyPressed) {
        //up and down to change selected option
        if (event.key.code == sf::Keyboard::Up) {
            //cannot select input and output layers
            selectedOption = (selectedOption - 1 + 6 + options.layerSizes.size()) % totalOptions;
            if (selectedOption >= 6 && selectedOption - 6 + 1 - 1 == 0 || selectedOption - 6 + 1 - 1 == options.layerSizes.size() - 1)
                selectedOption = (selectedOption - 1 + 6 + options.layerSizes.size()) % totalOptions;
            
            if (options.layerSizes.size() < 3 && selectedOption >= 6)
                selectedOption = (selectedOption - 6 + 6) % totalOptions;
        }else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % totalOptions;
            if (selectedOption >= 6 && selectedOption - 6 + 1 - 1 == 0 || selectedOption - 6 + 1 - 1 == options.layerSizes.size() - 1)
                selectedOption = (selectedOption + 1) % totalOptions;

            if (options.layerSizes.size() < 3 && selectedOption >= 6)
                selectedOption = (selectedOption + 6) % totalOptions;
        }
       

        //change value of selected option * 10 or / 10
        if (event.key.code == sf::Keyboard::Right) {
            switch (selectedOption) {
                case 0:
                    if (options.pointCount < 100000)
                        options.pointCount *= 10;
                    break;
                case 1:
                    options.dataset = (options.dataset + 1) % 2;
                    break;
                case 2:
                    if (options.learnRate < 100)
                        options.learnRate *= 10;
                    break;
                case 3:
                    if (options.batchSize < 100000)
                        options.batchSize *= 10;
                    break;
                case 4:
                    options.activation = Activation(ActivationType((options.activation.getType() + 1) % 3));
                    break;
                case 5:
                    //add a layer
                    if (options.layerSizes.size() < 5) //TODO: make this a constant
                        options.layerSizes.push_back(2);
                default: 
                    int layer = selectedOption - 6;
                    if (options.layerSizes[layer] < 100)
                        options.layerSizes[layer] += 1;
                    break;
            }
        }else if (event.key.code == sf::Keyboard::Left) {
            switch (selectedOption) {
                case 0:
                    options.pointCount /= 10;
                    break;
                case 1:
                    options.dataset = (options.dataset - 1 + 2) % 2;
                    break;
                case 2:
                    options.learnRate /= 10;
                    break;
                case 3:
                    options.batchSize /= 10;
                    break;
                case 4:
                    options.activation = Activation(ActivationType((options.activation.getType() - 1 +3) % 3));
                    break;
                case 5:
                    //remove a layer
                    if (options.layerSizes.size() > 2)
                        options.layerSizes.pop_back();
                    break;
                default:
                    int layer = selectedOption - 6;
                    if (options.layerSizes[layer] > 1)
                        options.layerSizes[layer] -= 1;
                    break;
            }
        }

        else if (event.key.code == sf::Keyboard::Enter ) {
            done = true;
        }
    }
    //first and last layer always have 2 nodes
    if (options.layerSizes.size() > 0) {
        options.layerSizes[0] = 2;
        options.layerSizes[options.layerSizes.size() - 1] = 2;
    }

}


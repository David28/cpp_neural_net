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
    std::string activationStr = "< Activation: " + std::to_string(options.activation.getType()) + " >";
    text.setString(activationStr);
    window->draw(text);



    sf::RectangleShape highlight = sf::RectangleShape(sf::Vector2f(200, 30));
    highlight.setFillColor(sf::Color(0, 0, 255, 128));

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
    }
    float boundx = text.getLocalBounds().width;
    float boundy = text.getLocalBounds().height;
    text.setPosition(0, 0);
    text.setString(std::to_string(boundx) + " " + std::to_string(boundy));
    window->draw(text);


    highlight.setOutlineThickness(2);
    highlight.setOutlineColor(sf::Color::White);
    window->draw(highlight);

}

void Menu::updateMenu(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        //up and down to change selected option
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + 5) % 5;
        }else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % 5;
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
            }
        }

        else if (event.key.code == sf::Keyboard::Enter ) {
            done = true;
        }
        
    }
}


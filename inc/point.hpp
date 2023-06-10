#ifndef POINT_HPP
#define POINT_HPP

class Point {
private:
    sf::Vector2f pos;
    sf::Color color;

public:
    Point(sf::Vector2f pos, sf::Color color);
    Point(sf::Vector2f position, sf::Color color, std::vector<double> expectedOutputs);
    void drawPoint(sf::Vector2f origin, float width, sf::RenderWindow *window, sf::Vector2f scale);
    sf::Vector2f getPosition();
    sf::Color getColor();
    void setColor(sf::Color newColor);
    void setExpectedOutputs(std::vector<double> expectedOutputs);
    std::vector<double> inputs();
    std::vector<double> expectedOutputs;

};

#endif
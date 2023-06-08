#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
class point {
private:
    sf::Vector2f pos;
    sf::Color color;

public:
    point(sf::Vector2f pos, sf::Color color);
    void drawPoint(sf::Vector2f origin, float width, sf::RenderWindow *window);
    sf::Vector2f getPosition();
    sf::Color getColor();
    void setColor(sf::Color newColor);
};

class plot {
private:
    std::vector<point> points; // list of points
    sf::Vector2i graphSize; 
    int pointWidth; // width of each point
    sf::RenderWindow* window; // render window object
    sf::Vector2f origin;

public:
    plot(std::vector<point> points, sf::Vector2i graphSize, int pointWidth, sf::RenderWindow* window);
    ~plot();
    void drawPlot();
    void addPoint(point newPoint);
};


std::vector<point> getRandomPoints(sf::Vector2i graphSize);


// How to randomly split in groups?
/*
    Take a random point, change his color. Now calculate euclidean distance to all other points. Change the color of the closest.
    The range is 40%-60% of a group
*/
void splitInTwoGroups(std::vector<point> *points, sf::Color otherColor);

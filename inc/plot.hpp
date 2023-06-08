#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
class Point {
private:
    sf::Vector2f pos;
    sf::Color color;

public:
    Point(sf::Vector2f pos, sf::Color color);
    void drawPoint(sf::Vector2f origin, float width, sf::RenderWindow *window);
    sf::Vector2f getPosition();
    sf::Color getColor();
    void setColor(sf::Color newColor);
};

class Plot {
private:
    std::vector<Point> points; // list of points
    sf::Vector2i graphSize; 
    int pointWidth; // width of each Point
    sf::RenderWindow* window; // render window object
    sf::Vector2f origin;

public:
    Plot(std::vector<Point> points, sf::Vector2i graphSize, int pointWidth, sf::RenderWindow* window);
    ~Plot();
    void drawPlot();
    void addPoint(Point newPoint);
};


std::vector<Point> getRandomPoints(sf::Vector2i graphSize);


// How to randomly split in groups?
/*
    Take a random Point, change his color. Now calculate euclidean distance to all other points. Change the color of the closest.
    The range is 40%-60% of a group
*/
void splitInTwoGroups(std::vector<Point> *points, sf::Color otherColor);

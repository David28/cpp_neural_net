#include <SFML/Graphics.hpp>
#include <vector>
#include <activation/activation.hpp>
#include <point.hpp>


class ScatterPlot {
private:
    std::vector<Point> points; // list of points
    sf::Vector2i graphSize; 
    int pointWidth; // width of each Point
    sf::RenderWindow* window; // render window object
    sf::Vector2f origin;

public:
    ScatterPlot(std::vector<Point> points, sf::Vector2i graphSize, int pointWidth, sf::RenderWindow* window);
    ~ScatterPlot();
    void drawPlot(sf::Sprite background, sf::Shader &shader);
    void addPoint(Point newPoint);
};

class TimePlot {
public:
    TimePlot(sf::Vector2i graphSize, sf::RenderWindow *window);
    void addPoint(float y);
    void drawTimePlot();

private:
    std::vector<float> points;
    sf::Vector2i graphSize;
    sf::RenderWindow *window;
    sf::Vector2f origin;
};

std::vector<Point> getRandomPoints(sf::Vector2i graphSize, int numPoints,std::vector<double> expected);

// How to randomly split in groups?
/*
    Take a random Point, change his color. Now calculate euclidean distance to all other points. Change the color of the closest.
    The range is 40%-60% of a group
*/
void splitInGroups(std::vector<Point> *points, sf::Color otherColor, int groupSize, int nSubGroups,std::vector<double> expected);
void splitInRadius(std::vector<Point> *points, sf::Color otherColor, std::vector<double> expected, double outerRadius, double innerRadius);
std::vector<Point> shuffle(std::vector<Point> points);

std::vector<std::vector<Point>> getBatches(std::vector<Point> points, int batchSize);
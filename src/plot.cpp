#include <SFML/Graphics.hpp>
#include <vector>
#include <plot.hpp>
#include <utils.hpp>

plot::plot(std::vector<point> points, sf::Vector2i graphSize, int pointWidth, sf::RenderWindow* window)
    : points(points), graphSize(graphSize), pointWidth(pointWidth)
{
    this->window = window;

    sf::Vector2u winSize = window->getSize();

    //Centering the graph
    this->origin = sf::Vector2f(winSize.x / 2 - graphSize
    .x / 2, winSize.y / 2 + graphSize.y / 2);

}

plot::~plot()
{
    window->close(); // close window
}

void plot::addPoint(point newPoint) {
    points.push_back(newPoint);
}

void plot::drawPlot()
{
    
    
    // draw X-axis
    sf::VertexArray xAxis(sf::PrimitiveType::Lines, 2);
    xAxis[0].position = origin;
    xAxis[1].position = sf::Vector2f(origin.x+graphSize
    .x,origin.y);
    xAxis[0].color = sf::Color::White;
    xAxis[1].color = sf::Color::White;
    // draw Y-axis
    sf::VertexArray yAxis(sf::PrimitiveType::Lines, 2);
    yAxis[0].position = origin;
    yAxis[1].position = sf::Vector2f(origin.x,origin.y-graphSize.y);
    yAxis[0].color = sf::Color::White;
    yAxis[1].color = sf::Color::White;

    window->draw(xAxis);
    window->draw(yAxis);
    // draw points
    for (point point : points) {
        point.drawPoint(origin, pointWidth, window);
    }
}


point::point(sf::Vector2f position, sf::Color color)
    : pos(position), color(color)
{
}



void point::drawPoint(sf::Vector2f origin, float width, sf::RenderWindow *window)
{
    sf::CircleShape circle(width);
    circle.setFillColor(color);
    circle.setPosition(sf::Vector2f(this->pos.x  + origin.x, -this->pos.y+ origin.y));
    window->draw(circle);
}

sf::Vector2f point::getPosition(){
    return this->pos;
}

sf::Color point::getColor(){
    return this->color;
}
void point::setColor(sf::Color newColor) {
    this->color = newColor;
}


std::vector<point> getRandomPoints(sf::Vector2i graphSize){
    std::vector<point> points;

    for (int i = 0; i < 100; i++) {
        float x = (float)(rand() % (graphSize.x));
        float y = (float)(rand() % (graphSize.y));
		points.push_back(point(sf::Vector2f(x, y), sf::Color::Red));
    }

    return points;
}

void splitInTwoGroups(std::vector<point>* points, sf::Color otherColor) {

    // Define random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, points->size()-1);
    // Choose a random point to start with
    int i = dis(gen);

    // Calculate group size between 40% and 60% of total number of points
    int minGroupSize = (int)std::ceil(0.4 * points->size());
    int maxGroupSize = (int)std::floor(0.6 * points->size());
    int groupSize = dis(gen) % (maxGroupSize - minGroupSize + 1) + minGroupSize;

    // Sort remaining points by distance to random point
    std::vector<int> sortedIdxs(points->size() - 2);
    for (int j = 0; j < sortedIdxs.size(); j++) {
        sortedIdxs[j] = j;
    }
    std::sort(sortedIdxs.begin(), sortedIdxs.end(), [&](int a, int b) {
        return euclideanDist((*points)[i].getPosition(), (*points)[a].getPosition()) < euclideanDist((*points)[i].getPosition(), (*points)[b].getPosition());
    });
    // Assign colors to points based on distance to random point
    int countGroup1 = 0;
    for (int j = 0; j < sortedIdxs.size(); j++) {
        if (countGroup1 > groupSize)
            break;
        (*points)[sortedIdxs[j]].setColor(otherColor);
        countGroup1++;
    }

}
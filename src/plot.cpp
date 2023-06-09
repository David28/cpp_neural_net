#include <SFML/Graphics.hpp>
#include <plot.hpp>
#include <utils.hpp>
#include <chrono>

ScatterPlot::ScatterPlot(std::vector<Point> points, sf::Vector2i graphSize, int pointWidth, sf::RenderWindow *window)
    : points(points), graphSize(graphSize), pointWidth(pointWidth), window(window)
{

    sf::Vector2u winSize = window->getSize();

    // Centering the graph
    this->origin = sf::Vector2f(winSize.x / 2,
                                winSize.y / 2);
}

ScatterPlot::~ScatterPlot()
{
    window->close(); // close window
}

void ScatterPlot::addPoint(Point newPoint)
{
    points.push_back(newPoint);
}

void ScatterPlot::drawPlot(sf::Sprite background)
{

    background.setPosition(sf::Vector2f(origin.x - graphSize
                                                           .x /
                                                       2,
                                        origin.y - graphSize.y / 2));
    window->draw(background);

    // draw X-axis
    sf::VertexArray xAxis(sf::PrimitiveType::Lines, 2);
    xAxis[0].position = sf::Vector2f(origin.x - graphSize
                                                        .x /
                                                    2,
                                     origin.y);
    ;
    xAxis[1].position = sf::Vector2f(origin.x + graphSize
                                                        .x /
                                                    2,
                                     origin.y);
    xAxis[0].color = sf::Color::White;
    xAxis[1].color = sf::Color::White;
    // draw Y-axis
    sf::VertexArray yAxis(sf::PrimitiveType::Lines, 2);
    yAxis[0].position = sf::Vector2f(origin.x, origin.y + graphSize.y / 2);
    yAxis[1].position = sf::Vector2f(origin.x, origin.y - graphSize.y / 2);
    yAxis[0].color = sf::Color::White;
    yAxis[1].color = sf::Color::White;

    window->draw(xAxis);
    window->draw(yAxis);
    // draw points
    for (Point Point : points)
    {
        Point.drawPoint(origin, pointWidth, window, sf::Vector2f(graphSize));
    }
}

TimePlot::TimePlot(sf::Vector2i graphSize, sf::RenderWindow *window)
    : graphSize(graphSize), window(window)
{
    sf::Vector2u winSize = window->getSize();
    // Centering the graph
    this->origin = sf::Vector2f(winSize.x * 0.05, winSize.y * 0.3);
}

void TimePlot::addPoint(float y)
{
    points.push_back(y);
}

void TimePlot::drawTimePlot()
{
    // draw X-axis
    sf::VertexArray xAxis(sf::PrimitiveType::Lines, 2);
    xAxis[0].position = origin;
    ;
    xAxis[1].position = sf::Vector2f(origin.x + graphSize
                                                    .x,
                                     origin.y);
    xAxis[0].color = sf::Color::White;
    xAxis[1].color = sf::Color::White;
    // draw Y-axis
    sf::VertexArray yAxis(sf::PrimitiveType::Lines, 2);
    yAxis[0].position = origin;
    yAxis[1].position = sf::Vector2f(origin.x, origin.y - graphSize.y);
    yAxis[0].color = sf::Color::White;
    yAxis[1].color = sf::Color::White;

    window->draw(xAxis);
    window->draw(yAxis);
    // draw X-axis
    if (!points.empty())
    {
        sf::VertexArray plot(sf::PrimitiveType::Lines, (points.size() - 1) * 2);
        // draw points
        for (int i = 0; i < points.size() - 1; i++)
        {
            int plotPoint = i*2;

            double x1 = (((double)i)/points.size());
            double x2 = (((double)i+1)/points.size());
            float y1 = points[i];
            float y2 = points[i + 1];
            plot[plotPoint].position = sf::Vector2f(x1 * graphSize.x + origin.x, -y1 * graphSize.y + origin.y);

            plot[plotPoint + 1].position = sf::Vector2f(x2 * graphSize.x + origin.x,
                                                        -y2 * graphSize.y + origin.y);
            plot[plotPoint].color = sf::Color::Red;
            plot[plotPoint + 1].color = sf::Color::Red;


        }
        window->draw(plot);
    }
}

// -- Points --

Point::Point(sf::Vector2f position, sf::Color color)
    : pos(position), color(color)
{
}

Point::Point(sf::Vector2f position, sf::Color color, std::vector<double> expectedOutputs)
    : pos(position), color(color), expectedOutputs(expectedOutputs)
{
}

void Point::drawPoint(sf::Vector2f origin, float width, sf::RenderWindow *window, sf::Vector2f scale)
{
    sf::CircleShape circle(width);
    circle.setFillColor(color);
    circle.setPosition(sf::Vector2f((this->pos.x * scale.x / 2 + origin.x), -this->pos.y * scale.y / 2 + origin.y));
    window->draw(circle);
}

sf::Vector2f Point::getPosition()
{
    return this->pos;
}

sf::Color Point::getColor()
{
    return this->color;
}
void Point::setColor(sf::Color newColor)
{
    this->color = newColor;
}

void Point::setExpectedOutputs(std::vector<double> expectedOutputs)
{
    this->expectedOutputs = expectedOutputs;
}

std::vector<double> Point::inputs()
{
    std::vector<double> res;
    res.push_back(pos.x);
    res.push_back(pos.y);
    return res;
}

std::vector<Point> getRandomPoints(sf::Vector2i graphSize, int numPoints)
{
    std::vector<Point> points;
    unsigned seed = time(nullptr);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(-1, 1);
    for (int i = 0; i < numPoints; i++)
    {
        float x = dis(gen);
        float y = dis(gen);
        std::vector<double> expectedOutputs;
        expectedOutputs.push_back(-1);
        points.push_back(Point(sf::Vector2f(x, y), sf::Color::Red, expectedOutputs));
    }

    return points;
}

void splitInTwoGroups(std::vector<Point> *points, sf::Color otherColor)
{

    // Define random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, points->size() - 1);
    // Choose a random Point to start with
    int i = dis(gen);

    // Calculate group size between 40% and 60% of total number of points
    int minGroupSize = (int)std::ceil(0.4 * points->size());
    int maxGroupSize = (int)std::floor(0.6 * points->size());
    int groupSize = dis(gen) % (maxGroupSize - minGroupSize + 1) + minGroupSize;

    // Sort remaining points by distance to random Point
    std::vector<int> sortedIdxs(points->size() - 2);
    for (int j = 0; j < sortedIdxs.size(); j++)
    {
        sortedIdxs[j] = j;
    }
    std::sort(sortedIdxs.begin(), sortedIdxs.end(), [&](int a, int b)
              { return euclideanDist((*points)[i].getPosition(), (*points)[a].getPosition()) < euclideanDist((*points)[i].getPosition(), (*points)[b].getPosition()); });
    // Assign colors to points based on distance to random Point
    int countGroup1 = 0;
    for (int j = 0; j < sortedIdxs.size(); j++)
    {
        if (countGroup1 > groupSize)
            break;
        std::vector<double> expectedOutputs;
        expectedOutputs.push_back(1);
        (*points)[sortedIdxs[j]].setColor(otherColor);
        (*points)[sortedIdxs[j]].setExpectedOutputs(expectedOutputs);
        countGroup1++;
    }
}

std::vector<std::vector<Point>> getBatches(std::vector<Point> points, int batchSize)
{
    std::vector<std::vector<Point>> batches;
    int nBatches = (points.size() + batchSize - 1) / batchSize;

    for (int i = 0; i < nBatches; ++i)
    {
        auto start = points.begin() + i * batchSize;
        auto end = start + batchSize;
        if (end > points.end())
        {
            end = points.end();
        }
        batches.emplace_back(start, end);
    }

    return batches;
}

std::vector<Point> shuffle(std::vector<Point> points)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(points.begin(), points.end(), std::default_random_engine(seed));
    return points;
}
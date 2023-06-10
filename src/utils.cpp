#include <utils.hpp>


bool randomBool()
{
	return rand() > (RAND_MAX / 2);
}

double euclideanDist(sf::Vector2f p1, sf::Vector2f p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

double rescale(double x, std::pair<double,double> oldScale, std::pair<double,double> newScale) {
    return ((x - oldScale.first) / (oldScale.second - oldScale.first)) * (newScale.second - newScale.first) + newScale.first;
}
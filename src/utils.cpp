#include <utils.hpp>


bool randomBool()
{
	return rand() > (RAND_MAX / 2);
}

double euclideanDist(sf::Vector2f p1, sf::Vector2f p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}
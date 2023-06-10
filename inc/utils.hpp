#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>

bool randomBool();
double euclideanDist(sf::Vector2f p1, sf::Vector2f p2);
double rescale(double x, std::pair<double,double> oldScale, std::pair<double,double> newScale) ;
#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP
#include <vector>
#include <string>
enum ActivationType {
    RELU = 0,
    SIGMOID = 1,
    SILU = 2
};


class Activation
{
private:
    ActivationType type;
public:
    Activation();
    Activation(ActivationType type);
    double activate(double input);

	double derivative(double input);
    double rescaleByActivation(double x, std::pair<double, double> newScale);
    double rescaleToActivation(double x, std::pair<double, double> oldScale);  //diferent scales are needed for different activations
    ActivationType getType();
};
std::string activationToString(ActivationType type);


double activateReLu(double input);

double derivateReLu(double input);


double activateSigmoid(double input);


double derivateSigmoid(double input);
double activateSiLU(double input);
double derivateSiLU(double input);

#endif // ACTIVATION_HPP
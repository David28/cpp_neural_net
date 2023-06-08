#include <vector>

enum ActivationType {
    RELU,
    SIGMOID,
    SILU
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
};



double activateReLu(double input);

double derivateReLu(double input);


double activateSigmoid(double input);


double derivateSigmoid(double input);
double activateSiLU(double input);
double derivateSiLU(double input);
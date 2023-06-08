#include <activation/activation.hpp>
#include <utils.hpp>

Activation::Activation() : type(SIGMOID) {}
Activation::Activation(ActivationType type) : type(type) {}

double Activation::activate(double input)
{
    double res = input;
    switch (type)
    {
    case RELU:
        res = activateReLu(input);
        break;
    case SIGMOID:
        res = activateSigmoid(input);
        break;
    case SILU:
        res = activateSiLU(input);
        break;
    default:
        break;
    }

    return res;
}

double Activation::derivative(double input)
{
    double res = input;
    switch (type)
    {
    case RELU:
        res = derivateReLu(input);
        break;
    case SIGMOID:
        res = derivateSigmoid(input);
        break;
    case SILU:
        res = derivateSiLU(input);
        break;
    default:
        break;
    }

    return res;
}

double activateReLu(double input)
{
    return __max(0, input);
}

double derivateReLu(double input)
{
    return (input > 0) ? 1 : 0;
}

double activateSigmoid(double input)
{
    return 1.0f / (1.f + exp(-input));
}

double derivateSigmoid(double input)
{
    double a = activateSigmoid(input);
    return a * (1.f - a);
}

double activateSiLU(double input)
{
    return input / (1 + exp(-input));
}

double derivateSiLU(double input)
{
    double sig = 1 / (1 + exp(-input));
    return input * sig * (1 - sig) + sig;
}

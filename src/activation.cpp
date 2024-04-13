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

double Activation::rescaleToActivation(double x, std::pair<double, double> oldScale)
{
    double res = x;
    switch (type)
    {
    case RELU:
        res = rescale(x, oldScale, std::make_pair(0.f, 1.f));
        break;
    case SIGMOID:
        res = rescale(x, oldScale, std::make_pair(0.f, 1.f));
        break;
    case SILU:
        res = rescale(x, oldScale, std::make_pair(-1.f, 1.f));
        break;
    default:
        break;
    }

    return res;
}
double Activation::rescaleByActivation(double x, std::pair<double, double> newScale)
{
    double res = x;
    switch (type)
    {
    case RELU:
        res = rescale(x, std::make_pair(0.f, 1.f), newScale);
        break;
    case SIGMOID:
        res = rescale(x, std::make_pair(0.f, 1.f), newScale);
        break;
    case SILU:
        res = rescale(x, std::make_pair(-1.f, 1.f), newScale);
        break;
    default:
        break;
    }

    return res;
}
ActivationType Activation::getType()
{
    return this->type;
}

std::string activationToString(ActivationType type)
{
    switch (type)
    {
    case RELU:
        return "RELU";
    case SIGMOID:
        return "SIGMOID";
    case SILU:
        return "SILU";
    default:
        return "UNKNOWN";
    }
}

double activateReLu(double input)
{
    return (input > 0.f) ? input : 0.f;
}

double derivateReLu(double input)
{
    return (input > 0.f) ? 1.f : 0.f;
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
    return input / (1.f + exp(-input));
}

double derivateSiLU(double input)
{
    double sig = 1.f / (1.f + exp(-input));
    return input * sig * (1.f - sig) + sig;
}

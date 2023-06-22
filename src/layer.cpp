#include <neural/layer.hpp>
#include <chrono>
#include <iostream>
Layer::Layer(int numNodesIn, int numNodesOut, Activation activation) : numNodesIn(numNodesIn), numNodesOut(numNodesOut), activation(activation), inputs(numNodesIn), weightedInputs(numNodesOut), activations(numNodesOut),
                                                                       weights(numNodesIn * numNodesOut), costGradientW(numNodesIn * numNodesOut),
                                                                       biases(numNodesOut),
                                                                       costGradientB(numNodesOut)
{
    initializeRandomWeigths();
}

std::vector<double> Layer::calculateOutputs(std::vector<double> inputs)
{
    this->inputs = inputs;
    for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
    {
        double weightedInput = biases[nodeOut];

        for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
        {
            weightedInput += inputs[nodeIn] * getWeight(nodeIn, nodeOut);
        }
        weightedInputs[nodeOut] = weightedInput;
        activations[nodeOut] = activation.activate(weightedInput);
    }

    return activations;
}

double Layer::getWeight(int nodeIn, int nodeOut)
{
    return weights[getFlatWeightIndex(nodeIn, nodeOut)];
}

void Layer::setWeight(int nodeIn, int nodeOut, double newWeight)
{
    weights[getFlatWeightIndex(nodeIn, nodeOut)] = newWeight;
}
double Layer::getBias(int bias)
{
    return this->biases[bias];
}
void Layer::setBias(int bias, double newBias)
{
    this->biases[bias] = newBias;
}

int Layer::getFlatWeightIndex(int inputNeuronIndex, int outputNeuronIndex)
{
    return outputNeuronIndex * numNodesIn + inputNeuronIndex;
}

void Layer::initializeRandomWeigths()
{
    // Define random number generator
    int seed = rand();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < weights.size(); i++)
    {
        weights[i] = dis(gen);
    }
}

void Layer::applyGradients(double learnRate)
{
    for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
    {
        for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
        {
            int flatIndex = getFlatWeightIndex(nodeIn, nodeOut);
            weights[flatIndex] -= costGradientW[flatIndex] * learnRate;
            costGradientW[flatIndex] = 0;
        }
        biases[nodeOut] -= costGradientB[nodeOut] * learnRate;
        costGradientB[nodeOut] = 0;
    }
}

void Layer::clearGradients()
{
    costGradientB = std::vector<double>(costGradientB.size());
    costGradientW = std::vector<double>(costGradientW.size());
}

double Layer::nodeCost(double expected, double actual)
{
    double error = actual - expected;
    return error * error;
}

double nodeCostDerivative(double expected, double actual)
{
    return 2*(actual-expected);
}

void Layer::updateGradients(std::vector<double> nodeValues)
{
    for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
    {
        double nodeValue = nodeValues[nodeOut];
        for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
        {
            // Evaluate the partial derivative: cost / weight of current connection
            double derivativeCostWrtWeight = inputs[nodeIn] * nodeValue;
            // The costGradientW array stores these partial derivatives for each weight.
            // Note: the derivative is being added to the array here because ultimately we want
            // to calculate the average gradient across all the data in the training batch
            costGradientW[getFlatWeightIndex(nodeIn, nodeOut)] += derivativeCostWrtWeight;
        }

        // Evaluate partial derivative: cost / bias
        double derivativeCostWrtBias = 1 * nodeValues[nodeOut];
        costGradientB[nodeOut] += derivativeCostWrtBias;
    }
}

std::vector<double> Layer::calculateOutputLayerNodeValues(std::vector<double> expectedOutputs)
{
    std::vector<double> nodeValues(expectedOutputs.size());
    for (int i = 0; i < nodeValues.size(); i++)
    {
        // Evaluate partial derivatives for current node: cost/activation & activation/weightedInput
        double costDerivative = nodeCostDerivative(expectedOutputs[i],activations[i]);
        double activationDerivative = activation.derivative(weightedInputs[i]);
        nodeValues[i] = costDerivative * activationDerivative;
    }

    return nodeValues;
}

std::vector<double> Layer::calculateHiddenLayerNodeValues(Layer oldLayer, std::vector<double> oldNodeValues)
{
    std::vector<double> newNodeValues(numNodesOut);
    for (int newNodeIndex = 0; newNodeIndex < numNodesOut; newNodeIndex++)
    {
        double newNodeValue = 0;
        for (int oldNodeIndex = 0; oldNodeIndex < oldNodeValues.size(); oldNodeIndex++)
        {
            // Partial derivative of the weighted input with respect to the input
            double weightedInputDerivative = oldLayer.getWeight(newNodeIndex, oldNodeIndex);
            newNodeValue += weightedInputDerivative * oldNodeValues[oldNodeIndex];
        }
        newNodeValue *= activation.derivative(weightedInputs[newNodeIndex]);
        newNodeValues[newNodeIndex] = newNodeValue;
    }
    return newNodeValues;
}
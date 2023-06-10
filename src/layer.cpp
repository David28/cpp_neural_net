#include <neural/layer.hpp>
#include <chrono>
#include <iostream>
Layer::Layer(int numNodesIn, int numNodesOut, Activation activation) :
 numNodesIn(numNodesIn), numNodesOut(numNodesOut), activation(activation)
{

    weights.resize(numNodesIn * numNodesOut);
    costGradientW.resize(numNodesIn * numNodesOut);
    biases.resize(numNodesOut);
    costGradientB.resize(numNodesOut);

    initializeRandomWeigths();
}

std::vector<double> Layer::calculateOutputs(std::vector<double> inputs)
{
    std::vector<double> weightedInputs(numNodesOut);

    for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
    {
        double weightedInput = biases[nodeOut];

        for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
        {
            weightedInput += inputs[nodeIn] * getWeight(nodeIn, nodeOut);
        }
        weightedInputs[nodeOut] = activation.activate(weightedInput);
        //weightedInputs[nodeOut] = weightedInput;
    }

    return weightedInputs;
}

double Layer::getWeight(int nodeIn, int nodeOut)
{
    return weights[getFlatWeightIndex(nodeIn, nodeOut)];
}

void Layer::setWeight(int nodeIn, int nodeOut, double newWeight)
{
    weights[getFlatWeightIndex(nodeIn, nodeOut)] = newWeight;
}
double Layer::getBias(int bias) {
    return this->biases[bias];
}
void Layer::setBias(int bias, double newBias){
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
    std::uniform_real_distribution<double> dis(-1, 1);
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
            weights[getFlatWeightIndex(nodeIn, nodeOut)] -= costGradientW[getFlatWeightIndex(nodeIn, nodeOut)] * learnRate;
        }
        biases[nodeOut] -= costGradientB[nodeOut] * learnRate;
    }
}

double Layer::nodeCost(double expected, double actual)
{
    double error = actual - expected;
    return error * error;
}
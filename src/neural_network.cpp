#include <neural/neural_network.hpp>
#include <cfloat>

NeuralNetwork::NeuralNetwork(std::vector<int> layerSizes, Activation activation) : layerSizes(layerSizes)
{
    for (int i = 0; i < layerSizes.size() - 1; i++)
        layers.push_back(Layer(layerSizes[i], layerSizes[i + 1],activation));
}

// Run the inputs through the network to predict which class they belong to.
// Also returns the activations from the output layer.
std::pair<int, std::vector<double>> NeuralNetwork::classify(std::vector<double> inputs)
{
    std::vector<double> outputs = calculateOutputs(inputs);
    
    int predictedClass = maxValueIndex(outputs);
    return std::pair<int, std::vector<double>>(predictedClass, outputs);
}

// Run the inputs through the network to calculate the outputs
std::vector<double> NeuralNetwork::calculateOutputs(std::vector<double> inputs)
{
    for (int i = 0; i < layers.size(); i++) {
        Layer *layer = &layers[i];
        inputs = layer->calculateOutputs(inputs);
    }

    return inputs;
}

int NeuralNetwork::maxValueIndex(std::vector<double> values)
{
    double maxValue = DBL_MIN;
    int index = 0;
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i] > maxValue)
        {
            maxValue = values[i];
            index = i;
        }
    }

    return index;
}

void NeuralNetwork::learn(std::vector<Point> data, double learnRate)
{

    const double h = 0.0001; // No derivate for now
    double originalCost = cost(data);

    for (int i = 0; i < layers.size(); i++) {
        Layer *layer = &layers[i];

        for (int nodeIn = 0; nodeIn < layer->numNodesIn; nodeIn++)
        {
            for (int nodeOut = 0; nodeOut < layer->numNodesOut; nodeOut++)
            {
                int weightIndex = layer->getFlatWeightIndex(nodeIn,nodeOut);
                layer->setWeight(nodeIn,nodeOut, layer->getWeight(nodeIn,nodeOut) + h);
                double deltaCost = cost(data) - originalCost;

                layer->setWeight(nodeIn,nodeOut, layer->getWeight(nodeIn,nodeOut) - h);

                layer->costGradientW[weightIndex] = deltaCost/h;
            }
        }

        for (int bias = 0; bias < layer->numNodesOut; bias++)
        {
            layer->setBias(bias, layer->getBias(bias)+ h) ;
            double deltaCost = cost(data) - originalCost;
            layer->setBias(bias, layer->getBias(bias)- h) ;
            
            layer->costGradientB[bias] = deltaCost/h;
        }
    }

    applyAllGradients(learnRate);
}


void NeuralNetwork::applyAllGradients(double learnRate) {
    for (int i = 0; i < layers.size(); i++) {
        Layer *layer = &layers[i];

        layer->applyGradients(learnRate);
    }
}


double NeuralNetwork::cost(std::vector<Point> data) {
    double totalCost = 0;

    for (size_t i = 0; i < data.size(); i++)
        totalCost += cost(data[i]);
    
    return totalCost / data.size();
}

double NeuralNetwork::cost(Point dataPoint) {

    std::vector<double> outputs = calculateOutputs(dataPoint.inputs());

    Layer outputLayer = layers[layers.size()-1];

    double cost = 0;
    for (int nodeOut = 0; nodeOut < outputs.size(); nodeOut++)
        cost += outputLayer.nodeCost(outputs[nodeOut], dataPoint.expectedOutputs[nodeOut]);
    return cost;
}


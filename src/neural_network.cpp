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
        inputs = layers[i].calculateOutputs(inputs);
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
    for (Point dataPoint : data) {
        updateAllGradients(dataPoint);
    }

    applyAllGradients(learnRate/data.size());

}


void NeuralNetwork::applyAllGradients(double learnRate) {
    for (int i = 0; i < layers.size(); i++) {
        Layer &layer = layers[i];

        layer.applyGradients(learnRate);
    }
}

void NeuralNetwork::clearAllGradients() {

    for (int i = 0; i < layers.size(); i++) {
        Layer &layer = layers[i];
        layer.clearGradients();
    }
}

void NeuralNetwork::updateAllGradients(Point dataPoint) {
		// Feed data through the network to calculate outputs.
		// Save all inputs/weightedinputs/activations along the way to use for backpropagation.
		calculateOutputs(dataPoint.inputs());

		// -- Backpropagation --
		int outputLayerIndex = layers.size() - 1;
		Layer &outputLayer = layers[outputLayerIndex];

		// Update output layer gradients
        std::vector<double> nodeValues = outputLayer.calculateOutputLayerNodeValues(dataPoint.expectedOutputs);
		outputLayer.updateGradients(nodeValues);

		// Update all hidden layer gradients
		for (int i = outputLayerIndex - 1; i >= 0; i--)
		{
			Layer &hiddenLayer = layers[i];
            nodeValues = hiddenLayer.calculateHiddenLayerNodeValues(layers[i + 1], nodeValues);
			hiddenLayer.updateGradients(nodeValues);
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
        cost += outputLayer.nodeCost(dataPoint.expectedOutputs[nodeOut],outputs[nodeOut]);
    return cost;
}


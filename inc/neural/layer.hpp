#include <utils.hpp>
#include <activation/activation.hpp>


class Layer
{
private:
	Activation activation;
	
	
	void initializeRandomWeigths();
    

public:
	std::vector<double> inputs;
	std::vector<double> weightedInputs;
	std::vector<double> activations;

	int numNodesIn;
	int numNodesOut;

	std::vector<double> weights;
	std::vector<double> biases;

	// Cost gradient with respect to weights and with respect to biases
	std::vector<double> costGradientW;
	std::vector<double> costGradientB;
    Layer(int numNodesIn, int numNodesOut, Activation activation);
	// Calculate layer output activations
	std::vector<double> calculateOutputs(std::vector<double> inputs);
	double getWeight(int nodeIn, int nodeOut);
	void setWeight(int nodeIn, int nodeOut, double newWeight);
	double getBias(int bias);
	void setBias(int bias, double newBias);
	int getFlatWeightIndex(int inputNeuronIndex, int outputNeuronIndex);
	void applyGradients(double learnRate);
	void clearGradients();
	void updateGradients(std::vector<double> nodeValues);
	std::vector<double> calculateOutputLayerNodeValues(std::vector<double>  expectedOutputs);
	std::vector<double> calculateHiddenLayerNodeValues(Layer oldLayer, std::vector<double> oldNodeValues);

	double nodeCost(double expected, double actual);
};


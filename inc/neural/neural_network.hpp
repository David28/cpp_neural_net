#include <neural/layer.hpp>
#include <plot.hpp>
class NeuralNetwork
{
private:
    
    std::vector<int> layerSizes;
public:
    std::vector<Layer> layers;
    NeuralNetwork(std::vector<int> layerSizes, ActivationType activation);
    std::pair<int, std::vector<double>> classify(std::vector<double> inputs);
    std::vector<double> calculateOutputs(std::vector<double> inputs);
    int maxValueIndex(std::vector<double> values);
    void learn(std::vector<Point> data, double learnRate);
    void applyAllGradients(double learnRate);
    double cost(std::vector<Point> data);
    double cost(Point dataPoint);
};


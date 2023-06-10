// Fragment shader
#version 120
#define MAX_INPUTS 100
varying vec2 vTexCoord;

uniform int actType;

uniform float layerSizesL;
uniform vec2 graphSize;

uniform float layerSizes[MAX_INPUTS]; //actually are 1d arrays but I need them to be variable so??
uniform float flatWeights[MAX_INPUTS];
uniform float flatBiases[MAX_INPUTS];

float sigmoid(float x) {
    return 1.0 / (1.0 + exp(-x));
}
float siLU(float x)
{
    return x / (1 + exp(-x));
}
float reLU(float x)
{
    return max(0,x);
}

float activate(float x) {
    if (actType == 0) {
        return reLU(x);
    }else if (actType== 1) {
        return sigmoid(x);
    }else if (actType == 2) {
        return siLU(x);
    }
    return x;
}
float rescale(float x, vec2 oldScale, vec2 newScale) {
    return ((x - oldScale.x) / (oldScale.y - oldScale.x)) * (newScale.y - newScale.x) + newScale.x;
}

float rescaleByActivation(float x, vec2 newScale) {
    if (actType == 0) {
        return rescale(x, vec2(0.f,1.f), newScale);
    }else if (actType == 1) {
        return rescale(x, vec2(0.f,1.f), newScale);
    }else if (actType == 2) {
        return rescale(x, vec2(-1.f,1.f), newScale);
    }
    return x;
}




float[MAX_INPUTS] layer_calculate_outputs(float[MAX_INPUTS] inputs, int offSetBias, int offsetWeights, int numNodesIn, int numNodesOut)
{
    float[MAX_INPUTS] outputs;

    for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
    {
        float weightedInput = flatBiases[offSetBias+nodeOut];

        for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
        {
            int flatWeigthIndex = nodeOut * numNodesIn + nodeIn;
            weightedInput += inputs[nodeIn] * flatWeights[offsetWeights+flatWeigthIndex];
        }
        outputs[nodeOut] = activate(weightedInput);
    }

    return outputs;
}

float[MAX_INPUTS] neural_net_classify(float x, float y) {
    // max inputs size


    // Initialize the input values
    float inputValues[MAX_INPUTS];

    // Run each layer
    int offsetWeights = 0;
    int offSetBias = 0;
    bool firstInput= true;
    for (int i = 1; i < layerSizesL; i++) {
        // Calculate the number of neurons in this layer
        int neuronInCount = int(layerSizes[i-1]);
        int neuronOutCount = int(layerSizes[i]);
        if (firstInput) {
            inputValues[0] = x;
            inputValues[1] = y;
            firstInput = false;
        }
        inputValues = layer_calculate_outputs(inputValues,offSetBias,offsetWeights,neuronInCount,neuronOutCount);
        offsetWeights += neuronInCount * neuronOutCount;
        offSetBias += neuronOutCount;
    }

    return inputValues;
}


void main() {
    // Get the texture-relative coordinates of the current pixel
    float u = vTexCoord.x;
    float v = vTexCoord.y;
	float x = ((u) / graphSize.x) * 2 - 1;
	float y = ((graphSize.y-v) / graphSize.y) * 2 - 1;
    float[MAX_INPUTS] classed = neural_net_classify(x,y);
    // Set the pixel color using the texture-relative coordinates
    //scale to [-1,1] first
    float scaled = rescaleByActivation(classed[0], vec2(-1,1));
    gl_FragColor = (scaled > 0)?vec4(0.0f, 0.0f, 1.0f, 0.3f):vec4(1.0f, 0.0f, 0.0f, .3f);
    //gl_FragColor = vec4(classed[0], 0.0f, 0.0f, 1.0f);
}
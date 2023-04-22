#include "../headers/layer.h"

// Memory management
layer* createLayer(layer* previousLayer, int nbNeurons, double (*ActivationFunction)(double), layerType Type) {
    layer* Layer = malloc(sizeof(layer));
    if (Layer == NULL) {
        printf("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    Layer->size = nbNeurons;
    Layer->previous = previousLayer;
    Layer->next = NULL;

    Layer->neurons = malloc(nbNeurons * sizeof(neuron));
    if (Layer->neurons == NULL) {
        printf("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    if (Type == lHIDDEN) {
        for (int n = 0; n < nbNeurons; n++) {
            Layer->neurons[n] = createNeuron(previousLayer->size, ActivationFunction, nHIDDEN);
        }
    }
    else if (Type == lOUTPUT) {
        for (int n = 0; n < nbNeurons; n++) {
            Layer->neurons[n] = createNeuron(previousLayer->size, ActivationFunction, nOUTPUT);
        }
    }
    else {
        for (int n = 0; n < nbNeurons; n++) {
            Layer->neurons[n] = createNeuron(0, NULL, nINPUT);
        }
    }

    return Layer;
}

void freeLayer(layer* Layer) {
    for (int n = 0; n < Layer->size; n++) {
        freeNeuron(Layer->neurons[n]);
    }
    free(Layer->neurons);
    free(Layer);
}

// Initialization
void randomizeLayerConnections(layer* Layer) {
    for (int n = 0; n < Layer->size; n++) {
        randomizeNeuronConnection(Layer->neurons[n], Layer->previous->size);
    }
}

// Feed forward
void updateLayer(layer* Layer) {
    double inputs[Layer->previous->size];
    for (int i = 0; i < Layer->previous->size; i++) {
        inputs[i] = Layer->previous->neurons[i]->activation;
    }

    for (int n = 0; n < Layer->size; n++) {
        calculateNeuronOutput(Layer->neurons[n], Layer->previous->size, inputs);
    }
}

// Cost (for SGD, Stochastic Gradient Descent)
double Cost(layer* Layer, double expectedOutputs[]) {
    double cost = 0;
    for (int n = 0; n < Layer->size; n++) {
        cost += Error(Layer->neurons[n], expectedOutputs[n]); // 4 looks like {0 0 0 0 1.0 0 0 0 0 0}
    }

    return cost / Layer->size; // Not necessary
}

/*
No need to do that, but in the partial derivative of the Cost with respect to a weight,
we need errorDerivative() for the weight.
*/

// With respect to one weight
double CostDerivative(neuron* Neuron, double expectedOutput, int layerSize) {
    return ErrorDerivative(Neuron, expectedOutput) / layerSize; 
}

// Testing connexions (initializing input layer)
void randomizeLayerActivations(layer* Layer) {
    for (int n = 0; n < Layer->size; n++) {
        randomizeNeuronActivation(Layer->neurons[n]);
    }
}

// Miscelleanous
void printLayerActivations(layer* Layer) {
    for (int n = 0; n < Layer->size; n++) {
        printNeuronActivation(Layer->neurons[n]);
    }
}
#ifndef LAYER_H
#define LAYER_H



#include "network.h"
#include "neuron.h"

typedef struct _neuron neuron; // For compilation

typedef enum _layerType {
    lINPUT,
    lHIDDEN,
    lOUTPUT,
} layerType;

typedef struct _layer layer;
struct _layer {
    layerType type;

    layer* previous;
    layer* next;

    int size; // Number of neurons
    neuron** neurons; // Read as an array of pointers, not a 2-dimensional array
};

// Memory management
layer* createLayer(layer* previousLayer, int nbNeurons, double (*ActivationFunction)(double), layerType Type);
void freeLayer(layer* Layer);

// Initialization
void randomizeLayerConnections(layer* Layer);

// Feed forward
void updateLayer(layer* Layer);

// Cost (for SGD, Stochastic Gradient Descent)
double Cost(layer* Layer, double expectedOutputs[]);
double CostDerivative(neuron* Neuron, double expectedOutput, int layerSize);

// Testing connexions (initializing input layer)
void randomizeLayerActivations(layer* Layer);

// Miscelleanous
void printLayerActivations(layer* Layer);



#endif // LAYER_H
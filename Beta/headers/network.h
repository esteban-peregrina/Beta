#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "layer.h"
#include "activation.h"
#include "data.h"

typedef struct _layer layer;
typedef struct _neuron neuron;
typedef struct _data data;


typedef struct _network network;
struct _network {
    int inputSize;
    int outputSize;
    
    int hiddenSize;
    int hiddenLenght; // Number of hidden layers
    
    double learningRate;
    
    layer** layers; // Each layer
};

// Memory management
network* createNetwork (int inputSize, int outputSize, int hiddenSize, int hLenght, double lRate, double (*ActivationFunction)(double));
void freeNetwork(network* Network);

// Initialization
void randomizeNetworkConnections(network* Network);

// Forward propagation
void forwardPropagation(network* Network);

// Back propagation
void backPropagation(network* Network, double expectedOutputs[]);
void updateWeights (network* Network);

// Training
void trainNetwork(network* Network, data** Dataset, int size);

// Predict 
double predictNetwork(network* Network, data** Dataset, int datasetSize);

// 
int maxIndex(layer* Layer);

// Miscelleanous
void printNetworkActivations(network* Network);



#endif // NETWORK_H
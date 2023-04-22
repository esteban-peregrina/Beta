#ifndef NEURON_H
#define NEURON_H



#include <network.h>

typedef enum _neuronType {
    nINPUT,
    nHIDDEN,
    nOUTPUT,
} neuronType; // Might be useless, as an output or input layer can be identified by their connexions

typedef struct _neuron neuron;
struct _neuron {
    double* weights;
    double bias;
    double (*activationFunction)(double);
    
    double weightedInput;
    double activation;

    double* weightsGradients;
    double biasGradient;
    neuronType type;
};

// Memory management
neuron* createNeuron(int nbInputs, double (*ActivationFunction)(double), neuronType Type);
void freeNeuron(neuron* Neuron);

// Initialization
void randomizeNeuronConnection(neuron* Neuron, int inputSize);

// Feed forward
void calculateNeuronOutput(neuron* Neuron, int inputSize, double inputs[]);

// Error
double Error(neuron* Neuron, double expectedOutput);
double ErrorDerivative(neuron* Neuron, double expectedOutput);

// Testing connexions (initializing input layer neurons)
void randomizeNeuronActivation(neuron* Neuron);

// Miscelleanous
void printNeuronActivation(neuron* Neuron);



#endif // NEURON_H
#include "../headers/neuron.h"

// Memory mangement
neuron* createNeuron(int nbInputs, double (*ActivationFunction)(double), neuronType Type) {
    neuron* Neuron = malloc(sizeof(neuron));
    if (Neuron == NULL) {
        printf("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    Neuron->type = Type;
    if (Type != nINPUT) {
        Neuron->activationFunction = ActivationFunction;
        Neuron->weights = malloc(nbInputs * sizeof(double));
         Neuron->weightsGradients = malloc(nbInputs * sizeof(double));
        if (Neuron->weights == NULL) {
            printf("Error: Memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }
    else {
        Neuron->activationFunction = NULL;
        Neuron->weights = NULL;
        Neuron->weightsGradients = NULL;
    }

    Neuron->bias = 0; // Initialised in any case (can't set to NULL)
    Neuron->biasGradient = 0;
    Neuron->weightedInput = 0;
    Neuron->activation = 0;
    return Neuron;
}

void freeNeuron(neuron* Neuron) {
    if (Neuron == NULL) {
        printf("Error: NULL parameter");
        exit(EXIT_FAILURE);
    }
    free(Neuron->weights);
    free(Neuron->weightsGradients);
    free(Neuron); 
}

// Initialization
void randomizeNeuronConnection(neuron* Neuron, int inputSize) {
    for (int w = 0; w < inputSize; w++) {
        Neuron->weights[w] = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    }
    Neuron->bias = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

// Feed forward
void calculateNeuronOutput(neuron* Neuron, int inputSize, double inputs[]) {
    // Reset the olds neurons
    Neuron->weightedInput = 0;
    Neuron->activation = 0;

    double wInput = Neuron->bias;
    for (int i = 0; i < inputSize; i++) {
        for (int w = 0; w < inputSize; w++) {
            wInput+= Neuron->weights[w] * inputs[i];
        }
    }
    
    Neuron->weightedInput = wInput;
    Neuron->activation = Neuron->activationFunction(wInput); //Update neuron output
}

// Error
double Error(neuron* Neuron, double expectedOutput) {
    double error = (Neuron->activation - expectedOutput);
    return error * error;
}
double ErrorDerivative(neuron* Neuron, double expectedOutput) {
    double error = (Neuron->activation - expectedOutput);
    return 2 * error;
}
/* 
No need to implement derivative of the weightedInput as it is very straight forward 
(1 with respect to bias, activation with respect to weight and weight with respect to activation) 
*/

// Testing connexions (initializing input layer neurons)
void randomizeNeuronActivation(neuron* Neuron) {
    Neuron->activation = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

// Miscelleanous
void printNeuronActivation(neuron* Neuron) {
    printf("%f\n", Neuron->activation);
}


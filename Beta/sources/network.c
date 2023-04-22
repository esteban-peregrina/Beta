#include "../headers/network.h"

// Memory management
network* createNetwork (int inputSize, int outputSize, int hiddenSize, int hLenght, double lRate, double (*ActivationFunction)(double)) {
    
    network* Network = malloc(sizeof(network));
    if (Network == NULL) {
        printf("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    Network->learningRate = lRate;
    Network->hiddenLenght = hLenght;
    
    Network->layers = malloc((hLenght + 2) * sizeof(layer));
    if (Network->layers == NULL) {
        printf("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    Network->layers[0] = createLayer(NULL, inputSize, NULL, lINPUT);
    for (int l = 1; l <= hLenght; l++) {
        Network->layers[l] = createLayer(Network->layers[l-1], hiddenSize, ActivationFunction, lHIDDEN); // Connect current layer to previous layer
        Network->layers[l-1]->next = Network->layers[l]; // Connect previous layer to current layer
    }
    Network->layers[hLenght + 1] = createLayer(Network->layers[hLenght], outputSize, ActivationFunction, lOUTPUT);
    Network->layers[hLenght]->next = Network->layers[hLenght + 1];
    
    return Network;
}

void freeNetwork(network* Network) {
    layer* currentLayer = Network->layers[0];
    while (currentLayer != NULL) {
        layer* temp = currentLayer->next;
        freeLayer(currentLayer);
        currentLayer = temp;
    }
    
    free(Network->layers);
    free(Network);
}

// Initialization
void randomizeNetworkConnections(network* Network) {
    for (int l = 1; l < Network->hiddenLenght + 2; l++) {
        randomizeLayerConnections(Network->layers[l]);
    }
}

// Forward propagation
void forwardPropagation(network* Network) {
    for (int l = 1; l < Network->hiddenLenght + 2; l++) {
        updateLayer(Network->layers[l]);
    }
}

// Back propagation (Currently storing in each neuron), can add regulation but not needed.
void backPropagation(network* Network, double expectedOutputs[]) {
    layer* outputLayer = Network->layers[Network->hiddenLenght + 1];
    // Output layer
    int previousSize = outputLayer->previous->size;
    for (int n = 0; n < outputLayer->size; n++) {
        neuron* currentNeuron =  outputLayer->neurons[n];
        double currentGradient = 0;
        currentGradient+= CostDerivative(currentNeuron, expectedOutputs[n], outputLayer->size); // ! Size !
        currentGradient*= SigmoidDerivative(currentNeuron->activation);

        currentGradient*= Network->learningRate; // Here
        currentNeuron->biasGradient = currentGradient; // Report to cost derivative with respect to bias
         
        for (int pn = 0; pn < previousSize; pn++) {
            currentGradient *= outputLayer->previous->neurons[pn]->activation; // Previous activation
            currentNeuron->weightsGradients[pn] = currentGradient;
        }
    }
    // Hidden layers
    for (int l = Network->hiddenLenght; l > 0; l--) {
        layer* currentLayer = Network->layers[l];
        layer* nextLayer = currentLayer->next;
        layer* previousLayer = currentLayer->previous;

        for (int n = 0; n < currentLayer->size; n++) {
            neuron* currentNeuron =  currentLayer->neurons[n];
            double currentGradient = 0;

            for (int nn = 0; nn < nextLayer->size; nn++) {
                double nextGradient = nextLayer->neurons[nn]->weightsGradients[n]; // back PROPAGATION
                currentGradient += nextGradient * SigmoidDerivative(currentNeuron->activation) * nextLayer->neurons[nn]->weights[nn]; // With respect to my schema, SigmoidDerivative seems awkward here but it's normal
            }
            
            currentGradient /= nextLayer->size; // In the schema, you divide by the number of starting path
            currentGradient *= Network->learningRate;
            currentNeuron->biasGradient = currentGradient;
            

            for (int np = 0; np < previousLayer->size; np++) {
                double inputActivation = previousLayer->neurons[np]->activation;
                double weightGradient = currentGradient * inputActivation;
                currentNeuron->weightsGradients[np] = weightGradient;
            }

        }
    }
}

// Updating the weights with gardients
void updateWeights (network* Network) {
    for (int l = 1; l < Network->hiddenLenght + 1; l++) {
        layer* currentLayer = Network->layers[l];
        layer* previousLayer = currentLayer->previous;
        
        for (int n = 0; n < currentLayer->size; n++) {
            neuron* currentNeuron = currentLayer->neurons[n];
            
            currentNeuron->bias -= currentNeuron->biasGradient;
            currentNeuron->biasGradient = 0;

            for (int w = 0; w < previousLayer->size; w++) {
                currentNeuron->weights[w] -= currentNeuron->weightsGradients[w];
                currentNeuron->weightsGradients[w] = 0;
            }
        }
    } 
}

// Training
void trainNetwork(network* Network, data** Dataset, int datasetSize) {
    for (int d = 0; d < datasetSize; d++) {
        if (d % 100 == 0) {
            printf("Data n°%d\n", d);
        }

        data* currentData = Dataset[d];
        for (int i = 0; i < currentData->size; i++) {
            // Fill the input layer
            Network->layers[0]->neurons[i]->activation = currentData->inputs[i];
        }
        forwardPropagation(Network);
        
        double* expectedOutput = malloc(Network->outputSize * sizeof(double));
        expectedOutput[currentData->label] = 1.0;
        backPropagation(Network, expectedOutput);
        updateWeights(Network);
    }
}

double predictNetwork(network* Network, data** Dataset, int datasetSize) {
    double correct = 0;
    for (int d = 0; d < datasetSize; d++) {
        data* currentData = Dataset[d];
        for (int i = 0; i < currentData->size; i++) {
            // Fill the input layer
            Network->layers[0]->neurons[i]->activation = currentData->inputs[i];
        }
        forwardPropagation(Network);
        layer* outputLayer = Network->layers[Network->hiddenLenght + 1];
        if (maxIndex(outputLayer) == currentData->label) {
            correct++;
        }
    }
    return 1.0 * correct / datasetSize;
}

int maxIndex(layer* Layer) {
    double maxIndex = 0;
    double maxScore = 0;
    for (int n = 0; n < Layer->size; n++) {
        neuron* currentNeuron = Layer->neurons[n];
        if (currentNeuron->activation > maxScore) {
            maxIndex = n;
            maxScore = currentNeuron->activation;
        }
    }
    return maxIndex;
} 

// Miscelleanous
void printNetworkActivations(network* Network) {
    layer* currentLayer = Network->layers[0];
    while (currentLayer != NULL) {
        printf("---------\n");
        printLayerActivations(currentLayer);
        currentLayer = currentLayer->next;
    }
}


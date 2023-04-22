#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "headers/data.h"
#include "headers/network.h"


/*
Next :
- CSV saving
- Backpropagation
*/

int main() {
    chdir("path to data");
    data** trainingImages = importCSV("dataset/mnist_train.csv", 60000, 784);
    data** testImages = importCSV("dataset/mnist_test.csv", 10000, 784);
    
    chdir("path to working directory");

    srand(time(NULL));
    
    network* myNetwork = createNetwork(784, 10, 20, 2, 0.2, Sigmoid);
    randomizeNetworkConnections(myNetwork);
    //printf("%f\n", myNetwork->layers[2]->neurons[3]->weights[1]);
    
    trainNetwork(myNetwork, trainingImages, 100);
    double score = predictNetwork(myNetwork, testImages, 10);
    printf("Score : %f\n", score);
    
    freeNetwork(myNetwork);
    
    return EXIT_SUCCESS;
}

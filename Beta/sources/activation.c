#include "../headers/activation.h"

double Sigmoid(double input) {
    return 1 / (1 + exp(-input));
}
double SigmoidDerivative(double input) {
    double sigmoid = Sigmoid(input);
    return sigmoid * (1 - sigmoid);
}
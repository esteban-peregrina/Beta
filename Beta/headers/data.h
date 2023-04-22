#ifndef DATA_H
#define DATA_H

#include "network.h"
#include <string.h>

#define MAXCHAR 100000

typedef struct _data data;
struct _data {
    double* inputs;
    double size;
    int label;
};

// Memory management
void freeData(data* Data);

// Importation
data** importCSV(char* filename,  int datasetSize, int dataSize);

// Miscellaneous
void printData(data* Data);

#endif // DATA_H
#include "../headers/data.h"

// Memory management
void freeData(data* Data) {
    if (Data == NULL) {
        printf("Error: NULL parameter");
        exit(EXIT_FAILURE);
    }
    free(Data->inputs);
    free(Data); 
}

// Importation
data** importCSV(char* filename, int datasetSize, int dataSize) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        data** Data = malloc(datasetSize * sizeof(data*)); 
        
        char row[MAXCHAR]; 
        fgets(row, MAXCHAR, file); 
        for(int d = 0; !feof(file) && d < datasetSize; d++) {
            fgets(row, MAXCHAR, file); 
            char* token = strtok(row, ","); 
            
            Data[d] = malloc(sizeof(data));
            Data[d]->size = dataSize;
            Data[d]->inputs = malloc(dataSize * sizeof(double));
            for(int i = 0; token != NULL; i++) {
                if (i == 0) {
                    Data[d]->label = atoi(token); // First token contain label
                } else {
                    Data[d]->inputs[i] = atoi(token) / 256.0;
                }
                token = strtok(NULL, ","); 
            }
        }
        fclose(file);
        
        return Data;
    }
    else {
        printf("Error : Can't read file %s in csv_to_datas.\n", filename);
        exit(EXIT_FAILURE);
    }
}

void printData(data* Data) {
    for (int d = 0; d < Data->size; d++) {
        printf("%f\n", Data->inputs[d]);
    }
}
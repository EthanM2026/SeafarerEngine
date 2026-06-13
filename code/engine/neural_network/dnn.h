#ifndef DNN_H_INCLUDED
#define DNN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "cnn.h"

// --- Data Structures ---

typedef struct {
    int input_size;
    int output_size;
    double **weights; // 2D array [output_size][input_size]
    double *biases;   // Array [output_size]
    double *inputs;   // Pointer to inputs used during forward pass (for backprop)
    double *outputs;  // Array [output_size] (activated values)
    double *zs;       // Array [output_size] (raw values before activation)
    double *dC_db;    // Bias gradients
    double **dC_dw;   // Weight gradients
} Layer;

typedef struct {
    int num_layers;
    int *layer_sizes; // Array holding the size of each layer (input, hiddens, output)
    Layer *layers;    // Array of Layer structs
} DenseNetwork;

// --- Helper Functions ---

double relu(double x);
double relu_derivative(double x);

// --- Network Setup & Allocation ---

DenseNetwork* create_network(int *sizes, int num_layers);
double* forward_pass(DenseNetwork *net, double *input_vector);
void backpropagate(DenseNetwork *net, int target_class, double lr);
int CreateDNN();

#endif // DNN_H_INCLUDED

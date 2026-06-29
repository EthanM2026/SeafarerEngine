#ifndef NEURAL_NETWORK_H_INCLUDED
#define NEURAL_NETWORK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --------------------------------------------------------
// 1. Structures
// --------------------------------------------------------

typedef struct {
    int num_inputs;      // Inputs from the previous NNLayer
    int num_neurons;     // Neurons in this NNLayer
    double *weights;     // 1D array of size [num_neurons * num_inputs]
    double *biases;      // 1D array of size [num_neurons]
    double *outputs;     // 1D array of size [num_neurons]
    double *deltas;      // 1D array of size [num_neurons] for backprop
} NNLayer;

typedef struct {
    int num_inputs;      // Network input size
    int num_NNLayers;      // Total number of NNLayers (hidden + output)
    NNLayer *NNLayers;       // Array of NNLayers
} Network;

// --------------------------------------------------------
// 2. Math & Activations
// --------------------------------------------------------

// Generates a random double between -0.5 and 0.5
double random_weight();

// Sigmoid activation function
double sigmoid(double x);

// Derivative of sigmoid (assuming x is already activated)
double sigmoid_derivative(double x);

// --------------------------------------------------------
// 3. Initialization & Memory Management
// --------------------------------------------------------

// Initialize a single NNLayer
void init_NNLayer(NNLayer *NNLayer, int num_inputs, int num_neurons);

// Create the network
Network* create_network(int num_inputs, int num_hidden_NNLayers, int hidden_size, int num_outputs);

// Free memory
void free_network(Network *net);

// --------------------------------------------------------
// 4. Feedforward & Backpropagation
// --------------------------------------------------------

// Forward pass
double* feed_forward(Network *net, double *inputs);

// Train a single sample
void backpropagate(Network *net, double *inputs, double *targets, double learning_rate);

// --------------------------------------------------------
// 5. File Saving and Loading
// --------------------------------------------------------

void save_network(Network *net, const char *filename);

Network* load_network(const char *filename);

// --------------------------------------------------------
// 6. Example Usage (Airplane Steering Context)
// --------------------------------------------------------

int CreateNet();
#endif // NEURAL_NETWORK_H_INCLUDED

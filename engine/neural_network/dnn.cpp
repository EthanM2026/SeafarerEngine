#include "dnn.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- Data Structures ---


double relu(double x) { return x > 0 ? x : 0; };
double relu_derivative(double x) { return x > 0 ? 1 : 0; };

// --- Network Setup & Allocation ---

DenseNetwork* create_network(int *sizes, int num_layers) {
    DenseNetwork *net = (DenseNetwork*)malloc(sizeof(DenseNetwork));
    net->num_layers = num_layers - 1; // Number of weight layers is total layers minus input layer
    net->layer_sizes = sizes;
    net->layers = (Layer*)malloc(net->num_layers * sizeof(Layer));

    for (int i = 0; i < net->num_layers; i++) {
        int in_dim = sizes[i];
        int out_dim = sizes[i + 1];

        net->layers[i].input_size = in_dim;
        net->layers[i].output_size = out_dim;
        net->layers[i].biases = (double*)calloc(out_dim, sizeof(double));
        net->layers[i].outputs = (double*)calloc(out_dim, sizeof(double));
        net->layers[i].zs = (double*)calloc(out_dim, sizeof(double));
        net->layers[i].dC_db = (double*)calloc(out_dim, sizeof(double));

        net->layers[i].weights = (double**)malloc(out_dim * sizeof(double*));
        net->layers[i].dC_dw = (double**)malloc(out_dim * sizeof(double*));

        // He (Kaiming) Initialization for weights
        double stddev = sqrt(2.0 / in_dim);
        for (int j = 0; j < out_dim; j++) {
            net->layers[i].weights[j] = (double*)malloc(in_dim * sizeof(double));
            net->layers[i].dC_dw[j] = (double*)calloc(in_dim, sizeof(double));
            net->layers[i].biases[j] = 0.0; // Initialize biases to 0
            for (int k = 0; k < in_dim; k++) {
                net->layers[i].weights[j][k] = random_normal() * stddev;
            }
        }
    }
    return net;
}

// --- Forward Propagation ---

double* forward_pass(DenseNetwork *net, double *input_vector) {
    double *current_input = input_vector;

    for (int l = 0; l < net->num_layers; l++) {
        Layer *layer = &net->layers[l];
        layer->inputs = current_input; // Cache input pointer for backprop

        for (int i = 0; i < layer->output_size; i++) {
            double sum = layer->biases[i];
            for (int j = 0; j < layer->input_size; j++) {
                sum += current_input[j] * layer->weights[i][j];
            }
            layer->zs[i] = sum;

            // Apply activation function
            if (l == net->num_layers - 1) {
                // Softmax raw outputs computed comprehensively at the end
                layer->outputs[i] = sum;
            } else {
                layer->outputs[i] = relu(sum);
            }
        }

        // Final Layer Softmax execution block
        if (l == net->num_layers - 1) {
            double max = layer->outputs[0];
            for (int i = 1; i < layer->output_size; i++) if (layer->outputs[i] > max) max = layer->outputs[i];

            double sum_exp = 0.0;
            for (int i = 0; i < layer->output_size; i++) {
                layer->outputs[i] = exp(layer->outputs[i] - max); // Numerical stability
                sum_exp += layer->outputs[i];
            }
            for (int i = 0; i < layer->output_size; i++) {
                layer->outputs[i] /= sum_exp;
            }
        }

        current_input = layer->outputs;
    }
    return net->layers[net->num_layers - 1].outputs;
}

// --- Backpropagation & Optimization ---

void backpropagate(DenseNetwork *net, int target_class, double lr) {
    int last_idx = net->num_layers - 1;
    Layer *output_layer = &net->layers[last_idx];

    // 1. Compute output layer error (Softmax + Cross Entropy derivative)
    double *dC_dz = (double*)malloc(output_layer->output_size * sizeof(double));
    for (int i = 0; i < output_layer->output_size; i++) {
        dC_dz[i] = output_layer->outputs[i];
    }
    dC_dz[target_class] -= 1.0;

    // 2. Loop backwards through all layers
    for (int l = last_idx; l >= 0; l--) {
        Layer *layer = &net->layers[l];
        double *next_dC_dz = NULL;

        if (l > 0) {
            next_dC_dz = (double*)calloc(layer->input_size, sizeof(double));
        }

        // Compute gradients for current layer
        for (int i = 0; i < layer->output_size; i++) {
            layer->dC_db[i] = dC_dz[i];
            for (int j = 0; j < layer->input_size; j++) {
                layer->dC_dw[i][j] = dC_dz[i] * layer->inputs[j];

                // Accumulate error for the prior hidden layer if it exists
                if (l > 0) {
                    next_dC_dz[j] += dC_dz[i] * layer->weights[i][j];
                }
            }
        }

        // Apply Gradient Descent to weights and biases instantly
        for (int i = 0; i < layer->output_size; i++) {
            layer->biases[i] -= lr * layer->dC_db[i];
            for (int j = 0; j < layer->input_size; j++) {
                layer->weights[i][j] -= lr * layer->dC_dw[i][j];
            }
        }

        free(dC_dz);

        // Prepare next iteration's error array by accounting for ReLU derivative
        if (l > 0) {
            Layer *prev_layer = &net->layers[l - 1];
            for (int j = 0; j < layer->input_size; j++) {
                next_dC_dz[j] *= relu_derivative(prev_layer->zs[j]);
            }
            dC_dz = next_dC_dz;
        }
    }
}

// --- Main Testing Framework ---

int CreateDNN() {
    srand(1337); // Seed for reproducible matrix checks

    // --- Dynamic Topology Configuration ---
    // Modify this array to change your network size and depth dynamically!
    // Example layout: { Input size, Hidden 1 size, Hidden 2 size, Output size }
    int topology[] = {8, 16, 10, 3};
    int num_layers = sizeof(topology) / sizeof(topology[0]);
    double learning_rate = 0.05;

    DenseNetwork *net = create_network(topology, num_layers);

    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            num_layers, topology[0], num_layers - 2, topology[num_layers - 1]);

    // Create a dummy training input (Vector size 8)
    double *dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) dummy_input[i] = (double)rand() / RAND_MAX;
    int target_label = 2; // We want to train the network to classify this pattern as Class '2'

    // --- Training Loop ---
    printf("\nTraining Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        double *outputs = forward_pass(net, dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(net, target_label, learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(outputs[target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, target_label, outputs[target_label] * 100.0);
        }
    }

    printf("\nOptimization complete!\n");

    // Memory Cleanup
    free(dummy_input);
    for (int i = 0; i < net->num_layers; i++) {
        for (int j = 0; j < net->layers[i].output_size; j++) {
            free(net->layers[i].weights[j]);
            free(net->layers[i].dC_dw[j]);
        }
        free(net->layers[i].weights);
        free(net->layers[i].dC_dw);
        free(net->layers[i].biases);
        free(net->layers[i].outputs);
        free(net->layers[i].zs);
        free(net->layers[i].dC_db);
    }
    free(net->layers);
    free(net);

    return 0;
}

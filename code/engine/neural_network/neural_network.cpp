#include "neural_network.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --------------------------------------------------------
// 1. Structures
// --------------------------------------------------------

// --------------------------------------------------------
// 2. Math & Activations
// --------------------------------------------------------

// Generates a random double between -0.5 and 0.5
double random_weight() {
    return ((double)rand() / (double)RAND_MAX) - 0.5;
}

// Sigmoid activation function
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of sigmoid (assuming x is already activated)
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// --------------------------------------------------------
// 3. Initialization & Memory Management
// --------------------------------------------------------

// Initialize a single NNLayer
void init_NNLayer(NNLayer *NNLayer, int num_inputs, int num_neurons) {
    NNLayer->num_inputs = num_inputs;
    NNLayer->num_neurons = num_neurons;

    NNLayer->weights = (double*)malloc(num_inputs * num_neurons * sizeof(double));
    NNLayer->biases = (double*)malloc(num_neurons * sizeof(double));
    NNLayer->outputs = (double*)malloc(num_neurons * sizeof(double));
    NNLayer->deltas = (double*)malloc(num_neurons * sizeof(double));

    for (int i = 0; i < num_inputs * num_neurons; i++) {
        NNLayer->weights[i] = random_weight();
    }
    for (int i = 0; i < num_neurons; i++) {
        NNLayer->biases[i] = random_weight();
        NNLayer->outputs[i] = 0.0;
        NNLayer->deltas[i] = 0.0;
    }
}

// Create the network
Network* create_network(int num_inputs, int num_hidden_NNLayers, int hidden_size, int num_outputs) {
    Network *net = (Network*)malloc(sizeof(Network));
    net->num_inputs = num_inputs;
    net->num_NNLayers = num_hidden_NNLayers + 1; // Hidden + 1 Output NNLayer
    net->NNLayers = (NNLayer*)malloc(net->num_NNLayers * sizeof(NNLayer));

    // First hidden NNLayer
    init_NNLayer(&net->NNLayers[0], num_inputs, hidden_size);

    // Additional hidden NNLayers
    for (int i = 1; i < num_hidden_NNLayers; i++) {
        init_NNLayer(&net->NNLayers[i], hidden_size, hidden_size);
    }

    // Output NNLayer
    init_NNLayer(&net->NNLayers[net->num_NNLayers - 1], hidden_size, num_outputs);

    return net;
}

// Free memory
void free_network(Network *net) {
    for (int i = 0; i < net->num_NNLayers; i++) {
        free(net->NNLayers[i].weights);
        free(net->NNLayers[i].biases);
        free(net->NNLayers[i].outputs);
        free(net->NNLayers[i].deltas);
    }
    free(net->NNLayers);
    free(net);
}

// --------------------------------------------------------
// 4. Feedforward & Backpropagation
// --------------------------------------------------------

// Forward pass
double* feed_forward(Network *net, double *inputs) {
    for (int l = 0; l < net->num_NNLayers; l++) {
        NNLayer *NNLayer = &net->NNLayers[l];
        double *NNLayer_inputs = (l == 0) ? inputs : net->NNLayers[l-1].outputs;

        for (int j = 0; j < NNLayer->num_neurons; j++) {
            double activation = NNLayer->biases[j];
            for (int i = 0; i < NNLayer->num_inputs; i++) {
                // index = neuron_index * num_inputs + input_index
                activation += NNLayer_inputs[i] * NNLayer->weights[j * NNLayer->num_inputs + i];
            }
            NNLayer->outputs[j] = sigmoid(activation);
        }
    }
    return net->NNLayers[net->num_NNLayers - 1].outputs; // Return final NNLayer outputs
}

// Train a single sample
void backpropagate(Network *net, double *inputs, double *targets, double learning_rate) {
    // 1. Perform forward pass
    feed_forward(net, inputs);

    // 2. Calculate Output NNLayer Deltas (Error * Derivative)
    NNLayer *output_NNLayer = &net->NNLayers[net->num_NNLayers - 1];
    for (int j = 0; j < output_NNLayer->num_neurons; j++) {
        double output = output_NNLayer->outputs[j];
        double error = targets[j] - output;
        output_NNLayer->deltas[j] = error * sigmoid_derivative(output);
    }

    // 3. Calculate Hidden NNLayer Deltas (Propagate backwards)
    for (int l = net->num_NNLayers - 2; l >= 0; l--) {
        NNLayer *current_NNLayer = &net->NNLayers[l];
        NNLayer *next_NNLayer = &net->NNLayers[l + 1];

        for (int j = 0; j < current_NNLayer->num_neurons; j++) {
            double error = 0.0;
            // Sum the weight * delta from the next NNLayer
            for (int k = 0; k < next_NNLayer->num_neurons; k++) {
                error += next_NNLayer->weights[k * next_NNLayer->num_inputs + j] * next_NNLayer->deltas[k];
            }
            current_NNLayer->deltas[j] = error * sigmoid_derivative(current_NNLayer->outputs[j]);
        }
    }

    // 4. Update Weights and Biases
    for (int l = 0; l < net->num_NNLayers; l++) {
        NNLayer *NNLayer = &net->NNLayers[l];
        double *NNLayer_inputs = (l == 0) ? inputs : net->NNLayers[l-1].outputs;

        for (int j = 0; j < NNLayer->num_neurons; j++) {
            NNLayer->biases[j] += learning_rate * NNLayer->deltas[j];
            for (int i = 0; i < NNLayer->num_inputs; i++) {
                NNLayer->weights[j * NNLayer->num_inputs + i] += learning_rate * NNLayer->deltas[j] * NNLayer_inputs[i];
            }
        }
    }
}

// --------------------------------------------------------
// 5. File Saving and Loading
// --------------------------------------------------------

void save_network(Network *net, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) { perror("Failed to open file for saving"); return; }

    fwrite(&net->num_inputs, sizeof(int), 1, file);
    fwrite(&net->num_NNLayers, sizeof(int), 1, file);

    for (int l = 0; l < net->num_NNLayers; l++) {
        NNLayer *NNLayer = &net->NNLayers[l];
        fwrite(&NNLayer->num_inputs, sizeof(int), 1, file);
        fwrite(&NNLayer->num_neurons, sizeof(int), 1, file);
        fwrite(NNLayer->weights, sizeof(double), NNLayer->num_inputs * NNLayer->num_neurons, file);
        fwrite(NNLayer->biases, sizeof(double), NNLayer->num_neurons, file);
    }

    fclose(file);
    printf("Network saved to %s\n", filename);
}

Network* load_network(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) { perror("Failed to open file for loading"); return NULL; }

    Network *net = (Network*)malloc(sizeof(Network));
    fread(&net->num_inputs, sizeof(int), 1, file);
    fread(&net->num_NNLayers, sizeof(int), 1, file);

    net->NNLayers = (NNLayer*)malloc(net->num_NNLayers * sizeof(NNLayer));

    for (int l = 0; l < net->num_NNLayers; l++) {
        NNLayer *NNLayer = &net->NNLayers[l];
        fread(&NNLayer->num_inputs, sizeof(int), 1, file);
        fread(&NNLayer->num_neurons, sizeof(int), 1, file);

        // Allocate memory for weights, biases, outputs, and deltas
        NNLayer->weights = (double*)malloc(NNLayer->num_inputs * NNLayer->num_neurons * sizeof(double));
        NNLayer->biases = (double*)malloc(NNLayer->num_neurons * sizeof(double));
        NNLayer->outputs = (double*)malloc(NNLayer->num_neurons * sizeof(double));
        NNLayer->deltas = (double*)malloc(NNLayer->num_neurons * sizeof(double));

        fread(NNLayer->weights, sizeof(double), NNLayer->num_inputs * NNLayer->num_neurons, file);
        fread(NNLayer->biases, sizeof(double), NNLayer->num_neurons, file);

        // Initialize runtime arrays to zero
        for (int i = 0; i < NNLayer->num_neurons; i++) {
            NNLayer->outputs[i] = 0.0;
            NNLayer->deltas[i] = 0.0;
        }
    }

    fclose(file);
    printf("Network loaded from %s\n", filename);
    return net;
}

// --------------------------------------------------------
// 6. Example Usage (Airplane Steering Context)
// --------------------------------------------------------

int CreateNet() {
    srand(time(NULL));

    // Define Network: 3 Inputs, 2 Hidden NNLayers (8 neurons each), 2 Outputs
    // Inputs: [Pitch angle, Roll angle, Altitude error]
    // Outputs: [Elevator command, Aileron command]
    int inputs = 3;
    int hidden_NNLayers = 2;
    int hidden_size = 8;
    int outputs = 2;

    printf("Creating network...\n");
    Network *autopilot = create_network(inputs, hidden_NNLayers, hidden_size, outputs);

    // Dummy training data:
    // If nose is down (negative pitch), pull up (high elevator)
    // Values normalized between 0.0 and 1.0 for the Sigmoid function
    double train_in[3]  = {0.2, 0.5, 0.5}; // Pitch down, wings level, altitude good
    double train_out[2] = {0.8, 0.5};      // Elevator UP, Ailerons neutral

    printf("Training over 10,000 epochs...\n");
    for (int epoch = 0; epoch < 10000; epoch++) {
        backpropagate(autopilot, train_in, train_out, 0.1); // Learning rate = 0.1
    }

    // Test the network
    double *result = feed_forward(autopilot, train_in);
    printf("After training - Target: [%.2f, %.2f] | Output: [%.2f, %.2f]\n",
           train_out[0], train_out[1], result[0], result[1]);

    // Save, Free, and Reload to demonstrate persistence
    save_network(autopilot, "autopilot_model.bin");
    free_network(autopilot);

    Network *loaded_autopilot = load_network("autopilot_model.bin");

    // Test loaded network to verify state
    result = feed_forward(loaded_autopilot, train_in);
    printf("Loaded network Output: [%.2f, %.2f]\n", result[0], result[1]);

    free_network(loaded_autopilot);
    return 0;
}

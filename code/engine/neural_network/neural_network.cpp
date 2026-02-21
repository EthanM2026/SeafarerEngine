#include <stdio.h>
#include <stdlib.h>
#include <math.h>   // For expf (e^x) and sqrtf
#include <time.h>   // For seeding random number generator

// --- Basic Vector/Matrix Operations (Simplified for this NN) ---

// A simple dynamic array/vector structure
typedef struct {
    float* data;
    int size;
} Vec;

// Function to create a new vector
Vec* create_vec(int size) {
    Vec* v = (Vec*)calloc(1,sizeof(Vec));
    if (!v) {
        fprintf(stderr, "Error: Could not allocate memory for Vec.\n");
        exit(EXIT_FAILURE);
    }
    v->size = size;
    v->data = (float*)calloc(1,sizeof(float) * size);
    if (!v->data) {
        fprintf(stderr, "Error: Could not allocate memory for Vec data.\n");
        free(v);
        exit(EXIT_FAILURE);
    }
    return v;
}

// Function to free a vector
void free_vec(Vec* v) {
    if (v) {
        free(v->data);
        free(v);
    }
}

// Function to print a vector (for debugging)
void print_vec(const char* name, const Vec* v) {
    printf("%s [", name);
    for (int i = 0; i < v->size; ++i) {
        printf("%.4f%s", v->data[i], (i == v->size - 1) ? "" : ", ");
    }
    printf("]\n");
}

// --- Neural Network Structures ---

// Structure for a single layer in the neural network
typedef struct {
    int num_neurons;    // Number of neurons in this layer
    int num_inputs;     // Number of inputs to this layer (from previous layer)

    Vec* weights;       // Weights connecting inputs to neurons (num_neurons * num_inputs)
                        // Stored as a flat array. Access weight[neuron_idx * num_inputs + input_idx]
    Vec* biases;        // Biases for each neuron in this layer (num_neurons)

    Vec* activations;   // Output activations of this layer (num_neurons)
    Vec* z_values;      // Weighted sums before activation (num_neurons)

    // For backpropagation:
    Vec* delta;         // Error delta for each neuron in this layer (num_neurons)
} Layer;

// Structure for the entire neural network
typedef struct {
    int num_layers;     // Total number of layers (including input, hidden, output)
    Layer** layers;     // Array of pointers to Layer structures
} NeuralNetwork;

// --- Activation Functions ---

// Sigmoid activation function
float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

// Derivative of the sigmoid function
float sigmoid_derivative(float x) {
    // This expects the *output* of the sigmoid function, not the raw input (z_value)
    return x * (1.0f - x);
}

// --- Neural Network Initialization and Memory Management ---

/**
 * @brief Initializes a new neural network.
 *
 * @param input_size Number of input features.
 * @param hidden_size Number of neurons in the hidden layer.
 * @param output_size Number of neurons in the output layer.
 * @return Pointer to the newly created NeuralNetwork.
 */
NeuralNetwork* nn_init(int input_size, int hidden_size, int output_size) {
    NeuralNetwork* nn = (NeuralNetwork*)calloc(1,sizeof(NeuralNetwork));
    if (!nn) {
        fprintf(stderr, "Error: Could not allocate memory for NeuralNetwork.\n");
        exit(EXIT_FAILURE);
    }
    nn->num_layers = 3; // Input, Hidden, Output

    nn->layers = (Layer**)calloc(1,sizeof(Layer*) * nn->num_layers);
    if (!nn->layers) {
        fprintf(stderr, "Error: Could not allocate memory for NN layers array.\n");
        free(nn);
        exit(EXIT_FAILURE);
    }

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // --- Create Hidden Layer (Layer 0 in our array, after input) ---
    Layer* hidden_layer = (Layer*)calloc(1,sizeof(Layer));
    if (!hidden_layer) { free(nn->layers); free(nn); exit(EXIT_FAILURE); }
    hidden_layer->num_neurons = hidden_size;
    hidden_layer->num_inputs = input_size;
    hidden_layer->weights = create_vec(hidden_size * input_size);
    hidden_layer->biases = create_vec(hidden_size);
    hidden_layer->activations = create_vec(hidden_size);
    hidden_layer->z_values = create_vec(hidden_size);
    hidden_layer->delta = create_vec(hidden_size);

    // Initialize weights and biases for hidden layer
    // Using a simple random initialization (e.g., Glorot/Xavier is better for deeper nets)
    for (int i = 0; i < hidden_size * input_size; ++i) {
        hidden_layer->weights->data[i] = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * sqrtf(2.0f / (input_size + hidden_size)); // He/Xavier-like
    }
    for (int i = 0; i < hidden_size; ++i) {
        hidden_layer->biases->data[i] = 0.0f; // Biases often initialized to 0
    }
    nn->layers[0] = hidden_layer;

    // --- Create Output Layer (Layer 1 in our array) ---
    Layer* output_layer = (Layer*)calloc(1,sizeof(Layer));
    if (!output_layer) { free_vec(hidden_layer->weights); free_vec(hidden_layer->biases); free_vec(hidden_layer->activations); free_vec(hidden_layer->z_values); free_vec(hidden_layer->delta); free(hidden_layer); free(nn->layers); free(nn); exit(EXIT_FAILURE); }
    output_layer->num_neurons = output_size;
    output_layer->num_inputs = hidden_size;
    output_layer->weights = create_vec(output_size * hidden_size);
    output_layer->biases = create_vec(output_size);
    output_layer->activations = create_vec(output_size);
    output_layer->z_values = create_vec(output_size);
    output_layer->delta = create_vec(output_size);

    // Initialize weights and biases for output layer
    for (int i = 0; i < output_size * hidden_size; ++i) {
        output_layer->weights->data[i] = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * sqrtf(2.0f / (hidden_size + output_size)); // He/Xavier-like
    }
    for (int i = 0; i < output_size; ++i) {
        output_layer->biases->data[i] = 0.0f;
    }
    nn->layers[1] = output_layer;

    return nn;
}

/**
 * @brief Frees all memory allocated for the neural network.
 * @param nn Pointer to the NeuralNetwork to free.
 */
void nn_free(NeuralNetwork* nn) {
    if (nn) {
        for (int i = 0; i < nn->num_layers -1 ; ++i) { // Iterate through actual layers (hidden, output)
            Layer* layer = nn->layers[i];
            if (layer) {
                free_vec(layer->weights);
                free_vec(layer->biases);
                free_vec(layer->activations);
                free_vec(layer->z_values);
                free_vec(layer->delta);
                free(layer);
            }
        }
        free(nn->layers);
        free(nn);
    }
}

// --- Forward Pass ---

/**
 * @brief Performs the forward pass through the neural network.
 *
 * @param nn Pointer to the NeuralNetwork.
 * @param input_data Pointer to the input features vector.
 * @return Pointer to the output activations vector of the network.
 */
Vec* nn_forward(NeuralNetwork* nn, const Vec* input_data) {
    // The input layer itself doesn't have weights/biases, its "activations" are the input_data.
    // So, the first actual computational layer is the hidden layer (nn->layers[0]).
    // The previous_activations will be updated layer by layer.
    const Vec* previous_activations = input_data;

    // Hidden Layer (nn->layers[0])
    Layer* hidden_layer = nn->layers[0];
    for (int j = 0; j < hidden_layer->num_neurons; ++j) {
        float sum = hidden_layer->biases->data[j]; // Start with bias
        for (int k = 0; k < hidden_layer->num_inputs; ++k) {
            // weights[neuron_idx * num_inputs + input_idx]
            sum += hidden_layer->weights->data[j * hidden_layer->num_inputs + k] * previous_activations->data[k];
        }
        hidden_layer->z_values->data[j] = sum;
        hidden_layer->activations->data[j] = sigmoid(sum);
    }

    // Output Layer (nn->layers[1])
    Layer* output_layer = nn->layers[1];
    previous_activations = hidden_layer->activations; // Output of hidden layer is input to output layer
    for (int j = 0; j < output_layer->num_neurons; ++j) {
        float sum = output_layer->biases->data[j]; // Start with bias
        for (int k = 0; k < output_layer->num_inputs; ++k) {
            sum += output_layer->weights->data[j * output_layer->num_inputs + k] * previous_activations->data[k];
        }
        output_layer->z_values->data[j] = sum;
        output_layer->activations->data[j] = sigmoid(sum); // Use sigmoid for output as well (good for binary classification)
    }

    return output_layer->activations; // Return the final output of the network
}

// --- Backpropagation ---

/**
 * @brief Performs the backpropagation algorithm to update weights and biases.
 *
 * @param nn Pointer to the NeuralNetwork.
 * @param input_data Pointer to the input features vector used in the forward pass.
 * @param target_output Pointer to the target output vector.
 * @param learning_rate The learning rate for weight and bias updates.
 */
void nn_backward(NeuralNetwork* nn, const Vec* input_data, const Vec* target_output, float learning_rate) {
    // 1. Calculate output layer error (delta)
    Layer* output_layer = nn->layers[1];
    for (int i = 0; i < output_layer->num_neurons; ++i) {
        float error = target_output->data[i] - output_layer->activations->data[i];
        output_layer->delta->data[i] = error * sigmoid_derivative(output_layer->activations->data[i]);
    }

    // 2. Calculate hidden layer error (delta)
    Layer* hidden_layer = nn->layers[0];
    for (int i = 0; i < hidden_layer->num_neurons; ++i) {
        float error_sum = 0.0f;
        // Sum of (weight_from_hidden_to_output * delta_of_output_neuron)
        for (int j = 0; j < output_layer->num_neurons; ++j) {
            // weights[output_neuron_idx * num_inputs_to_output_layer + hidden_neuron_idx]
            error_sum += output_layer->weights->data[j * output_layer->num_inputs + i] * output_layer->delta->data[j];
        }
        hidden_layer->delta->data[i] = error_sum * sigmoid_derivative(hidden_layer->activations->data[i]);
    }

    // 3. Update weights and biases for Output Layer
    // previous_activations for output layer are hidden_layer->activations
    const Vec* prev_activations_output = hidden_layer->activations;
    for (int i = 0; i < output_layer->num_neurons; ++i) { // For each neuron in output layer
        for (int j = 0; j < output_layer->num_inputs; ++j) { // For each input to this neuron (from hidden layer)
            // weights[neuron_idx * num_inputs + input_idx]
            output_layer->weights->data[i * output_layer->num_inputs + j] +=
                learning_rate * output_layer->delta->data[i] * prev_activations_output->data[j];
        }
        output_layer->biases->data[i] += learning_rate * output_layer->delta->data[i];
    }

    // 4. Update weights and biases for Hidden Layer
    // previous_activations for hidden layer are input_data
    const Vec* prev_activations_hidden = input_data;
    for (int i = 0; i < hidden_layer->num_neurons; ++i) { // For each neuron in hidden layer
        for (int j = 0; j < hidden_layer->num_inputs; ++j) { // For each input to this neuron (from input layer)
            hidden_layer->weights->data[i * hidden_layer->num_inputs + j] +=
                learning_rate * hidden_layer->delta->data[i] * prev_activations_hidden->data[j];
        }
        hidden_layer->biases->data[i] += learning_rate * hidden_layer->delta->data[i];
    }
}

// --- Training Example (XOR Problem) ---

int maintwo() {
    // Define the XOR dataset
    Vec* xor_inputs[] = {
        create_vec(2), create_vec(2), create_vec(2), create_vec(2)
    };
    xor_inputs[0]->data[0] = 0.0f; xor_inputs[0]->data[1] = 0.0f;
    xor_inputs[1]->data[0] = 0.0f; xor_inputs[1]->data[1] = 1.0f;
    xor_inputs[2]->data[0] = 1.0f; xor_inputs[2]->data[1] = 0.0f;
    xor_inputs[3]->data[0] = 1.0f; xor_inputs[3]->data[1] = 1.0f;

    Vec* xor_targets[] = {
        create_vec(1), create_vec(1), create_vec(1), create_vec(1)
    };
    xor_targets[0]->data[0] = 0.0f;
    xor_targets[1]->data[0] = 1.0f;
    xor_targets[2]->data[0] = 1.0f;
    xor_targets[3]->data[0] = 0.0f;

    int num_training_examples = 4;

    // Initialize the neural network
    // Input: 2 neurons (for 2 XOR inputs)
    // Hidden: 4 neurons (a common choice for XOR)
    // Output: 1 neuron (for 1 XOR output)
    NeuralNetwork* nn = nn_init(2, 4, 1);

    int epochs = 10000;
    float learning_rate = 0.1f;

    printf("Starting training for XOR problem...\n");

    for (int epoch = 0; epoch < epochs; ++epoch) {
        float total_loss = 0.0f;
        for (int i = 0; i < num_training_examples; ++i) {
            // Forward pass
            Vec* prediction = nn_forward(nn, xor_inputs[i]);

            // Calculate loss (Mean Squared Error for simplicity)
            float error = xor_targets[i]->data[0] - prediction->data[0];
            total_loss += 0.5f * error * error; // 0.5 * (target - predicted)^2

            // Backpropagation
            nn_backward(nn, xor_inputs[i], xor_targets[i], learning_rate);
        }

        if (epoch % 1000 == 0) {
            printf("Epoch %d, Total Loss: %.6f\n", epoch, total_loss);
        }
    }

    printf("\nTraining complete. Testing network:\n");

    // Test the trained network
    for (int i = 0; i < num_training_examples; ++i) {
        Vec* prediction = nn_forward(nn, xor_inputs[i]);
        printf("Input: (%.0f, %.0f) -> Expected: %.0f, Predicted: %.4f\n",
               xor_inputs[i]->data[0], xor_inputs[i]->data[1],
               xor_targets[i]->data[0], prediction->data[0]);
    }

    // Clean up memory
    nn_free(nn);
    for (int i = 0; i < num_training_examples; ++i) {
        free_vec(xor_inputs[i]);
        free_vec(xor_targets[i]);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Activation Functions
#define RELU(x) (1.0 / (1.0 + exp(-(x))))
#define SIGMOID(x) (1.0 / (1.0 + exp(-(x))))

// Configuration structure to easily alter layer sizes
typedef struct {
    int input_size;
    int hidden_size;
    int output_size;
} NetworkConfig;

// The Recurrent Neural Network Structure
typedef struct {
    NetworkConfig config;

    // Weights
    double **W_ih; // Input to Hidden weights
    double **W_hh; // Hidden to Hidden (Recurrent) weights
    double **W_ho; // Hidden to Output weights

    // Biases
    double *b_h;   // Hidden layer biases
    double *b_o;   // Output layer biases

    // States (Persist across steps for recurrence)
    double *h_state;
    double *output;

    double fitness;
} RNN;

// Helper to allocate a 2D weight matrix
double** allocate_matrix(int rows, int cols);

// Helper to free a 2D weight matrix
void free_matrix(double **matrix, int rows);

// Random double between -min and max
double rand_double(double min, double max);

// Initialize network with random weights
RNN* create_network(NetworkConfig config);

void free_network(RNN *net);

// Reset recurrent state between evaluation trials
void reset_rnn_state(RNN *net);

// Forward pass execution
void forward_pass(RNN *net, double *input);

// Neuroevolution: Gaussian Mutation
void mutate(RNN *net, double mutation_rate, double mutation_strength);

// Neuroevolution: Uniform Crossover
void crossover(RNN *parentA, RNN *parentB, RNN *child);

// Clone helper to preserve elites
void copy_network(RNN *dest, RNN *src);

// Comparison function for sorting population by fitness (descending)
int compare_rnn(const void *a, const void *b);

// --- MAIN RUNTIME & EXAMPLE USAGE ---
int CreateNEV();

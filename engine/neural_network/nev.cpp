#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "nev.h"

// Helper to allocate a 2D weight matrix
double** allocate_matrix(int rows, int cols) {
    double **matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

// Helper to free a 2D weight matrix
void free_matrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

// Random double between -min and max
double rand_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Initialize network with random weights
RNN* create_network(NetworkConfig config) {
    RNN *net = (RNN*)malloc(sizeof(RNN));
    net->config = config;
    net->fitness = 0.0;

    // Allocate weight matrices
    net->W_ih = allocate_matrix(config.hidden_size, config.input_size);
    net->W_hh = allocate_matrix(config.hidden_size, config.hidden_size);
    net->W_ho = allocate_matrix(config.output_size, config.hidden_size);

    // Allocate biases and states
    net->b_h = (double*)malloc(config.hidden_size * sizeof(double));
    net->b_o = (double*)malloc(config.output_size * sizeof(double));
    net->h_state = (double*)calloc(config.hidden_size, sizeof(double)); // Initialize to 0
    net->output = (double*)calloc(config.output_size, sizeof(double));

    // Randomize weights and biases (Xavier/Glorot-like initialization initialization)
    for (int i = 0; i < config.hidden_size; i++) {
        for (int j = 0; j < config.input_size; j++) net->W_ih[i][j] = rand_double(-0.1, 0.1);
        for (int j = 0; j < config.hidden_size; j++) net->W_hh[i][j] = rand_double(-0.1, 0.1);
        net->b_h[i] = rand_double(-0.1, 0.1);
    }
    for (int i = 0; i < config.output_size; i++) {
        for (int j = 0; j < config.hidden_size; j++) net->W_ho[i][j] = rand_double(-0.1, 0.1);
        net->b_o[i] = rand_double(-0.1, 0.1);
    }

    return net;
}

void free_network(RNN *net) {
    free_matrix(net->W_ih, net->config.hidden_size);
    free_matrix(net->W_hh, net->config.hidden_size);
    free_matrix(net->W_ho, net->config.output_size);
    free(net->b_h);
    free(net->b_o);
    free(net->h_state);
    free(net->output);
    free(net);
}

// Reset recurrent state between evaluation trials
void reset_rnn_state(RNN *net) {
    memset(net->h_state, 0, net->config.hidden_size * sizeof(double));
}

// Forward pass execution
void forward_pass(RNN *net, double *input) {
    int H = net->config.hidden_size;
    int I = net->config.input_size;
    int O = net->config.output_size;

    // Temporary array to hold the next hidden state
    double *next_h = (double*)malloc(H * sizeof(double));

    // 1. Calculate Hidden Layer: h_t = ReLU( W_ih * input + W_hh * h_{t-1} + b_h )
    for (int i = 0; i < H; i++) {
        double sum = net->b_h[i];
        for (int j = 0; j < I; j++) {
            sum += net->W_ih[i][j] * input[j];
        }
        for (int j = 0; j < H; j++) {
            sum += net->W_hh[i][j] * net->h_state[j]; // Recurrent step
        }
        next_h[i] = RELU(sum);
    }

    // Update the recurrent state memory
    memcpy(net->h_state, next_h, H * sizeof(double));
    free(next_h);

    // 2. Calculate Output Layer: output = Sigmoid( W_ho * h_t + b_o )
    for (int i = 0; i < O; i++) {
        double sum = net->b_o[i];
        for (int j = 0; j < H; j++) {
            sum += net->W_ho[i][j] * net->h_state[j];
        }
        net->output[i] = SIGMOID(sum);
    }
}

// Neuroevolution: Gaussian Mutation
void mutate(RNN *net, double mutation_rate, double mutation_strength) {
    // Mutate W_ih
    for (int i = 0; i < net->config.hidden_size; i++) {
        for (int j = 0; j < net->config.input_size; j++) {
            if (rand_double(0, 1) < mutation_rate) net->W_ih[i][j] += rand_double(-mutation_strength, mutation_strength);
        }
        if (rand_double(0, 1) < mutation_rate) net->b_h[i] += rand_double(-mutation_strength, mutation_strength);
    }
    // Mutate W_hh
    for (int i = 0; i < net->config.hidden_size; i++) {
        for (int j = 0; j < net->config.hidden_size; j++) {
            if (rand_double(0, 1) < mutation_rate) net->W_hh[i][j] += rand_double(-mutation_strength, mutation_strength);
        }
    }
    // Mutate W_ho
    for (int i = 0; i < net->config.output_size; i++) {
        for (int j = 0; j < net->config.hidden_size; j++) {
            if (rand_double(0, 1) < mutation_rate) net->W_ho[i][j] += rand_double(-mutation_strength, mutation_strength);
        }
        if (rand_double(0, 1) < mutation_rate) net->b_o[i] += rand_double(-mutation_strength, mutation_strength);
    }
}

// Neuroevolution: Uniform Crossover
void crossover(RNN *parentA, RNN *parentB, RNN *child) {
    for (int i = 0; i < child->config.hidden_size; i++) {
        for (int j = 0; j < child->config.input_size; j++) {
            child->W_ih[i][j] = (rand_double(0, 1) > 0.5) ? parentA->W_ih[i][j] : parentB->W_ih[i][j];
        }
        child->b_h[i] = (rand_double(0, 1) > 0.5) ? parentA->b_h[i] : parentB->b_h[i];
    }
    for (int i = 0; i < child->config.hidden_size; i++) {
        for (int j = 0; j < child->config.hidden_size; j++) {
            child->W_hh[i][j] = (rand_double(0, 1) > 0.5) ? parentA->W_hh[i][j] : parentB->W_hh[i][j];
        }
    }
    for (int i = 0; i < child->config.output_size; i++) {
        for (int j = 0; j < child->config.hidden_size; j++) {
            child->W_ho[i][j] = (rand_double(0, 1) > 0.5) ? parentA->W_ho[i][j] : parentB->W_ho[i][j];
        }
        child->b_o[i] = (rand_double(0, 1) > 0.5) ? parentA->b_o[i] : parentB->b_o[i];
    }
}

// Clone helper to preserve elites
void copy_network(RNN *dest, RNN *src) {
    for (int i = 0; i < src->config.hidden_size; i++) {
        memcpy(dest->W_ih[i], src->W_ih[i], src->config.input_size * sizeof(double));
        memcpy(dest->W_hh[i], src->W_hh[i], src->config.hidden_size * sizeof(double));
    }
    for (int i = 0; i < src->config.output_size; i++) {
        memcpy(dest->W_ho[i], src->W_ho[i], src->config.hidden_size * sizeof(double));
    }
    memcpy(dest->b_h, src->b_h, src->config.hidden_size * sizeof(double));
    memcpy(dest->b_o, src->b_o, src->config.output_size * sizeof(double));
}

// Comparison function for sorting population by fitness (descending)
int compare_rnn(const void *a, const void *b) {
    double fA = (*(RNN**)a)->fitness;
    double fB = (*(RNN**)b)->fitness;
    return (fB > fA) - (fB < fA);
}

// --- MAIN RUNTIME & EXAMPLE USAGE ---
int CreateNEV() {
    srand((unsigned int)time(NULL));

    // Dynamic configuration setup
    NetworkConfig my_config = {
        .input_size = 710,
        .hidden_size = 1000,
        .output_size = 40
    };

    int population_size = 20;
    int generations = 5;
    double mutation_rate = 0.15;
    double mutation_strength = 0.05;

    printf("Initializing Neuroevolution framework...\n");
    printf("Layers configured to -> Input: %d, Hidden: %d, Output: %d\n\n",
            my_config.input_size, my_config.hidden_size, my_config.output_size);

    // Allocate population
    RNN **population = (RNN**)malloc(population_size * sizeof(RNN*));
    for (int i = 0; i < population_size; i++) {
        population[i] = create_network(my_config);
    }

    // Dummy data generation for evaluation example
    double *mock_input = (double*)malloc(my_config.input_size * sizeof(double));
    for(int i = 0; i < my_config.input_size; i++) mock_input[i] = rand_double(0, 1);

    // Evolutionary Loop
    for (int gen = 1; gen <= generations; gen++) {

        // 1. Evaluate Fitness
        for (int i = 0; i < population_size; i++) {
            reset_rnn_state(population[i]);

            // Simulating sequential timesteps to leverage the recurrent memory
            for(int timestep = 0; timestep < 3; timestep++) {
                forward_pass(population[i], mock_input);
            }

            // Mock Fitness Function (In reality, replace this with game score, loss, etc.)
            // Here, we just reward a high value on the first output node
            population[i]->fitness = population[i]->output[0];
        }

        // 2. Sort by fitness (Elitism)
        qsort(population, population_size, sizeof(RNN*), compare_rnn);
        printf("Generation %d -> Best Fitness: %f\n", gen, population[0]->fitness);

        // 3. Breed next generation (Overwriting worst half of population)
        // Elites (top 20%) are preserved completely untouched.
        int elite_count = population_size * 0.2;

        for (int i = elite_count; i < population_size; i++) {
            // Select parents from top performers via tournament or random top selection
            int parentA_idx = rand() % elite_count;
            int parentB_idx = rand() % elite_count;

            crossover(population[parentA_idx], population[parentB_idx], population[i]);
            mutate(population[i], mutation_rate, mutation_strength);
        }
    }

    return 0;
}

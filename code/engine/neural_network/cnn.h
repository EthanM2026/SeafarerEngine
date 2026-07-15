#ifndef CNN_H_INCLUDED
#define CNN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- Data Structures ---

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

typedef struct {
    int num_filters;
    int kernel_size;
    int input_rows;
    int input_cols;
    double ***kernels; // [filter][row][col]
    double *biases;
    Matrix *outputs;   // Feature maps
} ConvLayer;

typedef struct {
    int input_size;
    int output_size;
    double **weights;
    double *biases;
    double *outputs;
} DenseLayer;

// --- Helper Functions ---

Matrix* create_matrix(int r, int c);
void free_matrix(Matrix *m);
double random_normal();
ConvLayer* init_conv_layer(int num_filters, int in_r, int in_cols);
DenseLayer* init_dense_layer(int input_size, int output_size);
// --- Forward Pass ---
void conv_forward(Matrix *input, ConvLayer *layer);
void dense_forward(double *flattened_input, DenseLayer *layer);
// --- Backpropagation & Training ---
void train_step(Matrix *input, ConvLayer **conv_depth, int depth, DenseLayer *dense, int target_class, double lr);
// --- Main Execution Block ---
int CreateCNN();

void backpropagate_cnn();

#endif // CNN_H_INCLUDED

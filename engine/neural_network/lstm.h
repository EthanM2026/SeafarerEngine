#ifndef LSTM_H_INCLUDED
#define LSTM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Activation Functions & Derivatives
static inline float sigmoid(float x);
static inline float sigmoid_deriv(float s);
static inline float tanh_deriv(float t);

// --- STRUCTURE DEFINITIONS ---

typedef struct {
    int input_dim;
    int hidden_dim;

    // Weights: shape [hidden_dim * input_dim]
    float *W_i, *W_f, *W_c, *W_o;
    // Recurrent Weights: shape [hidden_dim * hidden_dim]
    float *U_i, *U_f, *U_c, *U_o;
    // Biases: shape [hidden_dim]
    float *b_i, *b_f, *b_c, *b_o;

    // Gradients Accumulators
    float *dW_i, *dW_f, *dW_c, *dW_o;
    float *dU_i, *dU_f, *dU_c, *dU_o;
    float *db_i, *db_f, *db_c, *db_o;
} LSTMNetwork;

typedef struct {
    int seq_len;
    int input_dim;
    int hidden_dim;

    // Flattened histories over the sequence [seq_len * dim]
    float *x;
    float *i_gate;
    float *f_gate;
    float *c_cand;
    float *c;
    float *o_gate;
    float *h;
} LSTMHistory;

typedef struct {
    int input_dim;
    int output_dim;
    float *W;   // [output_dim * input_dim]
    float *b;   // [output_dim]
    float *dW;  // [output_dim * input_dim]
    float *db;  // [output_dim]
} LSTMDenseLayer;

// --- INITIALIZATION AND MEMORY MANAGEMENT ---

void init_weight(float *w, int rows, int cols);

LSTMNetwork* lstm_create(int input_dim, int hidden_dim);

LSTMHistory* lstm_create_history(int seq_len, int input_dim, int hidden_dim);

LSTMDenseLayer* dense_create(int input_dim, int output_dim);

void lstm_zero_grads(LSTMNetwork *nn);

// --- ALGORITHMIC EXECUTION PIPELINES ---

void lstm_forward(LSTMNetwork *nn, LSTMHistory *hist, const float *sequence_inputs);

void lstm_backward(LSTMNetwork *nn, LSTMHistory *hist, const float *dh_sequence, float *dx_sequence);

void lstm_update(LSTMNetwork *nn, float lr) ;

// --- CLEANUP MANAGEMENT ---

void lstm_free_history(LSTMHistory *hist);

void lstm_free(LSTMNetwork *nn);

void dense_free(LSTMDenseLayer *layer);

int CreateLSTM();


#endif // LSTM_H_INCLUDED

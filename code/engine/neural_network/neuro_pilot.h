#ifndef NEURO_PILOT_H_INCLUDED
#define NEURO_PILOT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Hyperparameters
#define INPUT_DIM 6
#define DENSE_HIDDEN 16
#define ATTN_DIM DENSE_HIDDEN
#define LSTM_HIDDEN 32
#define OUTPUT_DIM 4
#define SEQ_LEN 5
#define LEARNING_RATE 0.01f

// Activation Functions & Derivatives
float sigmoid(float x);
float d_sigmoid(float out);
float activation_tanh(float x);
float d_tanh(float out);
float relu(float x);
float d_relu(float out);

void softmax(float *input, int size);

// Helper utility for weight initialization (Xavier/Glorot)
void init_weights(float *w, int rows, int cols);

// --- SUB-NETWORK STRUCTS ---

typedef struct {
    int in_dim, out_dim;
    float *W;   // [in_dim * out_dim]
    float *b;   // [out_dim]
    float *dW;  // Gradients
    float *db;
    float *X;   // Cached input for backprop [SEQ_LEN * in_dim]
    float *Z;   // Cached linear output [SEQ_LEN * out_dim]
    float *A;   // Cached activated output [SEQ_LEN * out_dim]
} NPDenseLayer;

typedef struct {
    int dim; // ATTN_DIM
    float *Wq, *Wk, *Wv; // Weights [dim * dim]
    float *dWq, *dWk, *dWv;
    float *X;   // Cache input [SEQ_LEN * dim]
    float *Q, *K, *V; // Cache projections [SEQ_LEN * dim]
    float *S;   // Cache Attention Scores [SEQ_LEN * SEQ_LEN]
    float *A;   // Cache Context Output [SEQ_LEN * dim]
} SelfAttentionLayer;

typedef struct {
    NPDenseLayer layers[3];
    SelfAttentionLayer attn[3];
    int has_attention;
} DenseBranch;

typedef struct {
    int in_dim, hidden_dim;
    // Weight matrices concatenated for gates: i, f, o, g (cell candidate)
    // W maps input to gates [in_dim * 4 * hidden_dim]
    // U maps previous hidden state to gates [hidden_dim * 4 * hidden_dim]
    float *W, *U, *b;
    float *dW, *dU, *db;

    // Sequence Cache for Backpropagation through time (BPTT)
    float *X; // [SEQ_LEN * in_dim]
    float *h; // [(SEQ_LEN + 1) * hidden_dim] -> includes h_{-1}
    float *c; // [(SEQ_LEN + 1) * hidden_dim] -> includes c_{-1}
    float *i_gate, *f_gate, *o_gate, *g_gate; // [SEQ_LEN * hidden_dim]
} LSTMLayer;

// --- MASTER NETWORK STRUCT ---
typedef struct {
    DenseBranch branches[5];
    float *concat_out; // Cache for combined parallel branch outputs [SEQ_LEN * (5 * DENSE_HIDDEN)]
    float *d_concat;   // Gradient cache for concatenated states
    LSTMLayer lstm[3];
    NPDenseLayer final_dense;
} NeuralNetwork;

// --- ALLOCATION & INITIALIZATION ---

void init_dense(NPDenseLayer *l, int in_dim, int out_dim);

void init_attention(SelfAttentionLayer *l, int dim);

void init_lstm(LSTMLayer *l, int in_dim, int hidden_dim);

NeuralNetwork* create_network();

// --- FORWARD PROPAGATION ---

void dense_forward(NPDenseLayer *l, float *input_seq, int seq_len);

void attention_forward(SelfAttentionLayer *l, float *input_seq);

void lstm_forward(LSTMLayer *l, float *input_seq);

float* forward_network(NeuralNetwork *nn, float *input_6d);

// --- BACKPROPAGATION ---

void dense_backward(NPDenseLayer *l, float *d_A, float *d_in_out, int seq_len);

void attention_backward(SelfAttentionLayer *l, float *d_A, float *d_in_out);

void lstm_backward(LSTMLayer *l, float *d_h, float *d_in_out);

void backward_network(NeuralNetwork *nn, float *d_loss_output);

// --- OPTIMIZATION (Vanilla SGD Update) ---

void update_dense(NPDenseLayer *l);

void update_attention(SelfAttentionLayer *l);

void update_lstm(LSTMLayer *l);

void update_network_weights(NeuralNetwork *nn);

// --- FILE IO: SERIALIZATION ---

void save_dense(NPDenseLayer *l, FILE *f);

void load_dense(NPDenseLayer *l, FILE *f);

void save_attention(SelfAttentionLayer *l, FILE *f);

void load_attention(SelfAttentionLayer *l, FILE *f);

void save_lstm(LSTMLayer *l, FILE *f);

void load_lstm(LSTMLayer *l, FILE *f);

void save_network(NeuralNetwork *nn, const char *filename);

void load_network(NeuralNetwork *nn, const char *filename);

// --- MAIN IMPLEMENTATION EXECUTION ---

int CreateNeuroPilot();

#endif // NEURO_PILOT_H_INCLUDED

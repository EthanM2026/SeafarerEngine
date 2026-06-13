#ifndef ATTN_H_INCLUDED
#define ATTN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- Data Structures ---

typedef struct {
    int seq_len;
    int embed_dim;
    // Projection Weights (Matrix dimensions: [embed_dim][embed_dim])
    double **W_q;
    double **W_k;
    double **W_v;
    // Gradients
    double **d_Wq;
    double **d_Wk;
    double **d_Wv;
    // Intermediaries cached for backpropagation [seq_len][embed_dim]
    double **Q;
    double **K;
    double **V;
    double **scores; // Attention Map Matrix [seq_len][seq_len]
    double **output; // Layer Output [seq_len][embed_dim]
} AttentionLayer;

typedef struct {
    int depth;
    int seq_len;
    int embed_dim;
    AttentionLayer *layers;
} AttentionNetwork;

// --- Helper Functions ---

double** allocate_matrix(int r, int c);
void free_matrix(double **m, int r);
double random_xavier(int in_dim);
AttentionLayer init_attention_layer(int seq_len, int embed_dim);
double** attention_forward(AttentionLayer *layer, double **input);
double** attention_backward(AttentionLayer *layer, double **input, double **d_out, double lr);
int CreateAttn();

#endif // ATTN_H_INCLUDED

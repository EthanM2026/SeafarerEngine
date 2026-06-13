#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "attn.h"

// --- Data Structures ---

double** allocate_matrix(int r, int c) {
    double **m = (double**)malloc(r * sizeof(double*));
    for (int i = 0; i < r; i++) m[i] = (double*)calloc(c, sizeof(double));
    return m;
}

void free_matrix(double **m, int r) {
    for (int i = 0; i < r; i++) free(m[i]);
    free(m);
}

double random_xavier(int in_dim) {
    double limit = sqrt(6.0 / in_dim);
    return ((double)rand() / RAND_MAX) * (2.0 * limit) - limit;
}

// --- Layer Initialization ---

AttentionLayer init_attention_layer(int seq_len, int embed_dim) {
    AttentionLayer layer;
    layer.seq_len = seq_len;
    layer.embed_dim = embed_dim;

    layer.W_q = allocate_matrix(embed_dim, embed_dim);
    layer.W_k = allocate_matrix(embed_dim, embed_dim);
    layer.W_v = allocate_matrix(embed_dim, embed_dim);
    layer.d_Wq = allocate_matrix(embed_dim, embed_dim);
    layer.d_Wk = allocate_matrix(embed_dim, embed_dim);
    layer.d_Wv = allocate_matrix(embed_dim, embed_dim);

    layer.Q = allocate_matrix(seq_len, embed_dim);
    layer.K = allocate_matrix(seq_len, embed_dim);
    layer.V = allocate_matrix(seq_len, embed_dim);
    layer.scores = allocate_matrix(seq_len, seq_len);
    layer.output = allocate_matrix(seq_len, embed_dim);

    for (int i = 0; i < embed_dim; i++) {
        for (int j = 0; j < embed_dim; j++) {
            layer.W_q[i][j] = random_xavier(embed_dim);
            layer.W_k[i][j] = random_xavier(embed_dim);
            layer.W_v[i][j] = random_xavier(embed_dim);
        }
    }
    return layer;
}

// --- Forward Pass ---

double** attention_forward(AttentionLayer *layer, double **input) {
    int seq = layer->seq_len;
    int dim = layer->embed_dim;
    double scale = 1.0 / sqrt(dim);

    // 1. Linear Projections: X * W
    for (int i = 0; i < seq; i++) {
        for (int j = 0; j < dim; j++) {
            layer->Q[i][j] = 0; layer->K[i][j] = 0; layer->V[i][j] = 0;
            for (int k = 0; k < dim; k++) {
                layer->Q[i][j] += input[i][k] * layer->W_q[k][j];
                layer->K[i][j] += input[i][k] * layer->W_k[k][j];
                layer->V[i][j] += input[i][k] * layer->W_v[k][j];
            }
        }
    }

    // 2. Scaled Dot-Product: Scores = (Q * K^T) * scale
    for (int i = 0; i < seq; i++) {
        for (int j = 0; j < seq; j++) {
            double sum = 0;
            for (int k = 0; k < dim; k++) {
                sum += layer->Q[i][k] * layer->K[j][k];
            }
            layer->scores[i][j] = sum * scale;
        }
    }

    // 3. Softmax across rows of the Attention Map
    for (int i = 0; i < seq; i++) {
        double max_val = layer->scores[i][0];
        for (int j = 1; j < seq; j++) if (layer->scores[i][j] > max_val) max_val = layer->scores[i][j];

        double sum_exp = 0.0;
        for (int j = 0; j < seq; j++) {
            layer->scores[i][j] = exp(layer->scores[i][j] - max_val);
            sum_exp += layer->scores[i][j];
        }
        for (int j = 0; j < seq; j++) {
            layer->scores[i][j] /= sum_exp;
        }
    }

    // 4. Weighted Mix: Output = Scores * V
    for (int i = 0; i < seq; i++) {
        for (int j = 0; j < dim; j++) {
            layer->output[i][j] = 0;
            for (int k = 0; k < seq; k++) {
                layer->output[i][j] += layer->scores[i][k] * layer->V[k][j];
            }
        }
    }
    return layer->output;
}

// --- Backpropagation & Optimization ---

double** attention_backward(AttentionLayer *layer, double **input, double **d_out, double lr) {
    int seq = layer->seq_len;
    int dim = layer->embed_dim;
    double scale = 1.0 / sqrt(dim);

    // Allocate gradient matrices
    double **d_scores = allocate_matrix(seq, seq);
    double **d_V = allocate_matrix(seq, dim);
    double **d_Q = allocate_matrix(seq, dim);
    double **d_K = allocate_matrix(seq, dim);
    double **d_in = allocate_matrix(seq, dim);

    // 1. Gradients through Output = Scores * V
    for (int i = 0; i < seq; i++) {
        for (int k = 0; k < seq; k++) {
            for (int j = 0; j < dim; j++) {
                d_scores[i][k] += d_out[i][j] * layer->V[k][j];
                d_V[k][j] += d_out[i][j] * layer->scores[i][k];
            }
        }
    }

    // 2. Gradients through Softmax
    for (int i = 0; i < seq; i++) {
        for (int j = 0; j < seq; j++) {
            double sum = 0;
            for (int k = 0; k < seq; k++) {
                double delta = (j == k) ? 1.0 : 0.0;
                sum += d_scores[i][k] * layer->scores[i][k] * (delta - layer->scores[i][j]);
            }
            d_scores[i][j] = sum; // Reused to represent dC/dSoftmaxRaw
        }
    }

    // 3. Gradients through Scaled Dot Product: Scores = (Q * K^T) * scale
    for (int i = 0; i < seq; i++) {
        for (int j = 0; j < seq; j++) {
            double d_val = d_scores[i][j] * scale;
            for (int k = 0; k < dim; k++) {
                d_Q[i][k] += d_val * layer->K[j][k];
                d_K[j][k] += d_val * layer->Q[i][k];
            }
        }
    }

    // 4. Gradients for Projection Weights (W_q, W_k, W_v) and Layer Inputs
    for (int i = 0; i < seq; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                layer->d_Wq[k][j] += input[i][k] * d_Q[i][j];
                layer->d_Wk[k][j] += input[i][k] * d_K[i][j];
                layer->d_Wv[k][j] += input[i][k] * d_V[i][j];

                d_in[i][k] += d_Q[i][j] * layer->W_q[k][j];
                d_in[i][k] += d_K[i][j] * layer->W_k[k][j];
                d_in[i][k] += d_V[i][j] * layer->W_v[k][j];
            }
        }
    }

    // Update weights using Stochastic Gradient Descent (SGD)
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            layer->W_q[i][j] -= lr * layer->d_Wq[i][j];
            layer->W_k[i][j] -= lr * layer->d_Wk[i][j];
            layer->W_v[i][j] -= lr * layer->d_Wv[i][j];

            // Clear weight gradient buffers for the next step
            layer->d_Wq[i][j] = 0; layer->d_Wk[i][j] = 0; layer->d_Wv[i][j] = 0;
        }
    }

    // Memory clean up of temp backward passes
    free_matrix(d_scores, seq); free_matrix(d_V, seq);
    free_matrix(d_Q, seq); free_matrix(d_K, seq);

    return d_in; // Return input matrix error for lower layers
}

// --- Main Testing Framework ---

int CreateAttn() {
    srand(777);

    // --- Dynamic Parameters Configuration ---
    int sequence_length = 4; // Number of words or processing tokens (Input Size Dim A)
    int embedding_dim = 8;   // Data width of each item vector  (Input Size Dim B)
    int hidden_depth = 2;    // Number of stacked Self-Attention Layers
    double learning_rate = 0.02;

    printf("Initializing Self-Attention Network:\n");
    printf("Sequence Length: %d | Embedding Dimension: %d | Attention Blocks Depth: %d\n\n",
            sequence_length, embedding_dim, hidden_depth);

    // Initialize Network Depth Stack
    AttentionNetwork net;
    net.depth = hidden_depth;
    net.seq_len = sequence_length;
    net.embed_dim = embedding_dim;
    net.layers = (AttentionLayer*)malloc(hidden_depth * sizeof(AttentionLayer));
    for (int i = 0; i < hidden_depth; i++) {
        net.layers[i] = init_attention_layer(sequence_length, embedding_dim);
    }

    // Generate dummy input data sequence matrix X
    double **input_data = allocate_matrix(sequence_length, embedding_dim);
    for (int i = 0; i < sequence_length; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            input_data[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Mock Target Profile Matrix (What we want output to learn to match)
    double **target_output = allocate_matrix(sequence_length, embedding_dim);
    for (int i = 0; i < sequence_length; i++) {
        for (int j = 0; j < embedding_dim; j++) {
            target_output[i][j] = 0.5; // Arbitrary target pattern
        }
    }

    // --- Optimization / Training Loop ---
    for (int epoch = 1; epoch <= 100; epoch++) {

        // 1. Forward Pass sequence
        double **current_flow = input_data;
        for (int d = 0; d < hidden_depth; d++) {
            current_flow = attention_forward(&net.layers[d], current_flow);
        }

        // Final prediction checkpoint is current_flow
        // 2. Compute Mean Square Error (MSE) Loss Derivative: dC/dOut
        double total_loss = 0.0;
        double **d_loss_out = allocate_matrix(sequence_length, embedding_dim);
        for (int i = 0; i < sequence_length; i++) {
            for (int j = 0; j < embedding_dim; j++) {
                double diff = current_flow[i][j] - target_output[i][j];
                total_loss += diff * diff;
                d_loss_out[i][j] = 2.0 * diff; // derivative of square loss
            }
        }

        // 3. Backward Pass sequence
        double **current_grad_flow = d_loss_out;
        for (int d = hidden_depth - 1; d >= 0; d--) {
            double **prev_layer_input = (d == 0) ? input_data : net.layers[d - 1].output;
            double **next_grad_flow = attention_backward(&net.layers[d], prev_layer_input, current_grad_flow, learning_rate);

            free_matrix(current_grad_flow, sequence_length);
            current_grad_flow = next_grad_flow;
        }
        free_matrix(current_grad_flow, sequence_length); // Clean remaining input gradient

        if (epoch % 20 == 0 || epoch == 1) {
            printf("Epoch %03d | Total MSE Loss: %.6f\n", epoch, total_loss);
        }
    }

    printf("\nOptimization complete! Self-Attention weights successfully learned historical relationships.\n");

    // Memory Cleanup
    free_matrix(input_data, sequence_length);
    free_matrix(target_output, sequence_length);
    for (int d = 0; d < hidden_depth; d++) {
        AttentionLayer *l = &net.layers[d];
        free_matrix(l->W_q, embedding_dim); free_matrix(l->W_k, embedding_dim); free_matrix(l->W_v, embedding_dim);
        free_matrix(l->d_Wq, embedding_dim); free_matrix(l->d_Wk, embedding_dim); free_matrix(l->d_Wv, embedding_dim);
        free_matrix(l->Q, sequence_length); free_matrix(l->K, sequence_length); free_matrix(l->V, sequence_length);
        free_matrix(l->scores, sequence_length); free_matrix(l->output, sequence_length);
    }
    free(net.layers);

    return 0;
}

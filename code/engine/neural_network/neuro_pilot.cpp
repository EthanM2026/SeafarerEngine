#include "neuro_pilot.h"

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
float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }
float d_sigmoid(float out) { return out * (1.0f - out); } // out = sigmoid(x)
float activation_tanh(float x) { return tanhf(x); }
float d_tanh(float out) { return 1.0f - out * out; } // out = tanh(x)
float relu(float x) { return x > 0.0f ? x : 0.0f; }
float d_relu(float out) { return out > 0.0f ? 1.0f : 0.0f; }

void softmax(float *input, int size) {
    float max = input[0];
    for (int i = 1; i < size; i++) if (input[i] > max) max = input[i];
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        input[i] = expf(input[i] - max);
        sum += input[i];
    }
    for (int i = 0; i < size; i++) input[i] /= sum;
}

// Helper utility for weight initialization (Xavier/Glorot)
void init_weights(float *w, int rows, int cols) {
    float limit = sqrtf(6.0f / (rows + cols));
    for (int i = 0; i < rows * cols; i++) {
        w[i] = ((float)rand() / (float)RAND_MAX) * 2.0f * limit - limit;
    }
}
// --- ALLOCATION & INITIALIZATION ---

void init_dense(NPDenseLayer *l, int in_dim, int out_dim) {
    l->in_dim = in_dim; l->out_dim = out_dim;
    l->W = (float*)malloc(in_dim * out_dim * sizeof(float));
    l->b = (float*)calloc(out_dim, sizeof(float));
    l->dW = (float*)calloc(in_dim * out_dim, sizeof(float));
    l->db = (float*)calloc(out_dim, sizeof(float));
    l->X = (float*)malloc(SEQ_LEN * in_dim * sizeof(float));
    l->Z = (float*)malloc(SEQ_LEN * out_dim * sizeof(float));
    l->A = (float*)malloc(SEQ_LEN * out_dim * sizeof(float));
    init_weights(l->W, in_dim, out_dim);
}

void init_attention(SelfAttentionLayer *l, int dim) {
    l->dim = dim;
    l->Wq = (float*)malloc(dim * dim * sizeof(float)); l->Wk = (float*)malloc(dim * dim * sizeof(float)); l->Wv = (float*)malloc(dim * dim * sizeof(float));
    l->dWq = (float*)calloc(dim * dim, sizeof(float)); l->dWk = (float*)calloc(dim * dim, sizeof(float)); l->dWv = (float*)calloc(dim * dim, sizeof(float));
    l->X = (float*)malloc(SEQ_LEN * dim * sizeof(float));
    l->Q = (float*)malloc(SEQ_LEN * dim * sizeof(float)); l->K = (float*)malloc(SEQ_LEN * dim * sizeof(float)); l->V = (float*)malloc(SEQ_LEN * dim * sizeof(float));
    l->S = (float*)malloc(SEQ_LEN * SEQ_LEN * sizeof(float));
    l->A = (float*)malloc(SEQ_LEN * dim * sizeof(float));
    init_weights(l->Wq, dim, dim); init_weights(l->Wk, dim, dim); init_weights(l->Wv, dim, dim);
}

void init_lstm(LSTMLayer *l, int in_dim, int hidden_dim) {
    l->in_dim = in_dim; l->hidden_dim = hidden_dim;
    int g_dim = 4 * hidden_dim;
    l->W = (float*)malloc(in_dim * g_dim * sizeof(float));
    l->U = (float*)malloc(hidden_dim * g_dim * sizeof(float));
    l->b = (float*)calloc(g_dim, sizeof(float));
    l->dW = (float*)calloc(in_dim * g_dim, sizeof(float));
    l->dU = (float*)calloc(hidden_dim * g_dim, sizeof(float));
    l->db = (float*)calloc(g_dim, sizeof(float));

    l->X = (float*)malloc(SEQ_LEN * in_dim * sizeof(float));
    l->h = (float*)calloc((SEQ_LEN + 1) * hidden_dim, sizeof(float));
    l->c = (float*)calloc((SEQ_LEN + 1) * hidden_dim, sizeof(float));
    l->i_gate = (float*)malloc(SEQ_LEN * hidden_dim * sizeof(float));
    l->f_gate = (float*)malloc(SEQ_LEN * hidden_dim * sizeof(float));
    l->o_gate = (float*)malloc(SEQ_LEN * hidden_dim * sizeof(float));
    l->g_gate = (float*)malloc(SEQ_LEN * hidden_dim * sizeof(float));

    init_weights(l->W, in_dim, g_dim);
    init_weights(l->U, hidden_dim, g_dim);
}

NeuralNetwork* create_network() {
    NeuralNetwork *nn = (NeuralNetwork*)malloc(sizeof(NeuralNetwork));

    // Initialize 5 branches
    for (int i = 0; i < 5; i++) {
        init_dense(&nn->branches[i].layers[0], INPUT_DIM, DENSE_HIDDEN);
        init_dense(&nn->branches[i].layers[1], DENSE_HIDDEN, DENSE_HIDDEN);
        init_dense(&nn->branches[i].layers[2], DENSE_HIDDEN, DENSE_HIDDEN);

        nn->branches[i].has_attention = (i < 2) ? 1 : 0;
        if (nn->branches[i].has_attention) {
            for (int j = 0; j < 3; j++) {
                init_attention(&nn->branches[i].attn[j], DENSE_HIDDEN);
            }
        }
    }

    nn->concat_out = (float*)malloc(SEQ_LEN * 5 * DENSE_HIDDEN * sizeof(float));
    nn->d_concat = (float*)malloc(SEQ_LEN * 5 * DENSE_HIDDEN * sizeof(float));

    // 3 stacked LSTM layers
    init_lstm(&nn->lstm[0], 5 * DENSE_HIDDEN, LSTM_HIDDEN);
    init_lstm(&nn->lstm[1], LSTM_HIDDEN, LSTM_HIDDEN);
    init_lstm(&nn->lstm[2], LSTM_HIDDEN, LSTM_HIDDEN);

    // Final Output Head (maps last frame of final LSTM layer output to 4D)
    init_dense(&nn->final_dense, LSTM_HIDDEN, OUTPUT_DIM);

    return nn;
}

// --- FORWARD PROPAGATION ---

void dense_forward(NPDenseLayer *l, float *input_seq, int seq_len) {
    memcpy(l->X, input_seq, seq_len * l->in_dim * sizeof(float));
    for (int t = 0; t < seq_len; t++) {
        for (int o = 0; o < l->out_dim; o++) {
            float val = l->b[o];
            for (int i = 0; i < l->in_dim; i++) {
                val += l->X[t * l->in_dim + i] * l->W[i * l->out_dim + o];
            }
            l->Z[t * l->out_dim + o] = val;
            l->A[t * l->out_dim + o] = relu(val);
        }
    }
}

void attention_forward(SelfAttentionLayer *l, float *input_seq) {
    memcpy(l->X, input_seq, SEQ_LEN * l->dim * sizeof(float));
    int d = l->dim;
    float scale = 1.0f / sqrtf((float)d);

    // Compute Q, K, V projections
    for(int t=0; t<SEQ_LEN; t++) {
        for(int i=0; i<d; i++) {
            float q_val = 0, k_val = 0, v_val = 0;
            for(int j=0; j<d; j++) {
                q_val += l->X[t*d + j] * l->Wq[j*d + i];
                k_val += l->X[t*d + j] * l->Wk[j*d + i];
                v_val += l->X[t*d + j] * l->Wv[j*d + i];
            }
            l->Q[t*d + i] = q_val; l->K[t*d + i] = k_val; l->V[t*d + i] = v_val;
        }
    }

    // Attention Matrix Scaled Dot-Product & Softmax
    for(int i=0; i<SEQ_LEN; i++) {
        for(int j=0; j<SEQ_LEN; j++) {
            float score = 0;
            for(int k=0; k<d; k++) score += l->Q[i*d + k] * l->K[j*d + k];
            l->S[i*SEQ_LEN + j] = score * scale;
        }
        softmax(&l->S[i*SEQ_LEN], SEQ_LEN);
    }

    // Context Weighted Sum O = Attention * V
    for(int i=0; i<SEQ_LEN; i++) {
        for(int j=0; j<d; j++) {
            float out_val = 0;
            for(int k=0; k<SEQ_LEN; k++) {
                out_val += l->S[i*SEQ_LEN + k] * l->V[k*d + j];
            }
            l->A[i*d + j] = out_val;
        }
    }
}

void lstm_forward(LSTMLayer *l, float *input_seq) {
    memcpy(l->X, input_seq, SEQ_LEN * l->in_dim * sizeof(float));
    int h_dim = l->hidden_dim;

    for (int t = 0; t < SEQ_LEN; t++) {
        float *xt = &l->X[t * l->in_dim];
        float *hp = &l->h[t * h_dim]; // previous h
        float *cp = &l->c[t * h_dim]; // previous c

        for (int g = 0; g < h_dim; g++) {
            float i_act = l->b[0*h_dim + g];
            float f_act = l->b[1*h_dim + g];
            float o_act = l->b[2*h_dim + g];
            float g_act = l->b[3*h_dim + g];

            for (int i = 0; i < l->in_dim; i++) {
                i_act += xt[i] * l->W[i * (4*h_dim) + 0*h_dim + g];
                f_act += xt[i] * l->W[i * (4*h_dim) + 1*h_dim + g];
                o_act += xt[i] * l->W[i * (4*h_dim) + 2*h_dim + g];
                g_act += xt[i] * l->W[i * (4*h_dim) + 3*h_dim + g];
            }
            for (int h = 0; h < h_dim; h++) {
                i_act += hp[h] * l->U[h * (4*h_dim) + 0*h_dim + g];
                f_act += hp[h] * l->U[h * (4*h_dim) + 1*h_dim + g];
                o_act += hp[h] * l->U[h * (4*h_dim) + 2*h_dim + g];
                g_act += hp[h] * l->U[h * (4*h_dim) + 3*h_dim + g];
            }

            l->i_gate[t*h_dim + g] = sigmoid(i_act);
            l->f_gate[t*h_dim + g] = sigmoid(f_act);
            l->o_gate[t*h_dim + g] = sigmoid(o_act);
            l->g_gate[t*h_dim + g] = activation_tanh(g_act);

            // Cell State update
            l->c[(t+1)*h_dim + g] = l->f_gate[t*h_dim + g] * cp[g] + l->i_gate[t*h_dim + g] * l->g_gate[t*h_dim + g];
            // Hidden State update
            l->h[(t+1)*h_dim + g] = l->o_gate[t*h_dim + g] * activation_tanh(l->c[(t+1)*h_dim + g]);
        }
    }
}

float* forward_network(NeuralNetwork *nn, float *input_6d) {
    // 1. Process 5 branches in parallel
    for (int b = 0; b < 5; b++) {
        dense_forward(&nn->branches[b].layers[0], input_6d, SEQ_LEN);
        dense_forward(&nn->branches[b].layers[1], nn->branches[b].layers[0].A, SEQ_LEN);
        dense_forward(&nn->branches[b].layers[2], nn->branches[b].layers[1].A, SEQ_LEN);

        float *branch_head = nn->branches[b].layers[2].A;
        if (nn->branches[b].has_attention) {
            attention_forward(&nn->branches[b].attn[0], branch_head);
            attention_forward(&nn->branches[b].attn[1], nn->branches[b].attn[0].A);
            attention_forward(&nn->branches[b].attn[2], nn->branches[b].attn[1].A);
            branch_head = nn->branches[b].attn[2].A;
        }

        // Concat feature frames into shared tensor space
        for (int t = 0; t < SEQ_LEN; t++) {
            memcpy(&nn->concat_out[t * (5 * DENSE_HIDDEN) + b * DENSE_HIDDEN],
                   &branch_head[t * DENSE_HIDDEN], DENSE_HIDDEN * sizeof(float));
        }
    }

    // 2. Feed through 3 stacked LSTMs
    lstm_forward(&nn->lstm[0], nn->concat_out);
    lstm_forward(&nn->lstm[1], &nn->lstm[0].h[nn->lstm[0].hidden_dim]); // dynamic offset tracking seq safely
    lstm_forward(&nn->lstm[2], &nn->lstm[1].h[nn->lstm[1].hidden_dim]);

    // 3. Final Dense Layer maps last frame of the final LSTM layer to 4D
    float *last_lstm_frame = &nn->lstm[2].h[SEQ_LEN * LSTM_HIDDEN];
    dense_forward(&nn->final_dense, last_lstm_frame, 1); // treating 1 frame as seq length 1

    return nn->final_dense.A;
}

// --- BACKPROPAGATION ---

void dense_backward(NPDenseLayer *l, float *d_A, float *d_in_out, int seq_len) {
    memset(l->dW, 0, l->in_dim * l->out_dim * sizeof(float));
    memset(l->db, 0, l->out_dim * sizeof(float));
    memset(d_in_out, 0, seq_len * l->in_dim * sizeof(float));

    for (int t = 0; t < seq_len; t++) {
        for (int o = 0; o < l->out_dim; o++) {
            float d_z = d_A[t * l->out_dim + o] * d_relu(l->Z[t * l->out_dim + o]);
            l->db[o] += d_z;
            for (int i = 0; i < l->in_dim; i++) {
                l->dW[i * l->out_dim + o] += l->X[t * l->in_dim + i] * d_z;
                d_in_out[t * l->in_dim + i] += l->W[i * l->out_dim + o] * d_z;
            }
        }
    }
}

void attention_backward(SelfAttentionLayer *l, float *d_A, float *d_in_out) {
    int d = l->dim;
    float scale = 1.0f / sqrtf((float)d);

    float *d_V = (float*)calloc(SEQ_LEN * d, sizeof(float));
    float *d_S = (float*)calloc(SEQ_LEN * SEQ_LEN, sizeof(float));
    float *d_Q = (float*)calloc(SEQ_LEN * d, sizeof(float));
    float *d_K = (float*)calloc(SEQ_LEN * d, sizeof(float));
    memset(d_in_out, 0, SEQ_LEN * d * sizeof(float));

    // Gradient through Context weighted sum matrix mult
    for(int i=0; i<SEQ_LEN; i++) {
        for(int j=0; j<d; j++) {
            for(int k=0; k<SEQ_LEN; k++) {
                d_S[i*SEQ_LEN + k] += d_A[i*d + j] * l->V[k*d + j];
                d_V[k*d + j] += d_A[i*d + j] * l->S[i*SEQ_LEN + k];
            }
        }
    }

    // Gradient through Softmax matrix
    for(int i=0; i<SEQ_LEN; i++) {
        for(int j=0; j<SEQ_LEN; j++) {
            float sum = 0;
            for(int k=0; k<SEQ_LEN; k++) {
                float delta = (j == k) ? 1.0f : 0.0f;
                sum += d_S[i*SEQ_LEN + k] * l->S[i*SEQ_LEN + k] * (delta - l->S[i*SEQ_LEN + j]);
            }
            // Add back projection derivations mapping scaling
            float score_grad = sum * scale;
            for(int k=0; k<d; k++) {
                d_Q[i*d + k] += score_grad * l->K[j*d + k];
                d_K[j*d + k] += score_grad * l->Q[i*d + k];
            }
        }
    }

    // Gradient collection back onto projection weights
    memset(l->dWq, 0, d*d*sizeof(float)); memset(l->dWk, 0, d*d*sizeof(float)); memset(l->dWv, 0, d*d*sizeof(float));
    for(int t=0; t<SEQ_LEN; t++) {
        for(int i=0; i<d; i++) {
            for(int j=0; j<d; j++) {
                l->dWq[j*d + i] += l->X[t*d + j] * d_Q[t*d + i];
                l->dWk[j*d + i] += l->X[t*d + j] * d_K[t*d + i];
                l->dWv[j*d + i] += l->X[t*d + j] * d_V[t*d + i];

                d_in_out[t*d + j] += l->Wq[j*d + i] * d_Q[t*d + i] +
                                     l->Wk[j*d + i] * d_K[t*d + i] +
                                     l->Wv[j*d + i] * d_V[t*d + i];
            }
        }
    }
    free(d_V); free(d_S); free(d_Q); free(d_K);
}

void lstm_backward(LSTMLayer *l, float *d_h, float *d_in_out) {
    int h_dim = l->hidden_dim;
    int g_dim = 4 * h_dim;
    memset(l->dW, 0, l->in_dim * g_dim * sizeof(float));
    memset(l->dU, 0, h_dim * g_dim * sizeof(float));
    memset(l->db, 0, g_dim * sizeof(float));
    memset(d_in_out, 0, SEQ_LEN * l->in_dim * sizeof(float));

    float *dh_next = (float*)calloc(h_dim, sizeof(float));
    float *dc_next = (float*)calloc(h_dim, sizeof(float));

    for (int t = SEQ_LEN - 1; t >= 0; t--) {
        float *xt = &l->X[t * l->in_dim];
        float *hp = &l->h[t * h_dim];
        float *cp = &l->c[t * h_dim];
        float *c_curr = &l->c[(t+1) * h_dim];

        for (int g = 0; g < h_dim; g++) {
            float dh = d_h[t * h_dim + g] + dh_next[g];
            float dc = dh * l->o_gate[t * h_dim + g] * d_tanh(activation_tanh(c_curr[g])) + dc_next[g];

            float dg = dc * l->i_gate[t * h_dim + g] * d_tanh(l->g_gate[t * h_dim + g]);
            float do_g = dh * activation_tanh(c_curr[g]) * d_sigmoid(l->o_gate[t * h_dim + g]);
            float df = dc * cp[g] * d_sigmoid(l->f_gate[t * h_dim + g]);
            float di = dc * l->g_gate[t * h_dim + g] * d_sigmoid(l->i_gate[t * h_dim + g]);

            dc_next[g] = dc * l->f_gate[t * h_dim + g];

            float grads[4] = {di, df, do_g, dg};
            for (int i_g = 0; i_g < 4; i_g++) {
                l->db[i_g * h_dim + g] += grads[i_g];
                for (int i = 0; i < l->in_dim; i++) {
                    l->dW[i * g_dim + i_g * h_dim + g] += xt[i] * grads[i_g];
                    d_in_out[t * l->in_dim + i] += l->W[i * g_dim + i_g * h_dim + g] * grads[i_g];
                }
                dh_next[g] = 0;
                for (int h = 0; h < h_dim; h++) {
                    l->dU[h * g_dim + i_g * h_dim + g] += hp[h] * grads[i_g];
                    dh_next[g] += l->U[h * g_dim + i_g * h_dim + g] * grads[i_g];
                }
            }
        }
    }
    free(dh_next); free(dc_next);
}

void backward_network(NeuralNetwork *nn, float *d_loss_output) {
    // 1. Backprop through output Head
    float *d_last_lstm = (float*)malloc(LSTM_HIDDEN * sizeof(float));
    dense_backward(&nn->final_dense, d_loss_output, d_last_lstm, 1);

    // 2. Setup full sequence loss matrix for LSTM stack execution
    float *d_lstm3_seq = (float*)calloc(SEQ_LEN * LSTM_HIDDEN, sizeof(float));
    memcpy(&d_lstm3_seq[(SEQ_LEN - 1) * LSTM_HIDDEN], d_last_lstm, LSTM_HIDDEN * sizeof(float));
    free(d_last_lstm);

    float *d_lstm2_seq = (float*)malloc(SEQ_LEN * LSTM_HIDDEN * sizeof(float));
    float *d_lstm1_seq = (float*)malloc(SEQ_LEN * LSTM_HIDDEN * sizeof(float));

    lstm_backward(&nn->lstm[2], d_lstm3_seq, d_lstm2_seq);
    lstm_backward(&nn->lstm[1], d_lstm2_seq, d_lstm1_seq);
    lstm_backward(&nn->lstm[0], d_lstm1_seq, nn->d_concat);

    free(d_lstm3_seq); free(d_lstm2_seq); free(d_lstm1_seq);

    // 3. Demux/Split gradients into parallel paths and backpropagate
    float *d_branch_out = (float*)malloc(SEQ_LEN * DENSE_HIDDEN * sizeof(float));
    float *d_branch_in_buffer = (float*)malloc(SEQ_LEN * DENSE_HIDDEN * sizeof(float));

    for (int b = 0; b < 5; b++) {
        for (int t = 0; t < SEQ_LEN; t++) {
            memcpy(&d_branch_out[t * DENSE_HIDDEN],
                   &nn->d_concat[t * (5 * DENSE_HIDDEN) + b * DENSE_HIDDEN], DENSE_HIDDEN * sizeof(float));
        }

        if (nn->branches[b].has_attention) {
            attention_backward(&nn->branches[b].attn[2], d_branch_out, d_branch_in_buffer);
            attention_backward(&nn->branches[b].attn[1], d_branch_in_buffer, d_branch_out);
            attention_backward(&nn->branches[b].attn[0], d_branch_out, d_branch_in_buffer);
            memcpy(d_branch_out, d_branch_in_buffer, SEQ_LEN * DENSE_HIDDEN * sizeof(float));
        }

        dense_backward(&nn->branches[b].layers[2], d_branch_out, d_branch_in_buffer, SEQ_LEN);
        dense_backward(&nn->branches[b].layers[1], d_branch_in_buffer, d_branch_out, SEQ_LEN);
        // Base layer distribution gradient can be ignored unless connecting to preceding graphs
        float *d_dummy_input = (float*)malloc(SEQ_LEN * INPUT_DIM * sizeof(float));
        dense_backward(&nn->branches[b].layers[0], d_branch_out, d_dummy_input, SEQ_LEN);
        free(d_dummy_input);
    }
    free(d_branch_out); free(d_branch_in_buffer);
}

// --- OPTIMIZATION (Vanilla SGD Update) ---

void update_dense(NPDenseLayer *l) {
    for (int i = 0; i < l->in_dim * l->out_dim; i++) l->W[i] -= LEARNING_RATE * l->dW[i];
    for (int i = 0; i < l->out_dim; i++) l->b[i] -= LEARNING_RATE * l->db[i];
}

void update_attention(SelfAttentionLayer *l) {
    for (int i = 0; i < l->dim * l->dim; i++) {
        l->Wq[i] -= LEARNING_RATE * l->dWq[i];
        l->Wk[i] -= LEARNING_RATE * l->dWk[i];
        l->Wv[i] -= LEARNING_RATE * l->dWv[i];
    }
}

void update_lstm(LSTMLayer *l) {
    int g_dim = 4 * l->hidden_dim;
    for (int i = 0; i < l->in_dim * g_dim; i++) l->W[i] -= LEARNING_RATE * l->dW[i];
    for (int i = 0; i < l->hidden_dim * g_dim; i++) l->U[i] -= LEARNING_RATE * l->dU[i];
    for (int i = 0; i < g_dim; i++) l->b[i] -= LEARNING_RATE * l->db[i];
}

void update_network_weights(NeuralNetwork *nn) {
    for (int b = 0; b < 5; b++) {
        for (int j = 0; j < 3; j++) update_dense(&nn->branches[b].layers[j]);
        if (nn->branches[b].has_attention) {
            for (int j = 0; j < 3; j++) update_attention(&nn->branches[b].attn[j]);
        }
    }
    for (int i = 0; i < 3; i++) update_lstm(&nn->lstm[i]);
    update_dense(&nn->final_dense);
}

// --- FILE IO: SERIALIZATION ---

void save_dense(NPDenseLayer *l, FILE *f) {
    fwrite(l->W, sizeof(float), l->in_dim * l->out_dim, f);
    fwrite(l->b, sizeof(float), l->out_dim, f);
}

void load_dense(NPDenseLayer *l, FILE *f) {
    fread(l->W, sizeof(float), l->in_dim * l->out_dim, f);
    fread(l->b, sizeof(float), l->out_dim, f);
}

void save_attention(SelfAttentionLayer *l, FILE *f) {
    fwrite(l->Wq, sizeof(float), l->dim * l->dim, f);
    fwrite(l->Wk, sizeof(float), l->dim * l->dim, f);
    fwrite(l->Wv, sizeof(float), l->dim * l->dim, f);
}

void load_attention(SelfAttentionLayer *l, FILE *f) {
    fread(l->Wq, sizeof(float), l->dim * l->dim, f);
    fread(l->K, sizeof(float), l->dim * l->dim, f); // weights destination match array space
    fread(l->Wv, sizeof(float), l->dim * l->dim, f);
}

void save_lstm(LSTMLayer *l, FILE *f) {
    int g_dim = 4 * l->hidden_dim;
    fwrite(l->W, sizeof(float), l->in_dim * g_dim, f);
    fwrite(l->U, sizeof(float), l->hidden_dim * g_dim, f);
    fwrite(l->b, sizeof(float), g_dim, f);
}

void load_lstm(LSTMLayer *l, FILE *f) {
    int g_dim = 4 * l->hidden_dim;
    fread(l->W, sizeof(float), l->in_dim * g_dim, f);
    fread(l->U, sizeof(float), l->hidden_dim * g_dim, f);
    fread(l->b, sizeof(float), g_dim, f);
}

void save_network(NeuralNetwork *nn, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    for (int b = 0; b < 5; b++) {
        for (int j = 0; j < 3; j++) save_dense(&nn->branches[b].layers[j], f);
        if (nn->branches[b].has_attention) {
            for (int j = 0; j < 3; j++) save_attention(&nn->branches[b].attn[j], f);
        }
    }
    for (int i = 0; i < 3; i++) save_lstm(&nn->lstm[i], f);
    save_dense(&nn->final_dense, f);
    fclose(f);
    printf("Network state successfully saved to '%s'.\n", filename);
}

void load_network(NeuralNetwork *nn, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return;
    for (int b = 0; b < 5; b++) {
        for (int j = 0; j < 3; j++) load_dense(&nn->branches[b].layers[j], f);
        if (nn->branches[b].has_attention) {
            for (int j = 0; j < 3; j++) load_attention(&nn->branches[b].attn[j], f);
        }
    }
    for (int i = 0; i < 3; i++) load_lstm(&nn->lstm[i], f);
    load_dense(&nn->final_dense, f);
    fclose(f);
    printf("Network state successfully loaded from '%s'.\n", filename);
}

// --- MAIN IMPLEMENTATION EXECUTION ---

int CreateNeuroPilot() {
    srand(42); // Seed for reproducible testing

    printf("Initializing Network Graph Structure...\n");
    NeuralNetwork *my_nn = create_network();

    // Create random input sequence [SEQ_LEN, 6]
    float sample_input[SEQ_LEN * INPUT_DIM];
    for (int i = 0; i < SEQ_LEN * INPUT_DIM; i++) {
        sample_input[i] = ((float)rand() / (float)RAND_MAX);
    }

    // Target sample 4D expected output
    float target_output[OUTPUT_DIM] = {0.1f, 0.8f, 0.05f, 0.05f};

    printf("\n--- Running 1 Optimization/Training Step ---\n");

    // Forward step
    float *pred = forward_network(my_nn, sample_input);
    printf("Predictions: [%f, %f, %f, %f]\n", pred[0], pred[1], pred[2], pred[3]);

    // Compute MSE loss gradient relative to outputs: dE/dOut = (pred - target)
    float d_loss[OUTPUT_DIM];
    float total_loss = 0.0f;
    for (int i = 0; i < OUTPUT_DIM; i++) {
        d_loss[i] = pred[i] - target_output[i];
        total_loss += 0.5f * (d_loss[i] * d_loss[i]);
    }
    printf("Initial Loss Value: %f\n", total_loss);

    // Backpropagation step
    backward_network(my_nn, d_loss);

    // Weight update step
    update_network_weights(my_nn);

    // Verify loss changes after update
    pred = forward_network(my_nn, sample_input);
    total_loss = 0.0f;
    for (int i = 0; i < OUTPUT_DIM; i++) {
        float error = pred[i] - target_output[i];
        total_loss += 0.5f * (error * error);
    }
    printf("Loss Post Optimization Step: %f\n", total_loss);

    // Testing Binary Serialization Save/Load functionality
    printf("\n--- Testing Serialization ---\n");
    save_network(my_nn, "neural_net.bin");

    NeuralNetwork *loaded_nn = create_network();
    load_network(loaded_nn, "neural_net.bin");

    float *loaded_pred = forward_network(loaded_nn, sample_input);
    printf("Loaded Network Predictions: [%f, %f, %f, %f]\n",
            loaded_pred[0], loaded_pred[1], loaded_pred[2], loaded_pred[3]);

    // Clean up would happen here (free internal allocated pointers)
    return 0;
}

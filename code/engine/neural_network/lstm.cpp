#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "lstm.h"
// Activation Functions & Derivatives
static inline float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }
static inline float sigmoid_deriv(float s) { return s * (1.0f - s); }
static inline float tanh_deriv(float t) { return 1.0f - t * t; }

// --- INITIALIZATION AND MEMORY MANAGEMENT ---

void init_weight(float *w, int rows, int cols) {
    float scale = sqrtf(6.0f / (rows + cols));
    for (int i = 0; i < rows * cols; i++) {
        w[i] = ((float)rand() / RAND_MAX) * 2.0f * scale - scale;
    }
}

LSTMNetwork* lstm_create(int input_dim, int hidden_dim) {
    LSTMNetwork *nn = (LSTMNetwork*)malloc(sizeof(LSTMNetwork));
    nn->input_dim = input_dim;
    nn->hidden_dim = hidden_dim;

    int w_size = hidden_dim * input_dim;
    int u_size = hidden_dim * hidden_dim;
    int b_size = hidden_dim;

    // Allocate parameters with explicit casts
    nn->W_i = (float*)malloc(w_size * sizeof(float)); nn->W_f = (float*)malloc(w_size * sizeof(float));
    nn->W_c = (float*)malloc(w_size * sizeof(float)); nn->W_o = (float*)malloc(w_size * sizeof(float));
    nn->U_i = (float*)malloc(u_size * sizeof(float)); nn->U_f = (float*)malloc(u_size * sizeof(float));
    nn->U_c = (float*)malloc(u_size * sizeof(float)); nn->U_o = (float*)malloc(u_size * sizeof(float));
    nn->b_i = (float*)calloc(b_size, sizeof(float));  nn->b_f = (float*)calloc(b_size, sizeof(float));
    nn->b_c = (float*)calloc(b_size, sizeof(float));  nn->b_o = (float*)calloc(b_size, sizeof(float));

    // Allocate gradients with explicit casts
    nn->dW_i = (float*)malloc(w_size * sizeof(float)); nn->dW_f = (float*)malloc(w_size * sizeof(float));
    nn->dW_c = (float*)malloc(w_size * sizeof(float)); nn->dW_o = (float*)malloc(w_size * sizeof(float));
    nn->dU_i = (float*)malloc(u_size * sizeof(float)); nn->dU_f = (float*)malloc(u_size * sizeof(float));
    nn->dU_c = (float*)malloc(u_size * sizeof(float)); nn->dU_o = (float*)malloc(u_size * sizeof(float));
    nn->db_i = (float*)malloc(b_size * sizeof(float)); nn->db_f = (float*)malloc(b_size * sizeof(float));
    nn->db_c = (float*)malloc(b_size * sizeof(float)); nn->db_o = (float*)malloc(b_size * sizeof(float));

    // Initialize weights
    init_weight(nn->W_i, hidden_dim, input_dim); init_weight(nn->W_f, hidden_dim, input_dim);
    init_weight(nn->W_c, hidden_dim, input_dim); init_weight(nn->W_o, hidden_dim, input_dim);
    init_weight(nn->U_i, hidden_dim, hidden_dim); init_weight(nn->U_f, hidden_dim, hidden_dim);
    init_weight(nn->U_c, hidden_dim, hidden_dim); init_weight(nn->U_o, hidden_dim, hidden_dim);

    // Forget gate bias initialization trick
    for (int i = 0; i < hidden_dim; i++) nn->b_f[i] = 1.0f;

    return nn;
}

LSTMHistory* lstm_create_history(int seq_len, int input_dim, int hidden_dim) {
    LSTMHistory *hist = (LSTMHistory*)malloc(sizeof(LSTMHistory));
    hist->seq_len = seq_len;
    hist->input_dim = input_dim;
    hist->hidden_dim = hidden_dim;

    hist->x = (float*)calloc(seq_len * input_dim, sizeof(float));
    hist->i_gate = (float*)calloc(seq_len * hidden_dim, sizeof(float));
    hist->f_gate = (float*)calloc(seq_len * hidden_dim, sizeof(float));
    hist->c_cand = (float*)calloc(seq_len * hidden_dim, sizeof(float));
    hist->c = (float*)calloc(seq_len * hidden_dim, sizeof(float));
    hist->o_gate = (float*)calloc(seq_len * hidden_dim, sizeof(float));
    hist->h = (float*)calloc(seq_len * hidden_dim, sizeof(float));
    return hist;
}

LSTMDenseLayer* dense_create(int input_dim, int output_dim) {
    LSTMDenseLayer *layer = (LSTMDenseLayer*)malloc(sizeof(LSTMDenseLayer));
    layer->input_dim = input_dim;
    layer->output_dim = output_dim;

    layer->W = (float*)malloc(output_dim * input_dim * sizeof(float));
    layer->b = (float*)calloc(output_dim, sizeof(float));
    layer->dW = (float*)calloc(output_dim * input_dim, sizeof(float));
    layer->db = (float*)calloc(output_dim, sizeof(float));

    init_weight(layer->W, output_dim, input_dim);
    return layer;
}

void lstm_zero_grads(LSTMNetwork *nn) {
    int w_size = nn->hidden_dim * nn->input_dim;
    int u_size = nn->hidden_dim * nn->hidden_dim;
    int b_size = nn->hidden_dim;

    memset(nn->dW_i, 0, w_size * sizeof(float)); memset(nn->dW_f, 0, w_size * sizeof(float));
    memset(nn->dW_c, 0, w_size * sizeof(float)); memset(nn->dW_o, 0, w_size * sizeof(float));
    memset(nn->dU_i, 0, u_size * sizeof(float)); memset(nn->dU_f, 0, u_size * sizeof(float));
    memset(nn->dU_c, 0, u_size * sizeof(float)); memset(nn->dU_o, 0, u_size * sizeof(float));
    memset(nn->db_i, 0, b_size * sizeof(float)); memset(nn->db_f, 0, b_size * sizeof(float));
    memset(nn->db_c, 0, b_size * sizeof(float)); memset(nn->db_o, 0, b_size * sizeof(float));
}

// --- ALGORITHMIC EXECUTION PIPELINES ---

void lstm_forward(LSTMNetwork *nn, LSTMHistory *hist, const float *sequence_inputs) {
    int H = nn->hidden_dim;
    int I = nn->input_dim;
    memcpy(hist->x, sequence_inputs, hist->seq_len * I * sizeof(float));

    for (int t = 0; t < hist->seq_len; t++) {
        float *x_t = &hist->x[t * I];
        float *h_prev = (t == 0) ? NULL : &hist->h[(t - 1) * H];
        float *c_prev = (t == 0) ? NULL : &hist->c[(t - 1) * H];

        float *i_t = &hist->i_gate[t * H];
        float *f_t = &hist->f_gate[t * H];
        float *cc_t = &hist->c_cand[t * H];
        float *c_t = &hist->c[t * H];
        float *o_t = &hist->o_gate[t * H];
        float *h_t = &hist->h[t * H];

        for (int j = 0; j < H; j++) {
            float net_i = nn->b_i[j]; float net_f = nn->b_f[j];
            float net_c = nn->b_c[j]; float net_o = nn->b_o[j];

            for (int k = 0; k < I; k++) {
                net_i += nn->W_i[j * I + k] * x_t[k];
                net_f += nn->W_f[j * I + k] * x_t[k];
                net_c += nn->W_c[j * I + k] * x_t[k];
                net_o += nn->W_o[j * I + k] * x_t[k];
            }

            if (h_prev) {
                for (int k = 0; k < H; k++) {
                    net_i += nn->U_i[j * H + k] * h_prev[k];
                    net_f += nn->U_f[j * H + k] * h_prev[k];
                    net_c += nn->U_c[j * H + k] * h_prev[k];
                    net_o += nn->U_o[j * H + k] * h_prev[k];
                }
            }

            i_t[j] = sigmoid(net_i);
            f_t[j] = sigmoid(net_f);
            cc_t[j] = tanhf(net_c);
            o_t[j] = sigmoid(net_o);

            c_t[j] = i_t[j] * cc_t[j];
            if (c_prev) {
                c_t[j] += f_t[j] * c_prev[j];
            }
            h_t[j] = o_t[j] * tanhf(c_t[j]);
        }
    }
}

void lstm_backward(LSTMNetwork *nn, LSTMHistory *hist, const float *dh_sequence, float *dx_sequence) {
    int H = nn->hidden_dim;
    int I = nn->input_dim;

    float *dh_next = (float*)calloc(H, sizeof(float));
    float *dc_next = (float*)calloc(H, sizeof(float));
    float *dh_prev_step = (float*)calloc(H, sizeof(float));
    float *zero_buf = (float*)calloc(H, sizeof(float));

    lstm_zero_grads(nn);
    if (dx_sequence) {
        memset(dx_sequence, 0, hist->seq_len * I * sizeof(float));
    }

    for (int t = hist->seq_len - 1; t >= 0; t--) {
        float *x_t = &hist->x[t * I];
        float *i_t = &hist->i_gate[t * H];
        float *f_t = &hist->f_gate[t * H];
        float *cc_t = &hist->c_cand[t * H];
        float *c_t = &hist->c[t * H];
        float *o_t = &hist->o_gate[t * H];

        float *c_prev = (t == 0) ? zero_buf : &hist->c[(t - 1) * H];
        float *h_prev = (t == 0) ? zero_buf : &hist->h[(t - 1) * H];

        memset(dh_prev_step, 0, H * sizeof(float));

        for (int j = 0; j < H; j++) {
            float dh_total = dh_sequence[t * H + j] + dh_next[j];
            float dc_curr = dh_total * o_t[j] * tanh_deriv(tanhf(c_t[j])) + dc_next[j];

            float dnet_o = dh_total * tanhf(c_t[j]) * sigmoid_deriv(o_t[j]);
            float dnet_c = dc_curr * i_t[j] * tanh_deriv(cc_t[j]);
            float dnet_i = dc_curr * cc_t[j] * sigmoid_deriv(i_t[j]);
            float dnet_f = dc_curr * c_prev[j] * sigmoid_deriv(f_t[j]);

            nn->db_o[j] += dnet_o; nn->db_c[j] += dnet_c;
            nn->db_i[j] += dnet_i; nn->db_f[j] += dnet_f;

            for (int k = 0; k < I; k++) {
                nn->dW_o[j * I + k] += dnet_o * x_t[k];
                nn->dW_c[j * I + k] += dnet_c * x_t[k];
                nn->dW_i[j * I + k] += dnet_i * x_t[k];
                nn->dW_f[j * I + k] += dnet_f * x_t[k];

                if (dx_sequence) {
                    dx_sequence[t * I + k] += dnet_i * nn->W_i[j * I + k] +
                                              dnet_f * nn->W_f[j * I + k] +
                                              dnet_c * nn->W_c[j * I + k] +
                                              dnet_o * nn->W_o[j * I + k];
                }
            }

            for (int k = 0; k < H; k++) {
                nn->dU_o[j * H + k] += dnet_o * h_prev[k];
                nn->dU_c[j * H + k] += dnet_c * h_prev[k];
                nn->dU_i[j * H + k] += dnet_i * h_prev[k];
                nn->dU_f[j * H + k] += dnet_f * h_prev[k];

                dh_prev_step[k] += dnet_i * nn->U_i[j * H + k] +
                                   dnet_f * nn->U_f[j * H + k] +
                                   dnet_c * nn->U_c[j * H + k] +
                                   dnet_o * nn->U_o[j * H + k];
            }
            dc_next[j] = dc_curr * f_t[j];
        }
        memcpy(dh_next, dh_prev_step, H * sizeof(float));
    }

    free(dh_next); free(dc_next); free(dh_prev_step); free(zero_buf);
}

void lstm_update(LSTMNetwork *nn, float lr) {
    int w_size = nn->hidden_dim * nn->input_dim;
    int u_size = nn->hidden_dim * nn->hidden_dim;
    int b_size = nn->hidden_dim;

    for (int i = 0; i < w_size; i++) {
        nn->W_i[i] -= lr * nn->dW_i[i]; nn->W_f[i] -= lr * nn->dW_f[i];
        nn->W_c[i] -= lr * nn->dW_c[i]; nn->W_o[i] -= lr * nn->dW_o[i];
    }
    for (int i = 0; i < u_size; i++) {
        nn->U_i[i] -= lr * nn->dU_i[i]; nn->U_f[i] -= lr * nn->dU_f[i];
        nn->U_c[i] -= lr * nn->dU_c[i]; nn->U_o[i] -= lr * nn->dU_o[i];
    }
    for (int i = 0; i < b_size; i++) {
        nn->b_i[i] -= lr * nn->db_i[i]; nn->b_f[i] -= lr * nn->db_f[i];
        nn->b_c[i] -= lr * nn->db_c[i]; nn->b_o[i] -= lr * nn->db_o[i];
    }
}

// --- CLEANUP MANAGEMENT ---

void lstm_free_history(LSTMHistory *hist) {
    free(hist->x); free(hist->i_gate); free(hist->f_gate);
    free(hist->c_cand); free(hist->c); free(hist->o_gate); free(hist->h);
    free(hist);
}

void lstm_free(LSTMNetwork *nn) {
    free(nn->W_i); free(nn->W_f); free(nn->W_c); free(nn->W_o);
    free(nn->U_i); free(nn->U_f); free(nn->U_c); free(nn->U_o);
    free(nn->b_i); free(nn->b_f); free(nn->b_c); free(nn->b_o);
    free(nn->dW_i); free(nn->dW_f); free(nn->dW_c); free(nn->dW_o);
    free(nn->dU_i); free(nn->dU_f); free(nn->dU_c); free(nn->dU_o);
    free(nn->db_i); free(nn->db_f); free(nn->db_c); free(nn->db_o);
    free(nn);
}

void dense_free(LSTMDenseLayer *layer) {
    free(layer->W); free(layer->b);
    free(layer->dW); free(layer->db);
    free(layer);
}

// --- MAIN ARCHITECTURE ASSEMBLY ---

int CreateLSTM() {
    srand((unsigned int)time(NULL));

    // Structural Hyperparameters
    int seq_len    = 5;
    int input_dim  = 384;
    int hidden1    = 256;
    int hidden2    = 128;
    int output_dim = 16;
    float lr       = 0.01f;

    // Layer Object Allocations
    LSTMNetwork *layer1 = lstm_create(input_dim, hidden1);
    LSTMNetwork *layer2 = lstm_create(hidden1, hidden2);
    LSTMDenseLayer  *dense  = dense_create(hidden2, output_dim);

    // Context Sequence Memory History Structures
    LSTMHistory *hist1 = lstm_create_history(seq_len, input_dim, hidden1);
    LSTMHistory *hist2 = lstm_create_history(seq_len, hidden1, hidden2);

    // Variable Buffers
    float *raw_inputs    = (float*)calloc(seq_len * input_dim, sizeof(float));
    float *dense_outputs = (float*)calloc(seq_len * output_dim, sizeof(float));
    float *target_outs   = (float*)calloc(seq_len * output_dim, sizeof(float));

    // Recurrent Backward Gradient Trackers
    float *d_dense_out   = (float*)calloc(seq_len * output_dim, sizeof(float));
    float *dh_layer2     = (float*)calloc(seq_len * hidden2, sizeof(float));
    float *dh_layer1     = (float*)calloc(seq_len * hidden1, sizeof(float));

    // Populate mock training sequence data
    for (int i = 0; i < seq_len * input_dim; i++)  raw_inputs[i] = ((float)rand() / RAND_MAX) * 0.5f;
    for (int i = 0; i < seq_len * output_dim; i++) target_outs[i] = ((float)rand() / RAND_MAX);

    printf("Executing Deep Stacked Network Architecture Training Loop...\n\n");

    for (int epoch = 0; epoch <= 100; epoch++) {
        // ----------------------------------------------------
        // A. FORWARD PIPELINE
        // ----------------------------------------------------
        lstm_forward(layer1, hist1, raw_inputs);
        lstm_forward(layer2, hist2, hist1->h);

        // Project Output from Hidden Layer 2 through Dense Weights
        for (int t = 0; t < seq_len; t++) {
            for (int i = 0; i < output_dim; i++) {
                float net = dense->b[i];
                for (int j = 0; j < hidden2; j++) {
                    net += dense->W[i * hidden2 + j] * hist2->h[t * hidden2 + j];
                }
                dense_outputs[t * output_dim + i] = net;
            }
        }

        // Calculate loss (MSE Evaluation)
        float total_loss = 0.0f;
        for (int i = 0; i < seq_len * output_dim; i++) {
            float diff = dense_outputs[i] - target_outs[i];
            total_loss += 0.5f * diff * diff;
            d_dense_out[i] = diff; // Backprop seeding derivative
        }

        if (epoch % 20 == 0) {
            printf("Epoch %3d | Aggregated Sequence MSE Loss: %f\n", epoch, total_loss / seq_len);
        }

        // ----------------------------------------------------
        // B. BACKPROPAGATION THROUGH TIME (BPTT) CHAIN
        // ----------------------------------------------------

        // Step 1: Initialize Dense Gradients
        memset(dense->db, 0, output_dim * sizeof(float));
        memset(dense->dW, 0, output_dim * hidden2 * sizeof(float));
        memset(dh_layer2, 0, seq_len * hidden2 * sizeof(float));

        // Step 2: Clear and push dense deviations back down to hidden states
        for (int t = 0; t < seq_len; t++) {
            for (int i = 0; i < output_dim; i++) {
                float d_out = d_dense_out[t * output_dim + i];
                dense->db[i] += d_out;
                for (int j = 0; j < hidden2; j++) {
                    dense->dW[i * hidden2 + j] += d_out * hist2->h[t * hidden2 + j];
                    dh_layer2[t * hidden2 + j] += d_out * dense->W[i * hidden2 + j];
                }
            }
        }

        // Step 3: Recurrent step extraction back through Layer 2
        lstm_backward(layer2, hist2, dh_layer2, dh_layer1);

        // Step 4: Recurrent step extraction back through Layer 1
        lstm_backward(layer1, hist1, dh_layer1, NULL);

        // ----------------------------------------------------
        // C. DESCENT WEIGHT PARAMETER OPTIMIZATION
        // ----------------------------------------------------
        lstm_update(layer1, lr);
        lstm_update(layer2, lr);

        for (int i = 0; i < output_dim * hidden2; i++) dense->W[i] -= lr * dense->dW[i];
        for (int i = 0; i < output_dim; i++)           dense->b[i] -= lr * dense->db[i];
    }

    // Free all assigned dynamic spaces safely
    free(raw_inputs); free(dense_outputs); free(target_outs);
    free(d_dense_out); free(dh_layer2); free(dh_layer1);

    lstm_free_history(hist1); lstm_free_history(hist2);
    lstm_free(layer1);        lstm_free(layer2);
    dense_free(dense);

    printf("\nTraining terminated. Free routines executed cleanly.\n");
    return 0;
}

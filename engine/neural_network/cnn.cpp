#include "cnn.h"

Matrix* create_matrix(int r, int c) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->rows = r;
    m->cols = c;
    m->data = (double**)malloc(r * sizeof(double*));
    for(int i = 0; i < r; i++) {
        m->data[i] = (double*)calloc(c, sizeof(double));
    }
    return m;
}

void free_matrix(Matrix *m) {
    for(int i = 0; i < m->rows; i++) free(m->data[i]);
    free(m->data);
    free(m);
}

double random_normal() {
    // Basic Box-Muller transform for Gaussian initialization
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2) * 0.1;
}

// --- Layer Initialization ---

ConvLayer* init_conv_layer(int num_filters, int in_r, int in_cols) {
    ConvLayer *layer = (ConvLayer*)malloc(sizeof(ConvLayer));
    layer->num_filters = num_filters;
    layer->kernel_size = 3; // Fixed 3x3 for simplicity
    layer->input_rows = in_r;
    layer->input_cols = in_cols;

    layer->kernels = (double***)malloc(num_filters * sizeof(double**));
    layer->biases = (double*)malloc(num_filters * sizeof(double));
    layer->outputs = (Matrix*)malloc(num_filters * sizeof(Matrix));

    int out_r = in_r - 2; // No padding
    int out_c = in_cols - 2;

    for(int f = 0; f < num_filters; f++) {
        layer->biases[f] = random_normal();
        layer->kernels[f] = (double**)malloc(3 * sizeof(double*));
        for(int i = 0; i < 3; i++) {
            layer->kernels[f][i] = (double*)malloc(3 * sizeof(double));
            for(int j = 0; j < 3; j++) layer->kernels[f][i][j] = random_normal();
        }
        layer->outputs[f] = *create_matrix(out_r, out_c);
    }

    printf("There are %d Kernels.\n", num_filters);
    printf("There are %d Outputs.\n", num_filters);

    return layer;
}

DenseLayer* init_dense_layer(int input_size, int output_size) {
    DenseLayer *layer = (DenseLayer*)malloc(sizeof(DenseLayer));
    layer->input_size = input_size;
    layer->output_size = output_size;
    layer->outputs = (double*)calloc(output_size, sizeof(double));
    layer->biases = (double*)calloc(output_size, sizeof(double));

    layer->weights = (double**)malloc(output_size * sizeof(double*));
    for(int i = 0; i < output_size; i++) {
        layer->weights[i] = (double*)malloc(input_size * sizeof(double));
        layer->biases[i] = random_normal();
        for(int j = 0; j < input_size; j++) {
            layer->weights[i][j] = random_normal();
        }
    }
    return layer;
}

// --- Forward Pass ---

void conv_forward(Matrix *input, ConvLayer *layer) {
    int out_r = layer->input_rows - 2;
    int out_c = layer->input_cols - 2;

    for(int f = 0; f < layer->num_filters; f++) {
        for(int i = 0; i < out_r; i++) {
            for(int j = 0; j < out_c; j++) {
                double sum = layer->biases[f];
                // 3x3 Convolution
                for(int ki = 0; ki < 3; ki++) {
                    for(int kj = 0; kj < 3; kj++) {
                        sum += input->data[i + ki][j + kj] * layer->kernels[f][ki][kj];
                    }
                }
                // ReLU Activation
                layer->outputs[f].data[i][j] = (sum > 0) ? sum : 0;
            }
        }
    }
}

void dense_forward(double *flattened_input, DenseLayer *layer) {
    for(int i = 0; i < layer->output_size; i++) {
        double sum = layer->biases[i];
        for(int j = 0; j < layer->input_size; j++) {
            sum += flattened_input[j] * layer->weights[i][j];
        }
        layer->outputs[i] = sum;
    }

    // Softmax Activation
    double max = layer->outputs[0];
    for(int i = 1; i < layer->output_size; i++) if(layer->outputs[i] > max) max = layer->outputs[i];

    double sum_exp = 0.0;
    for(int i = 0; i < layer->output_size; i++) {
        layer->outputs[i] = exp(layer->outputs[i] - max); // Stability trick
        sum_exp += layer->outputs[i];
    }
    for(int i = 0; i < layer->output_size; i++) layer->outputs[i] /= sum_exp;
}

// --- Backpropagation & Training ---

void train_step(Matrix *input, ConvLayer **conv_depth, int depth, DenseLayer *dense, int target_class, double lr) {
    // 1. Forward Pass through customizable Conv Depth
    Matrix *current_in = input;
    for(int d = 0; d < depth; d++) {
        conv_forward(current_in, conv_depth[d]);
        // For multi-layer conv, we just pass the first filter map forward for simplicity
        // Real-world multi-channel convs track depth channels, but this keeps the pure C math readable.
        current_in = &conv_depth[d]->outputs[0];
    }

    // Flatten last conv layer outputs into 1D array
    ConvLayer *last_conv = conv_depth[depth - 1];
    int flatten_size = last_conv->num_filters * last_conv->outputs[0].rows * last_conv->outputs[0].cols;
    double *flattened = (double*)malloc(flatten_size * sizeof(double));

    int idx = 0;
    for(int f = 0; f < last_conv->num_filters; f++) {
        for(int i = 0; i < last_conv->outputs[f].rows; i++) {
            for(int j = 0; j < last_conv->outputs[f].cols; j++) {
                flattened[idx++] = last_conv->outputs[f].data[i][j];
            }
        }
    }

    // Forward Dense
    dense_forward(flattened, dense);

    // 2. Backward Pass (Calculate Gradients)
    double *dC_dz = (double*)malloc(dense->output_size * sizeof(double));
    for(int i = 0; i < dense->output_size; i++) {
        dC_dz[i] = dense->outputs[i];
    }
    dC_dz[target_class] -= 1.0; // Derivative of Softmax + Cross-Entropy Loss

    // Gradient for Dense Weights & Biases + Flattened error
    double *dC_dflat = (double*)calloc(flatten_size, sizeof(double));
    for(int i = 0; i < dense->output_size; i++) {
        for(int j = 0; j < dense->input_size; j++) {
            dC_dflat[j] += dC_dz[i] * dense->weights[i][j];
            // Update Dense Weight
            dense->weights[i][j] -= lr * dC_dz[i] * flattened[j];
        }
        dense->biases[i] -= lr * dC_dz[i];
    }

    // Backprop through Conv Layers (Backwards loop)
    idx = 0;
    for(int f = 0; f < last_conv->num_filters; f++) {
        for(int i = 0; i < last_conv->outputs[f].rows; i++) {
            for(int j = 0; j < last_conv->outputs[f].cols; j++) {
                double val = last_conv->outputs[f].data[i][j];
                // ReLU derivative
                double dC_da = (val > 0) ? dC_dflat[idx] : 0;
                idx++;

                // Update Conv Weights (Kernels) based on input to this specific layer
                Matrix *prev_input = (depth == 1) ? input : &conv_depth[depth - 2]->outputs[0];
                for(int ki = 0; ki < 3; ki++) {
                    for(int kj = 0; kj < 3; kj++) {
                        last_conv->kernels[f][ki][kj] -= lr * dC_da * prev_input->data[i + ki][j + kj];
                    }
                }
                last_conv->biases[f] -= lr * dC_da;
            }
        }
    }

    // Memory Cleanup
    free(flattened);
    free(dC_dz);
    free(dC_dflat);
}

// --- Main Execution Block ---

int CreateCNN() {
    srand(42); // Seed for reproducible sanity checks

    // --- Dynamic Parameters (Adjust these to change size/depth) ---
    int input_rows = 14;      // Adjust Input Size (e.g., 14x14 mini-images)
    int input_cols = 14;
    int hidden_depth = 2;     // Adjust Hidden Conv Depth
    int num_filters = 4;      // Filters per conv layer
    int num_classes = 3;      // Classification targets (e.g., Circle, Square, Triangle)
    double learning_rate = 0.01;

    printf("Initializing CNN: Input %dx%d | Depth: %d layers | Classes: %d\n", input_rows, input_cols, hidden_depth, num_classes);

    // Dynamic Allocation of Conv Hidden Depth
    ConvLayer **conv_layers = (ConvLayer**)malloc(hidden_depth * sizeof(ConvLayer*));
    int current_r = input_rows;
    int current_c = input_cols;

    for(int i = 0; i < hidden_depth; i++) {
        conv_layers[i] = init_conv_layer(num_filters, current_r, current_c);
        current_r -= 2; // Update dimension shrink due to valid convolution
        current_c -= 2;
    }

    if (current_r <= 0 || current_c <= 0) {
        printf("Error: Hidden depth is too deep for the chosen input size. Feature maps shrank to zero!\n");
        return -1;
    }

    // Initialize Dense Output Layer
    int flatten_size = num_filters * current_r * current_c;
    DenseLayer *dense = init_dense_layer(flatten_size, num_classes);

    // Create a dummy training sample (Mock 14x14 image)
    Matrix *sample_input = create_matrix(input_rows, input_cols);
    for(int i = 0; i < input_rows; i++) {
        for(int j = 0; j < input_cols; j++) {
            sample_input->data[i][j] = (double)rand() / RAND_MAX; // Random pixel values
        }
    }
    int target_label = 1; // We want the network to learn to classify this sample as Class '1'

    printf("\n--- CNN Weight Audit ---\n");
for (int d = 0; d < hidden_depth; d++) {
    int weights_per_filter = conv_layers[d]->kernel_size * conv_layers[d]->kernel_size;
    int total_conv_weights = conv_layers[d]->num_filters * weights_per_filter;

    printf("Conv Layer %d -> %d Filters of size [%d x %d] = %d weights | Biases: [%d]\n",
           d + 1, conv_layers[d]->num_filters, conv_layers[d]->kernel_size,
           conv_layers[d]->kernel_size, total_conv_weights, conv_layers[d]->num_filters);
}
printf("Dense Output Layer -> Weights Matrix: [%d x %d] (%d parameters)\n",
       dense->output_size, dense->input_size, dense->output_size * dense->input_size);

    // --- Training Loop ---
    printf("\nStarting Training Optimization...\n");
    for(int epoch = 1; epoch <= 50; epoch++) {
        train_step(sample_input, conv_layers, hidden_depth, dense, target_label, learning_rate);

        if (epoch % 10 == 0) {
            printf("Epoch %02d -> Predicted Probability for Class %d: %.4f\n",
                   epoch, target_label, dense->outputs[target_label]);
        }
    }

    printf("\nOptimization complete. Network successfully adjusted weights!\n");

    // Clean up allocated memory
    free_matrix(sample_input);
    // (In production code, deep loop-free allocations would go here to avoid memory leaks)
    return 0;
}

#include "npc.h"

struct _Pilot_Artificial_Brain* Create_Pilot_Brain()
{
    struct _Pilot_Artificial_Brain* PAB = (struct _Pilot_Artificial_Brain*)calloc(1,sizeof(struct _Pilot_Artificial_Brain));
    return PAB;
}

void Initialize_Pilot_Brain(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    Initialize_Pilot_Brain_Visual_Lobe(Engine, Brain);
    Initialize_Pilot_Brain_Enemy_Tracking_Lobe(Engine, Brain);
    Initialize_Pilot_Brain_Ally_Tracking_Lobe(Engine, Brain);
    Initialize_Pilot_Brain_Self_Status_Lobe(Engine, Brain);
    Initialize_Pilot_Brain_Parent_Ship_Status_Lobe(Engine, Brain);

    Initialize_LSTM(Engine, Brain);
}


void Initialize_LSTM(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nLSTM LOBE On\n");
    Brain->LSTM_Lobe=(struct _LSTM_Lobe*)calloc(1,sizeof(struct _LSTM_Lobe));

    srand((unsigned int)time(NULL));
    // Structural Hyperparameters
    Brain->LSTM_Lobe->seq_len    = 5;
    Brain->LSTM_Lobe->input_dim  = 384;
    Brain->LSTM_Lobe->hidden1    = 256;
    Brain->LSTM_Lobe->hidden2    = 128;
    Brain->LSTM_Lobe->output_dim = 16;
    Brain->LSTM_Lobe->lr       = 0.0001f;

    // Layer Object Allocations
    Brain->LSTM_Lobe->layer1 = lstm_create(Brain->LSTM_Lobe->input_dim, Brain->LSTM_Lobe->hidden1);
    Brain->LSTM_Lobe->layer2 = lstm_create(Brain->LSTM_Lobe->hidden1, Brain->LSTM_Lobe->hidden2);
    Brain->LSTM_Lobe->dense  = dense_create(Brain->LSTM_Lobe->hidden2, Brain->LSTM_Lobe->output_dim);

    // Context Sequence Memory History Structures
    Brain->LSTM_Lobe->hist1 = lstm_create_history(Brain->LSTM_Lobe->seq_len, Brain->LSTM_Lobe->input_dim, Brain->LSTM_Lobe->hidden1);
    Brain->LSTM_Lobe->hist2 = lstm_create_history(Brain->LSTM_Lobe->seq_len, Brain->LSTM_Lobe->hidden1, Brain->LSTM_Lobe->hidden2);

    // Variable Buffers
    Brain->LSTM_Lobe->raw_inputs    = (float*)calloc(Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->input_dim, sizeof(float));
    Brain->LSTM_Lobe->dense_outputs = (float*)calloc(Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->output_dim, sizeof(float));
    Brain->LSTM_Lobe->target_outs   = (float*)calloc(Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->output_dim, sizeof(float));

    // Recurrent Backward Gradient Trackers
    Brain->LSTM_Lobe->d_dense_out   = (float*)calloc(Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->output_dim, sizeof(float));
    Brain->LSTM_Lobe->dh_layer2     = (float*)calloc(Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->hidden2, sizeof(float));
    Brain->LSTM_Lobe->dh_layer1     = (float*)calloc(Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->hidden1, sizeof(float));

    // Populate mock training sequence data
    for (int i = 0; i < Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->input_dim; i++)  Brain->LSTM_Lobe->raw_inputs[i] = ((float)rand() / RAND_MAX) * 0.5f;
    for (int i = 0; i < Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->output_dim; i++) Brain->LSTM_Lobe->target_outs[i] = ((float)rand() / RAND_MAX);

    printf("Executing Deep Stacked Network Architecture Training Loop...\n\n");

    for (int epoch = 0; epoch <= 1000; epoch++) {
        // ----------------------------------------------------
        // A. FORWARD PIPELINE
        // ----------------------------------------------------
        lstm_forward(Brain->LSTM_Lobe->layer1, Brain->LSTM_Lobe->hist1, Brain->LSTM_Lobe->raw_inputs);
        lstm_forward(Brain->LSTM_Lobe->layer2, Brain->LSTM_Lobe->hist2, Brain->LSTM_Lobe->hist1->h);

        // Project Output from Hidden Layer 2 through Dense Weights
        for (int t = 0; t < Brain->LSTM_Lobe->seq_len; t++) {
            for (int i = 0; i < Brain->LSTM_Lobe->output_dim; i++) {
                float net = Brain->LSTM_Lobe->dense->b[i];
                for (int j = 0; j < Brain->LSTM_Lobe->hidden2; j++) {
                    net += Brain->LSTM_Lobe->dense->W[i * Brain->LSTM_Lobe->hidden2 + j] * Brain->LSTM_Lobe->hist2->h[t * Brain->LSTM_Lobe->hidden2 + j];
                }
                Brain->LSTM_Lobe->dense_outputs[t * Brain->LSTM_Lobe->output_dim + i] = net;
            }
        }

        // Calculate loss (MSE Evaluation)
        float total_loss = 0.0f;
        for (int i = 0; i < Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->output_dim; i++) {
            float diff = Brain->LSTM_Lobe->dense_outputs[i] - Brain->LSTM_Lobe->target_outs[i];
            total_loss += 0.5f * diff * diff;
            Brain->LSTM_Lobe->d_dense_out[i] = diff; // Backprop seeding derivative
        }

        if (epoch % 20 == 0) {
            printf("Epoch %3d | Aggregated Sequence MSE Loss: %f\n", epoch, total_loss / Brain->LSTM_Lobe->seq_len);
        }

        // ----------------------------------------------------
        // B. BACKPROPAGATION THROUGH TIME (BPTT) CHAIN
        // ----------------------------------------------------

        // Step 1: Initialize Dense Gradients
        memset(Brain->LSTM_Lobe->dense->db, 0, Brain->LSTM_Lobe->output_dim * sizeof(float));
        memset(Brain->LSTM_Lobe->dense->dW, 0, Brain->LSTM_Lobe->output_dim * Brain->LSTM_Lobe->hidden2 * sizeof(float));
        memset(Brain->LSTM_Lobe->dh_layer2, 0, Brain->LSTM_Lobe->seq_len * Brain->LSTM_Lobe->hidden2 * sizeof(float));

        // Step 2: Clear and push dense deviations back down to hidden states
        for (int t = 0; t < Brain->LSTM_Lobe->seq_len; t++) {
            for (int i = 0; i < Brain->LSTM_Lobe->output_dim; i++) {
                float d_out = Brain->LSTM_Lobe->d_dense_out[t * Brain->LSTM_Lobe->output_dim + i];
                Brain->LSTM_Lobe->dense->db[i] += d_out;
                for (int j = 0; j < Brain->LSTM_Lobe->hidden2; j++) {
                    Brain->LSTM_Lobe->dense->dW[i * Brain->LSTM_Lobe->hidden2 + j] += d_out * Brain->LSTM_Lobe->hist2->h[t * Brain->LSTM_Lobe->hidden2 + j];
                    Brain->LSTM_Lobe->dh_layer2[t * Brain->LSTM_Lobe->hidden2 + j] += d_out * Brain->LSTM_Lobe->dense->W[i * Brain->LSTM_Lobe->hidden2 + j];
                }
            }
        }

        // Step 3: Recurrent step extraction back through Layer 2
        lstm_backward(Brain->LSTM_Lobe->layer2, Brain->LSTM_Lobe->hist2, Brain->LSTM_Lobe->dh_layer2, Brain->LSTM_Lobe->dh_layer1);

        // Step 4: Recurrent step extraction back through Layer 1
        lstm_backward(Brain->LSTM_Lobe->layer1, Brain->LSTM_Lobe->hist1, Brain->LSTM_Lobe->dh_layer1, NULL);

        // ----------------------------------------------------
        // C. DESCENT WEIGHT PARAMETER OPTIMIZATION
        // ----------------------------------------------------
        lstm_update(Brain->LSTM_Lobe->layer1, Brain->LSTM_Lobe->lr);
        lstm_update(Brain->LSTM_Lobe->layer2, Brain->LSTM_Lobe->lr);

        for (int i = 0; i < Brain->LSTM_Lobe->output_dim * Brain->LSTM_Lobe->hidden2; i++) Brain->LSTM_Lobe->dense->W[i] -= Brain->LSTM_Lobe->lr * Brain->LSTM_Lobe->dense->dW[i];
        for (int i = 0; i < Brain->LSTM_Lobe->output_dim; i++)           Brain->LSTM_Lobe->dense->b[i] -= Brain->LSTM_Lobe->lr * Brain->LSTM_Lobe->dense->db[i];
    }


    printf("\nTraining terminated. Free routines executed cleanly.\n");
    //return 0;
}



void train_visual_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
     printf("\nTraining Visual Lobe...\n");
    for(int epoch = 1; epoch <= 50; epoch++) {
        train_step(Brain->Vision_Lobe->CNN_Input, Brain->Vision_Lobe->CNN_Conv, Brain->Vision_Lobe->hidden_depth, Brain->Vision_Lobe->dense, Brain->Vision_Lobe->target_label, Brain->Vision_Lobe->learning_rate);
    }
}

void train_ally_tracking_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nTraining AT Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        Brain->Ally_Tracking_Lobe->outputs = forward_pass(Brain->Ally_Tracking_Lobe->Ally_Tracking_Net, Brain->Ally_Tracking_Lobe->dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(Brain->Ally_Tracking_Lobe->Ally_Tracking_Net, Brain->Ally_Tracking_Lobe->target_label, Brain->Ally_Tracking_Lobe->learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(Brain->Ally_Tracking_Lobe->outputs[Brain->Ally_Tracking_Lobe->target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, Brain->Ally_Tracking_Lobe->target_label, Brain->Ally_Tracking_Lobe->outputs[Brain->Ally_Tracking_Lobe->target_label] * 100.0);
        }
    }
}

void train_enemy_tracking_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nTraining ET Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        Brain->Enemy_Tracking_Lobe->outputs = forward_pass(Brain->Enemy_Tracking_Lobe->Enemy_Tracking_Net, Brain->Enemy_Tracking_Lobe->dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(Brain->Enemy_Tracking_Lobe->Enemy_Tracking_Net, Brain->Enemy_Tracking_Lobe->target_label, Brain->Enemy_Tracking_Lobe->learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(Brain->Enemy_Tracking_Lobe->outputs[Brain->Enemy_Tracking_Lobe->target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, Brain->Enemy_Tracking_Lobe->target_label, Brain->Enemy_Tracking_Lobe->outputs[Brain->Enemy_Tracking_Lobe->target_label] * 100.0);
        }
    }
}


void train_self_status_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nTraining self status Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        Brain->Self_Status_Lobe->outputs = forward_pass(Brain->Self_Status_Lobe->Self_Status, Brain->Self_Status_Lobe->dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(Brain->Self_Status_Lobe->Self_Status, Brain->Self_Status_Lobe->target_label, Brain->Self_Status_Lobe->learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(Brain->Self_Status_Lobe->outputs[Brain->Self_Status_Lobe->target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, Brain->Self_Status_Lobe->target_label, Brain->Self_Status_Lobe->outputs[Brain->Self_Status_Lobe->target_label] * 100.0);
        }
    }
}

void Initialize_Pilot_Brain_Self_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nSelf_Status TRACKING LOBE On\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    Brain->Self_Status_Lobe = (struct _Self_Status_Lobe*)calloc(1,sizeof(struct _Self_Status_Lobe));
    int topology[] = {15, 64, 64, 15}; //Self_Status has 20 inputs * 8 enemies= 160. 64 x 64 dense, and then 160 embeddings, I believe.
    Brain->Self_Status_Lobe->num_layers = sizeof(topology) / sizeof(topology[0]);
    Brain->Self_Status_Lobe->learning_rate = 0.2;
    Brain->Self_Status_Lobe->Self_Status = create_network(topology, Brain->Self_Status_Lobe->num_layers);
    printf("Number of EL Self_Status tracking net dense Layers: %d\n", Brain->Self_Status_Lobe->num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            Brain->Self_Status_Lobe->num_layers, topology[0], Brain->Self_Status_Lobe->num_layers - 2, topology[Brain->Self_Status_Lobe->num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    Brain->Self_Status_Lobe->dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) Brain->Self_Status_Lobe->dummy_input[i] = (double)rand() / RAND_MAX;
    Brain->Self_Status_Lobe->target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    train_self_status_lobe(Engine, Brain);
}



void train_Parent_Ship_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nTraining Parent status Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        Brain->Parent_Ship_Status_Lobe->outputs = forward_pass(Brain->Parent_Ship_Status_Lobe->Parent_Status, Brain->Parent_Ship_Status_Lobe->dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(Brain->Parent_Ship_Status_Lobe->Parent_Status, Brain->Parent_Ship_Status_Lobe->target_label, Brain->Parent_Ship_Status_Lobe->learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(Brain->Parent_Ship_Status_Lobe->outputs[Brain->Parent_Ship_Status_Lobe->target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, Brain->Parent_Ship_Status_Lobe->target_label, Brain->Parent_Ship_Status_Lobe->outputs[Brain->Parent_Ship_Status_Lobe->target_label] * 100.0);
        }
    }
}

void Initialize_Pilot_Brain_Parent_Ship_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nParent_Status TRACKING LOBE On\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    Brain->Parent_Ship_Status_Lobe = (struct _Parent_Ship_Status_Lobe*)calloc(1,sizeof(struct _Parent_Ship_Status_Lobe));
    int topology[] = {15, 64, 64, 15}; //Parent_Status has 20 inputs * 8 enemies= 160. 64 x 64 dense, and then 160 embeddings, I believe.
    Brain->Parent_Ship_Status_Lobe->num_layers = sizeof(topology) / sizeof(topology[0]);
    Brain->Parent_Ship_Status_Lobe->learning_rate = 0.2;
    Brain->Parent_Ship_Status_Lobe->Parent_Status = create_network(topology, Brain->Parent_Ship_Status_Lobe->num_layers);
    printf("Number of EL Parent_Status tracking net dense Layers: %d\n", Brain->Parent_Ship_Status_Lobe->num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            Brain->Parent_Ship_Status_Lobe->num_layers, topology[0], Brain->Parent_Ship_Status_Lobe->num_layers - 2, topology[Brain->Parent_Ship_Status_Lobe->num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    Brain->Parent_Ship_Status_Lobe->dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) Brain->Parent_Ship_Status_Lobe->dummy_input[i] = (double)rand() / RAND_MAX;
    Brain->Parent_Ship_Status_Lobe->target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    train_Parent_Ship_Status_Lobe(Engine, Brain);
}




void Initialize_Pilot_Brain_Visual_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("VISION LOBE Start\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    Brain->Vision_Lobe = (struct _Vision_Lobe*)calloc(1,sizeof(struct _Vision_Lobe));
    // --- Dynamic Parameters (Adjust these to change size/depth) ---
    Brain->Vision_Lobe->input_rows = 18;      // Adjust Input Size (e.g., 14x14 mini-images)
    Brain->Vision_Lobe->input_cols = 18;
    Brain->Vision_Lobe->hidden_depth = 2;     // Adjust Hidden Conv Depth
    Brain->Vision_Lobe->num_filters = 4;      // Filters per conv layer
    Brain->Vision_Lobe->num_classes = 128;      // Classification targets (e.g., Circle, Square, Triangle), and 128 is how many outputs dense has
    Brain->Vision_Lobe->learning_rate = 0.01;
    printf("Initializing CNN: Input %dx%d | Depth: %d layers | Classes: %d\n", Brain->Vision_Lobe->input_rows, Brain->Vision_Lobe->input_cols, Brain->Vision_Lobe->hidden_depth, Brain->Vision_Lobe->num_classes);
    // Dynamic Allocation of Conv Hidden Depth
    Brain->Vision_Lobe->CNN_Conv = (ConvLayer**)malloc(Brain->Vision_Lobe->hidden_depth * sizeof(ConvLayer*));
    Brain->Vision_Lobe->current_r = Brain->Vision_Lobe->input_rows;
    Brain->Vision_Lobe->current_c = Brain->Vision_Lobe->input_cols;
    for(int i = 0; i < Brain->Vision_Lobe->hidden_depth; i++) {
        Brain->Vision_Lobe->CNN_Conv[i] = init_conv_layer(Brain->Vision_Lobe->num_filters, Brain->Vision_Lobe->current_r, Brain->Vision_Lobe->current_c);
        Brain->Vision_Lobe->current_r -= 2; // Update dimension shrink due to valid convolution
        Brain->Vision_Lobe->current_c -= 2;
    }
    // Initialize Dense Output Layer
    Brain->Vision_Lobe->flatten_size = Brain->Vision_Lobe->num_filters * Brain->Vision_Lobe->current_r * Brain->Vision_Lobe->current_c;
    Brain->Vision_Lobe->dense = init_dense_layer(Brain->Vision_Lobe->flatten_size, Brain->Vision_Lobe->num_classes);
    // Create a dummy training sample (Mock 14x14 image)
    Brain->Vision_Lobe->CNN_Input = create_matrix(Brain->Vision_Lobe->input_rows, Brain->Vision_Lobe->input_cols);
    for(int i = 0; i < Brain->Vision_Lobe->input_rows; i++) {
        for(int j = 0; j < Brain->Vision_Lobe->input_cols; j++) {
            Brain->Vision_Lobe->CNN_Input->data[i][j] = (double)rand() / RAND_MAX; // Random pixel values
        }
    }
    Brain->Vision_Lobe->target_label = 1; // We want the network to learn to classify this sample as Class '1'
    // --- Training Loop ---
    train_visual_lobe(Engine,Brain);

    printf("Outputs:\n");
    for(int j = 0; j < Brain->Vision_Lobe->dense->output_size; j++)
    {
        printf("%lf ", Brain->Vision_Lobe->dense->outputs[j]);
    }
}




void Initialize_Pilot_Brain_Enemy_Tracking_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nENEMY TRACKING LOBE On\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    Brain->Enemy_Tracking_Lobe = (struct _Enemy_Tracking_Lobe*)calloc(1,sizeof(struct _Enemy_Tracking_Lobe));
    int topology[] = {160, 64, 64, 160}; //Enemy has 20 inputs * 8 enemies= 160. 64 x 64 dense, and then 160 embeddings, I believe.
    Brain->Enemy_Tracking_Lobe->num_layers = sizeof(topology) / sizeof(topology[0]);
    Brain->Enemy_Tracking_Lobe->learning_rate = 0.00002;
    Brain->Enemy_Tracking_Lobe->Enemy_Tracking_Net = create_network(topology, Brain->Enemy_Tracking_Lobe->num_layers);
    printf("Number of EL Enemy tracking net dense Layers: %d\n", Brain->Enemy_Tracking_Lobe->num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            Brain->Enemy_Tracking_Lobe->num_layers, topology[0], Brain->Enemy_Tracking_Lobe->num_layers - 2, topology[Brain->Enemy_Tracking_Lobe->num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    Brain->Enemy_Tracking_Lobe->dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) Brain->Enemy_Tracking_Lobe->dummy_input[i] = (double)rand() / RAND_MAX;
    Brain->Enemy_Tracking_Lobe->target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    train_enemy_tracking_lobe(Engine, Brain);









    printf("Now for attention layer!\n");
    printf("I need 160 inputs!\n");

    // --- Dynamic Parameters Configuration ---
    Brain->Enemy_Tracking_Lobe->ATN_sequence_length = 8; // Number of words or processing tokens (Input Size Dim A). This should be 8!
    Brain->Enemy_Tracking_Lobe->ATN_embedding_dim = 20;   // Data width of each item vector  (Input Size Dim B). This Should be 20!
    Brain->Enemy_Tracking_Lobe->ATN_hidden_depth = 1;    // Number of stacked Self-Attention Layers. THIS SHOULD BE 1!
    Brain->Enemy_Tracking_Lobe->learning_rate = 0.02;

    printf("Initializing Self-Attention Network:\n");
    printf("Sequence Length: %d | Embedding Dimension: %d | Attention Blocks Depth: %d\n\n",
            Brain->Enemy_Tracking_Lobe->ATN_sequence_length, Brain->Enemy_Tracking_Lobe->ATN_embedding_dim, Brain->Enemy_Tracking_Lobe->ATN_hidden_depth);

    // Initialize Network Depth Stack

    Brain->Enemy_Tracking_Lobe->Attention_Layer.depth = Brain->Enemy_Tracking_Lobe->ATN_hidden_depth;
    Brain->Enemy_Tracking_Lobe->Attention_Layer.seq_len = Brain->Enemy_Tracking_Lobe->ATN_sequence_length;
    Brain->Enemy_Tracking_Lobe->Attention_Layer.embed_dim = Brain->Enemy_Tracking_Lobe->ATN_embedding_dim;
    Brain->Enemy_Tracking_Lobe->Attention_Layer.layers = (AttentionLayer*)malloc(Brain->Enemy_Tracking_Lobe->ATN_hidden_depth * sizeof(AttentionLayer));
    for (int i = 0; i < Brain->Enemy_Tracking_Lobe->ATN_hidden_depth; i++) {
        Brain->Enemy_Tracking_Lobe->Attention_Layer.layers[i] = init_attention_layer(Brain->Enemy_Tracking_Lobe->ATN_sequence_length, Brain->Enemy_Tracking_Lobe->ATN_embedding_dim);
    }

    // Generate dummy input data sequence matrix X
    double **input_data = allocate_matrix(Brain->Enemy_Tracking_Lobe->ATN_sequence_length, Brain->Enemy_Tracking_Lobe->ATN_embedding_dim);
    for (int i = 0; i < Brain->Enemy_Tracking_Lobe->ATN_sequence_length; i++) {
        for (int j = 0; j < Brain->Enemy_Tracking_Lobe->ATN_embedding_dim; j++) {
            input_data[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Mock Target Profile Matrix (What we want output to learn to match)
    double **target_output = allocate_matrix(Brain->Enemy_Tracking_Lobe->ATN_sequence_length, Brain->Enemy_Tracking_Lobe->ATN_embedding_dim);
    for (int i = 0; i < Brain->Enemy_Tracking_Lobe->ATN_sequence_length; i++) {
        for (int j = 0; j < Brain->Enemy_Tracking_Lobe->ATN_embedding_dim; j++) {
            target_output[i][j] = 0.5; // Arbitrary target pattern
        }
    }

    // --- Optimization / Training Loop ---
    for (int epoch = 1; epoch <= 100; epoch++) {

        // 1. Forward Pass sequence
        double **current_flow = input_data;
        for (int d = 0; d < Brain->Enemy_Tracking_Lobe->ATN_hidden_depth; d++) {
            current_flow = attention_forward(&Brain->Enemy_Tracking_Lobe->Attention_Layer.layers[d], current_flow);
        }

        // Final prediction checkpoint is current_flow
        // 2. Compute Mean Square Error (MSE) Loss Derivative: dC/dOut
        double total_loss = 0.0;
        double **d_loss_out = allocate_matrix(Brain->Enemy_Tracking_Lobe->ATN_sequence_length, Brain->Enemy_Tracking_Lobe->ATN_embedding_dim);
        for (int i = 0; i < Brain->Enemy_Tracking_Lobe->ATN_sequence_length; i++) {
            for (int j = 0; j < Brain->Enemy_Tracking_Lobe->ATN_embedding_dim; j++) {
                double diff = current_flow[i][j] - target_output[i][j];
                total_loss += diff * diff;
                d_loss_out[i][j] = 2.0 * diff; // derivative of square loss
            }
        }

        // 3. Backward Pass sequence
        double **current_grad_flow = d_loss_out;
        for (int d = Brain->Enemy_Tracking_Lobe->ATN_hidden_depth - 1; d >= 0; d--) {
            double **prev_layer_input = (d == 0) ? input_data : Brain->Enemy_Tracking_Lobe->Attention_Layer.layers[d - 1].output;
            double **next_grad_flow = attention_backward(&Brain->Enemy_Tracking_Lobe->Attention_Layer.layers[d], prev_layer_input, current_grad_flow, Brain->Enemy_Tracking_Lobe->learning_rate);

            free_matrix(current_grad_flow, Brain->Enemy_Tracking_Lobe->ATN_sequence_length);
            current_grad_flow = next_grad_flow;
        }
        free_matrix(current_grad_flow, Brain->Enemy_Tracking_Lobe->ATN_sequence_length); // Clean remaining input gradient

        if (epoch % 1 == 0 || epoch == 1) {
            printf("Epoch %03d | Total MSE Loss: %.6f\n", epoch, total_loss);
        }
    }

    printf("\nOptimization complete! Self-Attention weights successfully learned historical relationships.\n");
}




void Initialize_Pilot_Brain_Ally_Tracking_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain)
{
    printf("\nAlly TRACKING LOBE On\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    Brain->Ally_Tracking_Lobe = (struct _Ally_Tracking_Lobe*)calloc(1,sizeof(struct _Ally_Tracking_Lobe));
    int topology[] = {15, 64, 64, 15}; //Ally has 20 inputs * 8 enemies= 160. 64 x 64 dense, and then 160 embeddings, I believe.
    Brain->Ally_Tracking_Lobe->num_layers = sizeof(topology) / sizeof(topology[0]);
    Brain->Ally_Tracking_Lobe->learning_rate = 0.00002;
    Brain->Ally_Tracking_Lobe->Ally_Tracking_Net = create_network(topology, Brain->Ally_Tracking_Lobe->num_layers);
    printf("Number of EL Ally tracking net dense Layers: %d\n", Brain->Ally_Tracking_Lobe->num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            Brain->Ally_Tracking_Lobe->num_layers, topology[0], Brain->Ally_Tracking_Lobe->num_layers - 2, topology[Brain->Ally_Tracking_Lobe->num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    Brain->Ally_Tracking_Lobe->dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) Brain->Ally_Tracking_Lobe->dummy_input[i] = (double)rand() / RAND_MAX;
    Brain->Ally_Tracking_Lobe->target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    train_ally_tracking_lobe(Engine, Brain);









    printf("Now for attention layer!\n");
    printf("I need 160 inputs!\n");

    // --- Dynamic Parameters Configuration ---
    Brain->Ally_Tracking_Lobe->ATN_sequence_length = 8; // Number of words or processing tokens (Input Size Dim A). This should be 8!
    Brain->Ally_Tracking_Lobe->ATN_embedding_dim = 20;   // Data width of each item vector  (Input Size Dim B). This Should be 20!
    Brain->Ally_Tracking_Lobe->ATN_hidden_depth = 1;    // Number of stacked Self-Attention Layers. THIS SHOULD BE 1!
    Brain->Ally_Tracking_Lobe->learning_rate = 0.02;

    printf("Initializing Self-Attention Network:\n");
    printf("Sequence Length: %d | Embedding Dimension: %d | Attention Blocks Depth: %d\n\n",
            Brain->Ally_Tracking_Lobe->ATN_sequence_length, Brain->Ally_Tracking_Lobe->ATN_embedding_dim, Brain->Ally_Tracking_Lobe->ATN_hidden_depth);

    // Initialize Network Depth Stack

    Brain->Ally_Tracking_Lobe->Attention_Layer.depth = Brain->Ally_Tracking_Lobe->ATN_hidden_depth;
    Brain->Ally_Tracking_Lobe->Attention_Layer.seq_len = Brain->Ally_Tracking_Lobe->ATN_sequence_length;
    Brain->Ally_Tracking_Lobe->Attention_Layer.embed_dim = Brain->Ally_Tracking_Lobe->ATN_embedding_dim;
    Brain->Ally_Tracking_Lobe->Attention_Layer.layers = (AttentionLayer*)malloc(Brain->Ally_Tracking_Lobe->ATN_hidden_depth * sizeof(AttentionLayer));
    for (int i = 0; i < Brain->Ally_Tracking_Lobe->ATN_hidden_depth; i++) {
        Brain->Ally_Tracking_Lobe->Attention_Layer.layers[i] = init_attention_layer(Brain->Ally_Tracking_Lobe->ATN_sequence_length, Brain->Ally_Tracking_Lobe->ATN_embedding_dim);
    }

    // Generate dummy input data sequence matrix X
    double **input_data = allocate_matrix(Brain->Ally_Tracking_Lobe->ATN_sequence_length, Brain->Ally_Tracking_Lobe->ATN_embedding_dim);
    for (int i = 0; i < Brain->Ally_Tracking_Lobe->ATN_sequence_length; i++) {
        for (int j = 0; j < Brain->Ally_Tracking_Lobe->ATN_embedding_dim; j++) {
            input_data[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Mock Target Profile Matrix (What we want output to learn to match)
    double **target_output = allocate_matrix(Brain->Ally_Tracking_Lobe->ATN_sequence_length, Brain->Ally_Tracking_Lobe->ATN_embedding_dim);
    for (int i = 0; i < Brain->Ally_Tracking_Lobe->ATN_sequence_length; i++) {
        for (int j = 0; j < Brain->Ally_Tracking_Lobe->ATN_embedding_dim; j++) {
            target_output[i][j] = 0.5; // Arbitrary target pattern
        }
    }

    // --- Optimization / Training Loop ---
    for (int epoch = 1; epoch <= 100; epoch++) {

        // 1. Forward Pass sequence
        double **current_flow = input_data;
        for (int d = 0; d < Brain->Ally_Tracking_Lobe->ATN_hidden_depth; d++) {
            current_flow = attention_forward(&Brain->Ally_Tracking_Lobe->Attention_Layer.layers[d], current_flow);
        }

        // Final prediction checkpoint is current_flow
        // 2. Compute Mean Square Error (MSE) Loss Derivative: dC/dOut
        double total_loss = 0.0;
        double **d_loss_out = allocate_matrix(Brain->Ally_Tracking_Lobe->ATN_sequence_length, Brain->Ally_Tracking_Lobe->ATN_embedding_dim);
        for (int i = 0; i < Brain->Ally_Tracking_Lobe->ATN_sequence_length; i++) {
            for (int j = 0; j < Brain->Ally_Tracking_Lobe->ATN_embedding_dim; j++) {
                double diff = current_flow[i][j] - target_output[i][j];
                total_loss += diff * diff;
                d_loss_out[i][j] = 2.0 * diff; // derivative of square loss
            }
        }

        // 3. Backward Pass sequence
        double **current_grad_flow = d_loss_out;
        for (int d = Brain->Ally_Tracking_Lobe->ATN_hidden_depth - 1; d >= 0; d--) {
            double **prev_layer_input = (d == 0) ? input_data : Brain->Ally_Tracking_Lobe->Attention_Layer.layers[d - 1].output;
            double **next_grad_flow = attention_backward(&Brain->Ally_Tracking_Lobe->Attention_Layer.layers[d], prev_layer_input, current_grad_flow, Brain->Ally_Tracking_Lobe->learning_rate);

            free_matrix(current_grad_flow, Brain->Ally_Tracking_Lobe->ATN_sequence_length);
            current_grad_flow = next_grad_flow;
        }
        free_matrix(current_grad_flow, Brain->Ally_Tracking_Lobe->ATN_sequence_length); // Clean remaining input gradient

        if (epoch % 1 == 0 || epoch == 1) {
            printf("Epoch %03d | Total MSE Loss: %.6f\n", epoch, total_loss);
        }
    }

    printf("\nOptimization complete! Self-Attention weights successfully learned historical relationships.\n");
}


void Test_Vision_Lobe() //It works!
{
    //9x9x4
 //↓
//Conv2D(32)
 //↓
//Conv2D(64)
// ↓
//Flatten
// ↓
//Dense(128)
    printf("VISION LOBE TEST\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    struct _Vision_Lobe* VL = (struct _Vision_Lobe*)calloc(1,sizeof(struct _Vision_Lobe));
    // --- Dynamic Parameters (Adjust these to change size/depth) ---
    int input_rows = 18;      // Adjust Input Size (e.g., 14x14 mini-images)
    int input_cols = 18;
    int hidden_depth = 2;     // Adjust Hidden Conv Depth
    int num_filters = 4;      // Filters per conv layer
    int num_classes = 128;      // Classification targets (e.g., Circle, Square, Triangle), and 128 is how many outputs dense has
    double learning_rate = 0.01;
    printf("Initializing CNN: Input %dx%d | Depth: %d layers | Classes: %d\n", input_rows, input_cols, hidden_depth, num_classes);
    // Dynamic Allocation of Conv Hidden Depth
    VL->CNN_Conv = (ConvLayer**)malloc(hidden_depth * sizeof(ConvLayer*));
    int current_r = input_rows;
    int current_c = input_cols;
    for(int i = 0; i < hidden_depth; i++) {
        VL->CNN_Conv[i] = init_conv_layer(num_filters, current_r, current_c);
        current_r -= 2; // Update dimension shrink due to valid convolution
        current_c -= 2;
    }
    if (current_r <= 0 || current_c <= 0) {
        printf("Error: Hidden depth is too deep for the chosen input size. Feature maps shrank to zero!\n");
//        return -1;
    }
    // Initialize Dense Output Layer
    int flatten_size = num_filters * current_r * current_c;
    VL->dense = init_dense_layer(flatten_size, num_classes);
    // Create a dummy training sample (Mock 14x14 image)
    VL->CNN_Input = create_matrix(input_rows, input_cols);
    for(int i = 0; i < input_rows; i++) {
        for(int j = 0; j < input_cols; j++) {
            VL->CNN_Input->data[i][j] = (double)rand() / RAND_MAX; // Random pixel values
        }
    }
    int target_label = 1; // We want the network to learn to classify this sample as Class '1'
    printf("\n--- CNN Weight Check ---\n");
for (int d = 0; d < hidden_depth; d++) {
    int weights_per_filter = VL->CNN_Conv[d]->kernel_size * VL->CNN_Conv[d]->kernel_size;
    int total_conv_weights = VL->CNN_Conv[d]->num_filters * weights_per_filter;

    printf("Conv Layer %d -> %d Filters of size [%d x %d] = %d weights | Biases: [%d]\n",
           d + 1, VL->CNN_Conv[d]->num_filters, VL->CNN_Conv[d]->kernel_size,
           VL->CNN_Conv[d]->kernel_size, total_conv_weights, VL->CNN_Conv[d]->num_filters);
}
printf("Dense Output Layer -> Weights Matrix: [%d x %d] (%d parameters)\n",
       VL->dense->output_size, VL->dense->input_size, VL->dense->output_size * VL->dense->input_size);
    // --- Training Loop ---
    printf("\nStarting Training Optimization...\n");
    for(int epoch = 1; epoch <= 50; epoch++) {
        train_step(VL->CNN_Input, VL->CNN_Conv, hidden_depth, VL->dense, target_label, learning_rate);
    }

    printf("Outputs:\n");
    for(int j = 0; j < VL->dense->output_size; j++)
    {
        printf("%lf ", VL->dense->outputs[j]);
    }

    printf("\nOptimization complete. Network successfully adjusted weights!\n");

    //Need 128 outputs/

    // Clean up allocated memory
    free_matrix(VL->CNN_Input);
    // (In production code, deep loop-free allocations would go here to avoid memory leaks)
    printf("My Output Goes straight to LSTM!\n");
}

void Test_Enemy_Tracking_Lobe()
{
    printf("ENEMY TRACKING LOBE TEST\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    struct _Enemy_Tracking_Lobe* EL = (struct _Enemy_Tracking_Lobe*)calloc(1,sizeof(struct _Enemy_Tracking_Lobe));
    int topology[] = {160, 64, 64, 160}; //Enemy has 20 inputs * 8 enemies= 160. 64 x 64 dense, and then 160 embeddings, I believe.
    int num_layers = sizeof(topology) / sizeof(topology[0]);
    double learning_rate = 0.0002;
    EL->Enemy_Tracking_Net = create_network(topology, num_layers);
    printf("Number of EL Enemy tracking net dense Layers: %d\n", num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            num_layers, topology[0], num_layers - 2, topology[num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    double *dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) dummy_input[i] = (double)rand() / RAND_MAX;
    int target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    printf("\nTraining Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        double *outputs = forward_pass(EL->Enemy_Tracking_Net, dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(EL->Enemy_Tracking_Net, target_label, learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(outputs[target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, target_label, outputs[target_label] * 100.0);
        }
    }




    printf("Now for attention layer!\n");
    printf("I need 160 inputs!\n");

    // --- Dynamic Parameters Configuration ---
    int sequence_length = 8; // Number of words or processing tokens (Input Size Dim A). This should be 8!
    int embedding_dim = 20;   // Data width of each item vector  (Input Size Dim B). This Should be 20!
    int hidden_depth = 1;    // Number of stacked Self-Attention Layers. THIS SHOULD BE 1!
//    double learning_rate = 0.02;

    printf("Initializing Self-Attention Network:\n");
    printf("Sequence Length: %d | Embedding Dimension: %d | Attention Blocks Depth: %d\n\n",
            sequence_length, embedding_dim, hidden_depth);

    // Initialize Network Depth Stack

    EL->Attention_Layer.depth = hidden_depth;
    EL->Attention_Layer.seq_len = sequence_length;
    EL->Attention_Layer.embed_dim = embedding_dim;
    EL->Attention_Layer.layers = (AttentionLayer*)malloc(hidden_depth * sizeof(AttentionLayer));
    for (int i = 0; i < hidden_depth; i++) {
        EL->Attention_Layer.layers[i] = init_attention_layer(sequence_length, embedding_dim);
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
            current_flow = attention_forward(&EL->Attention_Layer.layers[d], current_flow);
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
            double **prev_layer_input = (d == 0) ? input_data : EL->Attention_Layer.layers[d - 1].output;
            double **next_grad_flow = attention_backward(&EL->Attention_Layer.layers[d], prev_layer_input, current_grad_flow, learning_rate);

            free_matrix(current_grad_flow, sequence_length);
            current_grad_flow = next_grad_flow;
        }
        free_matrix(current_grad_flow, sequence_length); // Clean remaining input gradient

        if (epoch % 1 == 0 || epoch == 1) {
            printf("Epoch %03d | Total MSE Loss: %.6f\n", epoch, total_loss);
        }
    }

    printf("\nOptimization complete! Self-Attention weights successfully learned historical relationships.\n");

    // Memory Cleanup
    free_matrix(input_data, sequence_length);
    free_matrix(target_output, sequence_length);
    for (int d = 0; d < hidden_depth; d++) {
        AttentionLayer *l = &EL->Attention_Layer.layers[d];
        free_matrix(l->W_q, embedding_dim); free_matrix(l->W_k, embedding_dim); free_matrix(l->W_v, embedding_dim);
        free_matrix(l->d_Wq, embedding_dim); free_matrix(l->d_Wk, embedding_dim); free_matrix(l->d_Wv, embedding_dim);
        free_matrix(l->Q, sequence_length); free_matrix(l->K, sequence_length); free_matrix(l->V, sequence_length);
        free_matrix(l->scores, sequence_length); free_matrix(l->output, sequence_length);
    }
    free(EL->Attention_Layer.layers);

    printf("My Output Goes straight to LSTM!\n");
}








void Test_Ally_Tracking_Lobe()
{
    printf("ALLY TRACKING LOBE TEST\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    struct _Ally_Tracking_Lobe* EL = (struct _Ally_Tracking_Lobe*)calloc(1,sizeof(struct _Ally_Tracking_Lobe));
    int topology[] = {15, 32, 32, 15}; //ally has 5 inputs * 3 enemies= 15. 32 x 32 dense, and then 15 embeddings, I believe.
    int num_layers = sizeof(topology) / sizeof(topology[0]);
    double learning_rate = 0.0002;
    EL->Ally_Tracking_Net = create_network(topology, num_layers);
    printf("Number of EL Ally tracking net dense Layers: %d\n", num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            num_layers, topology[0], num_layers - 2, topology[num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    double *dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) dummy_input[i] = (double)rand() / RAND_MAX;
    int target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    printf("\nTraining Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        double *outputs = forward_pass(EL->Ally_Tracking_Net, dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(EL->Ally_Tracking_Net, target_label, learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(outputs[target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, target_label, outputs[target_label] * 100.0);
        }
    }




    printf("Now for attention layer!\n");
    printf("I need 15 inputs!\n");

    // --- Dynamic Parameters Configuration ---
    int sequence_length = 5; // Number of words or processing tokens (Input Size Dim A). This should be 5!
    int embedding_dim = 3;   // Data width of each item vector  (Input Size Dim B). This Should be 3!
    int hidden_depth = 1;    // Number of stacked Self-Attention Layers. THIS SHOULD BE 1!
//    double learning_rate = 0.02;

    printf("Initializing Self-Attention Network:\n");
    printf("Sequence Length: %d | Embedding Dimension: %d | Attention Blocks Depth: %d\n\n",
            sequence_length, embedding_dim, hidden_depth);

    // Initialize Network Depth Stack

    EL->Attention_Layer.depth = hidden_depth;
    EL->Attention_Layer.seq_len = sequence_length;
    EL->Attention_Layer.embed_dim = embedding_dim;
    EL->Attention_Layer.layers = (AttentionLayer*)malloc(hidden_depth * sizeof(AttentionLayer));
    for (int i = 0; i < hidden_depth; i++) {
        EL->Attention_Layer.layers[i] = init_attention_layer(sequence_length, embedding_dim);
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
            current_flow = attention_forward(&EL->Attention_Layer.layers[d], current_flow);
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
            double **prev_layer_input = (d == 0) ? input_data : EL->Attention_Layer.layers[d - 1].output;
            double **next_grad_flow = attention_backward(&EL->Attention_Layer.layers[d], prev_layer_input, current_grad_flow, learning_rate);

            free_matrix(current_grad_flow, sequence_length);
            current_grad_flow = next_grad_flow;
        }
        free_matrix(current_grad_flow, sequence_length); // Clean remaining input gradient

        if (epoch % 1 == 0 || epoch == 1) {
            printf("Epoch %03d | Total MSE Loss: %.6f\n", epoch, total_loss);
        }
    }

    printf("\nOptimization complete! Self-Attention weights successfully learned historical relationships.\n");

    // Memory Cleanup
    free_matrix(input_data, sequence_length);
    free_matrix(target_output, sequence_length);
    for (int d = 0; d < hidden_depth; d++) {
        AttentionLayer *l = &EL->Attention_Layer.layers[d];
        free_matrix(l->W_q, embedding_dim); free_matrix(l->W_k, embedding_dim); free_matrix(l->W_v, embedding_dim);
        free_matrix(l->d_Wq, embedding_dim); free_matrix(l->d_Wk, embedding_dim); free_matrix(l->d_Wv, embedding_dim);
        free_matrix(l->Q, sequence_length); free_matrix(l->K, sequence_length); free_matrix(l->V, sequence_length);
        free_matrix(l->scores, sequence_length); free_matrix(l->output, sequence_length);
    }
    free(EL->Attention_Layer.layers);

    printf("My Output Goes straight to LSTM!\n");
}



void Test_Self_Status_Lobe()
{
    printf("Self status LOBE TEST\n");
    srand(time(NULL)); // Seed for reproducible sanity checks
    struct _Self_Status_Lobe* EL = (struct _Self_Status_Lobe*)calloc(1,sizeof(struct _Self_Status_Lobe));
    int topology[] = {4, 32, 32, 32}; //I have 4 inputs, then 2 32 neuron dense layers, and then 32 outputs
    int num_layers = sizeof(topology) / sizeof(topology[0]);
    double learning_rate = 0.2;
    EL->Self_Status = create_network(topology, num_layers);
    printf("Number of EL Ally tracking net dense Layers: %d\n", num_layers);
    printf("Dynamic DNN Initialized successfully!\n");
    printf("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
            num_layers, topology[0], num_layers - 2, topology[num_layers - 1]);
    // Create a dummy training input (Vector size 8)
    double *dummy_input = (double*)malloc(topology[0] * sizeof(double));
    for (int i = 0; i < topology[0]; i++) dummy_input[i] = (double)rand() / RAND_MAX;
    int target_label = 2; // We want to train the network to classify this pattern as Class '2'
    printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
    printf("\nTraining Network...\n");
    for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
        double *outputs = forward_pass(EL->Self_Status, dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
        backpropagate(EL->Self_Status, target_label, learning_rate);

        if (epoch % 20 == 0 || epoch == 1) {
            double loss = -log(outputs[target_label] + 1e-15); // Categorical Cross Entropy Loss
            printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
                   epoch, loss, target_label, outputs[target_label] * 100.0);
        }
    }
    printf("My Output Goes straight to LSTM!\n");
}











void Test_Parent_Ship_Status_Lobe()
{
 //   printf("Parent status LOBE TEST\n");
  //  srand(time(NULL)); // Seed for reproducible sanity checks
  //  struct _Parent_Ship_Status_Lobe* EL = (struct _Parent_Ship_Status_Lobe*)calloc(1,sizeof(struct _Parent_Ship_Status_Lobe));
  //  int topology[] = {5, 32, 32, 32}; //I have 4 inputs, then 2 32 neuron dense layers, and then 32 outputs
  // int num_layers = sizeof(topology) / sizeof(topology[0]);
  //  double learning_rate = 0.2;
  //  EL->Parent_Ship_Status = create_network(topology, num_layers);
  //  printf("Number of EL Ally tracking net dense Layers: %d\n", num_layers);
  //  printf("Dynamic DNN Initialized successfully!\n");
  //  printf("My Output Goes straight to LSTM!\n");("Layers: %d (Input: %d -> Hidden Depth: %d -> Output: %d)\n",
  //          num_layers, topology[0], num_layers - 2, topology[num_layers - 1]);
    // Create a dummy training input (Vector size 8)
 //   double *dummy_input = (double*)malloc(topology[0] * sizeof(double));
  //  for (int i = 0; i < topology[0]; i++) dummy_input[i] = (double)rand() / RAND_MAX;
 //   int target_label = 2; // We want to train the network to classify this pattern as Class '2'
 //   printf("I made a dummy input which is %d size.\n", topology[0]);

    // --- Training Loop ---
  //  printf("\nTraining Network...\n");
  //  for (int epoch = 1; epoch <= 100; epoch++) {

        // Step 1: Forward Pass
  //      double *outputs = forward_pass(EL->Parent_Ship_Status, dummy_input);

        // Step 2: Backward Pass (Optimizing weights based on categorical target)
  //      backpropagate(EL->Parent_Ship_Status, target_label, learning_rate);

  //      if (epoch % 20 == 0 || epoch == 1) {
   //         double loss = -log(outputs[target_label] + 1e-15); // Categorical Cross Entropy Loss
   //         printf("Epoch %03d | Loss: %.6f | Predicted Confidence for Target Class [%d]: %.4f%%\n",
   //                epoch, loss, target_label, outputs[target_label] * 100.0);
   //     }
   // }
   // printf("My Output Goes straight to LSTM!\n");
}




// Verification Program
void Test_LSTM() {

}


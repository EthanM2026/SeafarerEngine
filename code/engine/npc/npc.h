#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include "../neural_network/cnn.h"
#include "../neural_network/dnn.h"
#include "../neural_network/lstm.h"
#include "../neural_network/attn.h"

//Todo. Can complex comms be encoded as a wave?
//That would take 11,000 captures per second x 1 second,
//11,000 captures.


struct _Example_Vision_Input
{
    double Front_Short_Range_Vision[9*9]; //0 to 100 meters, measured by the meter.
    double Back_Short_Range_Vision[9*9]; //0 to 100 meters, measured by the meter.

    double Front_Long_Range_Vision[9*9]; //0 to 100 kilometers, measured by the kilometer.
    double Back_Long_Range_Vision[9*9]; //0 to 100 kilometers, measured by the kilometer.
};

struct _Example_Enemy_Track_Input
{
    double Distance_From_Me;
    double deltaDistance_From_Me;

    double Torpedo_Distance_From_Me;
    double Depth_Charge_Distance_From_Me;

    double Missile_Distance_From_Me;
    double Mine_Distance_From_Me;
    double Flare_Distance_From_Me;

    double ECM_Distance_From_Me;
    double ACM_Distance_From_Me;

    double Torpedo_deltaDistance_From_Me;
    double Depth_Charge_deltaDistance_From_Me;

    double Missile_deltaDistance_From_Me;
    double Mine_deltaDistance_From_Me;
    double Flare_deltaDistance_From_Me;

    double ECM_deltaDistance_From_Me;
    double ACM_deltaDistance_From_Me;

    double IFF_Frequency;
    double Comm_In_Frequency;

    double Shield_Strength;
    double Shield_Frequency;

    double deltaIFF_Frequency;
    double deltaComm_In_Frequency;

    double deltaShield_Strength;
    double deltaShield_Frequency;
};

struct _Example_Ally_Track_Input
{
    double Allied_Fighter_Distance_From_Me;
    double Allied_Fighter_deltaDistance_From_Me;
    double Allied_Fighter_Fuel_Remaining;
    double Allied_Fighter_Battery_Remaining;
    double Allied_Fighter_Damage;

    double Allied_Torpedo_Distance_From_Me;
    double Allied_Depth_Charge_Distance_From_Me;

    double Allied_Missile_Distance_From_Me;
    double Allied_Mine_Distance_From_Me;
    double Allied_Flare_Distance_From_Me;

    double Allied_ECM_Distance_From_Me;
    double Allied_ACM_Distance_From_Me;

    double Allied_Torpedo_deltaDistance_From_Me;
    double Allied_Depth_Charge_deltaDistance_From_Me;

    double Allied_Missile_deltaDistance_From_Me;
    double Allied_Mine_deltaDistance_From_Me;
    double Allied_Flare_deltaDistance_From_Me;

    double Allied_ECM_deltaDistance_From_Me;
    double Allied_ACM_deltaDistance_From_Me;

    double Allied_IFF_Frequency;
    double Allied_Comm_In_Frequency;

    double Allied_Shield_Strength;
    double Allied_Shield_Frequency;

    double Allied_deltaIFF_Frequency;
    double Allied_deltaComm_In_Frequency;

    double Allied_deltaShield_Strength;
    double Allied_deltaShield_Frequency;
};

struct _Example_Self_Status_Input
{
    double My_Comm_In_Frequency;
    double My_Fuel_Remaining;
    double My_Battery_Remaining;
    double My_Ship_Damage;

    double My_Torpedo_Distance_From_Me;
    double My_Depth_Charge_Distance_From_Me;

    double My_Missile_Distance_From_Me;
    double My_Mine_Distance_From_Me;
    double My_Flare_Distance_From_Me;

    double My_ECM_Distance_From_Me;
    double My_ACM_Distance_From_Me;

    double My_Torpedo_deltaDistance_From_Me;
    double My_Depth_Charge_deltaDistance_From_Me;

    double My_Missile_deltaDistance_From_Me;
    double My_Mine_deltaDistance_From_Me;
    double My_Flare_deltaDistance_From_Me;

    double My_ECM_deltaDistance_From_Me;
    double My_ACM_deltaDistance_From_Me;

    double My_IFF_Frequency;
//    double My_Comm_In_Frequency;

    double My_Shield_Strength;
    double My_Shield_Frequency;

    double My_deltaIFF_Frequency;
    double My_deltaComm_In_Frequency;

    double My_deltaShield_Strength;
    double My_deltaShield_Frequency;

};


struct _Example_Parent_Status_Input
{
    double Parent_Ship_Distance_From_Me;
    double Parent_Ship_deltaDistance_From_Me;
    double Parent_Ship_Fuel_Remaining;
    double Parent_Ship_Battery_Remaining;
    double Parent_Ship_Damage;


    double Parent_Torpedo_Distance_From_Me;
    double Parent_Depth_Charge_Distance_From_Me;

    double Parent_Missile_Distance_From_Me;
    double Parent_Mine_Distance_From_Me;
    double Parent_Flare_Distance_From_Me;

    double Parent_ECM_Distance_From_Me;
    double Parent_ACM_Distance_From_Me;

    double Parent_Torpedo_deltaDistance_From_Me;
    double Parent_Depth_Charge_deltaDistance_From_Me;

    double Parent_Missile_deltaDistance_From_Me;
    double Parent_Mine_deltaDistance_From_Me;
    double Parent_Flare_deltaDistance_From_Me;

    double Parent_ECM_deltaDistance_From_Me;
    double Parent_ACM_deltaDistance_From_Me;

    double Parent_IFF_Frequency;
    double Parent_Comm_In_Frequency;

    double Parent_Shield_Strength;
    double Parent_Shield_Frequency;

    double Parent_deltaIFF_Frequency;
    double Parent_deltaComm_In_Frequency;

    double Parent_deltaShield_Strength;
    double Parent_deltaShield_Frequency;
};


struct _Example_Pilot_Output
{
    double dRoll;
    double dYaw;
    double dPitch;

    double Velocity;

    double Fire_Torpedo;
    double Fire_Phonon;
    double Fire_Depth_Charge;

    double Fire_Missile;
    double Fire_Mine;
    double Fire_Flare;

    double Fire_ECM;
    double Fire_ACM;
    double dIFF_Frequency;

    double dComm_Out_Frequency;
    double dShield_Strength;
    double dShield_Frequency;
};

struct _Vision_Lobe
{
    Matrix *CNN_Input; //9 x 9 x 4!
    ConvLayer **CNN_Conv;
    DenseLayer *dense;
    //Dense has its own outputs. This is fine.

    int input_rows = 18;      // Adjust Input Size (e.g., 14x14 mini-images)
    int input_cols = 18;
    int hidden_depth = 2;     // Adjust Hidden Conv Depth
    int num_filters = 4;      // Filters per conv layer
    int num_classes = 128;      // Classification targets (e.g., Circle, Square, Triangle), and 128 is how many outputs dense has
    double learning_rate = 0.01;

    int current_r;
    int current_c;
    int flatten_size;
    int target_label;
};



struct _Enemy_Tracking_Lobe
{
    DenseNetwork *Enemy_Tracking_Net; //8 x 16 in, 2 with 64 neurons each.
    AttentionNetwork Attention_Layer;

    int num_layers;
    double learning_rate;
    double* dummy_input;
    int target_label;
    double* outputs;

    int ATN_sequence_length = 8; // Number of words or processing tokens (Input Size Dim A). This should be 8!
    int ATN_embedding_dim = 20;   // Data width of each item vector  (Input Size Dim B). This Should be 20!
    int ATN_hidden_depth = 1;    // Number of stacked Self-Attention Layers. THIS SHOULD BE 1!

    //Pass every contact through the same small network:

//Contact_i
// ↓
//Dense(64)
 //↓
//Dense(64)
// ↓
//Embedding_i

//128 outputs
};

struct _Ally_Tracking_Lobe
{
    DenseNetwork *Ally_Tracking_Net;
    AttentionNetwork Attention_Layer;

    int num_layers;
    double learning_rate;
    double* dummy_input;
    int target_label;
    double* outputs;

    int ATN_sequence_length = 8; // Number of words or processing tokens (Input Size Dim A). This should be 8!
    int ATN_embedding_dim = 20;   // Data width of each item vector  (Input Size Dim B). This Should be 20!
    int ATN_hidden_depth = 1;    // Number of stacked Self-Attention Layers. THIS SHOULD BE 1!

    //3 allies × 5 features

//Feed through shared encoder:

//Ally
// ↓
//Dense(32)
 //↓
//Dense(32)
};

struct _Self_Status_Lobe
{
    DenseNetwork *Self_Status;

    int num_layers;
    double learning_rate;
    double* dummy_input;
    int target_label;
    double* outputs;
};

struct _Parent_Ship_Status_Lobe
{
    DenseNetwork *Parent_Status;

    int num_layers;
    double learning_rate;
    double* dummy_input;
    int target_label;
    double* outputs;
};

struct _LSTM_Lobe
{
    // Layer Object Allocations
    int seq_len    = 5;
    int input_dim  = 384;
    int hidden1    = 256;
    int hidden2    = 128;
    int output_dim = 16;
    float lr       = 0.01f;

    LSTMNetwork *layer1;
    LSTMNetwork *layer2;
    LSTMDenseLayer  *dense;

    // Context Sequence Memory History Structures
    LSTMHistory *hist1;
    LSTMHistory *hist2;

    // Variable Buffers
    float *raw_inputs;
    float *dense_outputs;
    float *target_outs;

    // Recurrent Backward Gradient Trackers
    float *d_dense_out;
    float *dh_layer2;
    float *dh_layer1;
};

struct _Pilot_Artificial_Brain
{
    struct _Vision_Lobe* Vision_Lobe;
    struct _Enemy_Tracking_Lobe* Enemy_Tracking_Lobe;
    struct _Ally_Tracking_Lobe* Ally_Tracking_Lobe;
    struct _Self_Status_Lobe* Self_Status_Lobe;
    struct _Parent_Ship_Status_Lobe* Parent_Ship_Status_Lobe;

    double Concatted_LSTM_Input[128 + 160 + 15 + 32 + 32];

    struct _LSTM_Lobe* LSTM_Lobe;
};

struct _Pilot
{
    unsigned int Use_This_Neural_Net;
   // struct _Flight_Training_Input Input;
   //struct _Flight_Training_Output Output;
};

void Test_Vision_Lobe();
void Test_Enemy_Tracking_Lobe();
void Test_Ally_Tracking_Lobe();
void Test_Self_Status_Lobe();
void Test_Parent_Status_Lobe();
void Test_LSTM();

struct _Pilot_Artificial_Brain* Create_Pilot_Brain();
void Initialize_Pilot_Brain(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);

void Initialize_Pilot_Brain_Visual_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Initialize_Pilot_Brain_Enemy_Tracking_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Initialize_Pilot_Brain_Ally_Tracking_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Initialize_Pilot_Brain_Self_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Initialize_Pilot_Brain_Parent_Ship_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);

void Initialize_LSTM(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);




void train_visual_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void train_enemy_tracking_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void train_ally_tracking_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void train_self_status_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void train_parent_status_lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);

void Truly_Initialize_Vision_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Truly_Initialize_Enemy_Tracking_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Truly_Initialize_Ally_Tracking_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Truly_Initialize_Self_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);
void Truly_Initialize_Parent_Ship_Status_Lobe(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);

void Truly_Initialize_LSTM(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain);

void Train_Pilot_Brain(struct _Engine* Engine, struct _Pilot_Artificial_Brain* Brain, int Epochs);

#endif // NPC_H_INCLUDED

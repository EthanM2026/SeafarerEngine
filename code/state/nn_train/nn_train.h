#ifndef NN_TRAIN_H_INCLUDED
#define NN_TRAIN_H_INCLUDED
#include "../../engine/engine.h"
#include "../submarine/submarine.h"
#include "../../engine/npc/npc.h"
#include "../../engine/neural_network/neuro_pilot.h"
#include "../../engine/neural_network/neural_network.h"
#define MAX_THROTTLE 100
//90km/h.

//25 meters per 60 frames.
//0.42 meters per frame.

struct _Neuro_Pilot_Input
{
    double Values[18];
};

struct _Neuro_Pilot_Output
{
    double Values[4];
};


struct _Neural_Network_Training_State
{
    struct _Submarine_Object* Submarine;
    struct _SE3_Model* Skybox;
    struct _SE3_Model* Test_Sub;
    struct _Preloaded_Collision_Mesh* Mesh;

    unsigned char Current_IO_Example;
    unsigned char Current_IO_Capture;
    struct _Neuro_Pilot_Input Example_Inputs[5];
    struct _Neuro_Pilot_Output Example_Outputs[5];

    struct _Timer* Snapshot_Timer;
    double Snapshot_Time;

    unsigned int Current_Timestamp = 0;
    unsigned int Maximum_Timestamp;
    unsigned int Current_Sliding_Input_Window;
    unsigned int Current_Sliding_Output_Window;

    double SRS[11];
    double MRS[11];
    double LRS[11];

    double My_Long_Range_Sensors[6];
    double My_Medium_Range_Sensors[6];
    double My_Short_Range_Sensors[6];
    double My_Long_Range_Diagonal_Sensors[12];
    double My_Medium_Range_Diagonal_Sensors[12];
    double My_Short_Range_Diagonal_Sensors[12];

    double My_Velocity;
    double My_Roll;
    double My_Pitch;
    double My_Yaw;
    double My_dRoll;
    double My_dPitch;
    double My_dYaw;
    double My_dThrottle; //Not Throttle! Delta Throttle

    double Target_Proximity;
    double Target_Velocity;
    double Target_Roll;
    double Target_Pitch;
    double Target_Yaw;

    double Altitude;
    double Alert_Status;
    double My_Damage;
    double My_Comms;

    double My_Current_Output_Policy[4];
    double My_Neuropilot_Output_Policy[4];

    double My_NEW_LSTM_Input_Vector[71*10]; //71 inputs over 10 frames.

    double My_NEW_LSTM_Output_Vector[4*10]; //4 outputs over 10 frames.



    bool Manual_Control;

    bool Training;

    double LSTM_Sensor_Inputs[18*5];

    double Delta_Throttle;
    double Delta_Roll;
    double Delta_Pitch;
    double Delta_Yaw;


    // Structural Hyperparameters
    int seq_len    = 10;
    int input_dim  = 71;
    int hidden1    = 32;
    int hidden2    = 32;
    int output_dim = 4;
    float lr       = 0.001f;

    // Layer Object Allocations
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

    // Populate mock training sequence data
    bool Third_Person;

    Network *autopilot;

    struct _Submarine_Object* Neuropilot_Submarine;
};

struct _Neural_Network_Training_State* Create_Neural_Network_Training_State();
void Initialize_Neural_Network_Training_State(struct _Engine* Engine);

void Render_Neural_Network_Training_State(struct _Engine* Engine);
void Handle_Neural_Network_Training_State_Inputs(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Neural_Network_Training_State(struct _Engine* Engine);

#endif // NN_TRAIN_H_INCLUDED

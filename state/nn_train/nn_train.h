#ifndef NN_TRAIN_H_INCLUDED
#define NN_TRAIN_H_INCLUDED
#include "../../engine/engine.h"
#include "../submarine/submarine.h"
#include "../../engine/npc/npc.h"
#include "../../engine/neural_network/neuro_pilot.h"
#include "../../engine/neural_network/neural_network.h"
#include "../../engine/neural_network/lstm.h"
#include "../../engine/physics/physics.h"
#define MAX_THROTTLE 1000

#define MAXIMUM_ENTITIES_IN_PLAYFIELD 512

#define ORDER_FORM_UP 1

#define LASER_FIRING_RATE_PER_SECOND 2

//120 km per 216000 frames.

//0.00055555555km per frame.

//0.55555555 meters per frame

struct _Laser_Blast
{
    double x;
    double y;
    double z;

    Vec3 localRight;
    Vec3 localUp;
    Vec3 localForward;

    double Velocity;

    double Time_Created;

    bool Dead;
};

struct _Submarine_Flight
{
    //1, 2, 3 or 4 vessels.
    int Submarine_IDs[4];
};

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
    struct _Submarine_Object* Fake_Submarine[65536];

    double zoom;

    unsigned short Number_Of_Flights;
    struct _Submarine_Flight* Flights[65536];

    struct _SE3_Model* Laser_Model;

    unsigned short Number_Of_Laser_Blasts;
    unsigned short Current_Laser_Blast;
    struct _Laser_Blast* Laser_Blasts[65536];

    double Last_Fire_Time;
};

struct _Laser_Blast* Create_Laser_Blast();
void Initialize_Laser_Blast(struct _Engine* Engine,struct _Laser_Blast* Laser_Blast, double x, double y, double z, Vec3 localRight, Vec3 localUp, Vec3 localForward, double Velocity,double Time_Created);
void Render_Laser_Blast(struct _Engine* Engine,struct _Laser_Blast* Laser_Blast);
void Process_Laser_Blast(struct _Engine* Engine,struct _Laser_Blast* Laser_Blast);

void Process_Submarine_Flight(struct _Submarine_Flight* Flight, struct _Engine* Engine);

void Assign_Submarine_Leader(struct _Engine* Engine, int This_Submarine, int Will_Have_This_Leader);

struct _Neural_Network_Training_State* Create_Neural_Network_Training_State();
void Initialize_Neural_Network_Training_State(struct _Engine* Engine);

struct _Submarine_Object* Create_Fake_Submarine();
void Initialize_Fake_Submarine(struct _Submarine_Object* Submarine);

void Process_Fake_Submarine(struct _Engine* Engine, struct _Submarine_Object* Submarine);
void Render_Fake_Submarine(struct _Engine* Engine, struct _Submarine_Object* Submarine);
void Handle_Fake_Submarine_Inputs(struct _Engine* Engine, struct _Submarine_Object* Submarine);

void Render_Neural_Network_Training_State(struct _Engine* Engine);
void Handle_Neural_Network_Training_State_Inputs(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Neural_Network_Training_State(struct _Engine* Engine);

#endif // NN_TRAIN_H_INCLUDED

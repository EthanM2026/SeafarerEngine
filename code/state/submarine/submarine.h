/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SUBMARINE_H_INCLUDED
#define SUBMARINE_H_INCLUDED
#include "../../engine/engine.h"
#include "../../engine/graphics/image.h"
#include "../../engine/physics/vector.h"
#include "../../engine/physics/physics.h"
#include "../../engine/graphics/new_model.h"
#include "GLFW/glfw3.h"

#define CURRENT_STATE_WALKING 1

#define COLLISION_RESOLUTION 0.0001
#define MARGIN_OF_ERROR 5

#define PREPARING_TO_JUMP 2
#define JUMPING 3
#define FALLING 4
#define NOT_PREPARING_TO_JUMP 5

#define PLAYER_TYPE_ON_FOOT 1
#define PLAYER_TYPE_CAR 2
#define PLAYER_TYPE_SUBMARINE 3

#define TORPEDO_MODE_HOMING 1
#define TORPEDO_MODE_MINE 2
#define TORPEDO_MODE_VECTOR 3

struct _Character_Animation
{
    char Name[32];
    int ID;
    int Model_Number;
    int Start_Keyframe;
    int End_Keyframe;
    float Speed;
    int Loop;
};

struct _Character_Header
{
    char Name[256];
    char Description[256];

    float AABB_Height;
    float AABB_Length;
    float AABB_Width;

    int Number_Of_Models_Used;
    int Number_Of_Voice_Lines;
    int Number_Of_Animations;

    char Attack_Power_Percentage;
    char Defense_Power_Percentage;
    char Speed_Percentage;

    //Chris and Clyde: Explorer Class. Medium Speed, Medium Attack, Medium Defense.
    //BlueSpike and Rosie: Nymph Class. Max Speed, lowest attack and defense.
    //Bob and Wagner: Power Class. Low Speed, High Defense and Attack.

    int Override_Loadout; //Use the CSS loadout instead of this filepath's.
    char Loadout_Filepath[256];

    int Offset_To_Model_Filepaths;
    int Offset_To_Voice_Lines;
    int Offset_To_Animations;

};

struct _New_Player_Submarine
{
float SPEED_STEP = 0.005f;
float ANGLE_STEP = 2.0f;
float TIMER_MS = 16; // ~60 FPS update rate

// --- Global State Variables ---
// Position (World Coordinates)
float plane_x = 0.0f;
float plane_y = 0.0f; // +Y is FORWARDS
float plane_z = 1.0f; // +Z is UPWARDS

// Orientation Angles (Degrees)
float plane_pitch = 0.0f; // Rotation around X-axis (Right/Left)
float plane_roll = 0.0f;  // Rotation around Y-axis (Forward/Backward)
float plane_yaw = 0.0f;   // Rotation around Z-axis (Up/Down)

// Speed (Magnitude of movement)
float plane_speed = 0.05f;
    float xVector;
    float yVector;
    float zVector;

struct _MD2_Model* Model;
};


struct _New_Player_Car
{
    float lastTime = 0.0f;
    float frameTime = 100000.0f;  // 100ms per frame
    int currentFrame = 0;

    struct _Character_Header Header;

    unsigned char Player_Type;

    float dThrottle = 0;

    float Z_velocity = 0;

    Vec3 displacement;

    float x;
    float y;
    float z;

    float dx;
    float dy;
    float dz;

    int Start_Frame;
    int Current_Frame;
    int End_Frame;
    int Current_Model;

    int Current_State;

    Vec3 Forward_Vector;
    Vec3 Right_Vector;
    Vec3 Up_Vector;

    Vec3 Ray_Sensor_Vector;

    float Yaw_Angle;
    float Pitch_Angle;

    float Throttle;

    bool Floor_Collision;
    bool Wall_Collision;
    bool Ceiling_Collision;
    bool Object_Collision;

    bool Slope_Collision;

    bool On_Top_Of_An_AABB_Collision;
    bool Underneath_An_AABB_Collision;
    bool Colliding_With_An_AABB_Collision;

    bool On_Top_Of_Mesh_Collision;
    bool Underneath_Mesh_Collision;
    bool Colliding_With_Mesh_Collision;

    bool Able_To_Move_Away;
    bool Able_To_Move_Off_Platform;

    AABB Feet_Collision_Geometry;
    AABB Head_Collision_Geometry;

    bool Do_Not_Alter_Velocity;
    bool On_The_Ground;
    struct _MD2_Model* Models[5];
};

struct _Player_Submarine
{
    float lastTime = 0.0f;
    float frameTime = 100000.0f;  // 100ms per frame
    int currentFrame = 0;

    struct _Character_Header Header;

    unsigned char Player_Type;

    float dThrottle = 0;

    float Z_velocity = 0;

    Vec3 displacement;

    float x;
    float y;
    float z;

    float dx;
    float dy;
    float dz;

    int Start_Frame;
    int Current_Frame;
    int End_Frame;
    int Current_Model;

    int Current_State;

    Vec3 Forward_Vector;
    Vec3 Right_Vector;
    Vec3 Up_Vector;

    Vec3 Ray_Sensor_Vector;

    float Yaw_Angle;
    float Pitch_Angle;

    float Throttle;

    bool Floor_Collision;
    bool Wall_Collision;
    bool Ceiling_Collision;
    bool Object_Collision;

    bool Slope_Collision;

    bool On_Top_Of_An_AABB_Collision;
    bool Underneath_An_AABB_Collision;
    bool Colliding_With_An_AABB_Collision;

    bool On_Top_Of_Mesh_Collision;
    bool Underneath_Mesh_Collision;
    bool Colliding_With_Mesh_Collision;

    bool Able_To_Move_Away;
    bool Able_To_Move_Off_Platform;

    AABB Feet_Collision_Geometry;
    AABB Head_Collision_Geometry;

    bool Do_Not_Alter_Velocity;
    bool On_The_Ground;
    struct _MD2_Model* Models[5];
};

struct _Torpedo
{
    float Origin_X;
    float Origin_Y;
    float Origin_Z; //If it's too far away, just explode so the space can be overwritten.

    float xVector;
    float yVector;
    float zVector;

    float x;
    float y;
    float z;

    float Velocity;
    float Explosive_Yield_In_TNT;
    float Proximity_To_Explode;
    bool Exploded;

    float Destination_X;
    float Destination_Y;
    float Destination_Z;

    struct _MD2_Model* Model;
    unsigned char Mode;
};

struct _Circular_Shadow
{
    struct _MD2_Model* Shadow;
    float x;
    float y;
    float z;
    float Lifetime; //How low it should check for a collision before giving up.
};


struct _Circular_Shadow* Create_Circular_Shadow();
void Initialize_Circular_Shadow(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Z, float Parent_Radius);
void Recalculate_Circular_Shadow_Coordinates(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Z, float Parent_Radius);
void Render_Shadow(struct _Engine* Engine);

void Recalculate_Circular_Shadow_XY_Coordinates(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Radius);
void Recalculate_Circular_Shadow_Z_Coordinate(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Z, float Parent_Radius);


struct _Torpedo* Create_Torpedo();
void Initialize_Torpedo(struct _Engine* Engine, float x, float y, float z, float DestX, float DestY, float DestZ, float Velocity, unsigned char Mode, float xVector, float yVector, float zVector);
void Process_Torpedo(struct _Engine* Engine, int Torpedo_ID);
void Render_Torpedo(struct _Engine* Engine, int Torpedo_ID);



struct _Player_Submarine* Create_Player();
void Initialize_On_Foot_Player(struct _Engine* Engine, const char* Character_Filepath, float x, float y, float z);

void Render_Player(struct _Engine* Engine);
void Process_Player(struct _Engine* Engine);

struct _New_Player_Submarine* Create_New_Player_Submarine();
void Initialize_New_Player_Submarine(struct _Engine* Engine);
void Process_Player_Submarine(struct _Engine* Engine);

void Handle_Player_Submarine_Inputs(struct _Engine* Engine, struct _Keypad Keypad);
void Render_Player_Submarine(struct _Engine* Engine);


void Handle_Player_Inputs(struct _Engine* Engine, struct _Keypad Keypad);

void Write_Character(const char* Filename);
void Load_Character(const char* Filename);

void Write_Model_Filepaths(FILE* f);
void Write_Voice_Lines(FILE* f);
void Write_Animations(FILE* f);

void Run_Collision_Test(struct _Engine* Engine);








struct _New_Player_Car* Create_New_Player_Car();
void Initialize_New_Player_Car(struct _Engine* Engine);
void Process_Player_Car(struct _Engine* Engine);

void Handle_Player_Car_Inputs(struct _Engine* Engine, struct _Keypad Keypad);
void Render_Player_Car(struct _Engine* Engine);

#endif // SUBMARINE_H_INCLUDED






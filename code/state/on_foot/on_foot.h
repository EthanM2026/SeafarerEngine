/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
PROCEDURE FOR RENDERING SHIPS:

ONLY THE OUTSIDE MOVES AND IS ACTUALLY VISIBLE TO OUTSIDE PLAYERS.
THE INSIDE IS SEPARATE, RENDERED UNDERGROUND, and Static.

WE CAN DO THIS BECAUSE SUBMARINES ALREADY HAVE NO WINDOWS.
*/

#ifndef ON_FOOT_H
#define ON_FOOT_H
#include "../../engine/engine.h"
#include <stdio.h>
#include "../../engine/graphics/new_model.h"
#include "../submarine/submarine.h"
#include "../../engine/graphics/message_box.h"
#include "../../engine/advanced_physics/advanced_physics.h"
#include "../../engine/physics/broadcast.h"
#include "endian.h"
#include "../../engine/emulator/emulator.h"

#define EXIT_STATUS_LEVEL_LOCKED -1
#define EXIT_STATUS_LEVEL_NOT_CLEARED 0
#define EXIT_STATUS_LEVEL_CLEARED 1
#define EXIT_STATUS_LEVEL_CLEARED_EASY_PATH_FORWARD 2
#define EXIT_STATUS_LEVEL_CLEARED_MEDIUM_PATH_FORWARD 3
#define EXIT_STATUS_LEVEL_CLEARED_HARD_PATH_FORWARD 4
#define EXIT_STATUS_LEVEL_CLEARED_MEDIUM_PATH_UP 5
#define EXIT_STATUS_LEVEL_CLEARED_HARD_PATH_UP 6
#define EXIT_STATUS_LEVEL_CLEARED_EASY_PATH_DOWN 7
#define EXIT_STATUS_LEVEL_CLEARED_MEDIUM_PATH_DOWN 8
#define MINI_STATE_PAUSE 1
#define MINI_STATE_UNPAUSED 2
#define IS_FLOOR_CEILING 1
#define IS_WALL 2

#define SKYBOX_ATMOSPHERE 1
#define SKYBOX_OPEN_OCEAN 2
#define SKYBOX_UNDERWATER 3
#define SKYBOX_HYPERSPACE 4

#define STATE_ABOVE_WATER 1
#define STATE_UNDER_WATER 2
#define STATE_IN_HYPERSPACE 3

#define SHIP_INTERNALS_Z_MARGIN -500 //Each ship is placed -500 below the other.


#pragma pack(push, 1)
struct _Region
{
    char Region_Name[256];
    char Region_Description[256];
    double Default_Spawnpoint[3];

    int Number_Of_Skyboxes;
    int Number_Of_Ships;
    int Number_Of_Wall_Collision_Meshes;
    int Number_Of_Ceiling_Collision_Meshes;
    int Number_Of_Goalposts;
    int Number_Of_Characters;
    int Number_Of_Cars;
    int Number_Of_Submarines;
    int Number_Of_Fires;
    int Number_Of_Terrains;
    int Number_Of_Docking_Ports;
    int Number_Of_Switches;

    int Offset_To_Skyboxes;
    int Offset_To_Ships;
    int Offset_To_Wall_Collision_Meshes;
    int Offset_To_Ceiling_Collision_Meshes;
    int Offset_To_Goalposts;
    int Offset_To_Characters;
    int Offset_To_Cars;
    int Offset_To_Submarines;
    int Offset_To_Fires;
    int Offset_To_Terrains;
    int Offset_To_Docking_Ports;
    int Offset_To_Switches;
};
#pragma pack(pop)

struct _Switch_Object
{
    struct _SE3_Model* Model;
    AABB Collision_Box;
    int State;
    double x;
    double y;
    double z;
    int Frame;
};

struct _Switch
{
    char Model_Filepath[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;

    double x2;
    double y2;
    double z2;
    int State;

};

struct _Docking_Port
{
    double x;
    double y;
    double z;

    double Radius;

    double Destination_X;
    double Destination_Y;
    double Destination_Z;
};

struct _Ship //This is nothing but an empty shell!
{
    char Ship_Filepath[256];
    double Shell_x;
    double Shell_y;
    double Shell_z;

    double Shell_Roll;
    double Shell_Yaw;
    double Shell_Pitch;
};

struct _Ship_Details //This is nothing but an empty shell!
{
    char Shell_Model_Name[256];
    char Shell_Texture_Filepath[256];
    struct _Region Internal_Layout;
};

struct _Skybox
{
    char Name[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;
    double Rotational_Velocity;
};

struct _Goalpost
{
    char Name[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;
    int Enable_This_Exit;
};

struct _Terrain
{
    char Name[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;
};

struct _Neural_Network_List
{
    int Number_Of_Neural_Networks;
};

struct _Conversation_List
{
    int Number_Of_Conversations;
};

struct _Navigation_Mesh_List
{
    int Number_Of_Navigation_Meshes;
};

struct _Character
{
    char Character_Filepath[256];

    int Number_Of_Neural_Networks;
    char Character_Neural_Network_List_Filepath[256];

    int Number_Of_Conversations;
    char Character_Conversation_List_Filepath[256];

    int Number_Of_Navigation_Meshes;
    char Character_Navigation_Mesh_Filepath[256];

    unsigned char Player_Type;
    float dThrottle = 0;
    float Z_velocity = 0;
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

    float Yaw_Angle;
    float Pitch_Angle;

    float Throttle;
    double Minimum_Feet_Collision_Geometry[3];
    double Maximum_Feet_Collision_Geometry[3];

    double Minimum_Head_Collision_Geometry[3];
    double Maximum_Head_Collision_Geometry[3];

    double Current_Attack;
    double Current_Defense;
    double Current_Speed;
    double Current_Health;

    bool Dead;
    bool Playing_Dead_Animation;
};



struct _Car
{
    char Name[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;
    double Yaw;
};

struct _Submarine
{
    char Name[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;
    double Yaw;
    double Pitch;
    double Roll;
};

struct _Fire
{
    double Fire_Size;
    double Fire_Damage_Radius;
    double x;
    double y;
    double z;
};

struct _Ship_Object
{
    struct _SE3_Model* Ship_Shell_Model;
    double Shell_x;
    double Shell_y;
    double Shell_z;
    double Shell_Roll;
    double Shell_Yaw;
    double Shell_Pitch;

    double Actual_Origin_x;
    double Actual_Origin_y;
    double Actual_Origin_z;
};

struct _Skybox_Object
{
    struct _SE3_Model* Ship_Model;
    double x;
    double y;
    double z;
    double Rotational_Velocity;
};

struct _Goalpost_Object
{
    struct _SE3_Model* Model;
    struct _Preloaded_Collision_Mesh* Collision_Mesh;
    double x;
    double y;
    double z;
    int Enable_This_Exit;
};

struct _Fire_Object
{
    struct _Image* Fire_Image;
    double Fire_Size;
    double Fire_Damage_Radius;
    double x;
    double y;
    double z;
};

struct _Terrain_Object
{
    struct _SE3_Model* Model;
    double x;
    double y;
    double z;
};

struct _Ship_Internals_Pointer
{
    struct _Region Region;
    double Safe_X_Position;
    double Safe_Y_Position;
    double Safe_Z_Position;
    struct _Ship_Object* Ship_Objects[65536];
    struct _Preloaded_Collision_Mesh* Wall_Collision_Meshes[65536];
    struct _Preloaded_Collision_Mesh* Ceiling_Collision_Meshes[65536];
    struct _Goalpost_Object* Goalposts[256];
    struct _Player_Submarine* NPC_Characters[65536];
    struct _New_Player_Car* Cars[65536];
    struct _Submarine_Object* Submarines[65536];
    struct _Fire_Object* Fires[65536];
    struct _Terrain_Object* Terrain_Objects[16];
    struct _Computer* Computers[65536];
    struct _Torpedo* Torpedoes[65536];
    struct _Docking_Port* Docking_Ports[65536];
    struct _Switch_Object* Switch_Objects[65536];
};

struct _On_Foot_State
{
    struct _Region Region;
    struct _SE3_Model* Skybox;
    struct _SE3_Model* Clouds;
    struct _Player_Submarine* On_Foot_Player;
    double Camera_Y;
    double Camera_Z;
    bool Enter_Not_Pressed = true;
    int Mini_State;

    double Safe_X_Position;
    double Safe_Y_Position;
    double Safe_Z_Position;

        bool Go_Back_Timer_Was_Triggered;

    bool Up_Not_Pressed;
    bool Down_Not_Pressed;

    float Selector_Y;

    int Current_Choice;

    double r = 0;

    int Current_State;

    bool Player_Is_In_Car;
    int In_This_Car;

    bool Player_Is_In_Submarine;
    int In_This_Submarine;

    int Number_Of_Torpedoes = 0;

    bool FPS_Mode;

    int width = 1920;
    int height = 1080-32-32;

    // Camera configuration
    float yaw = -90;   // Vertical angle (degrees)
    float pitch = 0.0f;   // Vertical angle (degrees)
    float sensitivity = 0.05f; // Mouse sensitivity

    // Camera Vectors
    float camX = 0.0f;
    float camY = 0.0f;
    float camZ = 50.0f; // Camera position
    float frontX = 0.0f;
    float frontY = -1.0f;
    float frontZ = 0.0f; // Where we are looking
    float upX = 0.0f;
    float upY = 0.0f;
    float upZ = 1.0f; // The Up direction

    struct _Image* Reticle;

    struct _Image* Resume;
    struct _Image* Exit;
    struct _Image* Inventory;

    struct _Image* Resume_Highlighted;
    struct _Image* Exit_Highlighted;
    struct _Image* Inventory_Highlighted;

    struct _Timer* Go_Back_Timer;

    struct _Timer* Death_Timer;

    struct _Skybox_Object* Skybox_Objects[16];
    struct _Ship_Object* Ship_Objects[65536];
    struct _Preloaded_Collision_Mesh* Wall_Collision_Meshes[65536];
    struct _Preloaded_Collision_Mesh* Ceiling_Collision_Meshes[65536];
    struct _Goalpost_Object* Goalposts[256];
    struct _Player_Submarine* NPC_Characters[65536];
    struct _New_Player_Car* Cars[65536];
    struct _Submarine_Object* Submarines[65536];
    struct _Fire_Object* Fires[65536];
    struct _Terrain_Object* Terrain_Objects[16];
    struct _Computer* Computers[65536];
    struct _Torpedo* Torpedoes[65536];
    struct _Switch_Object* Switch_Objects[65536];

    struct _Ship_Internals_Pointer* Ship_Internals_Pointer[16];
};


struct _On_Foot_State* Create_On_Foot_State();
void Destroy_On_Foot_State(struct _Engine* Engine);

void Initialize_On_Foot_State(struct _Engine* Engine);

void Render_On_Foot_State(struct _Engine* Engine);
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_On_Foot_State(struct _Engine* Engine);

void Write_Region(struct _Engine* Engine, const char* Filename);
void Load_Region(struct _Engine* Engine, const char* Filename);

void Write_Character_Object(struct _Engine* Engine, const char* Filename);
void Write_Conversation_Object(struct _Engine* Engine, const char* Filename);
void Write_Neural_Network_Object(struct _Engine* Engine, const char* Filename);
void Write_Navigation_Mesh_Object(struct _Engine* Engine, const char* Filename);

void Load_Character_Object(struct _Engine* Engine, const char* Filename);
void Load_Conversation_Object(struct _Engine* Engine, const char* Filename);
void Load_Neural_Network_Object(struct _Engine* Engine, const char* Filename);
void Load_Navigation_Mesh_Object(struct _Engine* Engine, const char* Filename);



void Check_Car_Input(struct _Engine* Engine, struct _Keypad Keypad);
void Check_Submarine_Input(struct _Engine* Engine, struct _Keypad Keypad);
void updateCameraVectors(struct _Engine* Engine);
void mouseMotion(struct _Engine* Engine, double x, double y,struct _Keypad Keypad);

void Write_Ship(const char* Filename);

void Load_Ship(struct _Engine* Engine, int Ship_Index, const char* Filename);

#endif /* CUTSCENE_H */
//NEED A SEPARAT FUNCTION TO INCREMENT EACH FRAME!

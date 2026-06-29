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
#include "../../engine/item/item.h"
#include "../../engine/emulator/computer.h"
#include "../../engine/network/graph.h"

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

#define FIRE_COOLDOWN 0.5

#define SHIP_INTERNALS_Z_MARGIN -500 //Each ship is placed -500 below the other.

#define MAXIMUM_DRAW_RADIUS 16000*METER_CONVERSION

#pragma pack(push, 1)

struct _Door
{
    char Door_Model_Filepath[256];
    char Door_Texture_Filepath[256];
    double x;
    double y;
    double z;

    double x2;
    double y2;
    double z2;
    unsigned char Current_Frame;
};

struct _Door_Object
{
    struct _SE3_Model* Door_Model;
    unsigned char Current_Frame;
    AABB Door_Collision_Mesh;
};

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
    int Number_Of_On_Board_Cameras;
    int Number_Of_RT_Screens;
    int Number_Of_Slope_Collision_Meshes;
    int Number_Of_Seats;
    int Number_Of_Vortexes;
    int Number_Of_TVs;
    int Number_Of_Combo_Seat_Screens;

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
    int Offset_To_On_Board_Cameras;
    int Offset_To_RT_Screens;
    int Offset_To_Slope_Collision_Meshes;
    int Offset_To_Seats;
    int Offset_To_Vortexes;
    int Offset_To_TVs;
    int Offset_To_Combo_Seat_Screens;
};
#pragma pack(pop)

struct _TV_Object
{
    struct _SE3_Model* Model;

    struct _Broadcaster* Broadcaster;

    double x;
    double y;
    double z;

    double Screen_x1;
    double Screen_y1;
    double Screen_z1;

    double Screen_x2;
    double Screen_y2;
    double Screen_z2;

    double Screen_x3;
    double Screen_y3;
    double Screen_z3;

    double Screen_x4;
    double Screen_y4;
    double Screen_z4;

    double Receiver_Frequency;
    double Receiver_Audio_Frequency;
    int State;
};


struct _Combo_Seat_Screen_Object
{
};

struct _Combo_Seat_Screen
{
    char Model_Filepath[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;

    double x1;
    double y1;
    double z1;

    double x2;
    double y2;
    double z2;

    double Eye_Offset_x;
    double Eye_Offset_y;
    double Eye_Offset_z;

    int Is_A_Drivers_Seat;
    int Screen_Type;
    int Screen_State;


    double Screen_x1;
    double Screen_y1;
    double Screen_z1;

    double Screen_x2;
    double Screen_y2;
    double Screen_z2;

    double Screen_x3;
    double Screen_y3;
    double Screen_z3;

    double Screen_x4;
    double Screen_y4;
    double Screen_z4;

    double Receiver_Frequency;
    double Receiver_Audio_Frequency;

    double Red_Level_Dial;
    double Blue_Level_Dial;
    double Green_Level_Dial;
    double Contrast_Level;
    double Brightness_Level;

    unsigned char HDD_Slot_Status;
    unsigned char Removable_Slot_Status;

    unsigned char Sensor_Link_Port_Status;
    unsigned char Peripheral_Link_Port_Status;
    unsigned char Network_Link_Port_Status;

    unsigned char Monitor_Link_Port_Status;

    unsigned char Keyboard_Link_Port_Status;
    unsigned char Mouse_Link_Port_Status;
    unsigned char Controller_Link_Port_Status;

    unsigned char Power_Supply_Status;
    unsigned char Fan_Status;

    unsigned char CPU_Is_Running;

    unsigned char Color_Mode;
    unsigned char Background_Color;
    unsigned char Current_RAM_Bank;
};

struct _On_Foot_TV
{
    char Model_Filepath[256];
    char Texture_Filepath[256];

    double x;
    double y;
    double z;

    double Screen_x1;
    double Screen_y1;
    double Screen_z1;

    double Screen_x2;
    double Screen_y2;
    double Screen_z2;

    double Screen_x3;
    double Screen_y3;
    double Screen_z3;

    double Screen_x4;
    double Screen_y4;
    double Screen_z4;

    double Receiver_Frequency;
    double Receiver_Audio_Frequency;
    int State;
};

struct _Vortex
{
    double x;
    double y;
    double z;
    double Gravity_Radius;
    double Suction_Force;
    //double Size;
};

struct _Seat
{
    char Model_Filepath[256];
    char Texture_Filepath[256];
    double x;
    double y;
    double z;

    double x1;
    double y1;
    double z1;

    double x2;
    double y2;
    double z2;

    double Eye_Offset_x;
    double Eye_Offset_y;
    double Eye_Offset_z;

    int Is_A_Drivers_Seat;
    int Seat_Contains_Console;

    double Screen_x1;
    double Screen_y1;
    double Screen_z1;

    double Screen_x2;
    double Screen_y2;
    double Screen_z2;

    double Screen_x3;
    double Screen_y3;
    double Screen_z3;

    double Screen_x4;
    double Screen_y4;
    double Screen_z4;
};

struct _Seat_Object
{
    struct _SE3_Model* Model;
    AABB Collision_Box;
    double x;
    double y;
    double z;

    double Eye_Offset_x;
    double Eye_Offset_y;
    double Eye_Offset_z;

    int Is_A_Drivers_Seat;
    int Seat_Contains_Console;

    double Screen_x1;
    double Screen_y1;
    double Screen_z1;

    double Screen_x2;
    double Screen_y2;
    double Screen_z2;

    double Screen_x3;
    double Screen_y3;
    double Screen_z3;

    double Screen_x4;
    double Screen_y4;
    double Screen_z4;

    struct _New_Computer* New_Computer;
};


struct _RT_Screen
{
    int Status;
    int Channel;

    double x;
    double y;
    double z;

    char Model_Filepath[256];
};

struct _RT_Screen_Object
{
    int Status;
    int Channel;

    double x;
    double y;
    double z;

    struct _SE3_Model* Model;
};

struct _Camera_Object
{
    GLuint fbo;
    GLuint textureColorBuffer;
    GLuint rbo;

    double x;
    double y;
    double z;

    double Look_x;
    double Look_y;
    double Look_z;
};

struct _Camera
{
    double x;
    double y;
    double z;

    double Look_x;
    double Look_y;
    double Look_z;
};

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

    int Tied_To_This_Submarine;
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
    char Character_Model_Filepath[256];
    char Character_Texture_Filepath[256];
    double x;
    double y;
    double z;

    double FeetAABBMinX;
    double FeetAABBMinY;
    double FeetAABBMinZ;

    double FeetAABBMaxX;
    double FeetAABBMaxY;
    double FeetAABBMaxZ;

    double HeadAABBMinX;
    double HeadAABBMinY;
    double HeadAABBMinZ;

    double HeadAABBMaxX;
    double HeadAABBMaxY;
    double HeadAABBMaxZ;

    int Number_Of_Conversations;
    char Conversation_Filepath[256];
};

struct _Conversation
{
    int Number_Of_Turns;
};

struct _Conversation_Turn
{
    char Text_Filepath[256];
    char Voice_Filepath[256];
    double Line_Length_In_Seconds;
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
    int Single_Pilot;
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

    int Tied_To_This_Submarine;

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





struct _Character_Object
{
    struct _SE3_Model* Character_Model;

    float lastTime = 0.0f;
    float frameTime = 100000.0f;  // 100ms per frame

    double Look_Vector_Origin_X;
    double Look_Vector_Origin_Y;
    double Look_Vector_Origin_Z;

    double Look_Vector_Destination_X;
    double Look_Vector_Destination_Y;
    double Look_Vector_Destination_Z;

    bool Is_Third_Person; //NPC character.

    struct _Character Header;

    unsigned char Player_Type;

    float dThrottle = 0;

    float Z_velocity = 0;

    Vec3 displacement;

    float OldDisX;
    float OldDisY;
    float OldDisZ;

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

    double Current_Attack;
    double Current_Defense;
    double Current_Speed;
    double Current_Health;

    bool Dead;
    bool Playing_Dead_Animation;

    bool Jump_Not_Pressed;
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
    struct _Character_Object* NPC_Character_Objects[65536];
    struct _Player_Submarine* NPC_Characters[65536];
    struct _New_Player_Car* Cars[65536];
    struct _Submarine_Object* Submarines[65536];
    struct _Fire_Object* Fires[65536];
    struct _Terrain_Object* Terrain_Objects[16];
    struct _Computer* Computers[65536];
    struct _Torpedo* Torpedoes[65536];
    struct _Docking_Port* Docking_Ports[65536];
    struct _Switch_Object* Switch_Objects[65536];
    struct _Camera_Object* Camera_Objects[65536];
    struct _RT_Screen_Object* RT_Screen_Objects[65536];
    struct _Preloaded_Collision_Mesh* Slope_Collision_Meshes[65536];
    struct _Seat_Object* Seat_Objects[65536];
    struct _TV_Object* TV_Objects[65536];
    struct _Combo_Seat_Screen_Object* Combo_Seat_Screen_Object[65536];
   // struct _Graph* Graphs[65536];
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
    bool Interact_Not_Pressed = true;
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

    bool Player_Is_On_Ship;
    int On_This_Ship;

    int Number_Of_Torpedoes = 0;
    int Number_Of_Weapons = 0;

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

    float LightMag = 1;

    bool Debug = false;

    bool Flashlight = true;

    bool Is_In_Subspace = true;

    bool Show_Weapons_Menu = false;
    double Time_Weapons_Menu_Was_Born;

    bool Shaky_Camera = false;

    bool Mouse_Scroll_Up_Not_Pressed;
    bool Mouse_Scroll_Down_Not_Pressed;
    bool Mouse_Click_Not_Pressed;

    bool Unsheathing_Weapon = false;
    bool Firing_Weapon = false;
    bool Mouse_GUI_Active = false;

    double Time_Reload_Timer_Was_Born;

    double Subspace_Hole_x;
    double Subspace_Hole_y;
    double Subspace_Hole_z;
    double Time_Entered_Subspace;

    double Time_Refreshed_Screens;

    struct _Image* Reticle;

    struct _Image* FPS_Reticle;

    struct _Image* Resume;
    struct _Image* Exit;
    struct _Image* Inventory;

    struct _Image* Resume_Highlighted;
    struct _Image* Exit_Highlighted;
    struct _Image* Inventory_Highlighted;

    struct _Image* Debug_Text_Player_X;
    struct _Image* Debug_Text_Player_Y;
    struct _Image* Debug_Text_Player_Z;

    struct _Image* Debug_Text_Player_Yaw;
    struct _Image* Debug_Text_Player_Pitch;

    struct _Image* Debug_Text_Player_Look_X;
    struct _Image* Debug_Text_Player_Look_Y;
    struct _Image* Debug_Text_Player_Look_Z;

    struct _Image* Debug_Text_Player_Flashlight_Off;
    struct _Image* Debug_Text_Player_Flashlight_On;

    struct _Image* Torpedo_Billboard;
    struct _Image* Torpedo_Explosion_Billboard;

    struct _Image* Torpedo_Explosion_Billboard_Two;
    struct _Image* Torpedo_Explosion_Billboard_Three;
    struct _Image* Torpedo_Explosion_Billboard_Four;

    struct _SE3_Model* Reticle_Three;

    struct _SE3_Model* Atmosphere_Skybox;
    struct _SE3_Model* Ocean_Surface;
    struct _SE3_Model* Ocean_Seabox;

    struct _Image* Current_Weapon_Selector;

    struct _Timer* Go_Back_Timer;

    struct _Timer* Death_Timer;

    struct _Timer* Universal_Timer;

    struct _Timer* Cooldown_Timer;
    bool Cooldown_Complete;

    struct _Image* Ammo_Count;

    Mix_Chunk* Pause;
    Mix_Chunk* Pause_Select;
    Mix_Chunk* Pause_Confirm;
    struct _Image* Pause_Background;

    struct _Skybox_Object* Skybox_Objects[16];
    struct _Ship_Object* Ship_Objects[65536];
    struct _Preloaded_Collision_Mesh* Wall_Collision_Meshes[65536];
    struct _Preloaded_Collision_Mesh* Ceiling_Collision_Meshes[65536];
    struct _Goalpost_Object* Goalposts[256];
    struct _Character_Object* NPC_Character_Objects[65536];
    struct _Player_Submarine* NPC_Characters[65536];
    struct _New_Player_Car* Cars[65536];
    struct _Submarine_Object* Submarines[65536];
    struct _Fire_Object* Fires[65536];
    struct _Terrain_Object* Terrain_Objects[16];
    struct _Computer* Computers[65536];
    struct _Torpedo* Torpedoes[65536];
    struct _Switch_Object* Switch_Objects[65536];
    struct _Camera_Object* Camera_Objects[65536];
    struct _RT_Screen_Object* RT_Screen_Objects[65536];
    struct _Preloaded_Collision_Mesh* Slope_Collision_Meshes[65536];
    struct _Seat_Object* Seat_Objects[65536];
    struct _Vortex* Vortexes[65536];
    struct _Weapon* Weapons[65536];
    struct _TV_Object* TV_Objects[65536];
    struct _Combo_Seat_Screen_Object* Combo_Seat_Screen_Object[65536];
    struct _Ship_Internals_Pointer* Ship_Internals_Pointer[16];
    //struct _Graph* Graphs[65536];
};


struct _On_Foot_State* Create_On_Foot_State();
void Destroy_On_Foot_State(struct _Engine* Engine);

void Initialize_On_Foot_State(struct _Engine* Engine);

void Render_On_Foot_State(struct _Engine* Engine);
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad, struct GB_AUDIO_PROCESSOR apu);
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

void Write_NPC_Character(struct _Engine* Engine, const char* Filename);
void Load_NPC_Character(struct _Engine* Engine, int Index, const char* Filename);
void Process_NPC_Character(struct _Engine* Engine, int Index);


void Write_Conversation(struct _Engine* Engine, const char* Filename);
void Load_Conversation(struct _Engine* Engine, int Index, const char* Filename);

bool New_Check_If_Ray_Sensor_Detects_Mesh(float Ray_x1, float Ray_y1, float Ray_z1, float Ray_x2, float Ray_y2, float Ray_z2, struct _Preloaded_Collision_Mesh* Mesh);
bool lineIntersectsTriangle(Vec3 Start_Vector, Vec3 End_Vector, Vec3 v0, Vec3 v1, Vec3 v2);

void Enter_Subspace(struct _Engine* Engine);
void Exit_Subspace(struct _Engine* Engine);

void Render_Goalposts(struct _Engine* Engine);


#endif /* CUTSCENE_H */
//NEED A SEPARAT FUNCTION TO INCREMENT EACH FRAME!

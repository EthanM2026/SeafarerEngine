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
#include "../game_over/the_end.h"
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
#include "../../engine/network/client.h"

#define OFS_MODE_MULTIPLAYER 1

#define PLAYER_IS_ON_FOOT 1
#define PLAYER_IS_IN_VEHICLE 2
#define PLAYER_IS_ONBOARD 3
#define PLAYER_IS_DEAD 4
#define PLAYER_IS_IN_CUTSCENE 5

#define STATE_UNPAUSED 1
#define STATE_PAUSED 2

#define AREA_OF_OPERATIONS METER_CONVERSION * 35000

#define CURRENT_LOCATION_SEA 1
#define CURRENT_LOCATION_UNDERWATER 2
#define CURRENT_LOCATION_HYPERSPACE 3

#pragma pack(push, 1)

struct _Goalpost
{
    char Model[256];
    char Texture[256];
    double x;
    double y;
    double z;
    double Radius;
    int Clear_Category;
};

struct _Goalpost_Object
{
    struct _SE3_Model* Model;
    double x;
    double y;
    double z;
    double Radius;
    int Clear_Category;
};


struct _Character
{
    char Filepath[256];
    double x;
    double y;
    double z;
};

struct _Viewscreen
{
    char Model[256];
    char Texture[256];
    double x;
    double y;
    double z;

    double xTwo;
    double yTwo;
    double zTwo;

    bool Viewscreen_On;
    bool Viewscreen_Receiving_Power;
};

struct _Viewscreen_Object
{
    struct _SE3_Model* Viewscreen_Box;
    double x;
    double y;
    double z;

    double xTwo;
    double yTwo;
    double zTwo;

    bool Viewscreen_On;
    bool Viewscreen_Receiving_Power;
};

struct _Door
{
    double x;
    double y;
    double z;

    int Axis_Orientation;

    char Model[256];
    char Texture[256];

    char Ceiling_Geometry_File[256];
    char Wall_Geometry_File[256];

    bool Use_Custom_Collision_Geometry;

    AABB Entrance_AABB;
    AABB Exit_AABB;

    bool Entrance_Open;
    bool Exit_Open;

    int Number_Of_Frames;
    int Current_Frame;

    double Teleport_Destination_X;
    double Teleport_Destination_Y;
    double Teleport_Destination_Z;

    int Connected_To_Type;
    int Connection_Destination_Door;
};

struct _Door_Object
{
    double x;
    double y;
    double z;

    int Axis_Orientation;

    struct _SE3_Model* Door_Model;
    struct _Preloaded_Collision_Mesh* Custom_Ceiling_Collision_Geometry;
    struct _Preloaded_Collision_Mesh* Custom_Wall_Collision_Geometry;
    bool Use_Custom_ColGeo;

    AABB Entrance_AABB;
    AABB Exit_AABB;

    bool Entrance_Open;
    bool Exit_Open;

    int Number_Of_Frames;
    int Current_Frame;

    double Teleport_Destination_X;
    double Teleport_Destination_Y;
    double Teleport_Destination_Z;

    int Connected_To_Type;
    int Connection_Destination_Door;
};

struct _Vertical_Collision_Geometry
{
    char Filepath[256];
    double x;
    double y;
    double z;
};

struct _Horizontal_Collision_Geometry
{
    char Filepath[256];
    double x;
    double y;
    double z;
};

struct _Slope_Collision_Geometry
{
    char Filepath[256];
    double x;
    double y;
    double z;
};

struct _Terrain
{
    char Model[256];
    char Texture[256];
    bool Visible;
    double x;
    double y;
    double z;
};




struct _Vertical_Collision_Geometry_Object
{
    struct _Preloaded_Collision_Mesh* Collision_Mesh;
};

struct _Horizontal_Collision_Geometry_Object
{
    struct _Preloaded_Collision_Mesh* Collision_Mesh;
};

struct _Slope_Collision_Geometry_Object
{
    struct _Preloaded_Collision_Mesh* Collision_Mesh;
};

struct _Terrain_Object
{
    struct _SE3_Model* Terrain_Model;
    bool Visible;
    double x;
    double y;
    double z;
};




struct _On_Foot_Region_File
{
    char Name[256];

    int Number_Of_Vertical_Collision_Geometries;
    int Number_Of_Horizontal_Collision_Geometries;
    int Number_Of_Slope_Collision_Geometries;
    int Number_Of_Terrains;
    int Number_Of_Doors;
    int Number_Of_Ships;
    int Number_Of_Viewscreens;
    int Number_Of_Characters;
    int Number_Of_Goalposts;

    int Offset_To_Vertical_Collision_Geometries;
    int Offset_To_Horizontal_Collision_Geometries;
    int Offset_To_Slope_Collision_Geometries;
    int Offset_To_Terrains;
    int Offset_To_Doors;
    int Offset_To_Ships;
    int Offset_To_Viewscreens;
    int Offset_To_Characters;
    int Offset_To_Goalposts;
};

struct _On_Foot_Region
{
    struct _Vertical_Collision_Geometry_Object* Ceilings[65536];
    struct _Horizontal_Collision_Geometry_Object* Walls[65536];
    struct _Slope_Collision_Geometry_Object* Slopes[65536];
    struct _Terrain_Object* Terrains[65536];
    struct _Door_Object* Doors[65536];
    struct _Viewscreen_Object* Viewscreens[65536];
    struct _Goalpost_Object* Goalposts[65536];
    unsigned short Number_Of_Characters;
    unsigned short Current_Character;
    unsigned short Number_Of_Net_Characters;
    unsigned short Current_Net_Character;
    struct _Player_Submarine* Characters[32768];
    struct _Player_Submarine* Net_Characters[32768];
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

struct _Ship
{
    char Filepath[256];
};

struct _Ship_Object
{
    double My_Offset[3];
    struct _On_Foot_Region_File Ship_Interior_File;
    struct _On_Foot_Region Ship_Interior;
};

#pragma pack(pop)

struct _On_Foot_State
{
    unsigned char Mode;

    double Maximum_X_Coordinate;
    double Maximum_Y_Coordinate;
    double Maximum_Z_Coordinate;

    double Minimum_X_Coordinate;
    double Minimum_Y_Coordinate;
    double Minimum_Z_Coordinate;

    double Kill_Ceiling_Z; //Either stop me or kill me here.
    double Kill_Floor_Z; //Either stop me or kill me here.

    unsigned char Current_Player_State;

    bool Third_Person_Mode;
    bool Debug_Mode;
    unsigned char Current_State;
    unsigned char Current_Location;

    double Camera_X;
    double Camera_Y;
    double Camera_Z;
    double Camera_Yaw;
    double Camera_Pitch;
    double Camera_Front_X;
    double Camera_Front_Y;
    double Camera_Front_Z;
    double Camera_Sensitivity;

    double Light_Front_X;
    double Light_Front_Y;
    double Light_Front_Z;

    bool Pause_Button_Pressed;

    struct _Seafarer_Engine_Multiplayer_Client* Client;

    struct _SE3_Model* Skybox;
    struct _SE3_Model* Seabox;
    struct _SE3_Model* Seasurfacebox;
    struct _SE3_Model* Hyperbox;

    struct _SE3_Model* Rosechu;

    struct _Timer* Universal_Timer;
    struct _Timer* Fire_Timer;
    struct _Timer* Cooldown_Timer;
    struct _Timer* Reload_Timer;

    double Animation_Fired;
    double Reload_Fired;
    bool Weapon_Fired;
    bool Need_To_Reload;
    double Cooldown_Fired; //Track the time I fired the weapon.
    int Ammo;
    int Health;

    struct _Image* Numbers[10];

    struct _On_Foot_Region_File On_Foot_Region_File;
    struct _On_Foot_Region On_Foot_Region;

    struct _Player_Submarine* On_Foot_Player;

    struct _Image* Pause_Background;
    int Current_Pause_Choice;
    struct _Image* Pause_Inventory_Highlighted;
    struct _Image* Pause_Inventory;

    struct _Image* Pause_Resume_Highlighted;
    struct _Image* Pause_Resume;

    struct _Image* Pause_Exit_Highlighted;
    struct _Image* Pause_Exit;

    bool Down_Not_Pressed;
    bool Up_Not_Pressed;
    double Pause_Selector_Y;

    bool Interact_Not_Pressed;

    struct _Weapon_Object* Weapon_Objects[65536];

    unsigned short Number_Of_Weapon_Rounds;
    unsigned short Current_Weapon_Round;
    struct _Weapon_Round_Object* Weapon_Round_Objects[65536];

    struct _SE3_Model* Hand;

    struct _Ship_Object* Ship_Objects[65536];

    double Current_Safe_Ship_Altitude;

    bool Network_On;

    struct _Seafarer_Engine_Network_Client* SENC;
    int Connection_Status;

    struct _Timer* Go_Back_Timer;
    bool Go_Back_Timer_Was_Triggered;
    bool Render;
};

void Spawn_Character(struct _On_Foot_Region* On_Foot_Region, int ID, double x, double y, double z);

void numberToCharArray(int num, char result[3]);
struct _On_Foot_State* Create_On_Foot_State();
void Destroy_On_Foot_State(struct _Engine* Engine);

void Initialize_On_Foot_State(struct _Engine* Engine);

void Render_On_Foot_State(struct _Engine* Engine);
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_On_Foot_State(struct _Engine* Engine);

void updateCameraVectors(struct _Engine* Engine);
void mouseMotion(struct _Engine* Engine, double x, double y,struct _Keypad Keypad);

void Write_On_Foot_Region(const char* Filename);
void Load_On_Foot_Region(struct _Engine* Engine, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File, const char* Filename, double Z_Offset);

void Door_Collision_Detection(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File);

#endif /* CUTSCENE_H */
//NEED A SEPARAT FUNCTION TO INCREMENT EACH FRAME!

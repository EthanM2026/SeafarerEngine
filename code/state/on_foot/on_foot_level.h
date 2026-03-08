/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ON_FOOT_LEVEL_H_INCLUDED
#define ON_FOOT_LEVEL_H_INCLUDED
#include "stdio.h"
#include "../../engine/engine.h"
struct _On_Foot_Level_File_Header
{
    int Number_Of_Level_Segment_Files;
    int Offset_To_Level_Segment_Files;
    char Name[128];
    int Arc;
    int Level_ID;
    int Must_Be_Certain_Character;
    int Allowed_Characters[32];
    int Their_Starting_Level_Segments[32];
    int Their_Starting_Level_Spawnpoints[32];
    int Default_Starting_Level_Spawnpoint;
    int Virtual_DIP_Switch[32];
    int Possible_To_Fail_Level;
    int Score_Kept;
    int Time_Kept;
};

struct _Level_Filepath
{
    char Name[32];
};

struct _Level_Segment_File_Header //A segment is a part of a level. It is the smallest unit which can be loaded and unloaded from memory.
{
    int Number_Of_Spawnpoints;
    int Number_Of_Characters;

    int Offset_To_Spawnpoints;
    int Offset_To_Characters;
};


struct _Level_Spawnpoint_Entry
{
    float x;
    float y;
    float z;
};

struct _Level_Character_Entry //THIS IS JUST AN OUTLINE! THE ACTUAL OBJECT IS DIFFERENT!
{
    char Filepath_To_Base_Sprite[128];
    char Filepath_To_Base_Texture[128];

    float Start_x;
    float Start_y;
    float Start_z;

    int Start_Health;
    int Start_Energy;
    int Start_Phases_Through_Player;
    int Start_Attacks_Phase_Through_Player;
    int Start_Stays_Defeated;
    int Start_Damage_Dealt_With_Primary_Attack;
    int Start_Damage_Dealt_With_Secondary_Attack_A;
    int Start_Damage_Dealt_With_Secondary_Attack_B;
    int Start_Damage_Dealt_With_Super_Attack;
    int Start_Damage_Dealt_With_Desperation_Attack;

    int Start_Uses_Navigation_Mesh;
    int Start_Number_Of_Navigation_Meshes;
    int Start_Navigation_Mesh;

    int Start_Current_State; //State_T_Pose, Idle, Walk, Jog, Run, Defeated, Pick up Item, Opening Door, Stepping on Foot Switch, Flipping Wall Switch, Entering Vehicle, Starting Machine, Talking, Attacking
    int Start_Holding_Item;
    int Start_Holding_Squirt_Gun;
    int Start_Injured;

    int Can_Be_Idle;
    int Idle_Frames[2]; //1,2,3,4,5,6,7,8,9,10
    int Idle_Loops;
};

struct _On_Foot_Level_Character_Object
{
    float x;
    float y;
    float z;

    float dx;
    float dy;
    float dz;

    int Current_Health;
    int Current_Energy;
    int Current_Phases_Through_Player;
    int Current_Attacks_Phase_Through_Player;
    int Current_Stays_Defeated;
    int Current_Damage_Dealt_With_Primary_Attack;
    int Current_Damage_Dealt_With_Secondary_Attack_A;
    int Current_Damage_Dealt_With_Secondary_Attack_B;
    int Current_Damage_Dealt_With_Super_Attack;
    int Current_Damage_Dealt_With_Desperation_Attack;

    int Actively_Using_Navigation_Mesh;
    int Current_Navigation_Mesh;

    int Current_State; //State_T_Pose, Idle, Walk, Jog, Run, Defeated, Pick up Item, Opening Door, Stepping on Foot Switch, Flipping Wall Switch, Entering Vehicle, Starting Machine, Talking, Attacking
    int Holding_Item;
    int Holding_Squirt_Gun;
    int Injured;

    int Can_Be_Idle;
    int Idle_Frames[2]; //1,2,3,4,5,6,7,8,9,10
    int Current_Idle_Loops;

    int Number_Of_Models_Loaded;
    struct _MD2_Model* Models;
};

void Create_On_Foot_Level_File_Header(const char* Filename);
void Write_Level_Segment_Filenames(FILE* f);

void Create_On_Foot_Level_Segment_File_Header(const char* Filename);
void Write_On_Foot_Level_Segment_Spawnpoints(FILE* f);
void Write_On_Foot_Level_Segment_Characters(FILE* f);

void Load_On_Foot_Level_Segment_File_Header(struct _Engine* Engine, const char* Filename);

void Load_On_Foot_Level_File_Header(struct _Engine* Engine, const char* Filename);

#endif // ON_FOOT_LEVEL_H_INCLUDED

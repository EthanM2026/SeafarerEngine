/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

struct _New_Arc_List_Entry
{
    char This_Arcs_Name[256]; //To make the code less convoluted.
    char New_Arc_Container_Filename[256];
    char New_Arc_Container_Image_Filename[256];
    int Length_Of_Arc_Name;
};

struct _New_Arc_List
{
    int Number_Of_Arc_Entries;
    int Offset_To_Arc_Entries;
};

struct _CURRENT_CSS_New_Arc_Level_Container
{
    char This_Arcs_Name[256]; //Will be shown on Arc Select menu.
    char This_Arcs_Image[256];
    int Number_Of_New_Arc_Level_Headers;
    int Offset_To_New_Arc_Level_Headers;
    int Length_Of_Arc_Names;
};

struct _CURRENT_CSS_New_Arc_Level_Header //Then, the Arc Triumvirates on the Map Select Screen have these in threes.
{
    int Hard_Medium_Easy_Levels_Exist[3];
    int Hard_Medium_Easy_Level_Exit_Status[3];
    int Hard_Medium_Easy_Level_Accessed_Before[3];
    int Hard_Medium_Easy_Level_Lock_Status[3];

    int Hard_Level_ID;
    int Normal_Level_ID;
    int Easy_Level_ID;

    char Hard_Level_Name[256];
    char Filename_Of_Hard_Level_Icon[256];
    char Filename_Of_Hard_Level_Header[256];
    char Filename_Of_Hard_Level_Description[256];

    char Medium_Level_Name[256];
    char Filename_Of_Medium_Level_Icon[256];
    char Filename_Of_Medium_Level_Header[256];
    char Filename_Of_Medium_Level_Description[256];

    char Easy_Level_Name[256];
    char Filename_Of_Easy_Level_Icon[256];
    char Filename_Of_Easy_Level_Header[256];
    char Filename_Of_Easy_Level_Description[256];

    int Length_Of_Level_Names[3];
};

struct _On_Foot_Level_Header
{
    int Teaser_Exists;
    int Series_Intro_Exists;
    int Act_One_Exists;
    int Act_Two_Exists;
    int Act_Three_Exists;
    int Act_Four_Exists;
    int Act_Five_Exists;

    int Default_Level;
    int Load_This_Level_After_Teaser;
    int Load_This_Level_After_Intro;
    int Load_This_Level_After_Act_One;
    int Load_This_Level_After_Act_Two;
    int Load_This_Level_After_Act_Three;
    int Load_This_Level_After_Act_Four;
    int Load_This_Level_After_Act_Five;

    int Allowed_To_Only_Complete_Level;
    int Allowed_To_Fail_Level;

    int Number_Of_On_Foot_File_Headers;
    int Offset_To_On_Foot_File_Headers;

    int Global_DIP_Switch[32];
    int Global_Parameters[256];

    char Teaser_Cutscene_Filepath[256];
    char Act_One_Cutscene_Filepath[256];
    char Act_Two_Cutscene_Filepath[256];
    char Act_Three_Cutscene_Filepath[256];
    char Act_Four_Cutscene_Filepath[256];
    char Act_Five_Cutscene_Filepath[256];

    int Level_Mission_Accomplished_Objectives[16];
    int Level_Mission_Complete_Objectives[16];
    int Level_Mission_Failed_Objectives[16];

    int Mission_Accomplished_Awards[16];
    int Mission_Complete_Awards[16];
    int Mission_Failed_Consequences[16];


    int Current_Step[2]; //0 is the Cutscene, 1 is the Level.
    int Next_Step[2]; //0 is the Cutscene, 1 is the Level.
    int Region_Failed_Step[2]; //If Region is failed, do this.

    char Level_Name[256];

    char The_End_State_Ending_Line[256]; //When taken to The End state, have a character say this line.
    char The_End_State_Ending_Voice_Line[256];
};

struct _On_Foot_Filename
{
    char Name[256];
};

struct _On_Foot_File_Header
{
    //REMADE and COMPLETE
    int Number_Of_Backgrounds; //268
    //int Number_Of_Actors; //272
   // int Number_Of_Tracks; //276
  //  int Number_Of_SFX_Data; //280
  //  int Number_Of_Spawnpoints;
  //  int Number_Of_Props;
  //  int Number_Of_Endpoints; //Enter these to end the level!
    //

    //Room Controls
  //  int Number_Of_Doors;
  //  int Number_Of_Conveyor_Belts;
  //  int Number_Of_Ladders;
  //  int Number_Of_Switches;
  //  int Number_Of_Dials;
  //  int Number_Of_Pickup_Items;
   // int Number_Of_Fixed_Camera_Boxes; //Switch from third-person view to a security camera view in certain rooms.
    //

    //Dialogue controls
   // int Number_Of_Conversations;
  //  int Number_Of_Conversation_Nodes;
   // int Number_Of_Conversation_Connections;
   // int Number_Of_Shops;
   // int Number_Of_Stage_Changing_Doors; //These do not end the level! Only endpoints do so!
   // int Number_Of_Vehicle_Entry_Points;
   // int Number_Of_Elevators;
   // int Number_Of_Lights;

    //int Number_Of_Computers;
   // int Number_Of_Vehicles;
   // int Number_Of_Computer_Storage_Devices;


   // int Number_Of_Broadcast_Towers;
  //  int Number_Of_Volcanoes;
  //  int Number_Of_Sonic_Black_Holes;
   // int Number_Of_Rainstorms;
   // int Number_Of_Duststorms;
   // int Number_Of_Snowstorms;
   // int Number_Of_Hailstorms;
   // int Number_Of_Earthquakes;
   // int Number_Of_Tornadoes;
   // int Number_Of_Hurricanes;
   // int Number_Of_Auroras;

    //int Number_Of_Docking_Stations;

    //int Number_Of_Ships;

    //int Number_Of_NPC_Plants;
   // int Number_Of_NPC_Swimming_Creatures;
   // int Number_Of_NPC_Terrestrial_Creatures;
    //int Number_Of_NPC_Flying_Creatures;
    //int Number_Of_NPC_Fungi;
    //int Number_Of_NPC_Microbes;

   // int Number_Of_Pools_Of_Liquid;

   // int Number_Of_Mirrors;
   //int Number_Of_Fires;



    int Offset_To_Backgrounds; //268
    //int Offset_To_Actors; //272
   // int Offset_To_Tracks; //276
   // int Offset_To_SFX_Data; //280
   // int Offset_To_Spawnpoints;
  //  int Offset_To_Props;
  //  int Offset_To_Endpoints; //Enter these to end the level!
    //

    //Room Controls
  //  int Offset_To_Doors;
  //  int Offset_To_Conveyor_Belts;
  //  int Offset_To_Ladders;
  //  int Offset_To_Switches;
   // int Offset_To_Dials;
   // int Offset_To_Pickup_Items;
   // int Offset_To_Fixed_Camera_Boxes; //Switch from third-person view to a security camera view in certain rooms.
    //

    //Dialogue controls
  //  int Offset_To_Conversations;
   // int Offset_To_Conversation_Nodes;
   // int Offset_To_Conversation_Connections;
   // int Offset_To_Shops;
   // int Offset_To_Stage_Changing_Doors; //These do not end the level! Only endpoints do so!
   // int Offset_To_Vehicle_Entry_Points;
   // int Offset_To_Elevators;
   // int Offset_To_Lights;

   // int Offset_To_Computers;
    //int Offset_To_Vehicles;
    //int Offset_To_Computer_Storage_Devices;

    //int Offset_To_Broadcast_Towers;
    //int Offset_To_Volcanoes;
    //int Offset_To_Sonic_Black_Holes;
    //int Offset_To_Rainstorms;
   // int Offset_To_Duststorms;
   // int Offset_To_Snowstorms;
   // int Offset_To_Hailstorms;
   // int Offset_To_Earthquakes;
   // int Offset_To_Tornadoes;
   // int Offset_To_Hurricanes;
   // int Offset_To_Auroras;
//
    //int Offset_To_Docking_Stations;

    //int Offset_To_Ships;

    //int Offset_To_NPC_Plants;
    //int Offset_To_NPC_Swimming_Creatures;
    //int Offset_To_NPC_Terrestrial_Creatures;
    //int Offset_To_NPC_Flying_Creatures;
    //int Offset_To_NPC_Fungi;
    //int Offset_To_NPC_Microbes;

    //int Offset_To_Pools_Of_Liquid;

    //int Offset_To_Mirrors;
    //int Offset_To_Fires;
};




struct _On_Foot_Background
{
    char Filepath[256];
    char Texture_Filepath[256];
    int Visible;
    int DIP_Switch_ID;
    float Rotational_Velocity;
    int Phase_Through;
    int Is_Floor_Ceiling_Or_Wall;
    //int SFX_ID;
};

struct _On_Foot_Actor
{
    char Filepath[256];
    char Texture_Filepath[256];

    char Filepath_Two[256];
    char Texture_Filepath_Two[256];

    char Filepath_Three[256];
    char Texture_Filepath_Three[256];

    char Filepath_Four[256];
    char Texture_Filepath_Four[256];

    char Filepath_Five[256];
    char Texture_Filepath_Five[256];

    char Mesh_Collider_Filepath[256];
    char Navigation_Mesh_Filepath[256];

    char Loadout_Filepath[256];
    char Dropped_Item_Filepath[256]; //After defeat.

    char Conversation_Filepath[256];
    char Conversation_Voice_Filepath[256];

    char Conversation_Two_Filepath[256];
    char Conversation_Two_Voice_Filepath[256];

    char Conversation_Three_Filepath[256];
    char Conversation_Three_Voice_Filepath[256];

    char Conversation_Four_Filepath[256];
    char Conversation_Four_Voice_Filepath[256];

    char Conversation_Five_Filepath[256];
    char Conversation_Five_Voice_Filepath[256];

    char Corpse_Filepath[256];
    char Corpse_Texture_Filepath[256];

    char On_Spawn_Line_Filepath[256];
    char On_Spawn_Line_Two_Filepath[256];

    char Idle_Line_Filepath[256];
    char Idle_Line_Two_Filepath[256];

    char Walking_Line_Filepath[256];
    char Walking_Line_Two_Filepath[256];

    char Running_Line_Filepath[256];
    char Running_Line_Two_Filepath[256];

    char Searching_Line_Filepath[256];
    char Searching_Line_Two_Filepath[256];

    char Spotted_Player_Line_Filepath[256];
    char Spotted_Player_Line_Two_Filepath[256];

     char Attacking_Player_Line_Filepath[256];
    char Attacking_Player_Line_Two_Filepath[256];

    char Defeated_Player_Line_Filepath[256];
    char Defeated_Player_Line_Two_Filepath[256];

     char Injured_Player_Line_Filepath[256];
    char Injured_Player_Line_Two_Filepath[256];

    char Player_Walking_Towards_Line_Filepath[256];
    char Player_Walking_Towards_Line_Two_Filepath[256];
    char Chased_By_Player_Line_Filepath[256];
    char Chased_By_Player_Line_Two_Filepath[256];
    char Searched_By_Player_Line_Filepath[256];
    char Searched_By_Player_Line_Two_Filepath[256];
    char Spotted_By_Player_Line_Filepath[256];
    char Spotted_By_Player_Line_Two_Filepath[256];
    char Attacked_By_Player_Line_Filepath[256];
    char Attacked_By_Player_Line_Two_Filepath[256];
    char Injured_By_Player_Line_Filepath[256];
    char Injured_By_Player_Line_Two_Filepath[256];
    char Defeated_By_Player_Line_Filepath[256];
    char Defeated_By_Player_Line_Two_Filepath[256];


    char Idle_With_Player_Ally_Line_Filepath[256];
    char Idle_With_Player_Ally_Line_Two_Filepath[256];
    char Walking_With_Player_Ally_Line_Filepath[256];
    char Walking_With_Player_Ally_Line_Two_Filepath[256];
    char Running_With_Player_Ally_Line_Filepath[256];
    char Running_With_Player_Ally_Line_Two_Filepath[256];
    char Searching_With_Player_Ally_Line_Filepath[256];
    char Searching_With_Player_Ally_Line_Two_Filepath[256];
    char Spotted_Enemy_With_Player_Ally_Line_Filepath[256];
    char Spotted_Enemy_With_Player_Ally_Line_Two_Filepath[256];
    char Attacking_Enemy_With_Player_Ally_Line_Filepath[256];
    char Attacking_Enemy_With_Player_Ally_Line_Two_Filepath[256];
    char Defeated_Enemy_With_Player_Ally_Line_Filepath[256];
    char Defeated_Enemy_With_Player_Ally_Line_Two_Filepath[256];
    char Injured_Enemy_With_Player_Ally_Line_Filepath[256];
    char Injured_Enemy_With_Player_Ally_Line_Two_Filepath[256];
    char Defeated_With_Player_Ally_Line_Filepath[256];
    char Defeated_With_Player_Ally_Line_Two_Filepath[256];
    char Injured_With_Player_Ally_Line_Filepath[256];
    char Injured_With_Player_Ally_Line_Two_Filepath[256];





    int Visible;

    int Number_Of_Model_Filepaths_Used;

    int Actor_Is_Defeatable;

    int Current_Model;
    int Current_Keyframe_Range[2];
    float Current_Animation_Speed;

    int Collider_Type;


    int Uses_Navigation_Mesh;


    float AABB_Box_Dimensions[6];

    float Head_Height;

    float Spawn_X;
    float Spawn_Y;
    float Spawn_Z;

    float Current_X;
    float Current_Y;
    float Current_Z;

    float Current_Throttle;

    int Already_Spawned;
    int Preserve_Between_Regions; //Do not overwrite me upon region change.

    float Spawn_Pitch;
    float Spawn_Roll;
    float Spawn_Yaw;

    float Current_Pitch;
    float Current_Roll;
    float Current_Yaw;

    float Current_dPitch;
    float Current_dRoll;
    float Current_dYaw;

    float Current_Path_X[2]; //Navigation Mesh
    float Current_Path_Y[2];
    float Current_Path_Z[2];

    int Current_Health;
    int Current_Energy;

    int Current_Mode;

    int Current_Loadout_Item;


    int Number_Of_Conversations;

    int Current_Conversation;


    float Visual_Range;
    float Audio_Range;

    int Explode_Upon_Defeat;
    int Corpse_Type; //Gibs or body
    int Smoulder_Upon_Defeat;




    int On_Spawn_DIP_Switch_ID;



    int Idle_DIP_Switch_ID;


    int Walking_DIP_Switch_ID;


    int Running_DIP_Switch_ID;


    int Searching_DIP_Switch_ID;


    int Spotted_Player_DIP_Switch_ID;


    int Attacking_Player_DIP_Switch_ID;


    int Defeated_Player_DIP_Switch_ID;


    int Injured_Player_DIP_Switch_ID;





    int Player_Walking_Towards_DIP_Switch_ID;





    int Chased_By_Player_DIP_Switch_ID;


    int Searched_By_Player_DIP_Switch_ID;


    int Spotted_By_Player_DIP_Switch_ID;


    int Attacked_By_Player_DIP_Switch_ID;


    int Injured_By_Player_DIP_Switch_ID;


    int Defeated_By_Player_DIP_Switch_ID;










    int Idle_With_Player_Ally_DIP_Switch_ID;




    int Walking_With_Player_Ally_DIP_Switch_ID;


    int Running_With_Player_Ally_DIP_Switch_ID;


    int Searching_With_Player_Ally_DIP_Switch_ID;


    int Spotted_Enemy_With_Player_Ally_DIP_Switch_ID;


    int Attacking_Enemy_With_Player_Ally_DIP_Switch_ID;


    int Defeated_Enemy_With_Player_Ally_DIP_Switch_ID;


    int Injured_Enemy_With_Player_Ally_DIP_Switch_ID;


    int Defeated_With_Player_Ally_DIP_Switch_ID;


    int Injured_With_Player_Ally_DIP_Switch_ID;

    int DIP_Switch_ID;

};

struct _On_Foot_Track
{
    char Filepath[256];
    int Playing;
    int DIP_Switch_ID;
    float Volume;
    int Loop;
};

struct _On_Foot_SFX_Data
{
    char Filepath[256];
    int Playing;
    int DIP_Switch_ID;
    float Volume;
    int Loop;

};

struct _On_Foot_Spawnpoint
{
    float x;
    float y;
    float z;
    int DIP_Switch_ID;
    int Current_Spawnpoint;
};

struct _On_Foot_Prop
{
    char Filepath[256];
    char Texture_Filepath[256];
    char Damaged_Prop_Filepath[256];
    char Damaged_Prop_Texture_Filepath[256];
    char Destroyed_Prop_Filepath_Three[256];
    char Destroyed_Prop_Texture_Filepath_Three[256];
    char Mesh_Collider_Filepath[256];

    int Visible;
    int Prop_Is_Destroyable;

    int Current_Keyframe_Range[2];
    float Current_Animation_Speed;

    int Collider_Type;
    float Radius;

    float AABB_Box_Dimensions[6];
    float Head_Height;

    float Spawn_X;
    float Spawn_Y;
    float Spawn_Z;

    float Current_X;
    float Current_Y;
    float Current_Z;

    float Current_Throttle;

    int Already_Spawned;
    int Preserve_Between_Regions; //Do not overwrite me upon region change.

    float Spawn_Pitch;
    float Spawn_Roll;
    float Spawn_Yaw;

    float Current_Pitch;
    float Current_Roll;
    float Current_Yaw;

    float Current_dPitch;
    float Current_dRoll;
    float Current_dYaw;

    float Current_Path_X[2]; //If On a Conveyor Belt, this changes to the belt's x, y and z coordinates.
    float Current_Path_Y[2];
    float Current_Path_Z[2];

    int Current_Health;

    int Explode_Upon_Defeat;
    int Smoulder_Upon_Defeat;
    int DIP_Switch_ID;
};

struct _On_Foot_Endpoint
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;

    int This_Arc;
    int This_Level;
    int Clear_Status;
    int Level_Routes_Enabled[3];
    int DIP_Switch_ID;
};

    //

    //Room Controls
struct _On_Foot_Door
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;
    int DIP_Switch_ID;
};

struct _On_Foot_Conveyor_Belt
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int In_Motion;
    int Speed;
    int DIP_Switch_ID;
};

struct _On_Foot_Ladder
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2]; //This is so it can be an hangable mesh.
    float z[2];
    int Visible;
    int Ladder_Type;
    int Invincible_While_Climbing_Ladder;
    int DIP_Switch_ID;
};

struct _On_Foot_Switch
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;

    int State; //8 states.
    int DIP_Switch_ID;
};

struct _On_Foot_Dial
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;

    int State; //8 states.
    int DIP_Switch_ID;
};

struct _On_Foot_Pickup_Item
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;

    int Category;
    int Subcategory;
    int ID;
    int Variant;
    int DIP_Switch_ID;
};

struct _On_Foot_Fixed_Camera_Box
{
    float x[2];
    float y[2];
    float z[2];

    float Camera_x;
    float Camera_y;
    float Camera_z;

    int Remap_Controls;
    int Remap_Direction;
    int DIP_Switch_ID;
};

struct _On_Foot_Conversation
{
    char Filepath[256];
};

    //Dialogue controls
struct _On_Foot_Conversation_File_Header
{
    int Number_Of_Conversation_Nodes;
    int Number_Of_Conversation_Connections;

    int Offset_To_Conversation_Nodes;
    int Offset_To_Conversation_Connections;
};

struct _On_Foot_Conversation_Node
{
    int Conversation_Type;
    int Conversation_GUI_Type;

    int Node_DIP_Switch;

    char Conversation_Line_Text[256];
    char Conversation_Line_Voice[256];

};

struct _On_Foot_Conversation_Connection
{
    int Conversation_Nodes[2];

    char Option_One_Description[256];
    char Option_Two_Description[256];
    char Option_Three_Description[256];
    char Option_Four_Description[256];
    char Option_Five_Description[256];

    int Number_Of_Options;
};

struct _On_Foot_Shop
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;

    char Shopkeeper_Filepath[256];
    char Shopkeeper_Texture_Filepath[256];

    char Inventory_Filepath[256];

    char Shopkeeper_Voice_Line_Filepath[256];
    int DIP_Switch_ID;
};

struct _On_Foot_Shop_Inventory
{

};

struct _On_Foot_Stage_Changing_Door
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;
    int Go_To_This_Region;
    int Go_To_This_Spawnpoint;
    int DIP_Switch_ID;
};

struct _On_Foot_Vehicle_Entry_Point
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;
    int DIP_Switch_ID;
};

struct _On_Foot_Elevator
{
    char Filepath[256];
    char Texture_Filepath[256];
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;

    int Elevator_Is_Active;

    int Floor_Selection_GUI;
    int Number_Of_Floors;
    int Current_Floor;
    int DIP_Switch_ID;
};

struct _On_Foot_Light
{
    float x[2];
    float y[2];
    float z[2];

    int Active;

    float R;
    float G;
    float B;
    int DIP_Switch_ID;
};


struct _On_Foot_Computer
{

};

struct _On_Foot_Vehicle
{

};

struct _On_Foot_Computer_Storage_Device
{

};

struct _On_Foot_Broadcast_Tower
{

};

struct _On_Foot_Volcano
{

};

struct _On_Foot_Sonic_Black_Hole
{

};

struct _On_Foot_Rainstorm
{

};

struct _On_Foot_Duststorm
{

};

struct _On_Foot_Snowstorm
{

};

struct _On_Foot_Hailstorm
{

};

struct _On_Foot_Earthquake
{

};

struct _On_Foot_Tornado
{

};

struct _On_Foot_Hurricane
{

};

struct _On_Foot_Aurora
{

};

struct _On_Foot_Docking_Station
{

};

struct _On_Foot_Ship
{

};

struct _On_Foot_NPC_Plant
{

};

struct _On_Foot_NPC_Swimming_Creature
{

};

struct _On_Foot_NPC_Terrestrial_Creature
{

};

struct _On_Foot_NPC_Flying_Creature
{

};

struct _On_Foot_NPC_Fungi
{

};

struct _On_Foot_NPC_Microbes
{

};

struct _On_Foot_Pool_Of_Liquid
{

};

struct _On_Foot_Mirror
{

};

struct _C_SC_ID_V_Object
{
    int Category;
    int Subcategory;
    int ID;
    int Variant;
};


struct _On_Foot_Background_Object
{
    struct _MD2_Model* Model;
    int Visible;
    int DIP_Switch_ID;
    float Rotational_Velocity;
    struct _Preloaded_Collision_Mesh* Mesh;
    int Phase_Through;
    int Is_Floor_Ceiling_Or_Wall;
    //int SFX_ID;
};

struct _On_Foot_Actor_Object
{
    struct _MD2_Model* Model[5];

    int Visible;

    int Current_Frame;

    int Number_Of_Model_Filepaths_Used;

    int Actor_Is_Defeatable;

    int Current_Model;
    int Current_Keyframe_Range[2];
    float Current_Animation_Speed;

    int Collider_Type;

    char Mesh_Collider_Filepath[256];
    int Uses_Navigation_Mesh;
    char Navigation_Mesh_Filepath[256];

    float AABB_Box_Dimensions[6];

    AABB Collision_Geometry;

    float dx;
    float dy;
    float dz;

    float Head_Height;

    float Current_Throttle;

    int Already_Spawned;
    int Preserve_Between_Regions; //Do not overwrite me upon region change.

    float Spawn_Pitch;
    float Spawn_Roll;
    float Spawn_Yaw;

    float Current_Pitch;
    float Current_Roll;
    float Current_Yaw;

    float Current_dPitch;
    float Current_dRoll;
    float Current_dYaw;

    float Current_Path_X[2]; //Navigation Mesh
    float Current_Path_Y[2];
    float Current_Path_Z[2];

    int Current_Health;
    int Current_Energy;

    int Current_Mode;

    int Current_Loadout_Item;
    char Loadout_Filepath[256];
    char Dropped_Item_Filepath[256]; //After defeat.

    int Number_Of_Conversations;

    int Current_Conversation;
    char Conversation_Filepath[256];
    char Conversation_Voice_Filepath[256];

    char Conversation_Two_Filepath[256];
    char Conversation_Two_Voice_Filepath[256];

    char Conversation_Three_Filepath[256];
    char Conversation_Three_Voice_Filepath[256];

    char Conversation_Four_Filepath[256];
    char Conversation_Four_Voice_Filepath[256];

    char Conversation_Five_Filepath[256];
    char Conversation_Five_Voice_Filepath[256];

    float Visual_Range;
    float Audio_Range;

    int Explode_Upon_Defeat;
    int Corpse_Type; //Gibs or body
    int Smoulder_Upon_Defeat;

    char Corpse_Filepath[256];
    char Corpse_Texture_Filepath[256];


    int On_Spawn_DIP_Switch_ID;
    char On_Spawn_Line_Filepath[256];
    char On_Spawn_Line_Two_Filepath[256];


    int Idle_DIP_Switch_ID;
    char Idle_Line_Filepath[256];
    char Idle_Line_Two_Filepath[256];

    int Walking_DIP_Switch_ID;
    char Walking_Line_Filepath[256];
    char Walking_Line_Two_Filepath[256];

    int Running_DIP_Switch_ID;
    char Running_Line_Filepath[256];
    char Running_Line_Two_Filepath[256];

    int Searching_DIP_Switch_ID;
    char Searching_Line_Filepath[256];
    char Searching_Line_Two_Filepath[256];

    int Spotted_Player_DIP_Switch_ID;
    char Spotted_Player_Line_Filepath[256];
    char Spotted_Player_Line_Two_Filepath[256];

    int Attacking_Player_DIP_Switch_ID;
    char Attacking_Player_Line_Filepath[256];
    char Attacking_Player_Line_Two_Filepath[256];

    int Defeated_Player_DIP_Switch_ID;
    char Defeated_Player_Line_Filepath[256];
    char Defeated_Player_Line_Two_Filepath[256];

    int Injured_Player_DIP_Switch_ID;
    char Injured_Player_Line_Filepath[256];
    char Injured_Player_Line_Two_Filepath[256];





    int Player_Walking_Towards_DIP_Switch_ID;
    char Player_Walking_Towards_Line_Filepath[256];
    char Player_Walking_Towards_Line_Two_Filepath[256];

    int Chased_By_Player_DIP_Switch_ID;
    char Chased_By_Player_Line_Filepath[256];
    char Chased_By_Player_Line_Two_Filepath[256];

    int Searched_By_Player_DIP_Switch_ID;
    char Searched_By_Player_Line_Filepath[256];
    char Searched_By_Player_Line_Two_Filepath[256];

    int Spotted_By_Player_DIP_Switch_ID;
    char Spotted_By_Player_Line_Filepath[256];
    char Spotted_By_Player_Line_Two_Filepath[256];

    int Attacked_By_Player_DIP_Switch_ID;
    char Attacked_By_Player_Line_Filepath[256];
    char Attacked_By_Player_Line_Two_Filepath[256];

    int Injured_By_Player_DIP_Switch_ID;
    char Injured_By_Player_Line_Filepath[256];
    char Injured_By_Player_Line_Two_Filepath[256];

    int Defeated_By_Player_DIP_Switch_ID;
    char Defeated_By_Player_Line_Filepath[256];
    char Defeated_By_Player_Line_Two_Filepath[256];









    int Idle_With_Player_Ally_DIP_Switch_ID;
    char Idle_With_Player_Ally_Line_Filepath[256];
    char Idle_With_Player_Ally_Line_Two_Filepath[256];

    int Walking_With_Player_Ally_DIP_Switch_ID;
    char Walking_With_Player_Ally_Line_Filepath[256];
    char Walking_With_Player_Ally_Line_Two_Filepath[256];

    int Running_With_Player_Ally_DIP_Switch_ID;
    char Running_With_Player_Ally_Line_Filepath[256];
    char Running_With_Player_Ally_Line_Two_Filepath[256];

    int Searching_With_Player_Ally_DIP_Switch_ID;
    char Searching_With_Player_Ally_Line_Filepath[256];
    char Searching_With_Player_Ally_Line_Two_Filepath[256];

    int Spotted_Enemy_With_Player_Ally_DIP_Switch_ID;
    char Spotted_Enemy_With_Player_Ally_Line_Filepath[256];
    char Spotted_Enemy_With_Player_Ally_Line_Two_Filepath[256];

    int Attacking_Enemy_With_Player_Ally_DIP_Switch_ID;
    char Attacking_Enemy_With_Player_Ally_Line_Filepath[256];
    char Attacking_Enemy_With_Player_Ally_Line_Two_Filepath[256];

    int Defeated_Enemy_With_Player_Ally_DIP_Switch_ID;
    char Defeated_Enemy_With_Player_Ally_Line_Filepath[256];
    char Defeated_Enemy_With_Player_Ally_Line_Two_Filepath[256];

    int Injured_Enemy_With_Player_Ally_DIP_Switch_ID;
    char Injured_Enemy_With_Player_Ally_Line_Filepath[256];
    char Injured_Enemy_With_Player_Ally_Line_Two_Filepath[256];

    int Defeated_With_Player_Ally_DIP_Switch_ID;
    char Defeated_With_Player_Ally_Line_Filepath[256];
    char Defeated_With_Player_Ally_Line_Two_Filepath[256];

    int Injured_With_Player_Ally_DIP_Switch_ID;
    char Injured_With_Player_Ally_Line_Filepath[256];
    char Injured_With_Player_Ally_Line_Two_Filepath[256];

    int DIP_Switch_ID;

    float x;
    float y;
    float z;

    bool Show_Talk_Arrow;
};

struct _On_Foot_Track_Object
{
    char Filepath[256];
    int Playing;
    int DIP_Switch_ID;
    float Volume;
    int Loop;
};

struct _On_Foot_SFX_Data_Object
{
    char Filepath[256];
    int Playing;
    int DIP_Switch_ID;
    float Volume;
    int Loop;
    Mix_Chunk* SFX_Data;
};

struct _On_Foot_Spawnpoint_Object
{
    float x;
    float y;
    float z;
    int DIP_Switch_ID;
    int Current_Spawnpoint;
};

struct _On_Foot_Prop_Object
{
    struct _MD2_Model* Model;
    struct _MD2_Model* Damaged_Model;
    struct _MD2_Model* Destroyed_Model;

    AABB Collision_Geometry;

    int Visible;
    int Prop_Is_Destroyable;

    int Current_Keyframe_Range[2];
    float Current_Animation_Speed;

    int Collider_Type;
    float Radius;
    char Mesh_Collider_Filepath[256];
    float AABB_Box_Dimensions[6];
    float Head_Height;

    float Spawn_X;
    float Spawn_Y;
    float Spawn_Z;

    float Current_X;
    float Current_Y;
    float Current_Z;

    float Current_Throttle;

    int Already_Spawned;
    int Preserve_Between_Regions; //Do not overwrite me upon region change.

    float Spawn_Pitch;
    float Spawn_Roll;
    float Spawn_Yaw;

    float Current_Pitch;
    float Current_Roll;
    float Current_Yaw;

    float Current_dPitch;
    float Current_dRoll;
    float Current_dYaw;

    float Current_Path_X[2]; //If On a Conveyor Belt, this changes to the belt's x, y and z coordinates.
    float Current_Path_Y[2];
    float Current_Path_Z[2];

    int Current_Health;

    int Explode_Upon_Defeat;
    int Smoulder_Upon_Defeat;
    int DIP_Switch_ID;
};

struct _On_Foot_Endpoint_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;

    int This_Arc;
    int This_Level;
    int Clear_Status;
    int Level_Routes_Enabled[3];
    int DIP_Switch_ID;
};

    //

    //Room Controls
struct _On_Foot_Door_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;
    int DIP_Switch_ID;

    AABB Collision_Geometry;
};

struct _On_Foot_Conveyor_Belt_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int In_Motion;
    int Speed;
    int DIP_Switch_ID;
    AABB Collision_Geometry;
};

struct _On_Foot_Ladder_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2]; //This is so it can be an hangable mesh.
    float z[2];
    int Visible;
    int Ladder_Type;
    int Invincible_While_Climbing_Ladder;
    int DIP_Switch_ID;
    AABB Collision_Geometry;
};

struct _On_Foot_Switch_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;

    int State; //8 states.
    int DIP_Switch_ID;
};

struct _On_Foot_Dial_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;

    int State; //8 states.
    int DIP_Switch_ID;
};

struct _On_Foot_Pickup_Item_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;

    int Category;
    int Subcategory;
    int ID;
    int Variant;
    int DIP_Switch_ID;
};

struct _On_Foot_Fixed_Camera_Box_Object
{
    float x[2];
    float y[2];
    float z[2];

    float Camera_x;
    float Camera_y;
    float Camera_z;

    int Remap_Controls;
    int Remap_Direction;
    int DIP_Switch_ID;
};

    //

    //Dialogue controls
struct _On_Foot_Conversation_Object
{
    char Conversation_Name[256];
    int Conversation_Type;
    int Conversation_GUI_Type;
    int Number_Of_Conversation_Nodes;
    int Number_Of_Conversation_Connections;
};

struct _On_Foot_Conversation_Node_Object
{
    int Node_DIP_Switch;

    char Conversation_Line_Text[256];
    char Conversation_Line_Voice[256];

    float Line_Length_In_Seconds;
    int Cut_Off_Line;

};

struct _On_Foot_Conversation_Connection_Object
{
    int Conversation_Nodes[2];

    int Use_Options_List;

    char Option_One_Description[256];
    char Option_Two_Description[256];
    char Option_Three_Description[256];
    char Option_Four_Description[256];
    char Option_Five_Description[256];

    int Number_Of_Options;
};

struct _On_Foot_Shop_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;

    struct _MD2_Model* Shopkeeper_Model;

    char Inventory_Filepath[256];

    char Shopkeeper_Voice_Line_Filepath[256];
    int DIP_Switch_ID;
};

struct _On_Foot_Shop_Inventory_Object
{
    struct _C_SC_ID_V_Object Objects[256];
};

struct _On_Foot_Stage_Changing_Door_Object
{
    struct _MD2_Model* Model;
    float x;
    float y;
    float z;
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;
    int Go_To_This_Region;
    int Go_To_This_Spawnpoint;
    int DIP_Switch_ID;

    AABB Collision_Geometry;
    bool Show_Open_Arrow;
};

struct _On_Foot_Vehicle_Entry_Point_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;
    int DIP_Switch_ID;
};

struct _On_Foot_Elevator_Object
{
    struct _MD2_Model* Model;
    float x[2];
    float y[2];
    float z[2];
    int Visible;
    int Measured_From_Center;
    float Radius;
    int Type;
    int Current_Keyframe;
    int Locked;
    int Key_Category;
    int Key_Subcategory;
    int Key_ID;

    int Elevator_Is_Active;

    int Floor_Selection_GUI;
    int Number_Of_Floors;
    int Current_Floor;
    int DIP_Switch_ID;

    struct _Preloaded_Collision_Mesh* Mesh;
};

struct _On_Foot_Light_Object
{
    float x;
    float y;
    float z;

    int Active;

    float R;
    float G;
    float B;
    int DIP_Switch_ID;

    AABB Collision_Geometry;
};

struct _Underwater_Explosion
{
    bool Visible;
    bool Active;
    float Explosion_Pressure_Wave_Radius;
    float Explosion_Fireball_Radius;
    float Maximum_Explosion_Pressure_Wave_Radius;
    float Maximum_Explosion_Fireball_Radius;
    int Explosion_Frame;

    float x;
    float y;
    float z;
};


struct _On_Foot_Computer_Object
{
    struct _Computer* Computer;
    struct _MD2_Model* Computer_Model;
    float Screen_Vertex_One_x;
    float Screen_Vertex_One_y;
    float Screen_Vertex_One_z;

    float Screen_Vertex_Two_x;
    float Screen_Vertex_Two_y;
    float Screen_Vertex_Two_z;

    float Screen_Vertex_Three_x;
    float Screen_Vertex_Three_y;
    float Screen_Vertex_Three_z;

    float Screen_Vertex_Four_x;
    float Screen_Vertex_Four_y;
    float Screen_Vertex_Four_z;

    AABB Computer_Collision_Mesh;
};



struct _On_Foot_Computer_Storage_Device_Object
{
    struct _MD2_Model* Storage_Device_Model;
    unsigned char Load_This_File[256];
    AABB Collision_Mesh;
    float x;
    float y;
    float z;

    float Roll;
    float Yaw;
    float Pitch;
};



struct _On_Foot_State
{

    GLuint fbo, textureColorBuffer, rbo;
    //GLfloat light_direction[4];
//    struct _Rigid_Body_Engine Rigid_Body_Engine;
    unsigned char Current_Location;

    struct _Circular_Shadow* Universal_Circular_Shadow;

    struct _MD2_Model* Rigid_Body_Test_Block;

    struct _MD2_Model* Model;
    struct _Player_Submarine* On_Foot_Player;
    struct _New_Player_Submarine* On_Foot_Submarine_Player;
    struct _New_Player_Car* On_Foot_Car_Player;

    struct _Image* Talk_Arrow;
    struct _Image* Open_Arrow;
    struct _Image* Paused_Background;

    struct _Image* Resume;
    struct _Image* Exit_To_Level_Select;
    struct _Image* Exit_To_Title;

    struct _On_Foot_Computer_Object* On_Foot_Computer_Object;

    Mix_Chunk* Track = NULL;
    Mix_Chunk* SFX_Data = NULL;

    struct _Conversation_Mini_State* Conversation_Mini_State;

    struct _Preloaded_Collision_Mesh* Mesh;


    unsigned char Number_Of_Torpedoes;
    unsigned char Current_Torpedo;
    struct _Torpedo* On_Foot_Torpedoes[256];

    unsigned char Number_Of_Underwater_Explosions;
    unsigned char Current_Underwater_Explosion;
    struct _Underwater_Explosion* On_Foot_Underwater_Explosions[256];
    struct _TwoD_Sprite_Object* Underwater_Explosion_Billboard;

    struct _On_Foot_Background_Object* On_Foot_Background_Object; //268
    struct _On_Foot_Actor_Object* On_Foot_Actor_Object; //268
    struct _On_Foot_Track_Object* On_Foot_Track_Object; //268
    struct _On_Foot_SFX_Data_Object* On_Foot_SFX_Data_Object; //268
    struct _On_Foot_Spawnpoint_Object* On_Foot_Spawnpoint_Object; //268
    struct _On_Foot_Prop_Object* On_Foot_Prop_Object; //268
    struct _On_Foot_Endpoint_Object* On_Foot_Endpoint_Object; //268
    //

    //Room Controls
    struct _On_Foot_Door_Object* On_Foot_Door_Object; //268
    struct _On_Foot_Conveyor_Belt_Object* On_Foot_Conveyor_Belt_Object; //268
    struct _On_Foot_Ladder_Object* On_Foot_Ladder_Object; //268
    struct _On_Foot_Switch_Object* On_Foot_Switch_Object; //268
    struct _On_Foot_Dial_Object* On_Foot_Dial_Object; //268
    struct _On_Foot_Pickup_Item_Object* On_Foot_Pickup_Item_Object; //268
    struct _On_Foot_Fixed_Camera_Box_Object* On_Foot_Fixed_Camera_Box_Object; //268
    //

    //Dialogue controls
    struct _On_Foot_Conversation_Object* On_Foot_Conversation_Object; //268
    struct _On_Foot_Conversation_Node_Object* On_Foot_Conversation_Node_Object; //268
    struct _On_Foot_Conversation_Connection_Object* On_Foot_Conversation_Connection_Object; //268
    struct _On_Foot_Shop_Object* On_Foot_Shop_Object; //268
    struct _On_Foot_Stage_Changing_Door_Object* On_Foot_Stage_Changing_Door_Object; //268
    struct _On_Foot_Vehicle_Entry_Point_Object* On_Foot_Vehicle_Entry_Point_Object; //268
    struct _On_Foot_Elevator_Object* On_Foot_Elevator_Object; //268
    struct _On_Foot_Light_Object* On_Foot_Light_Object; //268

    struct _Timer* Timer;
    struct _Timer* Death_Timer; //Triggered to reload stage.

    struct _Broadcaster* Broadcaster;
    struct _Image* TV;

    float Red;
    float Green;
    float Blue;
    float Camera_Z;
    float Camera_Y;

    bool Paused = false;
    bool Debug = true;
    bool Enter_Pressed = false;
    bool Is_In_Submarine = true;
    int Laps = 1;
    int Current_Pause_Menu_Choice = 0;
    struct _On_Foot_Level_Header Level;

    struct _Image* Explosion_Placeholder;
};




struct _On_Foot_State* Create_On_Foot_State();
void Destroy_On_Foot_State(struct _Engine* Engine);

void Initialize_On_Foot_State(struct _Engine* Engine);

void Render_On_Foot_State(struct _Engine* Engine);
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_On_Foot_State(struct _Engine* Engine);

void Trigger_On_Foot_Death_Protocol_Injury(struct _Engine* Engine);
void Trigger_On_Foot_Death_Protocol_Objective_Failed(struct _Engine* Engine);
void Trigger_On_Foot_Death_Protocol_Bottomless_Pit(struct _Engine* Engine); //Freeze the Camera while player falls down.


void Write_CSS_Arc_List_Entry(FILE* f);
void Create_CSS_Arc_List(const char* Filepath);

void Create_CSS_Arc_Level_Container(const char* Filepath);
void Create_CSS_Arc_Level_Header(const char* Filepath);



void Create_CSS_On_Foot_Level_Header(const char* Filepath);

void Write_CSS_On_Foot_File_Header(const char* Filepath);


 void Write_On_Foot_Backgrounds(FILE* f); //268
    void Write_On_Foot_Actors(FILE* f); //272
    void Write_On_Foot_Tracks(FILE* f); //276
    void Write_On_Foot_SFX_Data(FILE* f); //280
    void Write_On_Foot_Spawnpoints(FILE* f);
    void Write_On_Foot_Props(FILE* f);
    void Write_On_Foot_Endpoints(FILE* f); //Enter these to end the level!
    //

    //Room Controls
    void Write_On_Foot_Doors(FILE* f);
    void Write_On_Foot_Conveyor_Belts(FILE* f);
    void Write_On_Foot_Ladders(FILE* f);
    void Write_On_Foot_Switches(FILE* f);
    void Write_On_Foot_Dials(FILE* f);
    void Write_On_Foot_Pickup_Items(FILE* f);
    void Write_On_Foot_Fixed_Camera_Boxes(FILE* f); //Switch from third-person view to a security camera view in certain rooms.
    //

    //Dialogue controls
    void Write_On_Foot_Conversations(FILE* f);
    void Write_On_Foot_Conversation_Nodes(FILE* f);
    void Write_On_Foot_Conversation_Connections(FILE* f);
    void Write_On_Foot_Shops(FILE* f);
    void Write_On_Foot_Stage_Changing_Doors(FILE* f); //These do not end the level! Only endpoints do so!
    void Write_On_Foot_Vehicle_Entry_Points(FILE* f);
    void Write_On_Foot_Elevators(FILE* f);
    void Write_On_Foot_Lights(FILE* f);

    void Write_On_Foot_Computers(FILE* f);
    void Write_On_Foot_Vehicles(FILE* f);



void Load_CSS_Arc_List_Entry(FILE* f);
void Load_CSS_Arc_List(struct _Engine* Engine, const char* Filepath);

void Load_CSS_Arc_Level_Container(struct _Engine* Engine, const char* Filename);
void Load_CSS_Arc_Level_Header(struct _Engine* Engine, const char* Filename);



void Load_CSS_On_Foot_Level_Header(struct _Engine* Engine, const char* Filename);
void Load_CSS_On_Foot_File_Header(struct _Engine* Engine,const char* Filename);
void Load_CSS_On_Foot_Backgrounds(struct _Engine* Engine);
void Load_CSS_On_Foot_Actors(struct _Engine* Engine);
void Load_CSS_On_Foot_Tracks(struct _Engine* Engine);
void Load_CSS_On_Foot_SFX_Data(struct _Engine* Engine);
void Load_CSS_On_Foot_Spawnpoints(struct _Engine* Engine);
void Load_CSS_On_Foot_Endpoints(struct _Engine* Engine);
void Load_CSS_On_Foot_Doors(struct _Engine* Engine);
void Load_CSS_On_Foot_Conveyor_Belts(struct _Engine* Engine);
void Load_CSS_On_Foot_Ladders(struct _Engine* Engine);
void Load_CSS_On_Foot_Switches(struct _Engine* Engine);
void Load_CSS_On_Foot_Pickup_Items(struct _Engine* Engine);
void Load_CSS_On_Foot_Fixed_Camera_Boxes(struct _Engine* Engine);
void Load_CSS_On_Foot_Conversations(struct _Engine* Engine);
void Load_CSS_On_Foot_Shops(struct _Engine* Engine);
void Load_CSS_On_Foot_Stage_Changing_Doors(struct _Engine* Engine);
void Load_CSS_On_Foot_Vehicle_Entry_Points(struct _Engine* Engine);
void Load_CSS_Conversations(struct _Engine* Engine);
void Load_CSS_Conversation_Nodes(struct _Engine* Engine);
void Load_CSS_Conversation_Connections(struct _Engine* Engine);


void Write_Conversation(const char* Filename);
void Load_Conversation(struct _Engine* Engine, const char* Filename);
void Start_Conversation_Mini_State(struct _Engine* Engine);
void End_Conversation_Mini_State(struct _Engine* Engine);


struct _CURRENT_CSS_On_Foot_Level_Header
{
    char Level_Name[256];
    char Level_Description[256];

    int Current_Segment; //Teaser, intro, act one, etc...

    int Teaser_Exists;
    int Play_Series_Intro;
    int Act_One_Exists;
    int Act_Two_Exists;
    int Act_Three_Exists;
    int Act_Four_Exists;
    int Act_Five_Exists;
    int Epilogue_Exists;

    int Possible_To_Fail_Level; //If this is not possible...

    char Teaser_Cutscene_Filepath[256];
    char Series_Intro_Cutscene_Filepath[256];
    char Act_One_Cutscene_Filepath[256];
    char Act_Two_Cutscene_Filepath[256];
    char Act_Three_Cutscene_Filepath[256];
    char Act_Four_Cutscene_Filepath[256];
    char Act_Five_Cutscene_Filepath[256];
    char Epilogue_Cutscene_Filepath[256];

    int Post_Teaser_Action; //Go to Region, End, or go to Next Cutscene.
    int Region_To_Load_After_Teaser;

    int Post_Series_Intro;
    int Region_To_Load_After_Intro;

    int Post_Act_One_Exists;
    int Region_To_Load_After_Act_One;

    int Post_Act_Two_Exists;
    int Region_To_Load_After_Act_Two;

    int Post_Act_Three_Exists;
    int Region_To_Load_After_Act_Three;

    int Post_Act_Four_Exists;
    int Region_To_Load_After_Act_Four;

    int Post_Act_Five_Exists;
    int Region_To_Load_After_Act_Five;

    int Post_Epilogue_Exists;
    int Region_To_Load_After_Epilogue;



    char Failed_Level_Lines_Filepath[256]; //Characters can say things if you fail the level.
    char Failed_Level_Voice_Lines_Filepath[256]; //Characters can say things if you fail the level.

    int Forced_To_Be_A_Character;
    int Character_Category;
    int Character_ID;

    int Current_Level_Exit_Type;

    int Hard_Level_Exit_Win_Condition; //1 = Reach Endpoint. 2 = Collect Items. 3 = Destroy TGTs. 4 = Survive for Timer. 5 = Reach Certain Score.

    int Hard_Level_Exit_Category;
    int Hard_Level_Exit_Subcategory;
    int Hard_Level_Exit_ID;
    int Hard_Level_Exit_Variant;
    int Hard_Level_Exit_Amount;



    int Medium_Level_Exit_Win_Condition; //1 = Reach Endpoint. 2 = Collect Items. 3 = Destroy TGTs. 4 = Survive for Timer. 5 = Reach Certain Score.

    int Medium_Level_Exit_Category;
    int Medium_Level_Exit_Subcategory;
    int Medium_Level_Exit_ID;
    int Medium_Level_Exit_Variant;
    int Medium_Level_Exit_Amount;




    int Easy_Level_Exit_Win_Condition; //1 = Reach Endpoint. 2 = Collect Items. 3 = Destroy TGTs. 4 = Survive for Timer. 5 = Reach Certain Score.

    int Easy_Level_Exit_Category;
    int Easy_Level_Exit_Subcategory;
    int Easy_Level_Exit_ID;
    int Easy_Level_Exit_Variant;
    int Easy_Level_Exit_Amount;



    int Global_DIP_Switch[256];

    int Number_Of_Regions_In_This_Level;
    int Offset_To_Regions;

    int Number_Of_Default_Spawnpoints;
    int Offset_To_Default_Spawnpoints;
};

struct _Conversation_Step
{
    char Voice_Line_Filepath[256];
    char Voice_Line[256];
    int Voice_Line_Length;
    int Step_Type;

    int Step_ID;
    int Next_Step;

    float Start_Time;
    float End_Time;
    int Complete;



    char Selection_Menu_Option_One[256];
    char Selection_Menu_Option_Two[256];
    char Selection_Menu_Option_Three[256];
    char Selection_Menu_Option_Four[256];
    char Selection_Menu_Option_Five[256];

    int Selection_One_DIP_Switch;
    int Selection_Two_DIP_Switch;
    int Selection_Three_DIP_Switch;
    int Selection_Four_DIP_Switch;
    int Selection_Five_DIP_Switch;
};

struct _Conversation_Step_Object
{
    struct _Image* Voice_Line_Caption;
    Mix_Chunk* Voice_Line;
    int Voice_Line_Played;
    int Voice_Line_Length;
    int Step_Type;

    int Step_ID;
    int Next_Step;

    float Start_Time;
    float End_Time;
    int Complete;

    struct _Image* Selection_Menu_Caption[5];

    int Selection_One_DIP_Switch;
    int Selection_Two_DIP_Switch;
    int Selection_Three_DIP_Switch;
    int Selection_Four_DIP_Switch;
    int Selection_Five_DIP_Switch;
};

struct _Conversation_Header
{
    char Conversation_Name[256];
    int Conversation_Type;
    int Conversation_Complete;
    float Maximum_Conversation_Duration;

    int Number_Of_Steps;
    int Offset_To_Steps;
};

struct _Conversation_Mini_State
{
    bool In_Conversation;
    int Current_Step;
    struct _Conversation_Header Header;
    struct _Timer* Timer;
    struct _Conversation_Step_Object* Steps;
};

void Write_Conversation_File(const char* Filename);
void Load_Conversation_File(struct _Engine* Engine, const char* Filename);

void Start_Conversation(struct _Engine* Engine);
void End_Conversation(struct _Engine* Engine);


void Process_Conversation(struct _Engine* Engine);
void Render_Conversation(struct _Engine* Engine);
void Handle_Conversation_Inputs(struct _Engine* Engine, struct _Keypad Keypad);

float Face_Object(float x1, float x2, float y1, float y2);

void calculateLookAtRotation(float x1, float y1, float z1, float x2, float y2, float z2, float *angle_out, float axis_out[3]);

#define INSTRUCTION_GO_TO_REGION 1
#define INSTRUCTION_GO_TO_CUTSCENE 2
#define INSTRUCTION_END_LEVEL 3
#define INSTRUCTION_THE_END 4
#define INSTRUCTION_SET_DIP_SWITCHES 5

struct _Interim_State
{
    int Current_Instruction;
    int Current_Instruction_Parameter;
    char Text_Instruction[256];

    int Next_Instruction;
    int Next_Instruction_Parameter;
    char Next_Text_Instruction[256];
};

struct _Interim_State* Create_Interim_State();
void Destroy_Interim_State(struct _Engine* Engine);
void Initialize_Interim_State(struct _Engine* Engine);
void Process_Interim_State(struct _Engine* Engine);

void Add_Underwater_Explosion(struct _Engine* Engine);
void Process_Underwater_Explosions(struct _Engine* Engine);

void Set_Underwater_Explosion_Parameters(struct _Engine* Engine, unsigned char ID,bool Visible,bool Active,float Explosion_Pressure_Wave_Radius,float Explosion_Fireball_Radius,float Maximum_Explosion_Pressure_Wave_Radius,float Maximum_Explosion_Fireball_Radius,int Explosion_Frame,float x,float y,float z);

void Render_Underwater_Explosion(struct _Engine* Engine, unsigned char ID);

#endif /* CUTSCENE_H */
//NEED A SEPARAT FUNCTION TO INCREMENT EACH FRAME!

/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ARC_SELECT_H_INCLUDED
#define ARC_SELECT_H_INCLUDED
#include "../../engine/engine.h"
#include "../../engine/timer/timer.h"
#include "../on_foot/on_foot.h"

#define MISSION_OBJECTIVE_REACH_GOALPOST 1
#define MISSION_OBJECTIVE_HIGH_SCORE 2
#define MISSION_OBJECTIVE_DESTROY_TARGETS 3
#define MISSION_OBJECTIVE_SURVIVE_TIME_LIMIT 4
#define MISSION_OBJECTIVE_PROTECT_ITEMS 5
#define MISSION_OBJECTIVE_COLLECT_ITEMS 6
#define MISSION_OBJECTIVE_PROTECT_SHIP 7

#define DEBRIEFING_TYPE_NONE 1
#define DEBRIEFING_TYPE_FLIGHT_SCREEN 2
#define DEBRIEFING_TYPE_SCORE_TALLY 3

#define STEP_TYPE_LOAD_STAGE 1

struct _Main_Arc_List //This is what Arc Select state loads.
{
    int Number_Of_Arcs;
};

struct _Arc_List_Arc_Entry
{
    char Arc_Filepath[256]; //resources/data/arcs + This.
};

struct _Level_List
{
    int Number_Of_Levels;
};

struct _Level_List_Level
{
    int Hard_Level_Exists;
    int Medium_Level_Exists;
    int Easy_Level_Exists;

    char Hard_Level_Icon_Filepath[256];
    char Medium_Level_Icon_Filepath[256];
    char Easy_Level_Icon_Filepath[256];

    char Hard_Level_Description_Filepath[256];
    char Medium_Level_Description_Filepath[256];
    char Easy_Level_Description_Filepath[256];

    char Hard_Level_Container_Filepath[256];
    char Medium_Level_Container_Filepath[256];
    char Easy_Level_Container_Filepath[256];

    int Hard_Level_Exit_Status;
    int Medium_Level_Exit_Status;
    int Easy_Level_Exit_Status;

    int Hard_Level_ID;
    int Medium_Level_ID;
    int Easy_Level_ID;
};

struct _Level_Container_Step
{
    unsigned int Step_ID;
    int Step_Type; //Load Cutscene, Load Region, Go to Other Step, Go back to Overworld, Go to The End, Go to Title, Offer to Save Progress.
    int Skip_This_Step;
    int Spawnpoint_ID;
    char Character_Filepath[256];
    char Filepath[256];
};

struct _Mission_Line_Filepath
{
    char Filepath[256];
};

struct _Level_List_Level_Container
{
    char Level_Name[256];
    char Level_Description[256];

    int Mission_Accomplished_Objective_Type;
    int Mission_Accomplished_Category;
    int Mission_Accomplished_Subcategory;
    int Mission_Accomplished_ID;
    int Mission_Accomplished_Variant;
    float Mission_Accomplished_Value;
    float Mission_Accomplished_Current_Value;

    int Mission_Complete_Objective_Type;
    int Mission_Complete_Category;
    int Mission_Complete_Subcategory;
    int Mission_Complete_ID;
    int Mission_Complete_Variant;
    float Mission_Complete_Value;
    float Mission_Complete_Current_Value;

    int Debriefing_Type;

    int Number_Of_Steps;
    int Offset_To_Steps;

    int Select_Random_Mission_Accomplished_Line; //Or play in sequence.
    int Select_Random_Mission_Complete_Line; //Or play in sequence.
    int Select_Random_Mission_Failed_Line; //Or play in sequence.

    int Number_Of_Mission_Accomplished_Voice_Line_Filepaths;
    int Number_Of_Mission_Accomplished_Line_Filepaths;
    int Number_Of_Mission_Complete_Voice_Line_Filepaths;
    int Number_Of_Mission_Complete_Line_Filepaths;
    int Number_Of_Mission_Failed_Voice_Line_Filepaths;
    int Number_Of_Mission_Failed_Line_Filepaths;

    int Offset_To_Mission_Accomplished_Voice_Line_Filepaths;
    int Offset_To_Mission_Accomplished_Line_Filepaths;
    int Offset_To_Mission_Complete_Voice_Line_Filepaths;
    int Offset_To_Mission_Complete_Line_Filepaths;
    int Offset_To_Mission_Failed_Voice_Line_Filepaths;
    int Offset_To_Mission_Failed_Line_Filepaths;
};

struct _Level_Region_Filepath
{
    char Filepath[256];
    int Default_Spawnpoint;
    int Default_Character_Category;
    int Default_Character_ID;
};

struct _Arc_Object
{
    struct _Image* Arc_Banner;
    char Filepath[256];
};


struct _Arc_Select_State
{
    int Number_Of_Arc_Objects;
    struct _Arc_Object* Arc_Objects;
    struct _Image* Selector;
    float Selector_Y;
    int Current_Choice;
    bool Enter_Pressed;
    bool Down_Not_Pressed;
    bool Up_Not_Pressed;
    int Times_Down_Pressed;
    Mix_Chunk* Select = NULL;
    struct _Image* Background;
    struct _Image* Title_Bar;
    struct _Timer* Timer;
    bool Go_Back_To_Main_Menu;
};

struct _Arc_Select_State* Create_Arc_Select_State();
void Destroy_Arc_Select_State(struct _Engine* Engine);

void Initialize_Arc_Select_State(struct _Engine* Engine);

void Render_Arc_Select_State(struct _Engine* Engine);
void Input_Arc_Select_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Arc_Select_State(struct _Engine* Engine);

void Set_Up_Arc_List(const char* Filename);
void Load_Arc_List(struct _Engine* Engine, const char* Filename);

void Load_Level_List_Level_Container(struct _Engine* Engine, const char* Filename);

#endif // ARC_SELECT_H_INCLUDED





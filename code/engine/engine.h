/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ENGINE_H
#define ENGINE_H
#include "text/text.h"
#include "../state/title/title.h"
#include "../state/mainmenu/mainmenu.h"
#include "../state/arc_select/arc_select.h"
#include "../state/overworld/overworld.h"
#include "../state/on_foot/on_foot.h"
#include "../state/title_cutscene/title_cutscene.h"
#include "../state/cutscene/cutscene.h"
#include "emulator/emulator.h"

#include "GLFW/glfw3.h"

#define STATE_TITLE 1
#define STATE_MAIN_MENU 2
#define STATE_CUTSCENE 3
#define STATE_PLATFORMING 4
#define STATE_MULTIPLAYER_PLATFORMING 5
#define STATE_THE_END 6
#define STATE_LEVEL_SELECT 7
#define STATE_CUTSCENE_SELECT 8
#define STATE_MODEL_SELECT 9
#define STATE_ERROR 10
#define STATE_DEBUG_MENU 11
#define STATE_OVERWORLD 12
#define STATE_ARC_SELECT 13
#define STATE_CUTSCENE_CREATOR 14
#define STATE_HANGAR 15
#define STATE_ON_FOOT 16
#define STATE_TITLE_CUTSCENE 17
#define STATE_INTERIM 18


struct _Filepath
{
    char Filepath[256];
};

struct _Keypad
{
    bool Up;
    bool Down;
    bool Left;
    bool Right;

    float Left_Analog_Rightness;
    float Left_Analog_Upness;

    float Right_Analog_Rightness;
    float Right_Analog_Upness;

    float Head_Rightness;
    float Head_Upness;

    float Right_Hand_Coordinates[3];
    float Left_Hand_Coordinates[3];

    float Right_Hand_Velocities[3];
    float Left_Hand_Velocities[3];

    float Left_Analog_Trigger;
    float Right_Analog_Trigger;

    bool Left_Trigger;
    bool Right_Trigger;

    bool Start;

    bool Select;

    bool A;
    bool B;
    bool C;
    bool D;

    bool E;
    bool F;
    bool G;
    bool H;

    double Mouse_X;
    double Mouse_Y;
    int Width;
    int Height;
    bool Mouse_Is_Clicked = false;
};

struct _CSS_Settings
{
    char Game_Name[33];
    unsigned int Build_ID;
    unsigned char Volume;
    unsigned char SFX_Volume;
    int Current_Language_ID;
    int Screen_Width;
    int Screen_Height;
    float Screen_Magnification;
    int Aspect_Ratio_Width;
    int Aspect_Ratio_Height;
};

struct _Engine
{
    struct _CSS_Settings Settings;
    unsigned char Current_State;
	struct _Text_Engine* Text_Engine;
	struct _CURRENT_CSS_New_Arc_Level_Header* New_Arc_Level_Headers; //UNDER CONSTRUCTION
	struct _CURRENT_CSS_New_Arc_Level_Container* Engine_Arc_Level_Container; //UNDER CONSTRUCTION
	struct _CURRENT_CSS_On_Foot_Level_Header* Loaded_Level_Header;  //UNDER CONSTRUCTION
	struct _On_Foot_File_Header* Loaded_Level_Regions;
	struct _Title_State* Title_State;
	struct _Main_Menu_State* Main_Menu_State;
	struct _Arc_Select_State* Arc_Select_State;
	struct _Overworld_State* Overworld_State;
	struct _On_Foot_State* On_Foot_State;
	struct _Title_Cutscene_State* Title_Cutscene_State;
	struct _Cutscene_State* Cutscene_State;
	struct _Interim_State* Interim_State;
    GLFWwindow* W;

	struct _Emulator* Emulator;

	int Number_Of_Levels_Loaded;
	int Current_Level;
	struct _Filepath* Level_Region_Filenames;

	int Number_Of_Cutscenes_Loaded;
	int Current_Cutscene;
	struct _Cutscene_File* Loaded_Cutscene;


    int Current_Step;

    int Number_Of_Levels;

	struct _Level_List_Level* Loaded_Levels;
	struct _Level_List_Level_Container* Loaded_Level_Container;
	struct _Level_Container_Step* Loaded_Steps;

	char Current_Root_Directory[256];
	char Catted_Root_Directory[256];
	char Current_Language[16];
	char Current_Level_Filepath[256];
	char Current_Region_Filepath[256];

	double Use_This_Spawnpoint[3];
	char Use_This_Character[256];
};

struct _Engine* Create_Engine();
void Initialize_Engine(struct _Engine* Engine);

float Center_Coordinates(float Input, float Screen_Size);

void Reload_Region(struct _Engine* Engine);

#endif /* ENGINE_H */











/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "arc_select.h"
#include <stdio.h>
#include "stdlib.h"
#include <string.h>

#define LOAD_REGION 0
#define LOAD_CUTSCENE 1

void Set_Up_Level_List(const char* Filename)
{
    struct _Level_List Level_List;
    Level_List.Number_Of_Levels = 3;

    struct _Level_List_Level Level_List_Levels[26];

    Level_List_Levels[0].Hard_Level_Exists = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[0].Medium_Level_Exists = LEVEL_ACCESSIBLE;
    Level_List_Levels[0].Easy_Level_Exists = LEVEL_DOES_NOT_EXIST;

    memset(Level_List_Levels[0].Hard_Level_Icon_Filepath,0,256);
    memset(Level_List_Levels[0].Medium_Level_Icon_Filepath,0,256);
    memset(Level_List_Levels[0].Easy_Level_Icon_Filepath,0,256);

    memset(Level_List_Levels[0].Hard_Level_Description_Filepath,0,256);
    memset(Level_List_Levels[0].Medium_Level_Description_Filepath,0,256);
    memset(Level_List_Levels[0].Easy_Level_Description_Filepath,0,256);

    memset(Level_List_Levels[0].Hard_Level_Container_Filepath,0,256);
    memset(Level_List_Levels[0].Medium_Level_Container_Filepath,0,256);
    memset(Level_List_Levels[0].Easy_Level_Container_Filepath,0,256);

    sprintf(Level_List_Levels[0].Hard_Level_Icon_Filepath, "None");
    sprintf(Level_List_Levels[0].Medium_Level_Icon_Filepath, "icons/test.bmp");
    sprintf(Level_List_Levels[0].Easy_Level_Icon_Filepath, "None");

    sprintf(Level_List_Levels[0].Hard_Level_Description_Filepath, "None");
    sprintf(Level_List_Levels[0].Medium_Level_Description_Filepath, "icons/test.bmp");
    sprintf(Level_List_Levels[0].Easy_Level_Description_Filepath, "None");

    sprintf(Level_List_Levels[0].Hard_Level_Container_Filepath, "None");
    sprintf(Level_List_Levels[0].Medium_Level_Container_Filepath, "level_0.dat");
    sprintf(Level_List_Levels[0].Easy_Level_Container_Filepath, "None");




    Level_List_Levels[0].Hard_Level_Icon_Filepath[4] = '\0';
    Level_List_Levels[0].Medium_Level_Icon_Filepath[14] = '\0';
    Level_List_Levels[0].Easy_Level_Icon_Filepath[4] = '\0';

    Level_List_Levels[0].Hard_Level_Description_Filepath[4] = '\0';
    Level_List_Levels[0].Medium_Level_Description_Filepath[14] = '\0';
    Level_List_Levels[0].Easy_Level_Description_Filepath[4] = '\0';

    Level_List_Levels[0].Hard_Level_Container_Filepath[4] = '\0';
    Level_List_Levels[0].Medium_Level_Container_Filepath[11] = '\0';
    Level_List_Levels[0].Easy_Level_Container_Filepath[4] = '\0';




    Level_List_Levels[0].Hard_Level_Exit_Status = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[0].Medium_Level_Exit_Status = LEVEL_ACCESSIBLE;
    Level_List_Levels[0].Easy_Level_Exit_Status = LEVEL_DOES_NOT_EXIST;

    Level_List_Levels[0].Hard_Level_ID = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[0].Medium_Level_ID = 100;
    Level_List_Levels[0].Easy_Level_ID = LEVEL_DOES_NOT_EXIST;









    Level_List_Levels[1].Hard_Level_Exists = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[1].Medium_Level_Exists = LEVEL_NOT_ACCESSIBLE;
    Level_List_Levels[1].Easy_Level_Exists = LEVEL_DOES_NOT_EXIST;

    memset(Level_List_Levels[1].Hard_Level_Icon_Filepath,0,256);
    memset(Level_List_Levels[1].Medium_Level_Icon_Filepath,0,256);
    memset(Level_List_Levels[1].Easy_Level_Icon_Filepath,0,256);

    memset(Level_List_Levels[1].Hard_Level_Description_Filepath,0,256);
    memset(Level_List_Levels[1].Medium_Level_Description_Filepath,0,256);
    memset(Level_List_Levels[1].Easy_Level_Description_Filepath,0,256);

    memset(Level_List_Levels[1].Hard_Level_Container_Filepath,0,256);
    memset(Level_List_Levels[1].Medium_Level_Container_Filepath,0,256);
    memset(Level_List_Levels[1].Easy_Level_Container_Filepath,0,256);

    sprintf(Level_List_Levels[1].Hard_Level_Icon_Filepath, "None");
    sprintf(Level_List_Levels[1].Medium_Level_Icon_Filepath, "icons/test.bmp");
    sprintf(Level_List_Levels[1].Easy_Level_Icon_Filepath, "None");

    sprintf(Level_List_Levels[1].Hard_Level_Description_Filepath, "None");
    sprintf(Level_List_Levels[1].Medium_Level_Description_Filepath, "icons/test.bmp");
    sprintf(Level_List_Levels[1].Easy_Level_Description_Filepath, "None");

    sprintf(Level_List_Levels[1].Hard_Level_Container_Filepath, "None");
    sprintf(Level_List_Levels[1].Medium_Level_Container_Filepath, "level_0.dat");
    sprintf(Level_List_Levels[1].Easy_Level_Container_Filepath, "None");




    Level_List_Levels[1].Hard_Level_Icon_Filepath[4] = '\0';
    Level_List_Levels[1].Medium_Level_Icon_Filepath[14] = '\0';
    Level_List_Levels[1].Easy_Level_Icon_Filepath[4] = '\0';

    Level_List_Levels[1].Hard_Level_Description_Filepath[4] = '\0';
    Level_List_Levels[1].Medium_Level_Description_Filepath[14] = '\0';
    Level_List_Levels[1].Easy_Level_Description_Filepath[4] = '\0';

    Level_List_Levels[1].Hard_Level_Container_Filepath[4] = '\0';
    Level_List_Levels[1].Medium_Level_Container_Filepath[11] = '\0';
    Level_List_Levels[1].Easy_Level_Container_Filepath[4] = '\0';




    Level_List_Levels[1].Hard_Level_Exit_Status = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[1].Medium_Level_Exit_Status = LEVEL_NOT_ACCESSIBLE;
    Level_List_Levels[1].Easy_Level_Exit_Status = LEVEL_DOES_NOT_EXIST;

    Level_List_Levels[1].Hard_Level_ID = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[1].Medium_Level_ID = 101;
    Level_List_Levels[1].Easy_Level_ID = LEVEL_DOES_NOT_EXIST;





    Level_List_Levels[2].Hard_Level_Exists = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[2].Medium_Level_Exists = LEVEL_NOT_ACCESSIBLE;
    Level_List_Levels[2].Easy_Level_Exists = LEVEL_DOES_NOT_EXIST;

    memset(Level_List_Levels[2].Hard_Level_Icon_Filepath,0,256);
    memset(Level_List_Levels[2].Medium_Level_Icon_Filepath,0,256);
    memset(Level_List_Levels[2].Easy_Level_Icon_Filepath,0,256);

    memset(Level_List_Levels[2].Hard_Level_Description_Filepath,0,256);
    memset(Level_List_Levels[2].Medium_Level_Description_Filepath,0,256);
    memset(Level_List_Levels[2].Easy_Level_Description_Filepath,0,256);

    memset(Level_List_Levels[2].Hard_Level_Container_Filepath,0,256);
    memset(Level_List_Levels[2].Medium_Level_Container_Filepath,0,256);
    memset(Level_List_Levels[2].Easy_Level_Container_Filepath,0,256);

    sprintf(Level_List_Levels[2].Hard_Level_Icon_Filepath, "None");
    sprintf(Level_List_Levels[2].Medium_Level_Icon_Filepath, "icons/test.bmp");
    sprintf(Level_List_Levels[2].Easy_Level_Icon_Filepath, "None");

    sprintf(Level_List_Levels[2].Hard_Level_Description_Filepath, "None");
    sprintf(Level_List_Levels[2].Medium_Level_Description_Filepath, "icons/test.bmp");
    sprintf(Level_List_Levels[2].Easy_Level_Description_Filepath, "None");

    sprintf(Level_List_Levels[2].Hard_Level_Container_Filepath, "None");
    sprintf(Level_List_Levels[2].Medium_Level_Container_Filepath, "level_0.dat");
    sprintf(Level_List_Levels[2].Easy_Level_Container_Filepath, "None");




    Level_List_Levels[2].Hard_Level_Icon_Filepath[4] = '\0';
    Level_List_Levels[2].Medium_Level_Icon_Filepath[14] = '\0';
    Level_List_Levels[2].Easy_Level_Icon_Filepath[4] = '\0';

    Level_List_Levels[2].Hard_Level_Description_Filepath[4] = '\0';
    Level_List_Levels[2].Medium_Level_Description_Filepath[14] = '\0';
    Level_List_Levels[2].Easy_Level_Description_Filepath[4] = '\0';

    Level_List_Levels[2].Hard_Level_Container_Filepath[4] = '\0';
    Level_List_Levels[2].Medium_Level_Container_Filepath[11] = '\0';
    Level_List_Levels[2].Easy_Level_Container_Filepath[4] = '\0';




    Level_List_Levels[2].Hard_Level_Exit_Status = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[2].Medium_Level_Exit_Status = LEVEL_NOT_ACCESSIBLE;
    Level_List_Levels[2].Easy_Level_Exit_Status = LEVEL_DOES_NOT_EXIST;

    Level_List_Levels[2].Hard_Level_ID = LEVEL_DOES_NOT_EXIST;
    Level_List_Levels[2].Medium_Level_ID = 200;
    Level_List_Levels[2].Easy_Level_ID = LEVEL_DOES_NOT_EXIST;



    FILE* f = fopen(Filename, "wb");
    fwrite(&Level_List, 1, sizeof(struct _Level_List), f);
    fwrite(&Level_List_Levels[0],1,sizeof(struct _Level_List_Level),f);
    fwrite(&Level_List_Levels[1],1,sizeof(struct _Level_List_Level),f);
    fwrite(&Level_List_Levels[2],1,sizeof(struct _Level_List_Level),f);
    fclose(f);

};

void Set_Up_Level_Container(const char* Filename)
{
    struct _Level_List_Level_Container Level;

    memset(Level.Level_Name,0,256);
    memset(Level.Level_Description,0,256);

    sprintf(Level.Level_Name,"ZUNDAMONel");
    sprintf(Level.Level_Description,"MIKUTETOSLOPtion");

    Level.Level_Name[10] = '\0';
    Level.Level_Description[16] = '\0';

    Level.Mission_Accomplished_Objective_Type = 42069;
    Level.Mission_Accomplished_Category = 0;
    Level.Mission_Accomplished_Subcategory = 0;
    Level.Mission_Accomplished_ID = 0;
    Level.Mission_Accomplished_Variant = 0;
    Level.Mission_Accomplished_Value = 0;
    Level.Mission_Accomplished_Current_Value = 0;

    Level.Mission_Complete_Objective_Type = MISSION_OBJECTIVE_REACH_GOALPOST;
    Level.Mission_Complete_Category = 0;
    Level.Mission_Complete_Subcategory = 0;
    Level.Mission_Complete_ID = 0;
    Level.Mission_Complete_Variant = 0;
    Level.Mission_Complete_Value = 0;
    Level.Mission_Complete_Current_Value = 0;

    Level.Debriefing_Type = DEBRIEFING_TYPE_NONE;

    Level.Number_Of_Steps = 1;
    Level.Offset_To_Steps = sizeof(struct _Level_List_Level_Container);
    int Size_Of_Steps = Level.Number_Of_Steps * sizeof(struct _Level_Container_Step);

    Level.Select_Random_Mission_Accomplished_Line = 0; //Or play in sequence.
    Level.Select_Random_Mission_Complete_Line = 0; //Or play in sequence.
    Level.Select_Random_Mission_Failed_Line = 0; //Or play in sequence.

    Level.Number_Of_Mission_Accomplished_Voice_Line_Filepaths = 1;
    Level.Number_Of_Mission_Accomplished_Line_Filepaths = 1;
    Level.Number_Of_Mission_Complete_Voice_Line_Filepaths = 1;
    Level.Number_Of_Mission_Complete_Line_Filepaths = 1;
    Level.Number_Of_Mission_Failed_Voice_Line_Filepaths = 1;
    Level.Number_Of_Mission_Failed_Line_Filepaths = 1;

    int Size_Of_Mission_Accomplished_Voice_Line_Filepaths = Level.Number_Of_Mission_Accomplished_Voice_Line_Filepaths * 256;
    int Size_Of_Mission_Accomplished_Line_Filepaths = Level.Number_Of_Mission_Accomplished_Line_Filepaths * 256;
    int Size_Of_Mission_Complete_Voice_Line_Filepaths = Level.Number_Of_Mission_Complete_Voice_Line_Filepaths * 256;
    int Size_Of_Mission_Complete_Line_Filepaths = Level.Number_Of_Mission_Complete_Line_Filepaths * 256;
    int Size_Of_Mission_Failed_Voice_Line_Filepaths = Level.Number_Of_Mission_Failed_Voice_Line_Filepaths * 256;
    int Size_Of_Mission_Failed_Line_Filepaths = Level.Number_Of_Mission_Failed_Line_Filepaths * 256;

    Level.Offset_To_Mission_Accomplished_Voice_Line_Filepaths = Level.Offset_To_Steps + Size_Of_Steps;
    Level.Offset_To_Mission_Accomplished_Line_Filepaths = Level.Offset_To_Mission_Accomplished_Voice_Line_Filepaths + Size_Of_Mission_Accomplished_Voice_Line_Filepaths;
    Level.Offset_To_Mission_Complete_Voice_Line_Filepaths = Level.Offset_To_Mission_Accomplished_Line_Filepaths + Size_Of_Mission_Accomplished_Line_Filepaths;
    Level.Offset_To_Mission_Complete_Line_Filepaths = Level.Offset_To_Mission_Complete_Voice_Line_Filepaths + Size_Of_Mission_Complete_Voice_Line_Filepaths;
    Level.Offset_To_Mission_Failed_Voice_Line_Filepaths = Level.Offset_To_Mission_Complete_Line_Filepaths + Size_Of_Mission_Complete_Line_Filepaths;
    Level.Offset_To_Mission_Failed_Line_Filepaths = Level.Offset_To_Mission_Failed_Voice_Line_Filepaths + Size_Of_Mission_Failed_Voice_Line_Filepaths;

    struct _Level_Container_Step Step;

    Step.Step_ID = 0;
    Step.Step_Type = STEP_TYPE_LOAD_STAGE;
    Step.Skip_This_Step = 0;
    Step.Spawnpoint_ID = 0;
    memset(Step.Character_Filepath,0,256);
    memset(Step.Filepath,0,256);
    sprintf(Step.Character_Filepath, "None");
    sprintf(Step.Filepath, "regions/test_region.dat");
    Step.Filepath[23] = '\0';

    struct _Mission_Line_Filepath Filepath;
    sprintf(Filepath.Filepath, "ThisIsAMissionAccomplishedVoiceLine");

    FILE* f = fopen(Filename, "wb");
    fwrite(&Level, 1, sizeof(struct _Level_List_Level_Container), f);
    rewind(f);

    fseek(f, Level.Offset_To_Steps, SEEK_SET);
    fwrite(&Step, 1, sizeof(struct _Level_Container_Step), f);
    rewind(f);

    fseek(f, Level.Offset_To_Mission_Accomplished_Voice_Line_Filepaths, SEEK_SET);
    fwrite(&Filepath, 1, sizeof(struct _Mission_Line_Filepath), f);
    rewind(f);

    fseek(f, Level.Offset_To_Mission_Accomplished_Line_Filepaths, SEEK_SET);
    fwrite(&Filepath, 1, sizeof(struct _Mission_Line_Filepath), f);
    rewind(f);


    fseek(f, Level.Offset_To_Mission_Complete_Voice_Line_Filepaths, SEEK_SET);
    fwrite(&Filepath, 1, sizeof(struct _Mission_Line_Filepath), f);
    rewind(f);

    fseek(f, Level.Offset_To_Mission_Complete_Line_Filepaths, SEEK_SET);
    fwrite(&Filepath, 1, sizeof(struct _Mission_Line_Filepath), f);
    rewind(f);


    fseek(f, Level.Offset_To_Mission_Failed_Voice_Line_Filepaths, SEEK_SET);
    fwrite(&Filepath, 1, sizeof(struct _Mission_Line_Filepath), f);
    rewind(f);

    fseek(f, Level.Offset_To_Mission_Failed_Line_Filepaths, SEEK_SET);
    fwrite(&Filepath, 1, sizeof(struct _Mission_Line_Filepath), f);
    rewind(f);

    fclose(f);
}

void Set_Up_Arc_List(const char* Filename)
{
    struct _Main_Arc_List Main_Arc_List;
    Main_Arc_List.Number_Of_Arcs = 2;

    struct _Arc_List_Arc_Entry TheDaysOfNoVA;
    struct _Arc_List_Arc_Entry LoveQuestFulfilled;

    memset(TheDaysOfNoVA.Arc_Filepath,0,256);
    memset(LoveQuestFulfilled.Arc_Filepath,0,256);

    sprintf(TheDaysOfNoVA.Arc_Filepath, "resources/data/arcs/the_days_of_nova/");
    sprintf(LoveQuestFulfilled.Arc_Filepath, "resources/data/arcs/love_quest_fulfilled/");

    TheDaysOfNoVA.Arc_Filepath[37] = '\0';
    LoveQuestFulfilled.Arc_Filepath[41] = '\0';

    FILE* f = fopen(Filename, "wb");
    fwrite(&Main_Arc_List, 1, sizeof(struct _Main_Arc_List), f);
    fwrite(&TheDaysOfNoVA,1,sizeof(struct _Arc_List_Arc_Entry),f);
    fwrite(&LoveQuestFulfilled,1,sizeof(struct _Arc_List_Arc_Entry),f);
    fclose(f);
};

void Load_Arc_List(struct _Engine* Engine, const char* Filename)
{
    const char* ID_LANGUAGE_ENGLISH_Banner_Filepath = "english/banner.bmp";
const char* ID_LANGUAGE_ICELANDIC_Banner_Filepath = "";
const char* ID_LANGUAGE_FRENCH_Banner_Filepath = "";
const char* ID_LANGUAGE_SPANISH_Banner_Filepath = "";
const char* ID_LANGUAGE_PORTUGESE_Banner_Filepath = "";
const char* ID_LANGUAGE_NORWEGIAN_Banner_Filepath = "";
const char* ID_LANGUAGE_SWEDISH_Banner_Filepath = "";
const char* ID_LANGUAGE_FINNISH_Banner_Filepath = "";
const char* ID_LANGUAGE_DANISH_Banner_Filepath = "";
const char* ID_LANGUAGE_DUTCH_Banner_Filepath = "";
const char* ID_LANGUAGE_GERMAN_Banner_Filepath = "";
const char* ID_LANGUAGE_POLISH_Banner_Filepath = "";
const char* ID_LANGUAGE_ITALIAN_Banner_Filepath = "";
const char* ID_LANGUAGE_GREEK_Banner_Filepath = "";
const char* ID_LANGUAGE_TURKISH_Banner_Filepath = "";
const char* ID_LANGUAGE_UKRAINIAN_Banner_Filepath = "";
const char* ID_LANGUAGE_RUSSIAN_Banner_Filepath = "";
const char* ID_LANGUAGE_CHINESE_Banner_Filepath = "";
const char* ID_LANGUAGE_KOREAN_Banner_Filepath = "";
const char* ID_LANGUAGE_JAPANESE_Banner_Filepath = "";
const char* ID_LANGUAGE_THAI_Banner_Filepath = "";
const char* ID_LANGUAGE_VIETNAMESE_Banner_Filepath = "";
const char* ID_LANGUAGE_FILIPINO_Banner_Filepath = "";
const char* ID_LANGUAGE_INDONESIAN_Banner_Filepath = "";
const char* ID_LANGUAGE_ARABIC_Banner_Filepath = "";
const char* ID_LANGUAGE_HINDI_Banner_Filepath = "";
const char* ID_LANGUAGE_BENGALI_Banner_Filepath = "";
const char* ID_LANGUAGE_URDU_Banner_Filepath = "";
const char* ID_LANGUAGE_PUNJABI_Banner_Filepath = "";
const char* ID_LANGUAGE_FARSI_Banner_Filepath = "";
const char* ID_LANGUAGE_UADL_Banner_Filepath = "";
const char* ID_LANGUAGE_LATIN_Banner_Filepath = "";
const char* ID_LANGUAGE_ESPERANTO_Banner_Filepath = "";
const char* ID_LANGUAGE_DARI_Banner_Filepath = "";
const char* ID_LANGUAGE_PASHTO_Banner_Filepath = "";
const char* ID_LANGUAGE_SERBOCROATIAN_Banner_Filepath = "";
const char* ID_LANGUAGE_ROMANIAN_Banner_Filepath = "";

    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(&Buffer, sizeof(unsigned char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;

    Engine->Arc_Select_State->Number_Of_Arc_Objects = *(int*)&(Buffer[0]);
    printf("There are %d Arcs.\n", Engine->Arc_Select_State->Number_Of_Arc_Objects);

    Engine->Arc_Select_State->Arc_Objects = (struct _Arc_Object*)calloc(Engine->Arc_Select_State->Number_Of_Arc_Objects,sizeof(struct _Arc_Object));

    for(int j = 0; j < Engine->Arc_Select_State->Number_Of_Arc_Objects; j++)
    {
        Buffer_Location = 4 + (j*sizeof(struct _Arc_List_Arc_Entry));
        char Filename[256];

        for(int l = 0; l < 256; l++)
        {
            Filename[l] = Buffer[Buffer_Location+l];
            printf("%c", Filename[l]);
        }
        printf("\n");

        for(int l = 0; l < 256; l++)
        {
            Engine->Arc_Select_State->Arc_Objects[j].Filepath[l] = Filename[l];
        }

        switch(Engine->Settings.Current_Language_ID)
        {
            case ID_LANGUAGE_ENGLISH:
                strcat(Filename, ID_LANGUAGE_ENGLISH_Banner_Filepath);
            break;
        }
        printf("New String: ");

        for(int l = 0; l < 256; l++)
        {
            printf("%c", Filename[l]);
        }
        printf("\n");

        Engine->Arc_Select_State->Arc_Objects[j].Arc_Banner = Create_Image();
        Load_Image(Engine->Arc_Select_State->Arc_Objects[j].Arc_Banner,Filename);


    }
};


struct _Arc_Select_State* Create_Arc_Select_State()
{
    struct _Arc_Select_State* I = (struct _Arc_Select_State*) calloc(1,sizeof (struct _Arc_Select_State));
    return I;
};
void Destroy_Arc_Select_State(struct _Engine* Engine)
{

};

void Initialize_Arc_Select_State(struct _Engine* Engine)
{
    Set_Up_Level_List("resources/data/arcs/love_quest_fulfilled/level_list.dat");
    Set_Up_Level_List("resources/data/arcs/the_days_of_nova/level_list.dat");
    Set_Up_Level_Container("resources/data/arcs/love_quest_fulfilled/level_0.dat");
    Set_Up_Level_Container("resources/data/arcs/the_days_of_nova/level_0.dat");

    Engine->Arc_Select_State->Timer = Create_Timer();
    Set_Timer(Engine->Arc_Select_State->Timer);
    Pause_Timer(Engine->Arc_Select_State->Timer);

    Engine->Arc_Select_State->Selector_Y = 48;
    glColor3f(1,1,1);
    Load_Arc_List(Engine, "resources/data/arcs/arc_list.dat");

    Engine->Arc_Select_State->Background = Create_Image();
    Load_Image(Engine->Main_Menu_State->Background, "resources/artwork/gui/title_background.bmp");

    Engine->Arc_Select_State->Selector = Create_Image();
    Load_Image(Engine->Main_Menu_State->Selector, "resources/artwork/gui/selector.bmp");

    Engine->Arc_Select_State->Select = Mix_LoadWAV( "resources/tracks_and_sounds/general/select.wav" );

    Engine->Arc_Select_State->Go_Back_To_Main_Menu = false;
};

void Render_Arc_Select_State(struct _Engine* Engine)
{
    Render_Image(Engine->Main_Menu_State->Background,0,0,1);
    int Start_Y = 48;
    for(int j = 0; j < Engine->Arc_Select_State->Number_Of_Arc_Objects; j++)
    {
        Render_Image(Engine->Arc_Select_State->Arc_Objects[j].Arc_Banner,176,Start_Y + (96*j), 1);
    }
    Render_Image(Engine->Main_Menu_State->Selector,176,Engine->Arc_Select_State->Selector_Y,1);
};

void Input_Arc_Select_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Down && Engine->Arc_Select_State->Current_Choice < Engine->Arc_Select_State->Number_Of_Arc_Objects-1 && Engine->Arc_Select_State->Down_Not_Pressed)
    {
         Engine->Arc_Select_State->Selector_Y += 96;
        glTranslatef(0,-96,0);
         Engine->Arc_Select_State->Current_Choice += 1;
         Engine->Arc_Select_State->Down_Not_Pressed = false;
         if(!Engine->Arc_Select_State->Enter_Pressed)
         {
            Mix_PlayChannel( -1, Engine->Arc_Select_State->Select, 0 );
         }
    }

    if(Keypad.Up && Engine->Arc_Select_State->Current_Choice > 0 && Engine->Arc_Select_State->Up_Not_Pressed)
    {
         Engine->Arc_Select_State->Selector_Y -= 96;
         glTranslatef(0,96,0);
         Engine->Arc_Select_State->Current_Choice -= 1;
         Engine->Arc_Select_State->Up_Not_Pressed = false;
         if(!Engine->Arc_Select_State->Enter_Pressed)
         {
            Mix_PlayChannel( -1, Engine->Arc_Select_State->Select, 0 );
         }
    }

    if(!Keypad.Up)
    {
        Engine->Arc_Select_State->Up_Not_Pressed = true;
    }

    if(!Keypad.Down)
    {
        Engine->Arc_Select_State->Down_Not_Pressed = true;
    }


    if(Keypad.Start)
    {
        printf("I will load arc %d\n", Engine->Arc_Select_State->Current_Choice);
        printf("Searching in root directory ");

        for(int l = 0; l < 256; l++)
        {
            Engine->Current_Root_Directory[l] = Engine->Arc_Select_State->Arc_Objects[Engine->Arc_Select_State->Current_Choice].Filepath[l];
            Engine->Catted_Root_Directory[l] = Engine->Arc_Select_State->Arc_Objects[Engine->Arc_Select_State->Current_Choice].Filepath[l];
        }

        printf(Engine->Current_Root_Directory);
        printf("\n");
        Resume_Timer(Engine->Arc_Select_State->Timer);
        Engine->Arc_Select_State->Enter_Pressed = true;
        glColor3f(0,0,0);
        Mix_HaltChannel(0);
        Mix_HaltMusic();
    }

    if(Keypad.B)
    {
        Engine->Arc_Select_State->Go_Back_To_Main_Menu = true;
        glColor3f(0,0,0);
//        glTranslate3f(0,0,0);
        Resume_Timer(Engine->Arc_Select_State->Timer);
    }

    if(Keypad.A)
    {

    }
};
void Process_Arc_Select_State(struct _Engine* Engine)
{
    double z = Get_Ticks(Engine->Arc_Select_State->Timer);
    if(z > 2 && Engine->Arc_Select_State->Go_Back_To_Main_Menu)
    {
        free(Engine->Main_Menu_State);
        Engine->Main_Menu_State = Create_Main_Menu_State();
        Initialize_Main_Menu_State(Engine);
        glColor3f(1,1,1);
        glTranslatef(0,0,0);
        Engine->Current_State = STATE_MAIN_MENU;
    }

    if(z > 2 && !Engine->Arc_Select_State->Go_Back_To_Main_Menu)
    {
        printf("Going to Overworld state!\n");
        printf("The Current Root Directory is ");
        printf(Engine->Current_Root_Directory);
        printf("\n");
        const char* Level_List_Extension = "level_list.dat";
        strcat(Engine->Catted_Root_Directory, Level_List_Extension);

        printf("The Catted Root Directory is ");
        printf(Engine->Catted_Root_Directory);
        printf("\n\n");

        FILE* f;
        f = fopen(Engine->Catted_Root_Directory, "rb");
        long File_Size;
        fseek(f, 0, SEEK_END);
        File_Size = ftell(f);
        rewind(f);
        unsigned char Buffer[File_Size];
        fread(&Buffer, sizeof(unsigned char), File_Size, f);
        fclose(f);
        long Buffer_Location = 0;

        Engine->Number_Of_Levels = *(int*)&(Buffer[0]);
        printf("There are %d Levels.\n", Engine->Number_Of_Levels);

        Engine->Overworld_State= Create_Overworld_State();
        Initialize_Overworld_State(Engine);

        Engine->Overworld_State->Level_Triples = (struct _CSS_Level_Triple_Object*)calloc(Engine->Number_Of_Levels,sizeof(struct _CSS_Level_Triple_Object));
        Engine->Loaded_Levels = (struct _Level_List_Level*)calloc(Engine->Number_Of_Levels,sizeof(struct _Level_List_Level));

        for(int j = 0; j < Engine->Number_Of_Levels; j++)
        {
            Buffer_Location = 4 + (j*sizeof(struct _Level_List_Level));

            Engine->Loaded_Levels[j].Hard_Level_Exists = *(int*)&(Buffer[Buffer_Location]);
            Engine->Loaded_Levels[j].Medium_Level_Exists = *(int*)&(Buffer[Buffer_Location+4]);
            Engine->Loaded_Levels[j].Easy_Level_Exists = *(int*)&(Buffer[Buffer_Location+8]);

            printf("Hard %d\n\n",Engine->Loaded_Levels[j].Hard_Level_Exists);
            printf("Medium %d\n\n",Engine->Loaded_Levels[j].Medium_Level_Exists);
            printf("Easy %d\n\n",Engine->Loaded_Levels[j].Easy_Level_Exists);

            Buffer_Location += 12;

            for(int l = 0; l < 256; l++)
            {
                Engine->Loaded_Levels[j].Hard_Level_Icon_Filepath[l] = Buffer[Buffer_Location + l];
                Engine->Loaded_Levels[j].Medium_Level_Icon_Filepath[l] = Buffer[Buffer_Location + l+256];
                Engine->Loaded_Levels[j].Easy_Level_Icon_Filepath[l] = Buffer[Buffer_Location + l+512];

                Engine->Loaded_Levels[j].Hard_Level_Description_Filepath[l] = Buffer[Buffer_Location + l+768];
                Engine->Loaded_Levels[j].Medium_Level_Description_Filepath[l] = Buffer[Buffer_Location + l+1024];
                Engine->Loaded_Levels[j].Easy_Level_Description_Filepath[l] = Buffer[Buffer_Location + l+1280];

                Engine->Loaded_Levels[j].Hard_Level_Container_Filepath[l] = Buffer[Buffer_Location + l+1536];
                Engine->Loaded_Levels[j].Medium_Level_Container_Filepath[l] = Buffer[Buffer_Location + l+1792];
                Engine->Loaded_Levels[j].Easy_Level_Container_Filepath[l] = Buffer[Buffer_Location + l+2048];
            }

            Buffer_Location += (256 * 9);

            Engine->Loaded_Levels[j].Hard_Level_Exit_Status = *(int*)&(Buffer[Buffer_Location]);
            Engine->Loaded_Levels[j].Medium_Level_Exit_Status = *(int*)&(Buffer[Buffer_Location+4]);
            Engine->Loaded_Levels[j].Easy_Level_Exit_Status = *(int*)&(Buffer[Buffer_Location+8]);

            Engine->Loaded_Levels[j].Hard_Level_ID = *(int*)&(Buffer[Buffer_Location+12]);
            Engine->Loaded_Levels[j].Medium_Level_ID = *(int*)&(Buffer[Buffer_Location+16]);
            Engine->Loaded_Levels[j].Easy_Level_ID = *(int*)&(Buffer[Buffer_Location+20]);

            Engine->Overworld_State->Level_Triples[j].Hard_Mode_Image = Create_Image();
            Engine->Overworld_State->Level_Triples[j].Medium_Mode_Image = Create_Image();
            Engine->Overworld_State->Level_Triples[j].Easy_Mode_Image = Create_Image();


            char True_Filepath[256];
            for(int l = 0; l < 256; l++)
            {
                True_Filepath[l] = Engine->Arc_Select_State->Arc_Objects[Engine->Arc_Select_State->Current_Choice].Filepath[l];
            }

            switch(Engine->Settings.Current_Language_ID)
            {
                case ID_LANGUAGE_ENGLISH:
                const char* English_Prefix = "english/";
                strcat(True_Filepath, English_Prefix);
                break;
            }

            strcat(True_Filepath,Engine->Loaded_Levels[j].Hard_Level_Icon_Filepath);



            char True_Filepath_Two[256];
            for(int l = 0; l < 256; l++)
            {
                True_Filepath_Two[l] = Engine->Arc_Select_State->Arc_Objects[Engine->Arc_Select_State->Current_Choice].Filepath[l];
            }

            switch(Engine->Settings.Current_Language_ID)
            {
                case ID_LANGUAGE_ENGLISH:
                const char* English_Prefix = "english/";
                strcat(True_Filepath_Two, English_Prefix);
                break;
            }

            strcat(True_Filepath_Two,Engine->Loaded_Levels[j].Medium_Level_Icon_Filepath);




            char True_Filepath_Three[256];
            for(int l = 0; l < 256; l++)
            {
                True_Filepath_Three[l] = Engine->Arc_Select_State->Arc_Objects[Engine->Arc_Select_State->Current_Choice].Filepath[l];
            }

            switch(Engine->Settings.Current_Language_ID)
            {
                case ID_LANGUAGE_ENGLISH:
                const char* English_Prefix = "english/";
                strcat(True_Filepath_Three, English_Prefix);
                break;
            }

            strcat(True_Filepath_Three,Engine->Loaded_Levels[j].Easy_Level_Icon_Filepath);



            printf(True_Filepath);
            printf("\n");
            printf(True_Filepath_Two);
            printf("\n");
            printf(True_Filepath_Three);
            printf("\n");


            Load_Image(Engine->Overworld_State->Level_Triples[j].Hard_Mode_Image, True_Filepath);
            Load_Image(Engine->Overworld_State->Level_Triples[j].Medium_Mode_Image, True_Filepath_Two);
            Load_Image(Engine->Overworld_State->Level_Triples[j].Easy_Mode_Image, True_Filepath_Three);

        }

        for(int l = 0; l < 256; l++)
        {
            Engine->Catted_Root_Directory[l] = 0;
        }

        Engine->Current_State = STATE_OVERWORLD;
    }
};

void Process_Engine_Steps(struct _Engine* Engine)
{

}

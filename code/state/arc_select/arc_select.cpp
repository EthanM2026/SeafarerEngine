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
    Engine->Arc_Select_State->Title_Bar = Create_Image();
    Load_Image(Engine->Arc_Select_State->Title_Bar, "resources/artwork/gui/arc_select_title_bar.bmp");

    glColor3f(1,1,1);
    Create_Arc_List("resources/data/arcs/arc_list.dat");
    Load_Arc_List(Engine, "resources/data/arcs/arc_list.dat");
    Engine->Arc_Select_State->Selector = Create_Image();
    Load_Image(Engine->Arc_Select_State->Selector, "resources/artwork/gui/gui_selector_right.bmp");
    Engine->Arc_Select_State->Selector_Y = 52;

    Engine->Arc_Select_State->Up_Not_Pressed = false;
    Engine->Arc_Select_State->Down_Not_Pressed = false;
    Engine->Arc_Select_State->Select = Mix_LoadWAV( "resources/tracks_and_sounds/general/select.wav" );
    Engine->Arc_Select_State->Current_Choice = 0;

    Engine->Arc_Select_State->Times_Down_Pressed = 0;

    Engine->Arc_Select_State->Background = Create_Image();
    Load_Image(Engine->Arc_Select_State->Background,"resources/artwork/gui/title_background.bmp");

    Engine->Arc_Select_State->Timer = Create_Timer();
    Set_Timer(Engine->Arc_Select_State->Timer);
    Pause_Timer(Engine->Arc_Select_State->Timer);

    Engine->Arc_Select_State->Enter_Pressed = false;

    Engine->Arc_Select_State->Go_Back_To_Main_Menu = false;
};

void Render_Arc_Select_State(struct _Engine* Engine)
{
    if(!Engine->Arc_Select_State->Enter_Pressed)
    {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Render_Image(Engine->Arc_Select_State->Background, 0,-150,1);
    glPopMatrix();
    float Start_Y = 32+16;
    for(int j = 0; j < Engine->Arc_Select_State->Number_Of_Arc_Objects; j++)
    {
        Render_Image(Engine->Arc_Select_State->Arc_Objects[j].Arc_Banner,Center_Coordinates(Engine->Arc_Select_State->Arc_Objects[j].Arc_Banner->Width,640),Start_Y+(96*j),1);
        Render_Image(Engine->Arc_Select_State->Arc_Objects[j].Arc_Text,Center_Coordinates(Engine->Arc_Select_State->Arc_Objects[j].Arc_Text->Width,640),Start_Y+(96*j),1);
    }
    Render_Image(Engine->Arc_Select_State->Selector, 128,Engine->Arc_Select_State->Selector_Y,1);

    Render_Image(Engine->Arc_Select_State->Title_Bar, 0,0,1);
    }
};

void Input_Arc_Select_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    int TBD_Mouse = 0;
    int Options_Mouse = 0;

    if(Keypad.Down && Engine->Arc_Select_State->Current_Choice < Engine->Arc_Select_State->Number_Of_Arc_Objects-1 && Engine->Arc_Select_State->Down_Not_Pressed)
    {
         Engine->Arc_Select_State->Selector_Y += 96;
         Engine->Arc_Select_State->Current_Choice += 1;
         Engine->Arc_Select_State->Down_Not_Pressed = false;
         Mix_PlayChannel( -1, Engine->Arc_Select_State->Select, 0 );
         Engine->Arc_Select_State->Times_Down_Pressed += 1;
    }

    if(Keypad.Up && Engine->Arc_Select_State->Current_Choice > 0 && Engine->Arc_Select_State->Up_Not_Pressed)
    {
         Engine->Arc_Select_State->Selector_Y -= 96;
         Engine->Arc_Select_State->Current_Choice -= 1;
         Engine->Arc_Select_State->Up_Not_Pressed = false;
         Mix_PlayChannel( -1, Engine->Arc_Select_State->Select, 0 );
         Engine->Arc_Select_State->Times_Down_Pressed -= 1;
    }

    if(Engine->Arc_Select_State->Times_Down_Pressed == 3)
    {
        Engine->Arc_Select_State->Times_Down_Pressed = 0;
        Engine->Arc_Select_State->Selector_Y += 72;
        glTranslatef(0,-360,0);
    }

    if(Engine->Arc_Select_State->Times_Down_Pressed == -1)
    {
        Engine->Arc_Select_State->Times_Down_Pressed = 2;
        Engine->Arc_Select_State->Selector_Y -= 72;
        glTranslatef(0,360,0);
    }

    if(!Keypad.Up)
    {
        Engine->Arc_Select_State->Up_Not_Pressed = true;
    }

    if(Keypad.A)
    {
        Engine->Arc_Select_State->Selector_Y+=1;
    }

    if(!Keypad.Down)
    {
        Engine->Arc_Select_State->Down_Not_Pressed = true;
    }

    if(Keypad.Start)
    {
        Resume_Timer(Engine->Arc_Select_State->Timer);
        Engine->Arc_Select_State->Enter_Pressed = true;
    }

    if(Keypad.B)
    {
        Resume_Timer(Engine->Arc_Select_State->Timer);
        Engine->Arc_Select_State->Go_Back_To_Main_Menu = true;
    }
};
void Process_Arc_Select_State(struct _Engine* Engine)
{
    //printf("Selector Coordinates: %f/n", Engine->Arc_Select_State->Selector_Y);
    double z = Get_Ticks(Engine->Arc_Select_State->Timer);
    if(z > 2)
    {
        if(!Engine->Arc_Select_State->Go_Back_To_Main_Menu)
        {
        Create_Level_List("resources/data/arcs/the_final_arc.lst");
        Load_Level_List(Engine, "resources/data/arcs/the_final_arc.lst");
        Engine->Current_State = STATE_OVERWORLD;
        }
        else if(Engine->Arc_Select_State->Go_Back_To_Main_Menu)
        {
            free(Engine->Main_Menu_State);
            Engine->Main_Menu_State= Create_Main_Menu_State();
            Initialize_Main_Menu_State(Engine);
            Engine->Current_State = STATE_MAIN_MENU;
        }
    }
};



void Create_Arc_List(const char* Filename)
{
    struct _Arc_List Arc_List;
    Arc_List.Number_Of_Arcs = 1;
    Arc_List.Offset_To_Arcs = sizeof(struct _Arc_List);

    struct _Arc_List_Arc A;
    struct _Arc_List_Arc ATwo;

    memset(&A.ENGLISH.Arc_Name, 0, 256);
    memset(&A.ENGLISH.Arc_Banner_Filepath, 0, 256);
    memset(A.Arc_Level_List_Filepath, 0, 256);

//    sprintf(A.ENGLISH.Arc_Name,"Test Arc");
    A.ENGLISH.Arc_Name_Length=13;
    sprintf(A.ENGLISH.Arc_Banner_Filepath,"resources/data/arcs/the_final_arc.bmp");
    sprintf(A.Arc_Level_List_Filepath,"resources/data/arcs/the_final_arc.lst");
//
//    A.ENGLISH.Arc_Name[13] = '\0';
    A.ENGLISH.Arc_Banner_Filepath[37] = '\0';
    A.Arc_Level_List_Filepath[37] = '\0';




   // memset(ATwo.Arc_Name, 0, 256);
    //memset(ATwo.Arc_Banner_Filepath, 0, 256);
   // memset(ATwo.Arc_Level_List_Filepath, 0, 256);

  //  sprintf(ATwo.Arc_Name,"The Days of NEATO");
  //  ATwo.Arc_Name_Length=17;
  //  sprintf(ATwo.Arc_Banner_Filepath,"resources/data/arcs/the_days_of_neato.bmp");
  //  sprintf(ATwo.Arc_Level_List_Filepath,"resources/data/arcs/the_days_of_neato.lst");

  //  ATwo.Arc_Name[17] = '\0';
 //   ATwo.Arc_Banner_Filepath[41] = '\0';
 //   ATwo.Arc_Level_List_Filepath[41] = '\0';

FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Arc_List, 1, sizeof(struct _Arc_List),f);

    fwrite(&A, 1, sizeof(struct _Arc_List_Arc),f);
    //fwrite(&ATwo, 1, sizeof(struct _Arc_List_Arc),f);
    fclose(f);
};

//resources/data/levels/final_arc/bay/bay.lvl

void Create_Level_List(const char* Filename)
{
    struct _Level_List List;

    List.Number_Of_Levels = 1;
    List.Offset_To_Levels = sizeof(struct _Level_List);

    struct _Level_List_Level Level;
    struct _Level_List_Level Level_Two;
    struct _Level_List_Level Level_Three;

    Level.Hard_Level_Exists = 0;
    Level.Medium_Level_Exists = 1;
    Level.Easy_Level_Exists = 0;


    memset(Level.Hard_Level_Icon_Filepath,0,256);
    memset(Level.Medium_Level_Icon_Filepath,0,256);
    memset(Level.Easy_Level_Icon_Filepath,0,256);

    memset(Level.Hard_Level_Description_Filepath,0,256);
    memset(Level.Medium_Level_Description_Filepath,0,256);
    memset(Level.Easy_Level_Description_Filepath,0,256);

    memset(Level.Hard_Level_Container_Filepath,0,256);
    memset(Level.Medium_Level_Container_Filepath,0,256);
    memset(Level.Easy_Level_Container_Filepath,0,256);

    memset(Level.Hard_Level_Text_Description,0,256);
    memset(Level.Medium_Level_Text_Description,0,256);
    memset(Level.Easy_Level_Text_Description,0,256);




    sprintf(Level.Hard_Level_Icon_Filepath,"None");
    sprintf(Level.Medium_Level_Icon_Filepath,"resources/data/levels/final_arc/bay/bay.bmp");
    sprintf(Level.Easy_Level_Icon_Filepath,"None");

    sprintf(Level.Hard_Level_Description_Filepath,"None");
    sprintf(Level.Medium_Level_Description_Filepath,"resources/data/levels/final_arc/bay/bay_description.bmp");
    sprintf(Level.Easy_Level_Description_Filepath,"None");

    sprintf(Level.Hard_Level_Container_Filepath,"None");
    sprintf(Level.Medium_Level_Container_Filepath,"resources/data/levels/final_arc/bay/bay.lvl");
    sprintf(Level.Easy_Level_Container_Filepath,"None");

    sprintf(Level.Hard_Level_Text_Description,"None");
    sprintf(Level.Medium_Level_Text_Description,"MegaCWCth");
    sprintf(Level.Easy_Level_Text_Description,"None");





    Level.Hard_Level_Icon_Filepath[4] = '\0';
    Level.Medium_Level_Icon_Filepath[43] = '\0';
    Level.Easy_Level_Icon_Filepath[4] = '\0';

    Level.Hard_Level_Description_Filepath[4] = '\0';
    Level.Medium_Level_Description_Filepath[55] = '\0';
    Level.Easy_Level_Description_Filepath[4] = '\0';

    Level.Hard_Level_Container_Filepath[4] = '\0';
    Level.Medium_Level_Container_Filepath[43] = '\0';
    Level.Easy_Level_Container_Filepath[4] = '\0';

    Level.Hard_Level_Text_Description[4] = '\0';
    Level.Medium_Level_Text_Description[9] = '\0';
    Level.Easy_Level_Text_Description[4] = '\0';





    Level.Hard_Level_Text_Description_Length = 5;
    Level.Medium_Level_Text_Description_Length = 10;
    Level.Easy_Level_Text_Description_Length = 5;

    Level.Hard_Level_Exit_Status = -1;
    Level.Medium_Level_Exit_Status = -1;
    Level.Easy_Level_Exit_Status = -1;

    Level.Hard_Level_ID = -1;
    Level.Medium_Level_ID = 100;
    Level.Easy_Level_ID = -1;








    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&List, 1, sizeof(struct _Level_List),f);

    fwrite(&Level, 1, sizeof(struct _Level_List_Level),f);
   // fwrite(&Level_Two, 1, sizeof(struct _Level_List_Level),f);
    //fwrite(&Level_Three, 1, sizeof(struct _Level_List_Level),f);
    fclose(f);


};

void Create_Level_List_Level_Container(const char* Filename)
{
    struct _Level_List_Level_Container Container;

    char Level_Name[256];
    char Level_Description[256];

    Container.Mission_Accomplished_Objective_Type = 0;
    Container.Mission_Accomplished_Category = 0;
    Container.Mission_Accomplished_Subcategory = 0;
    Container.Mission_Accomplished_ID = 0;
    Container.Mission_Accomplished_Variant = 0;
    Container.Mission_Accomplished_Value = 0;
    Container.Mission_Accomplished_Current_Value = 0;

    Container.Mission_Complete_Objective_Type = 0;
    Container.Mission_Complete_Category = 0;
    Container.Mission_Complete_Subcategory = 0;
    Container.Mission_Complete_ID = 0;
    Container.Mission_Complete_Variant = 0;
    Container.Mission_Complete_Value = 0;
    Container.Mission_Complete_Current_Value = 0;

    Container.Number_Of_Mission_Failed_Voice_Line_Filepaths = 0;
    Container.Number_Of_Mission_Failed_Voice_Lines = 0;

    Container.Number_Of_Steps = 2;


    int Size_Of_Mission_Failed_Voice_Line_Filepaths = Container.Number_Of_Mission_Failed_Voice_Line_Filepaths * sizeof(struct _Filepath);
    int Size_Of_Mission_Failed_Voice_Lines = Container.Number_Of_Mission_Failed_Voice_Lines * sizeof(struct _Filepath);
    int Size_Of_Steps = Container.Number_Of_Steps * sizeof(struct _Level_Container_Step);

    Container.Offset_To_Steps = sizeof(struct _Level_List_Level_Container);
    Container.Offset_To_Mission_Failed_Voice_Line_Filepaths=0;
    Container.Offset_To_Mission_Failed_Voice_Lines = 0;

    struct _Level_Container_Step Step;

    Step.Step_ID = 0;
    Step.Step_Type = LOAD_CUTSCENE; //Load Cutscene, Load Region, Go to Other Step, Go back to Overworld, Go to The End, Go to Title, Offer to Save Progress.
    Step.Skip_This_Step = 0;
    Step.Spawnpoint_ID = 0;

    memset(Step.Character_Filepath,0,256);
    memset(Step.Filepath,0,256);

    sprintf(Step.Character_Filepath, "resources/data/characters/default.chr");
    sprintf(Step.Filepath, "TESTCUTSCENE");

    Step.Character_Filepath[37] = '\0';
    Step.Filepath[12] = '\0';








    struct _Level_Container_Step StepTwo;

    StepTwo.Step_ID = 1;
    StepTwo.Step_Type = LOAD_REGION; //Load Cutscene, Load Region, Go to Other Step, Go back to Overworld, Go to The End, Go to Title, Offer to Save Progress.
    StepTwo.Skip_This_Step = 0;
    StepTwo.Spawnpoint_ID = 0;

    memset(StepTwo.Character_Filepath,0,256);
    memset(StepTwo.Filepath,0,256);

    sprintf(StepTwo.Character_Filepath, "resources/data/characters/default.chr");
    sprintf(StepTwo.Filepath, "resources/data/levels/final_arc/bay/bay_0.reg");

    StepTwo.Character_Filepath[37] = '\0';
    StepTwo.Filepath[45] = '\0';


    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Container, 1, sizeof(struct _Level_List_Level_Container),f);

    fwrite(&Step, 1, sizeof(struct _Level_Container_Step),f);
    fwrite(&StepTwo, 1, sizeof(struct _Level_Container_Step),f);
   // fwrite(&Level_Two, 1, sizeof(struct _Level_List_Level),f);
    //fwrite(&Level_Three, 1, sizeof(struct _Level_List_Level),f);
    fclose(f);
};



void Load_Arc_List(struct _Engine* Engine, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

    int Current_Language = Engine->Settings.Current_Language_ID;

    int Arc_List_Language_Location = Current_Language * sizeof(struct _Arc_List_Arc_Language_Entry);

    for(int j = 0; j < 256; j++)
    {
//        Arc_Banner_Filepath = Buffer[Arc_List_Language_Location+sizeof(struct _New_Encoding_String)+4+j];
    }
};



void Load_Level_List(struct _Engine* Engine, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

    Engine->Number_Of_Levels_Loaded = *(int*)&(Buffer[Buffer_Location]);
    int Offset_To_Levels = *(int*)&(Buffer[Buffer_Location+4]);

    Engine->Loaded_Levels = (struct _Level_List_Level*)calloc(1,Engine->Number_Of_Levels_Loaded*sizeof(struct _Level_List_Level));
    Engine->Overworld_State = Create_Overworld_State();
    Initialize_Overworld_State(Engine);
    Engine->Overworld_State->Level_Triples = (struct _CSS_Level_Triple_Object*)calloc(1,Engine->Number_Of_Levels_Loaded*sizeof(struct _CSS_Level_Triple_Object));

    for(int j = 0; j < Engine->Number_Of_Levels_Loaded; j++)
    {
        Buffer_Location = Offset_To_Levels + (j * sizeof(struct _Level_List_Level));

        Engine->Loaded_Levels[j].Hard_Level_Exists = *(int*)&(Buffer[Buffer_Location]);
        Engine->Loaded_Levels[j].Medium_Level_Exists = *(int*)&(Buffer[Buffer_Location+4]);
        Engine->Loaded_Levels[j].Easy_Level_Exists = *(int*)&(Buffer[Buffer_Location+8]);

        Buffer_Location += 12;

        for(int l = 0; l < 256; l++)
        {
        Engine->Loaded_Levels[j].Hard_Level_Icon_Filepath[l] = Buffer[(256*0)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Medium_Level_Icon_Filepath[l] = Buffer[(256*1)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Easy_Level_Icon_Filepath[l] = Buffer[(256*2)+Buffer_Location+l];

        Engine->Loaded_Levels[j].Hard_Level_Description_Filepath[l] = Buffer[(256*3)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Medium_Level_Description_Filepath[l] = Buffer[(256*4)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Easy_Level_Description_Filepath[l] = Buffer[(256*5)+Buffer_Location+l];

        Engine->Loaded_Levels[j].Hard_Level_Container_Filepath[l] = Buffer[(256*6)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Medium_Level_Container_Filepath[l] = Buffer[(256*7)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Easy_Level_Container_Filepath[l] = Buffer[(256*8)+Buffer_Location+l];

        Engine->Loaded_Levels[j].Hard_Level_Text_Description[l] = Buffer[(256*9)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Medium_Level_Text_Description[l] = Buffer[(256*10)+Buffer_Location+l];
        Engine->Loaded_Levels[j].Easy_Level_Text_Description[l] = Buffer[(256*11)+Buffer_Location+l];
        }

        Buffer_Location += 256*12;

        Engine->Loaded_Levels[j].Hard_Level_Text_Description_Length = *(int*)&(Buffer[Buffer_Location]);
        Engine->Loaded_Levels[j].Medium_Level_Text_Description_Length = *(int*)&(Buffer[Buffer_Location+4]);
        Engine->Loaded_Levels[j].Easy_Level_Text_Description_Length = *(int*)&(Buffer[Buffer_Location+8]);

        Engine->Loaded_Levels[j].Hard_Level_Exit_Status = *(int*)&(Buffer[Buffer_Location+12]);
        Engine->Loaded_Levels[j].Medium_Level_Exit_Status = *(int*)&(Buffer[Buffer_Location+16]);
        Engine->Loaded_Levels[j].Easy_Level_Exit_Status = *(int*)&(Buffer[Buffer_Location+20]);

        Engine->Loaded_Levels[j].Hard_Level_ID = *(int*)&(Buffer[Buffer_Location+24]);
        Engine->Loaded_Levels[j].Medium_Level_ID = *(int*)&(Buffer[Buffer_Location+28]);
        Engine->Loaded_Levels[j].Easy_Level_ID = *(int*)&(Buffer[Buffer_Location+32]);



        Engine->Overworld_State->Level_Triples[j].Hard_Mode_Image = Create_Image();
        Engine->Overworld_State->Level_Triples[j].Medium_Mode_Image = Create_Image();
        Engine->Overworld_State->Level_Triples[j].Easy_Mode_Image = Create_Image();

        Engine->Overworld_State->Level_Triples[j].Image_Descriptors[0] = Create_Image();
        Engine->Overworld_State->Level_Triples[j].Image_Descriptors[1] = Create_Image();
        Engine->Overworld_State->Level_Triples[j].Image_Descriptors[2] = Create_Image();

        Engine->Overworld_State->Level_Triples[j].Text_Descriptors[0] = Create_Image();
        Engine->Overworld_State->Level_Triples[j].Text_Descriptors[1] = Create_Image();
        Engine->Overworld_State->Level_Triples[j].Text_Descriptors[2] = Create_Image();


        Load_Image(Engine->Overworld_State->Level_Triples[j].Hard_Mode_Image, Engine->Loaded_Levels[j].Hard_Level_Icon_Filepath);
        Load_Image(Engine->Overworld_State->Level_Triples[j].Medium_Mode_Image, Engine->Loaded_Levels[j].Medium_Level_Icon_Filepath);
        Load_Image(Engine->Overworld_State->Level_Triples[j].Easy_Mode_Image, Engine->Loaded_Levels[j].Easy_Level_Icon_Filepath);

        Load_Image(Engine->Overworld_State->Level_Triples[j].Image_Descriptors[0], Engine->Loaded_Levels[j].Hard_Level_Description_Filepath);
        Load_Image(Engine->Overworld_State->Level_Triples[j].Image_Descriptors[1], Engine->Loaded_Levels[j].Medium_Level_Description_Filepath);
        Load_Image(Engine->Overworld_State->Level_Triples[j].Image_Descriptors[2], Engine->Loaded_Levels[j].Easy_Level_Description_Filepath);

        Initialize_Text_Image(Engine->Text_Engine, Engine->Overworld_State->Level_Triples[j].Text_Descriptors[0], Engine->Loaded_Levels[j].Hard_Level_Text_Description,Engine->Loaded_Levels[j].Hard_Level_Text_Description_Length,Engine->Loaded_Levels[j].Hard_Level_Text_Description_Length*17,24);
        Initialize_Text_Image(Engine->Text_Engine, Engine->Overworld_State->Level_Triples[j].Text_Descriptors[1], Engine->Loaded_Levels[j].Medium_Level_Text_Description,Engine->Loaded_Levels[j].Medium_Level_Text_Description_Length,Engine->Loaded_Levels[j].Medium_Level_Text_Description_Length*17,24);
        Initialize_Text_Image(Engine->Text_Engine, Engine->Overworld_State->Level_Triples[j].Text_Descriptors[2], Engine->Loaded_Levels[j].Easy_Level_Text_Description,Engine->Loaded_Levels[j].Easy_Level_Text_Description_Length,Engine->Loaded_Levels[j].Easy_Level_Text_Description_Length*17,24);
    }
}


void Load_Level_List_Level_Container(struct _Engine* Engine, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

    Engine->Loaded_Level_Container = (struct _Level_List_Level_Container*)calloc(1,sizeof(struct _Level_List_Level_Container));

    for(int l = 0; l < 256; l++)
    {
    Engine->Loaded_Level_Container->Level_Name[l] = Buffer[Buffer_Location+l];
    Engine->Loaded_Level_Container->Level_Description[l] = Buffer[Buffer_Location+l];
    }
    Buffer_Location += 512;

    Engine->Loaded_Level_Container->Mission_Accomplished_Objective_Type = *(int*)&(Buffer[Buffer_Location]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Category = *(int*)&(Buffer[Buffer_Location+4]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Subcategory = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->Loaded_Level_Container->Mission_Accomplished_ID = *(int*)&(Buffer[Buffer_Location+12]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Variant = *(int*)&(Buffer[Buffer_Location+16]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Value = *(float*)&(Buffer[Buffer_Location+20]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Current_Value = *(float*)&(Buffer[Buffer_Location+24]);

    Engine->Loaded_Level_Container->Mission_Complete_Objective_Type = *(int*)&(Buffer[Buffer_Location+28]);
    Engine->Loaded_Level_Container->Mission_Complete_Category = *(int*)&(Buffer[Buffer_Location+32]);
    Engine->Loaded_Level_Container->Mission_Complete_Subcategory = *(int*)&(Buffer[Buffer_Location+36]);
    Engine->Loaded_Level_Container->Mission_Complete_ID = *(int*)&(Buffer[Buffer_Location+40]);
    Engine->Loaded_Level_Container->Mission_Complete_Variant = *(int*)&(Buffer[Buffer_Location+44]);
    Engine->Loaded_Level_Container->Mission_Complete_Value = *(float*)&(Buffer[Buffer_Location+48]);
    Engine->Loaded_Level_Container->Mission_Complete_Current_Value = *(float*)&(Buffer[Buffer_Location+52]);

    Engine->Loaded_Level_Container->Number_Of_Mission_Failed_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location+56]);
    Engine->Loaded_Level_Container->Number_Of_Mission_Failed_Voice_Lines = *(int*)&(Buffer[Buffer_Location+60]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Failed_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location+64]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Failed_Voice_Lines = *(int*)&(Buffer[Buffer_Location+68]);
    Engine->Loaded_Level_Container->Number_Of_Steps = *(int*)&(Buffer[Buffer_Location+72]);
    Engine->Loaded_Level_Container->Offset_To_Steps = *(int*)&(Buffer[Buffer_Location+76]);

    printf("Number of Steps: %d\n", Engine->Loaded_Level_Container->Number_Of_Steps);

    Engine->Loaded_Steps = (struct _Level_Container_Step*)calloc(Engine->Loaded_Level_Container->Number_Of_Steps,sizeof(struct _Level_Container_Step));

    printf("Offset to steps: %d\n", Engine->Loaded_Level_Container->Offset_To_Steps);

    Buffer_Location += 80;

    for(int j = 0; j < Engine->Loaded_Level_Container->Number_Of_Steps; j++)
    {
        Buffer_Location = Engine->Loaded_Level_Container->Offset_To_Steps + (j * sizeof(struct _Level_Container_Step));

        Engine->Loaded_Steps[j].Step_ID = *(int*)&(Buffer[Buffer_Location]);
        Engine->Loaded_Steps[j].Step_Type = *(int*)&(Buffer[Buffer_Location+4]);
        Engine->Loaded_Steps[j].Skip_This_Step = *(int*)&(Buffer[Buffer_Location+8]);
        Engine->Loaded_Steps[j].Spawnpoint_ID = *(int*)&(Buffer[Buffer_Location+12]);

        printf("Filename: ");
        for(int l = 0; l < 256; l++)
        {
            Engine->Loaded_Steps[j].Character_Filepath[l] = Buffer[16+Buffer_Location+l];
            Engine->Loaded_Steps[j].Filepath[l] = Buffer[16+256+Buffer_Location+l];
            printf("%c",Engine->Loaded_Steps[j].Filepath[l]);
        }
        printf("\n");
        for(int l = 0; l < 256; l++)
        {
            printf("%c",Engine->Loaded_Steps[j].Character_Filepath[l]);
        }
    }
    printf("Level container loaded\n");
    Engine->Current_Step = 0;
    Process_Engine_Steps(Engine);
}

void Process_Engine_Steps(struct _Engine* Engine)
{
    switch(Engine->Loaded_Steps[Engine->Current_Step].Step_Type)
    {
        case LOAD_REGION:
            printf("Loading Region\n");
            Engine->On_Foot_State = Create_On_Foot_State();
            Initialize_On_Foot_State(Engine);
            Load_CSS_On_Foot_File_Header(Engine, Engine->Loaded_Steps[Engine->Current_Step].Filepath);
            Engine->Current_State = STATE_ON_FOOT;
        break;

        case LOAD_CUTSCENE:
            printf("Loading Cutscene\n");
            Engine->Cutscene_State = Create_Cutscene_State();
            Initialize_Cutscene_State(Engine);
            Load_Cutscene(Engine, Engine->Loaded_Steps[Engine->Current_Step].Filepath);
            Engine->Current_State = STATE_CUTSCENE;
        break;
    }
}

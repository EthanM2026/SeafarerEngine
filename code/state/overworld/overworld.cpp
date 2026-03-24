/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "overworld.h"
#include "../../engine/physics/physics.h"

struct _Overworld_State* Create_Overworld_State()
{
    struct _Overworld_State* I = (struct _Overworld_State*) calloc(1,sizeof (struct _Overworld_State));
    return I;
};
void Destroy_Overworld_State(struct _Engine* Engine)
{

};

void Initialize_Overworld_State(struct _Engine* Engine)
{
    Engine->Overworld_State->Go_Back_To_Arc_Select = false;
    //Assuming the Level file has been read
    Engine->Overworld_State->Enter_Pressed = false;
    Engine->Overworld_State->Timer = Create_Timer();
    Set_Timer(Engine->Overworld_State->Timer);
    Pause_Timer(Engine->Overworld_State->Timer);

    Engine->Overworld_State->Background = Create_Image();
    Engine->Overworld_State->Selector = Create_Image();

    Engine->Overworld_State->Green_Downwards = Create_Image();
    Engine->Overworld_State->Yellow_Downwards = Create_Image();

    Engine->Overworld_State->Green_Forwards = Create_Image();
    Engine->Overworld_State->Yellow_Forwards = Create_Image();
    Engine->Overworld_State->Red_Forwards = Create_Image();

    Engine->Overworld_State->Yellow_Upwards = Create_Image();
    Engine->Overworld_State->Red_Upwards = Create_Image();

    Engine->Overworld_State->Inaccessible_Level = Create_Image();


    Load_Image(Engine->Overworld_State->Background,"resources/artwork/gui/title_background.bmp");
    Load_Image(Engine->Overworld_State->Selector,"resources/artwork/gui/overworld_select.bmp");
    Load_Image(Engine->Overworld_State->Green_Downwards,"resources/artwork/gui/overworld_easy_down.bmp");
    Load_Image(Engine->Overworld_State->Yellow_Downwards,"resources/artwork/gui/overworld_medium_down.bmp");
    Load_Image(Engine->Overworld_State->Green_Forwards,"resources/artwork/gui/overworld_easy_forward.bmp");
    Load_Image(Engine->Overworld_State->Yellow_Forwards,"resources/artwork/gui/overworld_medium_forward.bmp");
    Load_Image(Engine->Overworld_State->Red_Forwards,"resources/artwork/gui/overworld_hard_forward.bmp");
    Load_Image(Engine->Overworld_State->Yellow_Upwards,"resources/artwork/gui/overworld_medium_up.bmp");
    Load_Image(Engine->Overworld_State->Red_Upwards,"resources/artwork/gui/overworld_hard_up.bmp");

    Load_Image(Engine->Overworld_State->Inaccessible_Level,"resources/artwork/gui/inaccessible_level.bmp");

    Engine->Overworld_State->Select_Level = Create_Image();
    Load_Image(Engine->Overworld_State->Select_Level, "resources/artwork/gui/overworld_select_ep.bmp");

    Engine->Overworld_State->Current_Choice = 100;
    Engine->Overworld_State->Selector_x = 48;
    Engine->Overworld_State->Selector_y = 180-64;
    glColor3f(1,1,1);


    Engine->Overworld_State->Enter_Pressed = false;

    Engine->Overworld_State->Times_Right_Pressed = 0;

    Engine->Overworld_State->Select = Mix_LoadWAV( "resources/tracks_and_sounds/general/select.wav" );

};



void Reinitialize_Overworld_State(struct _Engine* Engine)
{
    Engine->Overworld_State->Go_Back_To_Arc_Select = false;
    //Assuming the Level file has been read
    Engine->Overworld_State->Enter_Pressed = false;
    free(Engine->Overworld_State->Timer);
    Engine->Overworld_State->Timer = Create_Timer();
    Set_Timer(Engine->Overworld_State->Timer);
    Pause_Timer(Engine->Overworld_State->Timer);

    Engine->Overworld_State->Current_Choice = 100;
    Engine->Overworld_State->Selector_x = 48;
    Engine->Overworld_State->Selector_y = 180-64;
    glColor3f(1,1,1);
    glTranslatef(0,0,0);
    Engine->Overworld_State->Enter_Pressed = false;
    Engine->Overworld_State->Times_Right_Pressed = 0;
};

void Render_Overworld_State(struct _Engine* Engine)
{
    //printf("RENDERING");
    if(!Engine->Overworld_State->Enter_Pressed)
    {

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Render_Image(Engine->Overworld_State->Background,0,0,1);
    glPopMatrix();

    int Level_X = 64;
    for(int j = 0; j < Engine->Number_Of_Levels; j++)
    {
        if(Engine->Loaded_Levels[j].Hard_Level_Exists)
        {
            switch(Engine->Loaded_Levels[j].Hard_Level_Exit_Status)
            {
                case LEVEL_CLEARED_GREEN_DOWNWARDS:
                Render_Image(Engine->Overworld_State->Green_Downwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_DOWNWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Downwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_GREEN_FORWARDS:
                Render_Image(Engine->Overworld_State->Green_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_FORWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_RED_FORWARDS:
                Render_Image(Engine->Overworld_State->Red_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_UPWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Upwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_RED_UPWARDS:
                Render_Image(Engine->Overworld_State->Red_Upwards,Level_X,132,1);
                break;
            }
            if(Engine->Loaded_Levels[j].Hard_Level_Exists == LEVEL_ACCESSIBLE)
            {
                Render_Image(Engine->Overworld_State->Level_Triples[j].Hard_Mode_Image,Level_X,8,1);
            }
            else if(Engine->Loaded_Levels[j].Hard_Level_Exists == LEVEL_NOT_ACCESSIBLE)
            {
                Render_Image(Engine->Overworld_State->Inaccessible_Level,Level_X,8,1);
            }
        }

        if(Engine->Loaded_Levels[j].Medium_Level_Exists)
        {
            switch(Engine->Loaded_Levels[j].Medium_Level_Exit_Status)
            {
                case LEVEL_CLEARED_GREEN_DOWNWARDS:
                Render_Image(Engine->Overworld_State->Green_Downwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_DOWNWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Downwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_GREEN_FORWARDS:
                Render_Image(Engine->Overworld_State->Green_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_FORWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_RED_FORWARDS:
                Render_Image(Engine->Overworld_State->Red_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_UPWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Upwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_RED_UPWARDS:
                Render_Image(Engine->Overworld_State->Red_Upwards,Level_X,132,1);
                break;
            }
            if(Engine->Loaded_Levels[j].Medium_Level_Exists == LEVEL_ACCESSIBLE)
            {
                Render_Image(Engine->Overworld_State->Level_Triples[j].Medium_Mode_Image,Level_X,132,1);
            }
            else if(Engine->Loaded_Levels[j].Medium_Level_Exists == LEVEL_NOT_ACCESSIBLE)
            {
                Render_Image(Engine->Overworld_State->Inaccessible_Level,Level_X,132,1);
            }
        }

        if(Engine->Loaded_Levels[j].Easy_Level_Exists)
        {
            switch(Engine->Loaded_Levels[j].Easy_Level_Exit_Status)
            {
                case LEVEL_CLEARED_GREEN_DOWNWARDS:
                Render_Image(Engine->Overworld_State->Green_Downwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_DOWNWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Downwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_GREEN_FORWARDS:
                Render_Image(Engine->Overworld_State->Green_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_FORWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_RED_FORWARDS:
                Render_Image(Engine->Overworld_State->Red_Forwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_YELLOW_UPWARDS:
                Render_Image(Engine->Overworld_State->Yellow_Upwards,Level_X,132,1);
                break;

                case LEVEL_CLEARED_RED_UPWARDS:
                Render_Image(Engine->Overworld_State->Red_Upwards,Level_X,132,1);
                break;
            }
            if(Engine->Loaded_Levels[j].Easy_Level_Exists == LEVEL_ACCESSIBLE)
            {
                Render_Image(Engine->Overworld_State->Level_Triples[j].Easy_Mode_Image,Level_X,228+28,1);
            }
            else if(Engine->Loaded_Levels[j].Easy_Level_Exists == LEVEL_NOT_ACCESSIBLE)
            {
                Render_Image(Engine->Overworld_State->Inaccessible_Level,Level_X,228+28,1);
            }
        }
        Level_X += 96*2;
    }


    Render_Image(Engine->Overworld_State->Selector,Engine->Overworld_State->Selector_x,Engine->Overworld_State->Selector_y,1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Render_Image(Engine->Overworld_State->Select_Level,640-228-8,360-64,1);
    glPopMatrix();
    }

};
void Input_Overworld_State(struct _Engine* Engine, struct _Keypad Keypad)
{
   // printf("Current Choice: %d\n", Engine->Overworld_State->Current_Choice);
    if(Keypad.Down && Engine->Overworld_State->Down_Not_Pressed && !Engine->Overworld_State->Enter_Pressed)
    {
        if(Engine->Overworld_State->Current_Choice - 100 >= 0)
        {
        Engine->Overworld_State->Selector_y += 124;
        Engine->Overworld_State->Current_Choice -= 100;
        Engine->Overworld_State->Down_Not_Pressed = false;
        Mix_PlayChannel( -1, Engine->Main_Menu_State->Select, 0 );
        }
    }
    if(Keypad.Up && Engine->Overworld_State->Up_Not_Pressed && !Engine->Overworld_State->Enter_Pressed)
    {
        if(Engine->Overworld_State->Current_Choice + 100 < 300)
        {
        Engine->Overworld_State->Selector_y -= 124;
        Engine->Overworld_State->Current_Choice += 100;
        Engine->Overworld_State->Up_Not_Pressed = false;
        Mix_PlayChannel( -1, Engine->Main_Menu_State->Select, 0 );
        }
    }
    if(!Keypad.Up)
    {
        Engine->Overworld_State->Up_Not_Pressed = true;
    }
    if(!Keypad.Down)
    {
        Engine->Overworld_State->Down_Not_Pressed = true;
    }
    if(Keypad.Right && Engine->Overworld_State->Right_Not_Pressed && !Engine->Overworld_State->Enter_Pressed)
    {
        Engine->Overworld_State->Selector_x += 192;
        Engine->Overworld_State->Current_Choice += 1;
        Engine->Overworld_State->Right_Not_Pressed = false;
        Engine->Overworld_State->Times_Right_Pressed += 1;
        Mix_PlayChannel( -1, Engine->Main_Menu_State->Select, 0 );
        glTranslatef(-192,0,0);

    }
    if(Keypad.Left && Engine->Overworld_State->Left_Not_Pressed && !Engine->Overworld_State->Enter_Pressed)
    {
        if(Engine->Overworld_State->Selector_x - 192 >= 0)
        {
        Engine->Overworld_State->Selector_x -= 192;
        Engine->Overworld_State->Current_Choice -= 1;
        Engine->Overworld_State->Left_Not_Pressed = false;
        Engine->Overworld_State->Times_Right_Pressed -= 1;
        Mix_PlayChannel( -1, Engine->Main_Menu_State->Select, 0 );
        glTranslatef(192,0,0);
        }
    }






    if(!Keypad.Left)
    {
        Engine->Overworld_State->Left_Not_Pressed = true;
    }
    if(!Keypad.Right)
    {
        Engine->Overworld_State->Right_Not_Pressed = true;
    }


    if(Keypad.Start && !Engine->Overworld_State->Enter_Pressed)
    {
        for(int j = 0; j < Engine->Number_Of_Levels; j++)
        {
        if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice && Engine->Loaded_Levels[j].Easy_Level_Exists == LEVEL_ACCESSIBLE)
        {
            Resume_Timer(Engine->Overworld_State->Timer);
            Engine->Overworld_State->Enter_Pressed = true;
        }

        else if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice && Engine->Loaded_Levels[j].Medium_Level_Exists == LEVEL_ACCESSIBLE)
        {
            Resume_Timer(Engine->Overworld_State->Timer);
            Engine->Overworld_State->Enter_Pressed = true;
        }

        else if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice && Engine->Loaded_Levels[j].Hard_Level_Exists == LEVEL_ACCESSIBLE)
        {
            Resume_Timer(Engine->Overworld_State->Timer);
            Engine->Overworld_State->Enter_Pressed = true;
        }
        }
    }

    if(Keypad.B)
    {
        Engine->Overworld_State->Go_Back_To_Arc_Select = true;
        glColor3f(0,0,0);
        Resume_Timer(Engine->Overworld_State->Timer);
    }

};
void Process_Overworld_State(struct _Engine* Engine)
{
 //   printf("Current Choice: %d\n", Engine->Overworld_State->Current_Choice);
    float x = Get_Ticks(Engine->Overworld_State->Timer);
    if(x > 2 && Engine->Overworld_State->Go_Back_To_Arc_Select)
    {
        free(Engine->Arc_Select_State);
        Engine->Arc_Select_State = Create_Arc_Select_State();
        Initialize_Arc_Select_State(Engine);
        glColor3f(1,1,1);
        Engine->Current_State = STATE_ARC_SELECT;
    }

    if(x > 5 && !Engine->Overworld_State->Go_Back_To_Arc_Select)
    {
        for(int j = 0; j < Engine->Number_Of_Levels; j++)
        {
            if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice && Engine->Loaded_Levels[j].Easy_Level_Exists == LEVEL_ACCESSIBLE)
            {

                Resume_Timer(Engine->Overworld_State->Timer);
                Engine->Overworld_State->Enter_Pressed = true;
                for(int l = 0; l < 256; l++)
                {
                    Engine->Catted_Root_Directory[l] = Engine->Current_Root_Directory[l];
                }

                strcat(Engine->Catted_Root_Directory, Engine->Loaded_Levels[j].Easy_Level_Container_Filepath);
                printf("I will load ");
                printf(Engine->Catted_Root_Directory);
                printf(".\n");
                Load_Level_List_Level_Container(Engine, Engine->Catted_Root_Directory);
            }

            if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice && Engine->Loaded_Levels[j].Hard_Level_Exists == LEVEL_ACCESSIBLE)
            {

                 Resume_Timer(Engine->Overworld_State->Timer);
                Engine->Overworld_State->Enter_Pressed = true;
                for(int l = 0; l < 256; l++)
                {
                    Engine->Catted_Root_Directory[l] = Engine->Current_Root_Directory[l];
                }

                strcat(Engine->Catted_Root_Directory, Engine->Loaded_Levels[j].Hard_Level_Container_Filepath);
                printf("I will load ");
                printf(Engine->Catted_Root_Directory);
                printf(".\n");
                Load_Level_List_Level_Container(Engine, Engine->Catted_Root_Directory);
            }

            if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice && Engine->Loaded_Levels[j].Medium_Level_Exists == LEVEL_ACCESSIBLE)
            {

                 Resume_Timer(Engine->Overworld_State->Timer);
                Engine->Overworld_State->Enter_Pressed = true;
                for(int l = 0; l < 256; l++)
                {
                    Engine->Catted_Root_Directory[l] = Engine->Current_Root_Directory[l];
                }

                strcat(Engine->Catted_Root_Directory, Engine->Loaded_Levels[j].Medium_Level_Container_Filepath);
                printf("I will load ");
                printf(Engine->Catted_Root_Directory);
                printf(".\n");
                Load_Level_List_Level_Container(Engine, Engine->Catted_Root_Directory);
            }
        }
    }

};

void Load_Level_List_Level_Container(struct _Engine* Engine, const char* Filename)
{
    Engine->Loaded_Level_Container = (struct _Level_List_Level_Container*)calloc(1,sizeof(struct _Level_List_Level_Container));

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

    for(int j = 0; j < 256; j++)
    {
        Engine->Loaded_Level_Container->Level_Name[j] = Buffer[j];
        Engine->Loaded_Level_Container->Level_Description[j] = Buffer[j + 256];
    }

    Buffer_Location = 512;

    Engine->Loaded_Level_Container->Mission_Accomplished_Objective_Type = *(int*)&(Buffer[Buffer_Location + 0]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Category = *(int*)&(Buffer[Buffer_Location + 4]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Subcategory = *(int*)&(Buffer[Buffer_Location + 8]);
    Engine->Loaded_Level_Container->Mission_Accomplished_ID = *(int*)&(Buffer[Buffer_Location + 12]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Variant = *(int*)&(Buffer[Buffer_Location + 16]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Value = *(int*)&(Buffer[Buffer_Location + 20]);
    Engine->Loaded_Level_Container->Mission_Accomplished_Current_Value = *(int*)&(Buffer[Buffer_Location + 24]);

    Buffer_Location += 28;


    Engine->Loaded_Level_Container->Mission_Complete_Objective_Type = *(int*)&(Buffer[Buffer_Location + 0]);
    Engine->Loaded_Level_Container->Mission_Complete_Category = *(int*)&(Buffer[Buffer_Location + 4]);
    Engine->Loaded_Level_Container->Mission_Complete_Subcategory = *(int*)&(Buffer[Buffer_Location + 8]);
    Engine->Loaded_Level_Container->Mission_Complete_ID = *(int*)&(Buffer[Buffer_Location + 12]);
    Engine->Loaded_Level_Container->Mission_Complete_Variant = *(int*)&(Buffer[Buffer_Location + 16]);
    Engine->Loaded_Level_Container->Mission_Complete_Value = *(int*)&(Buffer[Buffer_Location + 20]);
    Engine->Loaded_Level_Container->Mission_Complete_Current_Value = *(int*)&(Buffer[Buffer_Location + 24]);

    Buffer_Location += 28;



    Engine->Loaded_Level_Container->Debriefing_Type = *(int*)&(Buffer[Buffer_Location + 0]);

    Engine->Loaded_Level_Container->Number_Of_Steps = *(int*)&(Buffer[Buffer_Location + 4]);
    Engine->Loaded_Level_Container->Offset_To_Steps = *(int*)&(Buffer[Buffer_Location + 8]);

    Engine->Loaded_Level_Container->Select_Random_Mission_Accomplished_Line = *(int*)&(Buffer[Buffer_Location + 12]); //Or play in sequence.
    Engine->Loaded_Level_Container->Select_Random_Mission_Complete_Line = *(int*)&(Buffer[Buffer_Location + 16]); //Or play in sequence.
    Engine->Loaded_Level_Container->Select_Random_Mission_Failed_Line = *(int*)&(Buffer[Buffer_Location + 20]); //Or play in sequence.

    Engine->Loaded_Level_Container->Number_Of_Mission_Accomplished_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 24]);
    Engine->Loaded_Level_Container->Number_Of_Mission_Accomplished_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 28]);
    Engine->Loaded_Level_Container->Number_Of_Mission_Complete_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 32]);
    Engine->Loaded_Level_Container->Number_Of_Mission_Complete_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 36]);
    Engine->Loaded_Level_Container->Number_Of_Mission_Failed_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 40]);
    Engine->Loaded_Level_Container->Number_Of_Mission_Failed_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 44]);

    Engine->Loaded_Level_Container->Offset_To_Mission_Accomplished_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 48]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Accomplished_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 52]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Complete_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 56]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Complete_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 60]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Failed_Voice_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 64]);
    Engine->Loaded_Level_Container->Offset_To_Mission_Failed_Line_Filepaths = *(int*)&(Buffer[Buffer_Location + 68]);

    printf(Engine->Loaded_Level_Container->Level_Name);
    printf("\n");
    printf(Engine->Loaded_Level_Container->Level_Description);
    printf("\n");
    printf("MISSION OBJECTIVE: %d\n", Engine->Loaded_Level_Container->Mission_Accomplished_Objective_Type);

    printf("There are %d steps.\n",Engine->Loaded_Level_Container->Number_Of_Steps);

    Engine->Loaded_Steps = (struct _Level_Container_Step*)calloc(Engine->Loaded_Level_Container->Number_Of_Steps,sizeof(struct _Level_Container_Step));

    for(int l = 0; l < Engine->Loaded_Level_Container->Number_Of_Steps; l++)
    {
        Buffer_Location = Engine->Loaded_Level_Container->Offset_To_Steps + (l * sizeof(struct _Level_Container_Step));

        Engine->Loaded_Steps[l].Step_ID = *(int*)&(Buffer[Buffer_Location + 0]);
        Engine->Loaded_Steps[l].Step_Type = *(int*)&(Buffer[Buffer_Location + 4]);
        Engine->Loaded_Steps[l].Skip_This_Step = *(int*)&(Buffer[Buffer_Location + 8]);
        Engine->Loaded_Steps[l].Spawnpoint_ID = *(int*)&(Buffer[Buffer_Location + 12]);

        for(int m = 0; m < 256; m++)
        {
            Engine->Loaded_Steps[l].Character_Filepath[m] = Buffer[Buffer_Location + 16 + m];
            Engine->Loaded_Steps[l].Filepath[m] = Buffer[Buffer_Location + 16 + m + 256];
        }
        printf("The character filepath is ");
        printf(Engine->Loaded_Steps[l].Character_Filepath);
        printf("\n");
        printf("The filepath is ");
        printf(Engine->Loaded_Steps[l].Filepath);
        printf("\n");

        char Region_True_Filepath[256];
        for(int m = 0; m < 256; m++)
        {
            Region_True_Filepath[m] = Engine->Current_Root_Directory[m];
        }
        strcat(Region_True_Filepath,Engine->Loaded_Steps[l].Filepath);

        printf("I will load ");
        printf(Region_True_Filepath);
        printf(".\n");

        for(int m = 0; m < 256; m++)
        {
            Engine->Current_Region_Filepath[m] = Region_True_Filepath[m];
        }

        printf("Current Region Filepath: ");
        printf(Engine->Current_Region_Filepath);
        printf(".\n");

        Engine->Current_Step = 0;
        New_Process_Steps(Engine);
    }
}

void New_Process_Steps(struct _Engine* Engine)
{
    switch(Engine->Loaded_Steps[Engine->Current_Step].Step_Type)
    {
        case STEP_TYPE_LOAD_STAGE:
            printf("Loading Region\n");
            Engine->On_Foot_State = Create_On_Foot_State();
            Initialize_On_Foot_State(Engine);
            //Load_Region(Engine, Engine->Current_Region_Filepath);
            Engine->Current_State = STATE_ON_FOOT;
        break;

        //case LOAD_CUTSCENE:
        //    printf("Loading Cutscene\n");
       //     Engine->Cutscene_State = Create_Cutscene_State();
       //     Initialize_Cutscene_State(Engine);
       //     Load_Cutscene(Engine, Engine->Loaded_Steps[Engine->Current_Step].Filepath);
        //    Engine->Current_State = STATE_CUTSCENE;
       // break;
    }
}

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


    Load_Image(Engine->Overworld_State->Background,"resources/artwork/gui/title_background.bmp");
    Load_Image(Engine->Overworld_State->Selector,"resources/artwork/gui/overworld_select.bmp");
    Load_Image(Engine->Overworld_State->Green_Downwards,"resources/artwork/gui/overworld_easy_down.bmp");
    Load_Image(Engine->Overworld_State->Yellow_Downwards,"resources/artwork/gui/overworld_medium_down.bmp");
    Load_Image(Engine->Overworld_State->Green_Forwards,"resources/artwork/gui/overworld_easy_forward.bmp");
    Load_Image(Engine->Overworld_State->Yellow_Forwards,"resources/artwork/gui/overworld_medium_forward.bmp");
    Load_Image(Engine->Overworld_State->Red_Forwards,"resources/artwork/gui/overworld_hard_forward.bmp");
    Load_Image(Engine->Overworld_State->Yellow_Upwards,"resources/artwork/gui/overworld_medium_up.bmp");
    Load_Image(Engine->Overworld_State->Red_Upwards,"resources/artwork/gui/overworld_hard_up.bmp");

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
    for(int j = 0; j < Engine->Number_Of_Levels_Loaded; j++)
    {
        if(Engine->Loaded_Levels[j].Hard_Level_Exists)
        {
            switch(Engine->Loaded_Levels[j].Hard_Level_Exit_Status)
            {
                case 0:
                Render_Image(Engine->Overworld_State->Green_Downwards,Level_X,132,1);
                break;

                case 1:
                Render_Image(Engine->Overworld_State->Yellow_Downwards,Level_X,132,1);
                break;

                case 2:
                Render_Image(Engine->Overworld_State->Green_Forwards,Level_X,132,1);
                break;

                case 3:
                Render_Image(Engine->Overworld_State->Yellow_Forwards,Level_X,132,1);
                break;

                case 4:
                Render_Image(Engine->Overworld_State->Red_Forwards,Level_X,132,1);
                break;

                case 5:
                Render_Image(Engine->Overworld_State->Yellow_Upwards,Level_X,132,1);
                break;

                case 6:
                Render_Image(Engine->Overworld_State->Red_Upwards,Level_X,132,1);
                break;
            }
            Render_Image(Engine->Overworld_State->Level_Triples[j].Hard_Mode_Image,Level_X,8,1);
        }

        if(Engine->Loaded_Levels[j].Medium_Level_Exists)
        {
            switch(Engine->Loaded_Levels[j].Medium_Level_Exit_Status)
            {
                case 0:
                Render_Image(Engine->Overworld_State->Green_Downwards,Level_X,132,1);
                break;

                case 1:
                Render_Image(Engine->Overworld_State->Yellow_Downwards,Level_X,132,1);
                break;

                case 2:
                Render_Image(Engine->Overworld_State->Green_Forwards,Level_X,132,1);
                break;

                case 3:
                Render_Image(Engine->Overworld_State->Yellow_Forwards,Level_X,132,1);
                break;

                case 4:
                Render_Image(Engine->Overworld_State->Red_Forwards,Level_X,132,1);
                break;

                case 5:
                Render_Image(Engine->Overworld_State->Yellow_Upwards,Level_X,132,1);
                break;

                case 6:
                Render_Image(Engine->Overworld_State->Red_Upwards,Level_X,132,1);
                break;
            }
            Render_Image(Engine->Overworld_State->Level_Triples[j].Medium_Mode_Image,Level_X,132,1);
        }

        if(Engine->Loaded_Levels[j].Easy_Level_Exists)
        {
            switch(Engine->Loaded_Levels[j].Easy_Level_Exit_Status)
            {
                case 0:
                Render_Image(Engine->Overworld_State->Green_Downwards,Level_X,132,1);
                break;

                case 1:
                Render_Image(Engine->Overworld_State->Yellow_Downwards,Level_X,132,1);
                break;

                case 2:
                Render_Image(Engine->Overworld_State->Green_Forwards,Level_X,132,1);
                break;

                case 3:
                Render_Image(Engine->Overworld_State->Yellow_Forwards,Level_X,132,1);
                break;

                case 4:
                Render_Image(Engine->Overworld_State->Red_Forwards,Level_X,132,1);
                break;

                case 5:
                Render_Image(Engine->Overworld_State->Yellow_Upwards,Level_X,132,1);
                break;

                case 6:
                Render_Image(Engine->Overworld_State->Red_Upwards,Level_X,132,1);
                break;
            }
            Render_Image(Engine->Overworld_State->Level_Triples[j].Easy_Mode_Image,Level_X,228+28,1);
        }
        Level_X += 96*2;
    }


    Render_Image(Engine->Overworld_State->Selector,Engine->Overworld_State->Selector_x,Engine->Overworld_State->Selector_y,1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Render_Image(Engine->Overworld_State->Select_Level,8,8,1);
    glPopMatrix();
    }

};
void Input_Overworld_State(struct _Engine* Engine, struct _Keypad Keypad)
{
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
    else if(Keypad.Start && Engine->Overworld_State->Current_Choice == 1)
    {
        //Select
        printf("Select\n");
    }

    if(Keypad.Start && !Engine->Overworld_State->Enter_Pressed)
    {
        Resume_Timer(Engine->Overworld_State->Timer);
        Engine->Overworld_State->Enter_Pressed = true;
    }



};
void Process_Overworld_State(struct _Engine* Engine)
{
   // printf("Current Choice: %d\n", Engine->Overworld_State->Current_Choice);
    float x = Get_Ticks(Engine->Overworld_State->Timer);
    if(x > 5)
    {
        for(int j = 0; j < Engine->Number_Of_Levels_Loaded; j++)
        {
            if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice)
            {

                        Resume_Timer(Engine->Overworld_State->Timer);
        Engine->Overworld_State->Enter_Pressed = true;
        Load_Level_List_Level_Container(Engine, Engine->Loaded_Levels[j].Easy_Level_Container_Filepath);
            }

            if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice)
            {

                        Resume_Timer(Engine->Overworld_State->Timer);
        Engine->Overworld_State->Enter_Pressed = true;
        Load_Level_List_Level_Container(Engine, Engine->Loaded_Levels[j].Hard_Level_Container_Filepath);
            }

            if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice)
            {

                        Resume_Timer(Engine->Overworld_State->Timer);
        Engine->Overworld_State->Enter_Pressed = true;
         Load_Level_List_Level_Container(Engine, Engine->Loaded_Levels[j].Medium_Level_Container_Filepath);
            }
        }
    }

};

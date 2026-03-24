/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "mainmenu.h"
#include "stdlib.h"
#include <SDL2/SDL_mixer.h>

struct _Main_Menu_State* Create_Main_Menu_State()
{
    struct _Main_Menu_State* I = (struct _Main_Menu_State*) calloc(1,sizeof (struct _Main_Menu_State));
    return I;
}

void Initialize_Main_Menu_State(struct _Engine* Engine)
{
    Engine->Main_Menu_State->Sound_Bar_Selected = Create_Image();
    Load_Image(Engine->Main_Menu_State->Sound_Bar_Selected, "resources/artwork/gui/main_menu_soundmeter_selected.bmp");


    Engine->Main_Menu_State->Sound = Create_Image();
    Load_Image(Engine->Main_Menu_State->Sound, "resources/artwork/gui/main_menu_soundmeter.bmp");
    Engine->Main_Menu_State->Sound_Level = Create_Image();
    Load_Image(Engine->Main_Menu_State->Sound_Level, "resources/artwork/gui/main_menu_soundlevel.bmp");
    Engine->Main_Menu_State->Main_Menu = Create_Image();
    Load_Image(Engine->Main_Menu_State->Main_Menu, "resources/artwork/gui/mainmenu.bmp");
    Engine->Main_Menu_State->Mini_State = 1;

    Engine->Main_Menu_State->Sound_x = 176;
    Engine->Main_Menu_State->SFX_x = 176;
    Engine->Main_Menu_State->Timer = Create_Timer();
    Set_Timer(Engine->Main_Menu_State->Timer);
    Pause_Timer(Engine->Main_Menu_State->Timer);

    Engine->Main_Menu_State->Selector_Y = 52;

    Engine->Main_Menu_State->Current_Choice = 0;

    Engine->Main_Menu_State->Selector = Create_Image();
    Load_Image(Engine->Main_Menu_State->Selector, "resources/artwork/gui/gui_selector_right.bmp");


    Engine->Main_Menu_State->Background = Create_Image();
    Load_Image(Engine->Main_Menu_State->Background, "resources/artwork/gui/title_background.bmp");


    Engine->Main_Menu_State->Episode_Select = Create_Image();
    Engine->Main_Menu_State->Story_Mode = Create_Image();
    Engine->Main_Menu_State->Options = Create_Image();
    Engine->Main_Menu_State->Story_Mode_Inverted = Create_Image();
    Engine->Main_Menu_State->Episode_Select_Inverted = Create_Image();
    Engine->Main_Menu_State->Options_Inverted = Create_Image();
    Engine->Main_Menu_State->Change_Language_Selected = Create_Image();
    Engine->Main_Menu_State->Language = Create_Image();

    Engine->Main_Menu_State->Music_Description = Create_Image();
    Engine->Main_Menu_State->SFX_Description = Create_Image();
    Engine->Main_Menu_State->Title_Bar = Create_Image();


    switch(Engine->Settings.Current_Language_ID)
    {
        case ID_LANGUAGE_ENGLISH:
            Load_Image(Engine->Main_Menu_State->Episode_Select, "resources/artwork/gui/english/main_menu_multiplayer.bmp");
            Load_Image(Engine->Main_Menu_State->Story_Mode, "resources/artwork/gui/english/main_menu_story.bmp");
            Load_Image(Engine->Main_Menu_State->Options, "resources/artwork/gui/english/main_menu_options.bmp");
            Load_Image(Engine->Main_Menu_State->Story_Mode_Inverted, "resources/artwork/gui/english/main_menu_story_inverted.bmp");
            Load_Image(Engine->Main_Menu_State->Episode_Select_Inverted, "resources/artwork/gui/english/main_menu_multiplayer_inverted.bmp");
            Load_Image(Engine->Main_Menu_State->Options_Inverted, "resources/artwork/gui/english/main_menu_options_inverted.bmp");
            Load_Image(Engine->Main_Menu_State->Change_Language_Selected, "resources/artwork/gui/english/main_menu_change_language_highlighted.bmp");
            Load_Image(Engine->Main_Menu_State->Language, "resources/artwork/gui/english/main_menu_change_language.bmp");
            Load_Image(Engine->Main_Menu_State->Music_Description, "resources/artwork/gui/english/main_menu_music_volume.bmp");
            Load_Image(Engine->Main_Menu_State->SFX_Description, "resources/artwork/gui/english/main_menu_sfx_volume.bmp");
            Load_Image(Engine->Main_Menu_State->Title_Bar, "resources/artwork/gui/english/main_menu_title_bar.bmp");
        break;
    }

    Engine->Main_Menu_State->Up_Not_Pressed = false;
    Engine->Main_Menu_State->Down_Not_Pressed = false;

    Engine->Main_Menu_State->Enter_Pressed = false;

    Engine->Main_Menu_State->Select = Mix_LoadWAV( "resources/tracks_and_sounds/general/select.wav" );

    Engine->Main_Menu_State->Track = Mix_LoadMUS( "resources/tracks_and_sounds/general/mainmenu.wav" );
    Mix_PlayMusic(Engine->Main_Menu_State->Track, -1);


   // Engine->Main_Menu_State->Message = Create_Message_Box();
    char Text[256];
   // Initialize_Message_Box(Engine->Main_Menu_State->Message,Engine->Text_Engine,UNDERSTOOD,Text);






    Engine->Main_Menu_State->Brightness = 0;


    Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen = true;
    Engine->Main_Menu_State->Multi_Player_Mode_Button_Is_Off_Screen = true;
    Engine->Main_Menu_State->Options_Button_Is_Off_Screen = true;

    Engine->Main_Menu_State->Single_Player_Mode_Button_X = 176 + 640;
    Engine->Main_Menu_State->Multi_Player_Mode_Button_X = 176 + 640;
    Engine->Main_Menu_State->Options_Button_X = 176 + 640;

    Engine->Main_Menu_State->Go_Back_To_Title = false;

    Engine->Main_Menu_State->Returned_From_On_Foot = false;
}

void Destroy_Main_Menu_State(struct _Main_Menu_State* Main_Menu_State)
{

}

void Render_Main_Menu_State(struct _Engine* Engine)
{
    if(Engine->Main_Menu_State->Returned_From_On_Foot)
    {
        free(Engine->On_Foot_State);
        Engine->Main_Menu_State->Returned_From_On_Foot = false;
    }

    if(Engine->Main_Menu_State->Brightness < 1 && !Engine->Main_Menu_State->Enter_Pressed)
    {
        Engine->Main_Menu_State->Brightness +=0.1;
        glColor3f(Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness);
    }

//if(!Engine->Main_Menu_State->Enter_Pressed)
    Render_Image(Engine->Main_Menu_State->Background, 0,-150,1);

    if(Engine->Main_Menu_State->Mini_State == 1)
    {
        if(Engine->Main_Menu_State->Current_Choice == 0)
        {
        Render_Image(Engine->Main_Menu_State->Story_Mode_Inverted,Engine->Main_Menu_State->Single_Player_Mode_Button_X,32+16,1);
        }
        if(Engine->Main_Menu_State->Current_Choice == 1 || Engine->Main_Menu_State->Current_Choice == 2)
        {
        Render_Image(Engine->Main_Menu_State->Story_Mode,Engine->Main_Menu_State->Single_Player_Mode_Button_X,32+16,1);
        }


        if(Engine->Main_Menu_State->Current_Choice == 1)
        {
        Render_Image(Engine->Main_Menu_State->Episode_Select_Inverted,Engine->Main_Menu_State->Multi_Player_Mode_Button_X,128+16,1);
        }
        if(Engine->Main_Menu_State->Current_Choice == 0 || Engine->Main_Menu_State->Current_Choice == 2)
        {
        Render_Image(Engine->Main_Menu_State->Episode_Select,Engine->Main_Menu_State->Multi_Player_Mode_Button_X,128+16,1);
        }



        if(Engine->Main_Menu_State->Current_Choice == 2)
        {
        Render_Image(Engine->Main_Menu_State->Options_Inverted,Engine->Main_Menu_State->Options_Button_X,224+16,1);
        }
        if(Engine->Main_Menu_State->Current_Choice == 0 || Engine->Main_Menu_State->Current_Choice == 1)
        {
        Render_Image(Engine->Main_Menu_State->Options,Engine->Main_Menu_State->Options_Button_X,224+16,1);
        }
  //      Render_Image(Engine->Main_Menu_State->Main_Menu,320 - (52),16,1);
    }

    if(Engine->Main_Menu_State->Mini_State == 2)
    {
        if(Engine->Main_Menu_State->Current_Choice == 0)
        {
        Render_Image(Engine->Main_Menu_State->Sound_Bar_Selected,176,32+20,1);
        }
        else if(Engine->Main_Menu_State->Current_Choice != 0)
        {
        Render_Image(Engine->Main_Menu_State->Sound,176,32+20,1);
        }



        Render_Image(Engine->Main_Menu_State->Sound_Level,Engine->Main_Menu_State->Sound_x,32,1);
        Render_Image(Engine->Main_Menu_State->Music_Description,32,32+20,1);


        if(Engine->Main_Menu_State->Current_Choice == 1)
        {
        Render_Image(Engine->Main_Menu_State->Sound_Bar_Selected,176,128+20,1);
        }
        else if(Engine->Main_Menu_State->Current_Choice != 1)
        {
        Render_Image(Engine->Main_Menu_State->Sound,176,128+20,1);
        }

        Render_Image(Engine->Main_Menu_State->Sound_Level,Engine->Main_Menu_State->SFX_x,128,1);
        Render_Image(Engine->Main_Menu_State->SFX_Description,32,128+20,1);


        if(Engine->Main_Menu_State->Current_Choice == 2)
        {
            Render_Image(Engine->Main_Menu_State->Change_Language_Selected,176,224,1);
        }
        else if(Engine->Main_Menu_State->Current_Choice != 2)
        {
            Render_Image(Engine->Main_Menu_State->Language,176,224,1);
        }


    }

    Render_Image(Engine->Main_Menu_State->Title_Bar,0,0,1);



//Render_Message_Box(Engine->Main_Menu_State->Message);
}

void Input_Main_Menu_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    int Story_Mode_Mouse = Check_If_Mouse_Clicked_On_Button(Keypad,0,0,176,176+288,32,32+80);
    int TBD_Mouse = 0;
    int Options_Mouse = 0;

    if(!Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen && !Engine->Main_Menu_State->Multi_Player_Mode_Button_Is_Off_Screen && !Engine->Main_Menu_State->Options_Button_Is_Off_Screen)
    {
    if(Keypad.Down && Engine->Main_Menu_State->Current_Choice < 2 && Engine->Main_Menu_State->Down_Not_Pressed)
    {
         Engine->Main_Menu_State->Selector_Y += 96;
         Engine->Main_Menu_State->Current_Choice += 1;
         Engine->Main_Menu_State->Down_Not_Pressed = false;
         if(!Engine->Main_Menu_State->Enter_Pressed)
         {
         Mix_PlayChannel( -1, Engine->Main_Menu_State->Select, 0 );
         }
    }

    if(Keypad.Up && Engine->Main_Menu_State->Current_Choice > 0 && Engine->Main_Menu_State->Up_Not_Pressed)
    {
         Engine->Main_Menu_State->Selector_Y -= 96;
         Engine->Main_Menu_State->Current_Choice -= 1;
         Engine->Main_Menu_State->Up_Not_Pressed = false;
         if(!Engine->Main_Menu_State->Enter_Pressed)
         {
         Mix_PlayChannel( -1, Engine->Main_Menu_State->Select, 0 );
         }
    }

    if(!Keypad.Up)
    {
        Engine->Main_Menu_State->Up_Not_Pressed = true;
    }

    if(!Keypad.Down)
    {
        Engine->Main_Menu_State->Down_Not_Pressed = true;
    }


    if(Keypad.Start || Story_Mode_Mouse == 2)
    {
        if(Engine->Main_Menu_State->Current_Choice == 0 && Engine->Main_Menu_State->Mini_State== 1)
        {
            if(!Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen)
            {
            Resume_Timer(Engine->Main_Menu_State->Timer);
            Engine->Main_Menu_State->Enter_Pressed = true;
            Engine->Main_Menu_State->Brightness -=0.25;
            glColor3f(Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness);
            Mix_HaltChannel(0);
            Mix_HaltMusic();
            }
        }
    }

    if(Keypad.Start || TBD_Mouse == 2)
    {
        if(Engine->Main_Menu_State->Current_Choice == 0 && Engine->Main_Menu_State->Mini_State == 1)
        {
            if(!Engine->Main_Menu_State->Multi_Player_Mode_Button_Is_Off_Screen)
            {
            Resume_Timer(Engine->Main_Menu_State->Timer);
            Engine->Main_Menu_State->Enter_Pressed = true;
            Engine->Main_Menu_State->Brightness -=0.25;
            glColor3f(Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness);
            Mix_HaltChannel(0);
            Mix_HaltMusic();
            }
        }
    }

    if(Keypad.Start || Options_Mouse == 2)
    {
        if(Engine->Main_Menu_State->Current_Choice == 2 && Engine->Main_Menu_State->Mini_State == 1)
        {
            if(!Engine->Main_Menu_State->Options_Button_Is_Off_Screen)
            {
            Engine->Main_Menu_State->Mini_State = 2;
            Engine->Main_Menu_State->Current_Choice = 0;
            Engine->Main_Menu_State->Selector_Y = 52;
            }
        }









        else if(Engine->Main_Menu_State->Current_Choice == 2 && Engine->Main_Menu_State->Mini_State == 2)
        {
            Resume_Timer(Engine->Main_Menu_State->Timer);
            Engine->Main_Menu_State->Enter_Pressed = true;
            Engine->Main_Menu_State->Brightness -=0.25;
            glColor3f(Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness,Engine->Main_Menu_State->Brightness);
            Mix_HaltChannel(0);
            Mix_HaltMusic();
        }
    }

    else if(Keypad.Start && Engine->Main_Menu_State->Current_Choice == 1)
    {
        //Select
        printf("Select\n");
    }

    if(Keypad.Right)
    {
        if(Engine->Main_Menu_State->Mini_State == 2 && Engine->Main_Menu_State->Current_Choice == 0)
        {
            if(Engine->Main_Menu_State->Sound_x < 176+288-32)
            {
            //432 to 175
            Engine->Main_Menu_State->Sound_x +=1;
            int New_Volume = Engine->Main_Menu_State->Sound_x - 175;
            if(New_Volume < 0)
            {
                New_Volume = 0;
            }
            else if(New_Volume > 128)
            {
                New_Volume = 128;
            }
            Mix_VolumeMusic((unsigned char)New_Volume);
            Engine->Settings.Volume = (unsigned char)New_Volume;
            printf("Sound x: %f\n", Engine->Main_Menu_State->Sound_x);
            }
        }

        else if(Engine->Main_Menu_State->Mini_State == 2 && Engine->Main_Menu_State->Current_Choice == 1)
        {
            if(Engine->Main_Menu_State->SFX_x < 176+288-32)
            {
            Engine->Main_Menu_State->SFX_x +=1;
            }
        }

    }

    if(Keypad.Left)
    {
        if(Engine->Main_Menu_State->Mini_State == 2 && Engine->Main_Menu_State->Current_Choice == 0)
        {
            if(Engine->Main_Menu_State->Sound_x >= 176)
            {
             //432 to 175
            Engine->Main_Menu_State->Sound_x -=1;
            int New_Volume = Engine->Main_Menu_State->Sound_x - 175;
            if(New_Volume < 0)
            {
                New_Volume = 0;
            }
            else if(New_Volume > 128)
            {
                New_Volume = 128;
            }
            Mix_VolumeMusic((unsigned char)New_Volume);
            Engine->Settings.Volume = (unsigned char)New_Volume;
            printf("Sound x: %f\n", Engine->Main_Menu_State->Sound_x);
            }
        }

        else if(Engine->Main_Menu_State->Mini_State == 2 && Engine->Main_Menu_State->Current_Choice == 1)
        {
            if(Engine->Main_Menu_State->SFX_x >= 176)
            {
            Engine->Main_Menu_State->SFX_x -=1;
            }
        }
    }

    if(Keypad.B)
    {
        if(Engine->Main_Menu_State->Mini_State == 2)
        {
            FILE* f;
            f = fopen("resources/data/settings", "wb");
            fwrite(&Engine->Settings, 1, sizeof(struct _CSS_Settings),f);
            fclose(f);

            Engine->Main_Menu_State->Mini_State = 1;
            Engine->Main_Menu_State->Current_Choice = 2;
            Engine->Main_Menu_State->Selector_Y = 52-96-96;
        }

    }

    if(Keypad.A)
    {
        printf("Volume %d\n", Mix_VolumeMusic(-1));
    }
    }
}
void Process_Main_Menu_State(struct _Engine* Engine)
{
    if(Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen)
    {
        if(Engine->Main_Menu_State->Single_Player_Mode_Button_X > 176)
        {
            Engine->Main_Menu_State->Single_Player_Mode_Button_X -=40;
        }
        else if(Engine->Main_Menu_State->Single_Player_Mode_Button_X <= 176)
        {
            Engine->Main_Menu_State->Single_Player_Mode_Button_X = 176;
            Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen = false;
        }
    }


    if(Engine->Main_Menu_State->Multi_Player_Mode_Button_Is_Off_Screen && !Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen)
    {
        if(Engine->Main_Menu_State->Multi_Player_Mode_Button_X > 176)
        {
            Engine->Main_Menu_State->Multi_Player_Mode_Button_X -=40;
        }
        else if(Engine->Main_Menu_State->Multi_Player_Mode_Button_X <= 176)
        {
            Engine->Main_Menu_State->Multi_Player_Mode_Button_X = 176;
            Engine->Main_Menu_State->Multi_Player_Mode_Button_Is_Off_Screen = false;
        }
    }



    if(Engine->Main_Menu_State->Options_Button_Is_Off_Screen && !Engine->Main_Menu_State->Multi_Player_Mode_Button_Is_Off_Screen && !Engine->Main_Menu_State->Single_Player_Mode_Button_Is_Off_Screen)
    {
        if(Engine->Main_Menu_State->Options_Button_X > 176)
        {
            Engine->Main_Menu_State->Options_Button_X -=40;
        }
        else if(Engine->Main_Menu_State->Options_Button_X <= 176)
        {
            Engine->Main_Menu_State->Options_Button_X = 176;
            Engine->Main_Menu_State->Options_Button_Is_Off_Screen = false;
        }
    }

    double z = Get_Ticks(Engine->Main_Menu_State->Timer);
    if(z > 2)
    {
        if(Engine->Main_Menu_State->Current_Choice == 0 && Engine->Main_Menu_State->Mini_State == 1)
        {
        printf("Going to Arc Select state!\n");
        Engine->Arc_Select_State= Create_Arc_Select_State();
        Initialize_Arc_Select_State(Engine);
        Engine->Current_State = STATE_ARC_SELECT;
        }

        else if(Engine->Main_Menu_State->Current_Choice == 2 && Engine->Main_Menu_State->Mini_State == 2)
        {
            //Engine->Arc_Select_State= Create_Arc_Select_State();
            //Initialize_Arc_Select_State(Engine);
           // Engine->Current_State = STATE_ARC_SELECT;
        }
    }
}




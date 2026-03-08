/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "title.h"
#include "stdlib.h"
#include "../../engine/graphics/image.h"
#include "../on_foot/on_foot_level.h"
#include "../../engine/emulator/new_opcodes.h"
#include "../../engine/emulator/basic.h"
//Intro needs to be on screen for 30 seconds.

struct _Title_State* Create_Title_State()
{
    struct _Title_State* I = (struct _Title_State*) calloc(1,sizeof (struct _Title_State));
    return I;
}

void Initialize_Title_State(struct _Engine* Engine)
{
    for(int j = 0; j < 9; j++)
    {
        Engine->Title_State->Key_Buffer[j] = 0;
    }

    Engine->Title_State->Current_Key_Buffer_Position = 0;
    Engine->Title_State->Debug_Enabled = false;


    Engine->Title_State->Enter_Pressed = false;
    Engine->Title_State->Timer = Create_Timer();
    Set_Timer(Engine->Title_State->Timer);
    Pause_Timer(Engine->Title_State->Timer);

    Engine->Title_State->Background = Create_Image();
    Engine->Title_State->Logo = Create_Image();

    Load_Image(Engine->Title_State->Background,"resources/artwork/gui/title_background_3.bmp");
    Load_Image(Engine->Title_State->Logo, "resources/artwork/gui/title.bmp");

    Engine->Title_State->Decrease_Light = false;
    Engine->Title_State->Light = 0;

    Engine->Title_State->Start = Mix_LoadWAV( "resources/tracks_and_sounds/general/start.wav" );

    Write_CSS_On_Foot_File_Header("resources/data/levels/final_arc/bay/bay_0.reg");
    Write_CSS_On_Foot_File_Header("resources/data/levels/final_arc/bay/bay_1.reg");
    Create_Level_List_Level_Container("resources/data/levels/final_arc/bay/bay.lvl");

    Engine->Title_State->Track = Mix_LoadMUS( "resources/tracks_and_sounds/general/CSSTITLE3.mp3" );

    Engine->Title_State->Title_Music_Played = false;

    //struct _AQ_BASIC_Compiler* Comp = Create_AQ_BASIC_Compiler();
   // Initialize_AQ_BASIC_Compiler(Comp);
   // int x = Compile_This_Text(Comp, "test.bas", "basic.rom");
   //// Engine->Title_State->Computer = Create_Computer();
   // Initialize_Computer(Engine->Title_State->Computer);

   // Turn_On_Computer(Engine->Title_State->Computer);

   // Load_Program_Into_RAM_And_Start(Engine->Title_State->Computer, "basic.rom");
}

void Destroy_Title_State(struct _Title_State* Title_State)
{

}

void Render_Title_State(struct _Engine* Engine)
{
    if(Engine->Title_State->Light < 1)
    {
        Render_Image_With_Vertex_Color(Engine->Title_State->Background, 0,0,1, Engine->Title_State->Light/2, Engine->Title_State->Light/2, Engine->Title_State->Light/2);
        Render_Image_With_Vertex_Color(Engine->Title_State->Logo, 32,40,1, 0,0,Engine->Title_State->Light);
        Engine->Title_State->Light += 0.01;
    }
    else if(Engine->Title_State->Light >= 1)
    {
        if(!Engine->Title_State->Title_Music_Played)
        {
            Mix_PlayMusic(Engine->Title_State->Track, -1);
            Engine->Title_State->Title_Music_Played = true;
        }
        Render_Image_With_Vertex_Color(Engine->Title_State->Background, 0,0,1, 1,1,1);
        Render_Image_With_Vertex_Color(Engine->Title_State->Logo, 32,40,1, 1,1,1);
    }
}
void Input_Title_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Start)
    {
        Resume_Timer(Engine->Title_State->Timer);
        if(!Engine->Title_State->Enter_Pressed)
        {
            Mix_HaltMusic();
            Mix_PlayChannel( -1, Engine->Title_State->Start, 0 );
        }
                Engine->Title_State->Enter_Pressed = true;
        Engine->Title_State->Decrease_Light = true;
    }
            glColor3f(1,1,1);
     //   Process_Computer(Engine->Title_State->Computer,50, Keypad);
       // Update_Computer_Video_Card(Engine->Title_State->Computer);
     //   Render_Computer(Engine->Title_State->Computer, 0,0);

}
void Process_Title_State(struct _Engine* Engine)
{
//    Process_Computer(Engine->Title_State->Computer,5);
    float x = Get_Ticks(Engine->Title_State->Timer);
    if(x > 5 && Engine->Title_State->Enter_Pressed)
    {
        Engine->Main_Menu_State = Create_Main_Menu_State();
        Initialize_Main_Menu_State(Engine);
        glColor3f(1,1,1);
        Engine->Current_State = STATE_MAIN_MENU;
    }

    if(Engine->Title_State->Decrease_Light && Engine->Title_State->Light > 0)
    {
        Engine->Title_State->Light -= 0.1;
    }
}

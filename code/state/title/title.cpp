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

struct _Title_State* Create_Title_State()
{
    struct _Title_State* I = (struct _Title_State*) calloc(1,sizeof (struct _Title_State));
    return I;
}

void Initialize_Title_State(struct _Engine* Engine)
{
    Write_Ship("resources/data/ships/test_ship_details.det");

    Engine->Title_State->Enter_Pressed = false;
    Engine->Title_State->Timer = Create_Timer();
    Set_Timer(Engine->Title_State->Timer);
    Pause_Timer(Engine->Title_State->Timer);

    Engine->Title_State->Background = Create_Image();
    Engine->Title_State->Logo = Create_Image();

    Load_Image(Engine->Title_State->Background,"resources/artwork/gui/title_background_3.bmp");
    switch(Engine->Settings.Current_Language_ID)
    {
        case ID_LANGUAGE_ENGLISH:
            Load_Image(Engine->Title_State->Logo, "resources/artwork/gui/english/title.bmp");
        break;
    }

    Engine->Title_State->Decrease_Light = false;
    Engine->Title_State->Light = 0;
    Engine->Title_State->Text = Create_Image();
    Initialize_Text_Image_From_Encoded_Text(Engine->Text_Engine, Engine->Title_State->Text, 16, 64, "resources/data/test_text.etx");
    Engine->Title_State->Start = Mix_LoadWAV( "resources/tracks_and_sounds/general/start.wav" );
    Engine->Title_State->Track = Mix_LoadMUS( "resources/tracks_and_sounds/general/CSSTITLE3.mp3" );
    Engine->Title_State->Title_Music_Played = false;
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
        Render_Image_With_Vertex_Color(Engine->Title_State->Text, 0,360-64,1, 1,1,1);
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
}
void Process_Title_State(struct _Engine* Engine)
{
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

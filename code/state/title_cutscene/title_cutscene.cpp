/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "title_cutscene.h"
#include "stdlib.h"
#include "../../engine/graphics/image.h"
#include "../on_foot/on_foot_level.h"

struct _Title_Cutscene_State* Create_Cutscene_Title_State()
{
    struct _Title_Cutscene_State* I = (struct _Title_Cutscene_State*) calloc(1,sizeof (struct _Title_Cutscene_State));
    return I;
}

void Initialize_Title_Cutscene_State(struct _Engine* Engine)
{


    Engine->Title_State = Create_Title_State();
        Engine->Title_State->Logo = Create_Image();
        Load_Image(Engine->Title_State->Logo, "resources/artwork/gui/english/title.sei");

    Engine->Title_Cutscene_State->Multiplier = 0;
    Engine->Title_Cutscene_State->Enter_Pressed = false;

    Engine->Title_Cutscene_State->PrologueOne = Create_Image();
    Engine->Title_Cutscene_State->PrologueTwo = Create_Image();
    Engine->Title_Cutscene_State->PrologueThree = Create_Image();
    Engine->Title_Cutscene_State->PrologueFour = Create_Image();
    Engine->Title_Cutscene_State->PrologueFive = Create_Image();

    Engine->Title_Cutscene_State->PrologueSix = Create_Image();
    Engine->Title_Cutscene_State->PrologueSeven = Create_Image();

    switch(Engine->Settings.Current_Language_ID)
    {
        case ID_LANGUAGE_ENGLISH:
            BMPtoSEI("resources/artwork/gui/english/prologue1.bmp", "resources/artwork/gui/english/prologue1.sei");
            BMPtoSEI("resources/artwork/gui/english/prologue2.bmp", "resources/artwork/gui/english/prologue2.sei");
            BMPtoSEI("resources/artwork/gui/english/prologue3.bmp", "resources/artwork/gui/english/prologue3.sei");
            BMPtoSEI("resources/artwork/gui/english/prologue4.bmp", "resources/artwork/gui/english/prologue4.sei");
            BMPtoSEI("resources/artwork/gui/english/prologue5.bmp", "resources/artwork/gui/english/prologue5.sei");
            BMPtoSEI("resources/artwork/gui/english/prologue6.bmp", "resources/artwork/gui/english/prologue6.sei");
            BMPtoSEI("resources/artwork/gui/english/prologue7.bmp", "resources/artwork/gui/english/prologue7.sei");

            BMPtoSEI("resources/artwork/gui/title_background_3.bmp", "resources/artwork/gui/title_background_3.sei");

            Load_Image(Engine->Title_Cutscene_State->PrologueOne,"resources/artwork/gui/english/prologue1.sei");
            Load_Image(Engine->Title_Cutscene_State->PrologueTwo,"resources/artwork/gui/english/prologue2.sei");
            Load_Image(Engine->Title_Cutscene_State->PrologueThree,"resources/artwork/gui/english/prologue3.sei");
            Load_Image(Engine->Title_Cutscene_State->PrologueFour,"resources/artwork/gui/english/prologue4.sei");
            Load_Image(Engine->Title_Cutscene_State->PrologueFive,"resources/artwork/gui/english/prologue5.sei");
            Load_Image(Engine->Title_Cutscene_State->PrologueSix,"resources/artwork/gui/english/prologue6.sei");
            Load_Image(Engine->Title_Cutscene_State->PrologueSeven,"resources/artwork/gui/english/prologue7.sei");
        break;
    }

    Engine->Title_Cutscene_State->Mini_State = -1;

    Engine->Title_Cutscene_State->Timer = Create_Timer();
    Set_Timer(Engine->Title_Cutscene_State->Timer);

    Engine->Title_Cutscene_State->Track = Mix_LoadMUS( "resources/tracks_and_sounds/general/prologue.mp3" );
    Mix_PlayMusic(Engine->Title_Cutscene_State->Track, 0);
}

void Destroy_Title_Cutscene_State(struct _Title_Cutscene_State* Title_Cutscene_State)
{

}

void Render_Title_Cutscene_State(struct _Engine* Engine)
{
    switch(Engine->Title_Cutscene_State->Mini_State)
    {
        case 0:
        Render_Image(Engine->Title_Cutscene_State->PrologueOne,0,0,1);
        break;
        case 1:
    Render_Image(Engine->Title_Cutscene_State->PrologueTwo,0,0,1);
            break;
            case 2:
    Render_Image(Engine->Title_Cutscene_State->PrologueThree,0,0,1);
            break;
            case 3:
    Render_Image(Engine->Title_Cutscene_State->PrologueFour,0,0,1);
            break;
            case 4:
    Render_Image(Engine->Title_Cutscene_State->PrologueFive,0,0,1);
            break;

            case 5:
    Render_Image(Engine->Title_Cutscene_State->PrologueSix,0,0,1);
            break;

            case 6:
    Render_Image(Engine->Title_Cutscene_State->PrologueSeven,0,0,1);
            break;
    }

}
void Input_Title_Cutscene_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Keyboard_Start && !Engine->Title_Cutscene_State->Enter_Pressed)
    {
        Engine->Title_Cutscene_State->Multiplier = 74;
        Engine->Title_Cutscene_State->Enter_Pressed=true;
    }

    if(Keypad.Keyboard_A && !Engine->Title_Cutscene_State->Enter_Pressed)
    {
        Engine->Title_Cutscene_State->Multiplier = 74;
        Engine->Title_Cutscene_State->Enter_Pressed=true;
    }

    if(Keypad.Keyboard_B && !Engine->Title_Cutscene_State->Enter_Pressed)
    {
        Engine->Title_Cutscene_State->Multiplier = 74;
        Engine->Title_Cutscene_State->Enter_Pressed=true;
    }
}
void Process_Title_Cutscene_State(struct _Engine* Engine)
{
    float x = Get_Ticks(Engine->Title_Cutscene_State->Timer);
    x+=Engine->Title_Cutscene_State->Multiplier;
    if(x > 3 && x <= 9.25)
    {
        glColor3f(x-3,x-3,x-3);
        Engine->Title_Cutscene_State->Mini_State = 0;
    }


    if(x > 9.25 && x <= 21)
    {
        glColor3f(1,1,1);
        Engine->Title_Cutscene_State->Mini_State = 1;
    }


    if(x > 21 && x <= 31.5)
    {
        Engine->Title_Cutscene_State->Mini_State = 2;
    }

    if(x > 31.5 && x <= 43)
    {
        Engine->Title_Cutscene_State->Mini_State = 3;
    }

    if(x > 43 && x <= 52)
    {
        Engine->Title_Cutscene_State->Mini_State = 4;
    }

    if(x > 52 && x <= 62)
    {
        Engine->Title_Cutscene_State->Mini_State = 5;
    }

    if(x > 62 && x <= 73)
    {
        Engine->Title_Cutscene_State->Mini_State = 6;
    }

    if(x > 73)
    {
        Initialize_Title_State(Engine);
        Engine->Current_State = STATE_TITLE;
    }
}

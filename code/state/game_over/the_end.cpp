/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdio.h"
#include "the_end.h"
struct _The_End_State* Create_The_End_State()
{
    struct _The_End_State* I = (struct _The_End_State*) malloc(sizeof (struct _The_End_State));
    return I;

}

void Initialize_The_End_State(struct _Engine* Engine)
{
    Engine->The_End_State->Selector_Y = 204;

    Engine->The_End_State->Current_Choice = 0;

    Engine->The_End_State->Selector = Create_Image();
    Load_Image(Engine->The_End_State->Selector, "resource/gui/selector_right.bmp");


   Engine->The_End_State->Background = Create_Image();
   Load_Image(Engine->The_End_State->Background, "resource/gui/titleback.bmp");


   Engine->The_End_State->Episode_Select = Create_Image();
   Engine->The_End_State->Story_Mode = Create_Image();


   Load_Image(Engine->The_End_State->Episode_Select, "resource/gui/quit.bmp");

   Load_Image(Engine->The_End_State->Story_Mode, "resource/gui/retry.bmp");



   Engine->The_End_State->Up_Not_Pressed = false;
   Engine->The_End_State->Down_Not_Pressed = false;

   Engine->The_End_State->Logo = Create_Image();
   Load_Image(Engine->The_End_State->Logo, "resource/gui/theend.bmp");
}

void Destroy_The_End_State(struct _The_End_State* The_End_State)
{

}

void Render_The_End_State(struct _Engine* Engine)
{
//Render_Image(Engine->The_End_State->Background, 0,-150,1);
Render_Image(Engine->The_End_State->Logo, 32,32,1);
Render_Image(Engine->The_End_State->Story_Mode,257,200,1);

Render_Image(Engine->The_End_State->Episode_Select,257,264,1);



Render_Image(Engine->The_End_State->Selector, 216,Engine->The_End_State->Selector_Y,1);
}
void Input_The_End_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Down && Engine->The_End_State->Current_Choice < 1 && Engine->The_End_State->Down_Not_Pressed)
    {
         Engine->The_End_State->Selector_Y += 64;
         Engine->The_End_State->Current_Choice += 1;
         Engine->The_End_State->Down_Not_Pressed = false;
    }

    if(Keypad.Up && Engine->The_End_State->Current_Choice > 0 && Engine->The_End_State->Up_Not_Pressed)
    {
         Engine->The_End_State->Selector_Y -= 64;
         Engine->The_End_State->Current_Choice -= 1;
         Engine->The_End_State->Up_Not_Pressed = false;
    }

    if(!Keypad.Up)
    {
        Engine->The_End_State->Up_Not_Pressed = true;
    }

    if(!Keypad.Down)
    {
        Engine->The_End_State->Down_Not_Pressed = true;
    }


    if(Keypad.Start && Engine->The_End_State->Current_Choice == 0)
    {
        //Story Mode
        printf("Story\n");
    }

    else if(Keypad.Start && Engine->The_End_State->Current_Choice == 1)
    {
        Engine->Title_State = Create_Title_State();
        Initialize_Title_State(Engine);
        Engine->Current_State = STATE_TITLE;
    }
}
void Process_The_End_State(struct _Engine* Engine)
{

}


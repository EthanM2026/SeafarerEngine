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
    Engine->The_End_State->Arc_Cleared = Create_Image();
    Load_Image(Engine->The_End_State->Arc_Cleared, "arc_cleared.sei");

    Engine->The_End_State->Timer = Create_Timer();
    Set_Timer(Engine->The_End_State->Timer);

    Engine->The_End_State->Render = true;
}

void Destroy_The_End_State(struct _The_End_State* The_End_State)
{

}

void Render_The_End_State(struct _Engine* Engine)
{
    if(Engine->The_End_State->Render)
    {
        Render_Image(Engine->The_End_State->Arc_Cleared,162,128,1);
    }
}
void Input_The_End_State(struct _Engine* Engine, struct _Keypad Keypad)
{

}
void Process_The_End_State(struct _Engine* Engine)
{
    double x = Get_Ticks(Engine->The_End_State->Timer);
    if(x >= 5)
    {
        Engine->The_End_State->Render = false;
    }

    if(x >= 8)
    {
        free(Engine->Main_Menu_State);
        Engine->Main_Menu_State = Create_Main_Menu_State();
        Initialize_Main_Menu_State(Engine);
        Engine->Current_State = STATE_MAIN_MENU;
    }
}


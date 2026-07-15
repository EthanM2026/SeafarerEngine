/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "arc_complete.h"
#include <stdio.h>
#include "stdlib.h"
#include <string.h>

struct _Arc_Complete_State* Create_Arc_Complete_State()
{
    struct _Arc_Complete_State* Arc_Complete_State = (struct _Arc_Complete_State*) calloc(1,sizeof(struct _Arc_Complete_State));
    return Arc_Complete_State;
};

void Initialize_Arc_Complete_State(struct _Engine* Engine, unsigned char Type)
{
    Engine->Arc_Complete_State->Completion_Reason = Type;
    switch(Type)
    {
        case 0:
            Engine->Arc_Complete_State->Arc_Complete = Create_Image();
            Load_Image(Engine->Arc_Complete_State->Arc_Complete,"resources/artwork/gui/english/arc_complete.sei");
        break;

        case 1:
            Engine->Arc_Complete_State->Arc_Complete = Create_Image();
            Load_Image(Engine->Arc_Complete_State->Arc_Complete,"resources/artwork/gui/english/level_complete.sei");
        break;

        case 2:
            Engine->Arc_Complete_State->Arc_Complete = Create_Image();
            Load_Image(Engine->Arc_Complete_State->Arc_Complete,"resources/artwork/gui/english/arc_cleared.sei");
        break;

        case 3:
            Engine->Arc_Complete_State->Arc_Complete = Create_Image();
            Load_Image(Engine->Arc_Complete_State->Arc_Complete,"resources/artwork/gui/english/the_end.sei");
        break;
    }
    Engine->Arc_Complete_State->Timer = Create_Timer();
    Set_Timer(Engine->Arc_Complete_State->Timer);
};

void Render_Arc_Complete_State(struct _Engine* Engine)
{
    double x = Get_Ticks(Engine->Arc_Complete_State->Timer);
    if(x < 5)
    {
        Render_Image(Engine->Arc_Complete_State->Arc_Complete,0,0,1);
    }
};

void Process_Arc_Complete_State(struct _Engine* Engine)
{
    double x = Get_Ticks(Engine->Arc_Complete_State->Timer);
    if(x > 8)
    {
        switch(Engine->Arc_Complete_State->Completion_Reason)
        {
            case 3:
                Engine->Title_Cutscene_State = Create_Cutscene_Title_State();
                Initialize_Title_Cutscene_State(Engine);
                Engine->Current_State = STATE_TITLE_CUTSCENE;
            break;

            default:
                Engine->Overworld_State = Create_Overworld_State();
                Initialize_Overworld_State(Engine);
                Engine->Current_State = STATE_OVERWORLD;
            break;
        }
    }
};

void Handle_Arc_Complete_State_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{

};

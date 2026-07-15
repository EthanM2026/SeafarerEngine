/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ARC_COMPLETE_H_INCLUDED
#define ARC_COMPLETE_H_INCLUDED
#include "../../engine/engine.h"
#include "../../engine/timer/timer.h"
#include "../on_foot/on_foot.h"


struct _Arc_Complete_State
{
    double Light;
    unsigned char Completion_Reason;
    struct _Image* Arc_Complete;
    struct _Timer* Timer;
};

struct _Arc_Complete_State* Create_Arc_Complete_State();
void Initialize_Arc_Complete_State(struct _Engine* Engine, unsigned char Type);

void Render_Arc_Complete_State(struct _Engine* Engine);
void Process_Arc_Complete_State(struct _Engine* Engine);
void Handle_Arc_Complete_State_Inputs(struct _Engine* Engine, struct _Keypad Keypad);
#endif // ARC_SELECT_H_INCLUDED





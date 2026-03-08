/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef THE_END_H_INCLUDED
#define THE_END_H_INCLUDED
#include "../../engine/engine.h"
struct _The_End_State
{
    struct _Image* Logo;
    struct _Image* Background;

    struct _Image* Story_Mode;
    struct _Image* Episode_Select;
    struct _Image* Options;

    struct _Image* Selector;

    float Selector_Y;
    unsigned char Current_Choice;

    bool Up_Not_Pressed;
    bool Down_Not_Pressed;
};

struct _The_End_State* Create_The_End_State();
void Destroy_The_End_State(struct _Engine* Engine);

void Initialize_The_End_State(struct _Engine* Engine);

void Render_The_End_State(struct _Engine* Engine);
void Input_The_End_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_The_End_State(struct _Engine* Engine);

#endif // THE_END_H_INCLUDED

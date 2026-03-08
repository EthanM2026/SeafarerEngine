/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef OVERWORLD_H_INCLUDED
#define OVERWORLD_H_INCLUDED
#include "../../engine/engine.h"
//EPISODE SELECT

struct _CSS_Level_Triple_Object
{
    struct _Image* Hard_Mode_Image;
    struct _Image* Medium_Mode_Image;
    struct _Image* Easy_Mode_Image;
    struct _Image* Image_Descriptors[3];
    struct _Image* Text_Descriptors[3];
};


struct _Overworld_State
{
    bool Enter_Pressed;
    struct _Timer* Timer;
    struct _Image* Background;
    struct _Image* Selector;
    float Selector_x;
    float Selector_y;
    struct _CSS_Level_Triple_Object* Level_Triples;


    bool Right_Not_Pressed;
    bool Left_Not_Pressed;
    bool Up_Not_Pressed;
    bool Down_Not_Pressed;

    int Current_Choice = 100;

    struct _Image* Green_Downwards;
    struct _Image* Yellow_Downwards;

    struct _Image* Green_Forwards;
    struct _Image* Yellow_Forwards;
    struct _Image* Red_Forwards;

    struct _Image* Yellow_Upwards;
    struct _Image* Red_Upwards;

    struct _Image* Select_Level;

    struct _Image* Title_Bar;

    Mix_Chunk* Select;

    int Times_Right_Pressed;
};


struct _Overworld_State* Create_Overworld_State();
void Destroy_Overworld_State(struct _Engine* Engine);

void Initialize_Overworld_State(struct _Engine* Engine);

void Render_Overworld_State(struct _Engine* Engine);
void Input_Overworld_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Overworld_State(struct _Engine* Engine);


#endif // OVERWORLD_H_INCLUDED

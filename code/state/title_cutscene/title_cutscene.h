/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TITLE_CUTSCENE_H_INCLUDED
#define TITLE_CUTSCENE_H_INCLUDED
#include "../../engine/engine.h"
#include "../../engine/timer/timer.h"
#include "../../engine/text/text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "../../engine/graphics/message_box.h"
struct _Title_Cutscene_State
{
    int Mini_State;
    struct _Timer* Timer;
    struct _Image* PrologueOne;
    struct _Image* PrologueTwo;
    struct _Image* PrologueThree;
    struct _Image* PrologueFour;
    struct _Image* PrologueFive;
    struct _Image* PrologueSix;
    struct _Image* PrologueSeven;
    struct _Image* PrologueEight;
    struct _Image* PrologueNine;
};

struct _Title_Cutscene_State* Create_Cutscene_Title_State();
void Destroy_Title_Cutscene_State(struct _Engine* Engine);

void Initialize_Title_Cutscene_State(struct _Engine* Engine);

void Render_Title_Cutscene_State(struct _Engine* Engine);
void Input_Title_Cutscene_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Title_Cutscene_State(struct _Engine* Engine);

#endif // TITLE_CUTSCENE_H_INCLUDED

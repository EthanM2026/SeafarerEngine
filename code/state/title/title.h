/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TITLE_H
#define TITLE_H
#include "../../engine/engine.h"
#include "../../engine/timer/timer.h"
#include "../../engine/text/text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "../../engine/graphics/message_box.h"
#include "../../engine/emulator/emulator.h"

//Debug Enable = LEFT DOWN RIGHT     DOWN UP DOWN UP     LEFT DOWN RIGHT

struct _Title_State
{
    unsigned char Key_Buffer[9];
    unsigned char Current_Key_Buffer_Position;
    bool Debug_Enabled;
    bool Enter_Pressed;
    float Light;
    bool Decrease_Light;
    struct _Timer* Timer;
    struct _Image* Logo;
    struct _Image* Background;
    struct _Image* Text;
    struct _Image* Percy;
    bool Title_Music_Played;
    Mix_Music* Track = NULL;
    Mix_Chunk* Start;

    struct _Computer* Computer;

    int Number_Of_TwoD_Sprite_Objects;
    struct _TwoD_Sprite_Object* Sprite_Objects;
};

struct _Title_State* Create_Title_State();
void Destroy_Title_State(struct _Engine* Engine);
void Initialize_Title_State(struct _Engine* Engine);
void Render_Title_State(struct _Engine* Engine);
void Input_Title_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Title_State(struct _Engine* Engine);


#endif /* TITLE_H */


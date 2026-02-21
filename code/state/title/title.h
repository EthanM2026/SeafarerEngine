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


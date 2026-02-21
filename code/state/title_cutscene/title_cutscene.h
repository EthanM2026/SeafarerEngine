

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

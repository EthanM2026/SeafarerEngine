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

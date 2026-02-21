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

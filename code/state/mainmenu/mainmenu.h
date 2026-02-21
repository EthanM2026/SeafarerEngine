/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   mainmenu.h
 * Author: drew
 *
 * Created on April 24, 2023, 4:30 PM
 */

#ifndef MAINMENU_H
#define MAINMENU_H
#include "../../engine/engine.h"
#include "../../engine/timer/timer.h"
#include "../../engine/graphics/message_box.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#define CHOICE_STORY_MODE 1
#define CHOICE_EPISODE_SELECT 2

struct _Main_Menu_State
{
    struct _Image* Title_Bar;
    struct _Timer* Timer;
    struct _Image* Logo;
    struct _Image* Background;
    struct _Image* Story_Mode;
    struct _Image* Episode_Select;
    struct _Image* Options;
    struct _Image* Selector;
    struct _Image* Sound;
    struct _Image* Sound_Level;
    struct _Image* Sound_Description;
    struct _Image* Language;
    struct _Image* Main_Menu;

    struct _Image* Story_Mode_Inverted;
    struct _Image* Episode_Select_Inverted;
    struct _Image* Options_Inverted;

    bool Enter_Pressed = false;
    float Selector_Y;
    unsigned char Current_Choice;
    Mix_Chunk* Select = NULL;
    Mix_Music* Track = NULL;
    bool Up_Not_Pressed;
    bool Down_Not_Pressed;
    float Sound_x;
    float SFX_x;
    float Brightness= 0;

    bool Single_Player_Mode_Button_Is_Off_Screen;
    bool Multi_Player_Mode_Button_Is_Off_Screen;
    bool Options_Button_Is_Off_Screen;

    float Single_Player_Mode_Button_X;
    float Multi_Player_Mode_Button_X;
    float Options_Button_X;

    bool Go_Back_To_Title;

    struct _Image* Sound_Bar_Selected;

    struct _Message_Box* Message;
    int Mini_State;
};

struct _Main_Menu_State* Create_Main_Menu_State();
void Destroy_Main_Menu_State(struct _Engine* Engine);

void Initialize_Main_Menu_State(struct _Engine* Engine);

void Render_Main_Menu_State(struct _Engine* Engine);
void Input_Main_Menu_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Main_Menu_State(struct _Engine* Engine);

#endif /* MAINMENU_H */


/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ENGINE_H
#define ENGINE_H
#include "text/text.h"
#include "../state/title/title.h"
#include "../state/mainmenu/mainmenu.h"
#include "../state/arc_select/arc_select.h"
#include "../state/overworld/overworld.h"
#include "../state/on_foot/on_foot.h"
#include "../state/title_cutscene/title_cutscene.h"
#include "../state/cutscene/cutscene.h"
#include "../state/arc_complete/arc_complete.h"
#include "../state/nn_train/nn_train.h"
#include "../state/emulator/em_view.h"
#include "emulator/emulator.h"

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define SAMPLING_RATE 44100
#define BUFFER_SIZE 512

#include "GLFW/glfw3.h"

#define STATE_TITLE 1
#define STATE_MAIN_MENU 2
#define STATE_CUTSCENE 3
#define STATE_PLATFORMING 4
#define STATE_MULTIPLAYER_PLATFORMING 5
#define STATE_THE_END 6
#define STATE_LEVEL_SELECT 7
#define STATE_CUTSCENE_SELECT 8
#define STATE_MODEL_SELECT 9
#define STATE_ERROR 10
#define STATE_DEBUG_MENU 11
#define STATE_OVERWORLD 12
#define STATE_ARC_SELECT 13
#define STATE_CUTSCENE_CREATOR 14
#define STATE_HANGAR 15
#define STATE_ON_FOOT 16
#define STATE_TITLE_CUTSCENE 17
#define STATE_INTERIM 18
#define STATE_ARC_COMPLETE 19
#define STATE_NN_TRAIN 20
#define STATE_EM_VIEW 21

#define CONTROL_MODE_KEYBOARD_AND_MOUSE 1
#define CONTROL_MODE_CONTROLLER 2
#define CONTROL_MODE_KEYBOARD_AND_MOUSE_AND_CONTROLLER 3
#define CONTROL_MODE_VR_HEADSET 4

#define SAMPLING_RATE 44100
#define BUFFER_SIZE 512

struct _Universal_ID
{
    char Name[64];
    unsigned long Category;
    unsigned long Subcategory;
    unsigned long Variant;
    unsigned long ID;
};

    #define SAMPLE_RATE 44100
#define AMP_SCALE 0.3f // Prevents clipping when mixing 3 channels

// SID Waveform types
#define WAVE_TRIANGLE 0x10
#define WAVE_SAWTOOTH 0x20
#define WAVE_PULSE    0x40
#define WAVE_NOISE    0x80

// ADSR States
typedef enum { ENV_IDLE, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE } EnvState;

typedef struct {
    // Registers
    uint16_t frequency;
    uint16_t pulse_width; // 12-bit
    uint8_t waveform;
    float attack, decay, sustain, release;

    // Internal Synthesis State
    double phase;
    EnvState env_state;
    float env_level;
    uint32_t noise_reg;
} SID_Channel;


// --- APU Structures (Pulse, Envelope, etc.) ---
typedef struct { uint8_t initial_vol, current_vol, direction, period, timer; } Envelope;
typedef struct {
    bool enabled; uint16_t frequency, shadow_freq; float phase; uint8_t duty_idx; Envelope env;
    bool sweep_enabled, sweep_negate; uint8_t sweep_period, sweep_shift, sweep_timer;
} PulseChannel;
typedef struct {
    bool enabled; uint16_t lfsr; uint8_t shift_clock, divisor_code; float timer; Envelope env;
} NoiseChannel;

struct GB_AUDIO_PROCESSOR
{
    PulseChannel ch1, ch2;
    NoiseChannel ch4;
    uint8_t nr50, nr51;
    int fs_step; float fs_timer;
    float seq_timer; // For the 60Hz music sequencer
};









struct _Filepath
{
    char Filepath[256];
};

struct _Controller
{
    float Controller_Primary_Thumbpad[2]; //Axis 0 and 1
    float Controller_Camera_Thumbpad[2]; //Axis 3 and 4
    float Controller_Triggers[2]; //Axis 2 and 5

    bool Controller_Plus;
    bool Controller_Minus;
    bool Controller_Home;

    bool Controller_LTrigger;
    bool Controller_RTrigger;

    bool Controller_A;
    bool Controller_B;
    bool Controller_X;
    bool Controller_Y;

    bool Controller_Up;
    bool Controller_Down;
    bool Controller_Left;
    bool Controller_Right;
};

struct _Keypad
{
    int Controller_Mode;

    struct _Controller Controller;

    bool Keyboard_Up;
    bool Keyboard_Down;
    bool Keyboard_Left;
    bool Keyboard_Right;

    bool Keyboard_Start;

    bool Keyboard_A;
    bool Keyboard_B;
    bool Keyboard_C;
    bool Keyboard_D;

    bool Keyboard_E;
    bool Keyboard_F;
    bool Keyboard_G;
    bool Keyboard_H;

    bool Keyboard_W;

    double Mouse_X;
    double Mouse_Y;

    bool Mouse_Scroll_Up;
    bool Mouse_Scroll_Down;

    int Screen_Width;
    int Screen_Height;
    bool Mouse_Is_Clicked = false;
};

struct _CSS_Settings
{
    char Game_Name[33];
    unsigned int Build_ID;
    unsigned char Volume;
    unsigned char SFX_Volume;
    int Current_Language_ID;
    int Screen_Width;
    int Screen_Height;
    float Screen_Magnification;
    int Aspect_Ratio_Width;
    int Aspect_Ratio_Height;
};

struct _Engine
{
    int Control_Mode;

    struct _CSS_Settings Settings;
    unsigned char Current_State;
	struct _Text_Engine* Text_Engine;

	Mix_Chunk* USND_Thud;
    Mix_Chunk* USND_Walking;
    Mix_Chunk* USND_Fire;
    Mix_Chunk* USND_Flashlight;
    Mix_Chunk* USND_Explosion;

    Mix_Chunk* USND_Glass_Thud;
    Mix_Chunk* USND_Glass_Roll;

    Mix_Chunk* USND_Vortex;

    Mix_Chunk* USND_Computer_Beep;

	struct _CURRENT_CSS_New_Arc_Level_Header* New_Arc_Level_Headers; //UNDER CONSTRUCTION
	struct _CURRENT_CSS_New_Arc_Level_Container* Engine_Arc_Level_Container; //UNDER CONSTRUCTION
	struct _CURRENT_CSS_On_Foot_Level_Header* Loaded_Level_Header;  //UNDER CONSTRUCTION
	struct _On_Foot_File_Header* Loaded_Level_Regions;
	struct _Title_State* Title_State;
	struct _Main_Menu_State* Main_Menu_State;
	struct _Arc_Select_State* Arc_Select_State;
	struct _Overworld_State* Overworld_State;
	struct _On_Foot_State* On_Foot_State;
	struct _Title_Cutscene_State* Title_Cutscene_State;
	struct _Cutscene_State* Cutscene_State;
	struct _Interim_State* Interim_State;
	struct _Arc_Complete_State* Arc_Complete_State;
	struct _Neural_Network_Training_State* Neural_Network_Training_State;
	struct _Emulator_Viewer_State* Emulator_Viewer_State;
    GLFWwindow* W;

	struct _Emulator* Emulator;

	int Number_Of_Levels_Loaded;
	int Current_Level;
	struct _Filepath* Level_Region_Filenames;

	int Number_Of_Cutscenes_Loaded;
	int Current_Cutscene;
	struct _Cutscene_File* Loaded_Cutscene;


    int Current_Step;

    int Number_Of_Levels;

	struct _Level_List_Level* Loaded_Levels;
	struct _Level_List_Level_Container* Loaded_Level_Container;
	struct _Level_Container_Step* Loaded_Steps;

	char Current_Root_Directory[256];
	char Catted_Root_Directory[256];
	char Current_Language[16];
	char Current_Level_Filepath[256];
	char Current_Region_Filepath[256];

	double Use_This_Spawnpoint[3];
	char Use_This_Character[256];
};

struct _Audio_Chip
{
    SID_Channel channels[3];
    SDL_mutex *audio_mutex;

    // Approximate SID ADSR rate lookup tables (in seconds)
    float attack_times[16]  = {0.002, 0.008, 0.016, 0.024, 0.038, 0.056, 0.068, 0.080, 0.100, 0.250, 0.500, 0.800, 1.000, 3.000, 5.000, 8.000};
    float decay_rel_times[16] = {0.006, 0.024, 0.048, 0.072, 0.114, 0.168, 0.204, 0.240, 0.300, 0.750, 1.500, 2.400, 3.000, 9.000, 15.00, 24.00};

    bool Chip_On;
    bool Sound_Playing;
};

struct _Engine* Create_Engine();
void Initialize_Engine(struct _Engine* Engine);

float Center_Coordinates(float Input, float Screen_Size);

void Reload_Region(struct _Engine* Engine);


void tick_env(Envelope *e);
void tick_music_sequencer();
void audio_callback(void *udata, Uint8 *stream, int len);




void init_sid(struct _Audio_Chip* Audio_Chip);
void sid_gate_on(struct _Audio_Chip* Audio_Chip, int ch, uint16_t freq_reg, uint8_t wave, uint8_t attack_idx, uint8_t decay_idx, uint8_t sustain_idx, uint8_t release_idx);
void sid_gate_off(struct _Audio_Chip* Audio_Chip, int ch);


// Converts a note ID (1 to 96) to a 16-bit SID frequency register value
// Note 1 = C-1, Note 13 = C-2, Note 49 = A-4 (440Hz standard)
uint16_t note_to_sid_freq(uint8_t note);

#endif /* ENGINE_H */

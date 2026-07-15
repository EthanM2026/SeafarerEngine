/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "engine.h"
#include "stdlib.h"
#include <vorbis/vorbisfile.h>
#include <alsa/asoundlib.h>
#include "text/language.h"

struct _Engine* Create_Engine()
{
	struct _Engine* Engine = (struct _Engine*) calloc(1,sizeof (struct _Engine));
	return Engine;
}
void Initialize_Engine(struct _Engine* Engine)
{
    struct _CSS_Settings Settings;

    FILE* f = fopen("resources/data/settings", "rb");
    fread(&Engine->Settings, 1, sizeof(struct _CSS_Settings),f);
    fclose(f);

    Engine->Settings.Build_ID += 1;

   // memset(Engine->Settings.Game_Name,0,33);
    //sprintf(Engine->Settings.Game_Name, "CRUISER SUBMARINE SONICHU");
    //Engine->Settings.Game_Name[32] = '\0';

   //Engine->Settings.Volume = 64;
    //Engine->Settings.SFX_Volume = 64;
    Engine->Settings.Current_Language_ID = ID_LANGUAGE_ENGLISH;
    //Engine->Settings.Screen_Width = 640;
   // Engine->Settings.Screen_Height = 360;
   // Engine->Settings.Screen_Magnification = 1;
   // Engine->Settings.Aspect_Ratio_Width = 16;
   // Engine->Settings.Aspect_Ratio_Height = 9;

    Mix_VolumeMusic((unsigned char)Engine->Settings.Volume);

    f = fopen("resources/data/settings", "wb");
    fwrite(&Engine->Settings, 1, sizeof(struct _CSS_Settings),f);
    fclose(f);

    Engine->Text_Engine=Create_Text_Engine();
    Initialize_Text_Engine(Engine);
    memset(Engine->Current_Root_Directory,0,256);
    memset(Engine->Catted_Root_Directory,0,256);
    memset(Engine->Current_Language,0,16);

    memset(Engine->Current_Level_Filepath,0,256);
	memset(Engine->Current_Region_Filepath,0,256);
	memset(Engine->Use_This_Character,0,256);

    switch(Engine->Settings.Current_Language_ID)
    {
        case ID_LANGUAGE_ENGLISH:
        Load_Language(Engine->Text_Engine, "resources/data/languages/english_usa.lng");

        sprintf(Engine->Current_Language,"english");
        Engine->Current_Language[7] = '\0';
        break;
    }

    Engine->Title_Cutscene_State = Create_Cutscene_Title_State();
    Initialize_Title_Cutscene_State(Engine);
  Engine->Current_State = STATE_TITLE_CUTSCENE;
  //BMPtoSEI("resources/artwork/gui/english/prologue1.bmp","resources/artwork/gui/english/prologue1.sei");

    //Write_Language("resources/data/languages/english_usa.lng");

    //Create_English_Encoding_File();
}


void Reload_Region(struct _Engine* Engine)
{
    free(Engine->On_Foot_State);
    Engine->On_Foot_State = Create_On_Foot_State();
    Initialize_On_Foot_State(Engine);
            //Load_Region(Engine, Engine->Current_Region_Filepath);
    Engine->Current_State = STATE_ON_FOOT;
};

float Center_Coordinates(float Input, float Screen_Size)
{
    float Result;
    Result = (Screen_Size - Input)/2;
    return Result;
}

void init_sid(struct _Audio_Chip* Audio_Chip) {
    Audio_Chip->audio_mutex = SDL_CreateMutex();

    Audio_Chip->Sound_Playing = false;
    Audio_Chip->Chip_On = false;

    Audio_Chip->attack_times[0] = 0.002;
    Audio_Chip->attack_times[1] = 0.008;
    Audio_Chip->attack_times[2] = 0.016;
    Audio_Chip->attack_times[3] = 0.024;
    Audio_Chip->attack_times[4] = 0.038;
    Audio_Chip->attack_times[5] = 0.056;
    Audio_Chip->attack_times[6] = 0.068;
    Audio_Chip->attack_times[7] = 0.08;
    Audio_Chip->attack_times[8] = 0.1;
    Audio_Chip->attack_times[9] = 0.25;
    Audio_Chip->attack_times[10] = 0.5;
    Audio_Chip->attack_times[11] = 0.8;
    Audio_Chip->attack_times[12] = 1;
    Audio_Chip->attack_times[13] = 3;
    Audio_Chip->attack_times[14] = 5;
    Audio_Chip->attack_times[15] = 8;

    Audio_Chip->decay_rel_times[0] = 0.006;
    Audio_Chip->decay_rel_times[1] = 0.024;
    Audio_Chip->decay_rel_times[2] = 0.048;
    Audio_Chip->decay_rel_times[3] = 0.072;
    Audio_Chip->decay_rel_times[4] = 0.114;
    Audio_Chip->decay_rel_times[5] = 0.168;
    Audio_Chip->decay_rel_times[6] = 0.204;
    Audio_Chip->decay_rel_times[7] = 0.240;
    Audio_Chip->decay_rel_times[8] = 0.300;
    Audio_Chip->decay_rel_times[9] = 0.75;
    Audio_Chip->decay_rel_times[10] = 1.5;
    Audio_Chip->decay_rel_times[11] = 2.4;
    Audio_Chip->decay_rel_times[12] = 3;
    Audio_Chip->decay_rel_times[13] = 9;
    Audio_Chip->decay_rel_times[14] = 15;
    Audio_Chip->decay_rel_times[15] = 24;

    for (int i = 0; i < 3; i++) {
        Audio_Chip->channels[i].frequency = 0;
        Audio_Chip->channels[i].pulse_width = 2048; // 50% duty cycle default
        Audio_Chip->channels[i].waveform = WAVE_SAWTOOTH;
        Audio_Chip->channels[i].attack = Audio_Chip->attack_times[0];
        Audio_Chip->channels[i].decay = Audio_Chip->decay_rel_times[0];
        Audio_Chip->channels[i].sustain = 1.0f;
        Audio_Chip->channels[i].release = Audio_Chip->decay_rel_times[0];
        Audio_Chip->channels[i].phase = 0.0;
        Audio_Chip->channels[i].env_state = ENV_IDLE;
        Audio_Chip->channels[i].env_level = 0.0f;
        Audio_Chip->channels[i].noise_reg = 0x7FFFFF; // Max 23-bit LFSR
    }
}

// Trigger a note on a specific channel
void sid_gate_on(struct _Audio_Chip* Audio_Chip, int ch, uint16_t freq_reg, uint8_t wave, uint8_t attack_idx, uint8_t decay_idx, uint8_t sustain_idx, uint8_t release_idx) {
    SDL_LockMutex(Audio_Chip->audio_mutex);
    Audio_Chip->channels[ch].frequency = freq_reg;
    Audio_Chip->channels[ch].waveform = wave;
    Audio_Chip->channels[ch].attack = Audio_Chip->attack_times[attack_idx & 0x0F];
    Audio_Chip->channels[ch].decay = Audio_Chip->decay_rel_times[decay_idx & 0x0F];
    Audio_Chip->channels[ch].sustain = (sustain_idx & 0x0F) / 15.0f;
    Audio_Chip->channels[ch].release = Audio_Chip->decay_rel_times[release_idx & 0x0F];
    Audio_Chip->channels[ch].env_state = ENV_ATTACK;
    SDL_UnlockMutex(Audio_Chip->audio_mutex);
}

// Release the note on a specific channel
void sid_gate_off(struct _Audio_Chip* Audio_Chip, int ch) {
    SDL_LockMutex(Audio_Chip->audio_mutex);
    if (Audio_Chip->channels[ch].env_state != ENV_IDLE) {
        Audio_Chip->channels[ch].env_state = ENV_RELEASE;
    }
    SDL_UnlockMutex(Audio_Chip->audio_mutex);
}


// Converts a note ID (1 to 96) to a 16-bit SID frequency register value
// Note 1 = C-1, Note 13 = C-2, Note 49 = A-4 (440Hz standard)
uint16_t note_to_sid_freq(uint8_t note) {
    if (note == 0) return 0; // 0 represents a rest (silence)

    // 1. Calculate the actual frequency in Hz
    // A-4 (note 49) is our 440Hz baseline tuning anchor
    float freq_hz = 440.0f * powf(2.0f, ((float)note - 49.0f) / 12.0f);

    // 2. Convert Hz to the C64 SID 16-bit register value
    float sid_reg_value = freq_hz / 0.0596046f;

    // 3. Clamp to maximum 16-bit value (65535) to prevent integer overflow
    if (sid_reg_value > 65535.0f) sid_reg_value = 65535.0f;
    if (sid_reg_value < 0.0f) sid_reg_value = 0.0f;

    return (uint16_t)sid_reg_value;
}

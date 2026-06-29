/*
Copyright (c) 2024 Ethan D. Carbonate.
*/
#include <cstdlib>
#include <GL/glew.h>"

#include "code/engine/engine.h"
#include "code/engine/text/text.h"
#include <vorbis/vorbisfile.h>
#include <alsa/asoundlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "code/engine/emulator/basic.h"

#include "code/engine/neural_network/lstm.h"
#include "code/engine/neural_network/dnn.h"
#include "code/engine/neural_network/cnn.h"
#include "code/engine/neural_network/attn.h"

#include "code/engine/npc/npc.h"

#define MAGNIFICATION 1

struct _Keypad Keypad;

GB_AUDIO_PROCESSOR apu;
const float DUTY_TABLE[4] = {0.125f, 0.25f, 0.50f, 0.75f};
const int NOISE_DIVISORS[] = {8, 16, 32, 48, 64, 80, 96, 112};

// --- Music Data ---
typedef struct { uint16_t freq; int duration; } Note;
Note melody[] = { {1500, 8}, {1500, 8}, {1700, 8}, {1500, 8}, {1900, 8}, {1800, 16}, {1500, 8}, {0, 8} };
int cur_note = 0, note_wait = 0;


void tick_env(Envelope *e) {
    if (e->period == 0) return;
    if (e->timer > 0) e->timer--;
    if (e->timer == 0) {
        e->timer = e->period;
        if (e->direction && e->current_vol < 15) e->current_vol++;
        else if (!e->direction && e->current_vol > 0) e->current_vol--;
    }
}

void tick_music_sequencer() {
    if (note_wait > 0) { note_wait--; }
    else {
        Note n = melody[cur_note];
        if (n.freq > 0) {
            apu.ch2.enabled = true; apu.ch2.frequency = n.freq;
            apu.ch2.env = (Envelope){8, 8, 0, 4, 4}; // Short decay
        }
        note_wait = n.duration;
        cur_note = (cur_note + 1) % 8;
    }
}





void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(Keypad.Controller_Mode == CONTROL_MODE_KEYBOARD_AND_MOUSE || Keypad.Controller_Mode == CONTROL_MODE_KEYBOARD_AND_MOUSE_AND_CONTROLLER)
    {
    	if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_Start = true;
    	}

    	else if(key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_Start = false;
    	}

    	if(key == GLFW_KEY_A && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_A = true;
    	}

    	else if(key == GLFW_KEY_A && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_A = false;
    	}



    	if(key == GLFW_KEY_B && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_B = true;
    	}

    	else if(key == GLFW_KEY_B && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_B = false;
    	}


    	if(key == GLFW_KEY_C && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_C = true;
    	}

    	else if(key == GLFW_KEY_C && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_C = false;
    	}


    	if(key == GLFW_KEY_D && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_D = true;
    	}

    	else if(key == GLFW_KEY_D && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_D = false;
    	}



    	if(key == GLFW_KEY_E && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_E = true;
    	}

    	else if(key == GLFW_KEY_E && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_E = false;
    	}





    	if(key == GLFW_KEY_F && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_F = true;
    	}

    	else if(key == GLFW_KEY_F && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_F = false;
    	}



    	if(key == GLFW_KEY_G && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_G = true;
    	}

    	else if(key == GLFW_KEY_G && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_G = false;
    	}



    	if(key == GLFW_KEY_W && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_W = true;
    	}

    	else if(key == GLFW_KEY_W && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_W = false;
    	}



    	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_Left = true;
    	}

    	else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_Left = false;
    	}



    	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_Right = true;
    	}

    	else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_Right = false;
    	}



    	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_Up = true;
    	}

    	else if(key == GLFW_KEY_UP && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_Up = false;
    	}



    	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    	{
        	Keypad.Keyboard_Down = true;
    	}

    	else if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    	{
        	Keypad.Keyboard_Down = false;
    	}

    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Keypad.Mouse_X = xpos;
    Keypad.Mouse_Y = ypos;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width*MAGNIFICATION,height*MAGNIFICATION);
    Keypad.Screen_Width = width;
    Keypad.Screen_Height = height;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Keypad.Mouse_Is_Clicked = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        Keypad.Mouse_Is_Clicked = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset > 0)
    {
        Keypad.Mouse_Scroll_Up = true;
        Keypad.Mouse_Scroll_Down = false;
    }

    else if(yoffset < 0)
    {
        Keypad.Mouse_Scroll_Up = false;
        Keypad.Mouse_Scroll_Down = true;
    }
}

void joystick_callback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        // The joystick/gamepad was connected
        if (glfwJoystickIsGamepad(jid))
        {
            const char* name = glfwGetGamepadName(jid);
            //printf("Connected Joystick ");
            //printf(name);
            //printf(".\n");
        }
    }
}




struct _Audio_Chip* Audio_Chip = (struct _Audio_Chip*)calloc(1,sizeof(struct _Audio_Chip));



// Main Audio Callback Function
void audio_callback(void *userdata, Uint8 *stream, int len) {
    float *buffer = (float *)stream;
    int samples = len / sizeof(float);
    float dt = 1.0f / SAMPLE_RATE;

    SDL_LockMutex(Audio_Chip->audio_mutex);

    for (int s = 0; s < samples; s++) {
        float mixed_sample = 0.0f;

        for (int i = 0; i < 3; i++) {
            SID_Channel *ch = &Audio_Chip->channels[i];
            if (ch->env_state == ENV_IDLE) continue;

            // 1. ADSR Envelope Update
            switch (ch->env_state) {
                case ENV_ATTACK:
                    ch->env_level += (dt / ch->attack);
                    if (ch->env_level >= 1.0f) {
                        ch->env_level = 1.0f;
                        ch->env_state = ENV_DECAY;
                    }
                    break;
                case ENV_DECAY:
                    ch->env_level -= (dt / ch->decay);
                    if (ch->env_level <= ch->sustain) {
                        ch->env_level = ch->sustain;
                        ch->env_state = ENV_SUSTAIN;
                    }
                    break;
                case ENV_SUSTAIN:
                    ch->env_level = ch->sustain;
                    break;
                case ENV_RELEASE:
                    ch->env_level -= (dt / ch->release);
                    if (ch->env_level <= 0.0f) {
                        ch->env_level = 0.0f;
                        ch->env_state = ENV_IDLE;
                    }
                    break;
                default: break;
            }

            // 2. Frequency Calculation (C64 Formula: Fout = (Fn * Clk) / 16777216)
            // C64 Clock is ~1MHz. Simplified here to standard Hz map:
            double freq_hz = (ch->frequency * 0.0596);
            ch->phase += freq_hz * dt;
            if (ch->phase >= 1.0) ch->phase -= 1.0;

            // 3. Waveform Generation
            float raw_sample = 0.0f;
            if (ch->waveform & WAVE_TRIANGLE) {
                raw_sample = (ch->phase < 0.5) ? (4.0f * ch->phase - 1.0f) : (3.0f - 4.0f * ch->phase);
            } else if (ch->waveform & WAVE_SAWTOOTH) {
                raw_sample = (2.0f * ch->phase) - 1.0f;
            } else if (ch->waveform & WAVE_PULSE) {
                double duty = (double)ch->pulse_width / 4095.0;
                raw_sample = (ch->phase < duty) ? 1.0f : -1.0f;
            } else if (ch->waveform & WAVE_NOISE) {
                // Galois LFSR for SID pseudo-random noise
                if (ch->phase < dt * freq_hz) { // clock shift register
                    unsigned int bit0 = ((ch->noise_reg >> 22) ^ (ch->noise_reg >> 17)) & 1;
                    ch->noise_reg = ((ch->noise_reg << 1) | bit0) & 0x7FFFFF;
                }
                raw_sample = ((float)((ch->noise_reg >> 15) & 0xFF) / 127.5f) - 1.0f;
            }

            mixed_sample += raw_sample * ch->env_level * AMP_SCALE;
        }

        buffer[s] = mixed_sample;
    }

    SDL_UnlockMutex(Audio_Chip->audio_mutex);
}





void Process_Audio_Chip(struct _Audio_Chip Audio_Chip)
{

}






int main(int argc, char** argv)
{
    //Write_Character("resources/data/characters/chris/character_rosemary.dat");
    //BMPtoSEI("resources/artwork/gui/demo.bmp","resources/artwork/gui/demo.sei");

   // Write_ROM_Memory_To_Program("test.rom");
   char Text[256];
   memset(Text,0,256);
   sprintf(Text, "The quick brown fox`jumps");

    Encode_Text_As_File(Text, 25, "test.aqbenc");

    Load_Image_And_Convert_To_Bitmap("test.bmp", "test.dat");
    Load_Image_And_Convert_To_Bitmap("test2.bmp", "test2.dat");
    Load_Image_And_Convert_To_Bitmap("eng_text_tilemap.bmp", "eng_text_tilemap.dat");
    if(!Compile_AQ_BASIC_Program("code.aqb", "test.rom"))
    {
        printf("failed\n");
    }


    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );

    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    glfwInit();

    BMPtoSEI("resources/data/arcs/the_days_of_nova/english/icons/test.bmp","resources/data/arcs/the_days_of_nova/english/icons/test.sei");

    struct _Engine* Engine = Create_Engine();
    Keypad.Controller_Mode = CONTROL_MODE_KEYBOARD_AND_MOUSE;
    Engine->Control_Mode = Keypad.Controller_Mode;

    Engine->W = glfwCreateWindow(640*MAGNIFICATION,360*MAGNIFICATION, "Cruiser Submarine Sonichu Demorgasbord", NULL, NULL); //SCREEN SIZE
    glfwMakeContextCurrent(Engine->W);
    glfwSwapInterval(1); //
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glViewport(0, 0, 640*MAGNIFICATION,360*MAGNIFICATION); //MAGNIFICATION
    glfwSetWindowSizeCallback(Engine->W, window_size_callback);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glewInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glfwSetKeyCallback(Engine->W, key_callback);
    glfwSetCursorPosCallback(Engine->W, cursor_position_callback);
    glfwSetMouseButtonCallback(Engine->W, mouse_button_callback);
    glfwSetScrollCallback(Engine->W, scroll_callback);
    glfwSetJoystickCallback(joystick_callback);
    glClearColor(0.f,0.f, 0.f, 1.f );
    glClear(GL_COLOR_BUFFER_BIT);

    //BMPtoSEI("resources/models/texture_rosemary.bmp","resources/models/texture_rosemary.sei");








    init_sid(Audio_Chip);

    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_F32SYS;
    want.channels = 1;
    want.samples = 512;
    want.callback = audio_callback;

    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (audio_device == 0) {
        printf("Failed to open audio: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudioDevice(audio_device, 0); // Start audio

    printf("--- C64 SID Chip Emulator Loaded ---\n");
    printf("Keys Q, W, E -> Trigger Channel 1 (Sawtooth)\n");
    printf("Keys A, S, D -> Trigger Channel 2 (Pulse/Square)\n");
    printf("Keys Z, X, C -> Trigger Channel 3 (Noise Mode)\n");
    printf("Press ESC to exit.\n");

    bool running = true;








    Initialize_Engine(Engine);
    //Engine->Interim_State = Create_Interim_State();
    printf("Build ID: %d\n", Engine->Settings.Build_ID);
    double xpos, ypos;

    const float* axes;
    const unsigned char* buttons;

    int joystickId = GLFW_JOYSTICK_1;
    if(glfwJoystickPresent(joystickId))
    {
        const char* name = glfwGetJoystickName(joystickId);
    }

    //CreateLSTM();
    //CreateDNN();
   // CreateCNN();
    //CreateAttn();
    //Test_Vision_Lobe();
    //Test_Enemy_Tracking_Lobe();
    //Test_Ally_Tracking_Lobe();
    //Test_Self_Status_Lobe();
   // Test_Parent_Status_Lobe();
    //CreateLSTM();

    //struct _Pilot_Artificial_Brain* PAB = Create_Pilot_Brain();
    //Initialize_Pilot_Brain(Engine, PAB);

    while (!glfwWindowShouldClose(Engine->W))
    {
        glfwGetCursorPos(Engine->W, &xpos, &ypos);
    	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    	switch(Engine->Current_State)
		{
            case STATE_TITLE_CUTSCENE:
    		Render_Title_Cutscene_State(Engine);
    		Input_Title_Cutscene_State(Engine, Keypad);
    		Process_Title_Cutscene_State(Engine);
    		break;

    		case STATE_TITLE:
    		Render_Title_State(Engine);
    		Input_Title_State(Engine, Keypad);
    		Process_Title_State(Engine);
    		break;

    		case STATE_MAIN_MENU:
    		Render_Main_Menu_State(Engine);
    		Input_Main_Menu_State(Engine, Keypad);
    		Process_Main_Menu_State(Engine);
    		break;

    		case STATE_ARC_SELECT:
    		Render_Arc_Select_State(Engine);
    		Input_Arc_Select_State(Engine, Keypad);
            Process_Arc_Select_State(Engine);
    		break;

    		case STATE_OVERWORLD:
    		Render_Overworld_State(Engine);
    		Input_Overworld_State(Engine, Keypad);
    		Process_Overworld_State(Engine);
    		break;

    		case STATE_ON_FOOT:
    		Render_On_Foot_State(Engine);
            Input_On_Foot_State(Engine, Keypad, apu);
    		Process_On_Foot_State(Engine);
    		break;

    		case STATE_INTERIM:
    		//Process_Interim_State(Engine);
    		break;

    		case STATE_CUTSCENE:
    		Render_Cutscene_State(Engine);
            Input_Cutscene_State(Engine, Keypad);
    		Process_Cutscene_State(Engine);
    		break;

    		case STATE_ARC_COMPLETE:
    		Render_Arc_Complete_State(Engine);
            Handle_Arc_Complete_State_Inputs(Engine, Keypad);
    		Process_Arc_Complete_State(Engine);
    		break;

    		case STATE_NN_TRAIN:
    		Render_Neural_Network_Training_State(Engine);
            Handle_Neural_Network_Training_State_Inputs(Engine, Keypad);
    		Process_Neural_Network_Training_State(Engine);
    		break;

    		case STATE_EM_VIEW:
    		Render_Emulator_Viewer_State(Engine);
            Input_Emulator_Viewer_State(Engine, Keypad);
    		Process_Emulator_Viewer_State(Engine, Audio_Chip);
    		break;
		}
        Keypad.Mouse_Scroll_Up = false;
        Keypad.Mouse_Scroll_Down = false;
	glfwSwapBuffers(Engine->W);


            glfwPollEvents();
           // //printf("Controller Connected: %s\n\n", name);
            // 5. Read Axes (Thumbsticks, Analog Triggers)
            if (glfwJoystickPresent(joystickId))
	{
            int axesCount;
            axes = glfwGetJoystickAxes(joystickId, &axesCount);

            Keypad.Controller.Controller_Primary_Thumbpad[0] = axes[0]; //Axis 0 and 1
            Keypad.Controller.Controller_Primary_Thumbpad[1] = axes[1];

            Keypad.Controller.Controller_Camera_Thumbpad[0] = axes[3];  //Axis 3 and 4
            Keypad.Controller.Controller_Camera_Thumbpad[1] = axes[4];

            if(Keypad.Controller.Controller_Camera_Thumbpad[0] < 0.06 && Keypad.Controller.Controller_Camera_Thumbpad[0] > -0.06)
            {
                Keypad.Controller.Controller_Camera_Thumbpad[0] = 0;
            }

            if(Keypad.Controller.Controller_Camera_Thumbpad[1] < 0.06 && Keypad.Controller.Controller_Camera_Thumbpad[1] > -0.06)
            {
                Keypad.Controller.Controller_Camera_Thumbpad[1] = 0;
            }

            Keypad.Controller.Controller_Triggers[0] = axes[2];  //Axis 2 and 5
            Keypad.Controller.Controller_Triggers[1] = axes[5];

            ////printf("--- Axes (%d) ---\n", axesCount);
            for (int i = 0; i < axesCount; i++) {
                // Axes range from -1.0 to 1.0
               // //printf("  Axis %d: % .2f\n", i, axes[i]);
            }

            // 6. Read Buttons (A, B, X, Y, Bumpers, D-Pad, etc.)
            int buttonCount;
            buttons = glfwGetJoystickButtons(joystickId, &buttonCount);

            if(buttons[11] == GLFW_PRESS || Keypad.Controller.Controller_Primary_Thumbpad[1] <= -0.9)
            {
                ////printf("Up BUTTON BEING PRESSED!\n");
                Keypad.Controller.Controller_Up = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_Up = true;
                }
            }

            else if(buttons[11] == GLFW_RELEASE)
            {
                ////printf("Up BUTTON NOT BEING PRESSED!\n");
                Keypad.Controller.Controller_Up = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_Up = false;
                }
            }

            if(buttons[13] == GLFW_PRESS || Keypad.Controller.Controller_Primary_Thumbpad[1] >= 0.9)
            {
                ////printf("Down BUTTON BEING PRESSED!\n");
                Keypad.Controller.Controller_Down = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_Down = true;
                }
            }

            else if(buttons[13] == GLFW_RELEASE)
            {
                ////printf("Down BUTTON NOT BEING PRESSED!\n");
                Keypad.Controller.Controller_Down = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_Down = false;
                }
            }



            if(buttons[12] == GLFW_PRESS)
            {
                ////printf("RIGHT BUTTON BEING PRESSED!\n");
                Keypad.Controller.Controller_Right = true;
            }

            else if(buttons[12] == GLFW_RELEASE)
            {
                ////printf("RIGHT BUTTON NOT BEING PRESSED!\n");
                Keypad.Controller.Controller_Right = false;
            }

            if(buttons[14] == GLFW_PRESS)
            {
                ////printf("LEFT BUTTON BEING PRESSED!\n");
                Keypad.Controller.Controller_Left = true;
            }

            else if(buttons[14] == GLFW_RELEASE)
            {
                ////printf("Left BUTTON NOT BEING PRESSED!\n");
                Keypad.Controller.Controller_Left = false;
            }




            if(buttons[0] == GLFW_PRESS)
            {
                ////printf("B BUTTON BEING PRESSED!");
                Keypad.Controller.Controller_B = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    //Keypad.Keyboard_Start = true;
                }
            }
            else if(buttons[0] == GLFW_RELEASE)
            {
                ////printf("B BUTTON not BEING PRESSED!");
                Keypad.Controller.Controller_B = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    //Keypad.Keyboard_Start = false;
                }
            }
            if(buttons[1] == GLFW_PRESS)
            {
                ////printf("a BUTTON BEING PRESSED!");
                Keypad.Controller.Controller_A = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_A = true;
                }
            }
            else if(buttons[1] == GLFW_RELEASE)
            {
                ////printf("a BUTTON not BEING PRESSED!");
                Keypad.Controller.Controller_A = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_A = false;
                }
            }





            if(buttons[2] == GLFW_PRESS)
            {
                ////printf("Y BUTTON BEING PRESSED!");
                Keypad.Controller.Controller_Y = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Mouse_Is_Clicked = true;
                }
            }
            else if(buttons[2] == GLFW_RELEASE)
            {
                ////printf("Y BUTTON not BEING PRESSED!");
                Keypad.Controller.Controller_Y = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Mouse_Is_Clicked = false;
                }
            }
            if(buttons[3] == GLFW_PRESS)
            {
                ////printf("X BUTTON BEING PRESSED!");
                Keypad.Controller.Controller_X = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_Start = true;
                }
            }
            else if(buttons[3] == GLFW_RELEASE)
            {
                ////printf("X BUTTON not BEING PRESSED!");
                Keypad.Controller.Controller_X = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Keyboard_Start = false;
                }
            }


            if(buttons[4] == GLFW_PRESS)
            {
                ////printf("L DIGITRIGGER BUTTON BEING PRESSED!");
                Keypad.Controller.Controller_LTrigger = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Mouse_Scroll_Down = true;
                }
            }
            else if(buttons[4] == GLFW_RELEASE)
            {
                ////printf("L DIGITRIGGER BUTTON not BEING PRESSED!");
                Keypad.Controller.Controller_LTrigger = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Mouse_Scroll_Down = false;
                }
            }

            if(buttons[5] == GLFW_PRESS)
            {
                ////printf("R DIGITRIGGER BUTTON BEING PRESSED!");
                Keypad.Controller.Controller_RTrigger = true;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Mouse_Scroll_Up = true;
                }
            }
            else if(buttons[5] == GLFW_RELEASE)
            {
                ////printf("R DIGITRIGGER BUTTON not BEING PRESSED!");
                Keypad.Controller.Controller_RTrigger = false;
                if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
                {
                    Keypad.Mouse_Scroll_Up = false;
                }
            }


            ////printf("\n");
        }
    }
    Mix_Quit();
//    SDL_CloseAudioDevice(dev);
    SDL_Quit();
    return 0;
}



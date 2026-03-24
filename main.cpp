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

#define MAGNIFICATION 1

struct _Keypad Keypad;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        Keypad.Up = true;
    }
    else if(key == GLFW_KEY_UP && action == GLFW_RELEASE)
    {
        Keypad.Up = false;
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        Keypad.Down = true;
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    {
        Keypad.Down = false;
    }
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        Keypad.Right = true;
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    {
        Keypad.Right = false;
    }
    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        Keypad.Start = true;
    }
    else if(key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    {
        Keypad.Start = false;
    }
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        Keypad.Left = true;
    }
    else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        Keypad.Left = false;
    }
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        Keypad.A = true;
    }
    else if(key == GLFW_KEY_Q && action == GLFW_RELEASE)
    {
        Keypad.A = false;
    }
    if(key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        Keypad.B = true;
    }
    else if(key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        Keypad.B = false;
    }
    if(key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        Keypad.C = true;
    }
    else if(key == GLFW_KEY_E && action == GLFW_RELEASE)
    {
        Keypad.C = false;
    }
    if(key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        Keypad.D = true;
    }
    else if(key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
        Keypad.D = false;
    }


    if(key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        Keypad.E = true;
    }
    else if(key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        Keypad.E = false;
    }


    if(key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        Keypad.F = true;
    }
    else if(key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        Keypad.F = false;
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
    Keypad.Width = width;
    Keypad.Height = height;
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

int main(int argc, char** argv)
{
    Write_Character("resources/data/characters/chris/character_rosemary.dat");

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    glfwInit();

    struct _Engine* Engine = Create_Engine();

    Engine->W = glfwCreateWindow(640*MAGNIFICATION,360*MAGNIFICATION, "Cruiser Submarine Sonichu", NULL, NULL); //SCREEN SIZE
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
    glClearColor(0.f,0.f, 0.f, 1.f );
    glClear(GL_COLOR_BUFFER_BIT);

    Initialize_Engine(Engine);
    //Engine->Interim_State = Create_Interim_State();
    printf("Build ID: %d\n", Engine->Settings.Build_ID);
    double xpos, ypos;

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
            Input_On_Foot_State(Engine, Keypad);
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
		}
	glfwSwapBuffers(Engine->W);
    glfwPollEvents();
    }
    Mix_Quit();
    SDL_Quit();
    return 0;
}

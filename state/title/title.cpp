/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "title.h"
#include "stdlib.h"
#include "../../engine/graphics/image.h"
#include "../on_foot/on_foot_level.h"
#include "../../engine/emulator/new_opcodes.h"
#include "../../engine/emulator/basic.h"

struct _Title_State* Create_Title_State()
{
    struct _Title_State* I = (struct _Title_State*) calloc(1,sizeof (struct _Title_State));
    return I;
}

void Initialize_Title_State(struct _Engine* Engine)
{
    Write_Weapon("test.wpn");
    Write_Weapon("test2.wpn");
    Write_Weapon("test3.wpn");

    Engine->Title_State->Boom_Played = false;

    Engine->Title_State->Demo = Create_Image();
    Load_Image(Engine->Title_State->Demo,"resources/artwork/gui/demo.sei");


    Mix_HaltMusic();
    Write_Ship("resources/data/ships/test_ship_details.det");
    Write_NPC_Character(Engine, "resources/data/characters/npc/rosechu/rosechu.npc");

    //struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler = (struct _Aqueous_BASIC_Compiler*)calloc(1,sizeof(Aqueous_BASIC_Compiler));
    //Initialize_Aqueous_BASIC_Compiler(Aqueous_BASIC_Compiler);
    //Compile_Aqueous_BASIC_Source_Code(Aqueous_BASIC_Compiler, "test.bas", 1,0,"aqueous.rom", 1,1);

    Engine->Title_State->Enter_Pressed = false;
    Engine->Title_State->Timer = Create_Timer();
    Set_Timer(Engine->Title_State->Timer);
    Pause_Timer(Engine->Title_State->Timer);

    Engine->Title_State->Press_Banner_Visible = false;
    Engine->Title_State->Cooldown_Timer = Create_Timer();
    Set_Timer(Engine->Title_State->Cooldown_Timer);

    Engine->Title_State->Time_Press_Banner_Appeared = Get_Ticks(Engine->Title_State->Cooldown_Timer);


    Engine->Title_State->Background = Create_Image();
    Load_Image(Engine->Title_State->Background,"resources/artwork/gui/title_background_3.sei");
    switch(Engine->Settings.Current_Language_ID)
    {
        case ID_LANGUAGE_ENGLISH:
           // BMPtoSEI("resources/artwork/gui/english/title.bmp","resources/artwork/gui/english/title.sei");
           // BMPtoSEI("resources/artwork/gui/english/title_trm.bmp","resources/artwork/gui/english/title_trm.sei");

        break;
    }

    Engine->Title_State->Decrease_Light = false;
    Engine->Title_State->Light = 0;
    Engine->Title_State->Text = Create_Image();

    char Text[256];
    sprintf(Text, "Beta Build 8621");

    Initialize_Text_Image(Engine->Text_Engine,Engine->Title_State->Text,Text,16,16+(17*16),64);

Engine->Title_State->Boom = Mix_LoadWAV( "resources/tracks_and_sounds/general/sonichu_boom.wav" );
    Engine->Title_State->Start = Mix_LoadWAV( "resources/tracks_and_sounds/general/start.wav" );
    Engine->Title_State->Track = Mix_LoadMUS( "resources/tracks_and_sounds/general/CSSTITLE3.mp3" );
    Engine->Title_State->Title_Music_Played = false;

    glColor3f(1,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
//    glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH); // Enable smooth shading (Gouraud shading)
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.7777,1.0f, METER_CONVERSION * 32000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4,-4,-4,0,0,0,0,0,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
    // --- LIGHT 1: GENERAL LIGHT (Dim & verywhere) ---
    GLfloat gn_ambient[] = { 1, 1, 1, 1.0 }; // Base light so shadows aren't pitch black
    GLfloat gn_diffuse[] = { 1, 1, 1, 1.0 }; // Dim diffuse light
    GLfloat gn_specular[] = { 1,1,1, 1.0 }; // Very little shine

    glLightfv(GL_LIGHT2, GL_AMBIENT, gn_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, gn_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, gn_specular);

    // Position the general light
    // IMPORTANT: The 0.0 at the end makes this a DIRECTIONAL light (like the sun)
    // coming from the top-right-front, rather than a positional point light.
    GLfloat gn_position[] = { 1, 1, 1, 0.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, gn_position);

    Engine->Title_State->Sonichu_Forwards = -100;

    Engine->Title_State->Current_State = STATE_PRETITLE;

    Engine->Title_State->Sonichu_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->Title_State->Sonichu_Model, "resources/models/title_sonichu.se3");
    Load_SE3_Texture(Engine->Title_State->Sonichu_Model, "resources/models/title_sonichu.sei");

    Engine->Title_State->Sonichu_Model_Engine_Trail = Create_SE3_Model();
    Load_SE3_Model(Engine->Title_State->Sonichu_Model_Engine_Trail, "resources/models/title_sonichu_engine_trail.se3");
    BMPtoSEI("resources/models/skyboxnebula.bmp", "resources/models/texture_skybox_nebula.sei");
    Load_SE3_Texture(Engine->Title_State->Sonichu_Model_Engine_Trail, "resources/models/title_sonichu_trail.sei");

    Engine->Title_State->Sonichu_Model_Skybox = Create_SE3_Model();
    Load_SE3_Model(Engine->Title_State->Sonichu_Model_Skybox, "resources/models/title_sonichu_skybox.se3");
    Load_SE3_Texture(Engine->Title_State->Sonichu_Model_Skybox, "resources/models/texture_skybox_nebula.sei");

    Engine->Title_State->Sonichu_Model_Skybox_Light = Create_SE3_Model();
    Load_SE3_Model(Engine->Title_State->Sonichu_Model_Skybox_Light, "resources/models/title_sonichu_skybox_light.se3");
    //BMPtoSEI("resources/models/title_background_3.bmp","resources/models/title_sonichu_skybox_light.sei");
    Load_SE3_Texture(Engine->Title_State->Sonichu_Model_Skybox_Light, "resources/models/title_sonichu_skybox_light.sei");

    Engine->Title_State->Press_Enter = Create_Image();
    BMPtoSEI("resources/artwork/gui/english/press_enter.bmp","resources/artwork/gui/english/press_enter.sei");
    BMPtoSEI("resources/artwork/gui/english/press_enter.bmp","resources/artwork/gui/english/press_button.sei");

    //if(Engine->Control_Mode == CONTROL_MODE_KEYBOARD_AND_MOUSE)
    //{
    Load_Image(Engine->Title_State->Press_Enter,"resources/artwork/gui/english/press_enter.sei");
    //}

    //else if(Engine->Control_Mode == CONTROL_MODE_CONTROLLER)
    //{
   // Load_Image(Engine->Title_State->Press_Enter,"resources/artwork/gui/english/press_button.sei");
   // }
    //Write_Cutscene("resources/data/arcs/the_days_of_nova/english/cutscenes/test.cts");

   // Engine->Cutscene_State = Create_Cutscene_State();
   // Initialize_Cutscene_State(Engine);
   // Load_Cutscene(Engine, "resources/data/arcs/the_days_of_nova/english/cutscenes/test.cts");

    //Engine->Current_State = STATE_CUTSCENE;
}

void Destroy_Title_State(struct _Title_State* Title_State)
{

}

void Render_Title_State(struct _Engine* Engine)
{
    if(Engine->Title_State->Current_State == STATE_PRETITLE)
    {

        Engine->Title_State->Sonichu_Forwards += 1;
        if(!Engine->Title_State->Boom_Played)
        {
            Mix_PlayChannel( -1, Engine->Title_State->Boom, 0 );
            Engine->Title_State->Boom_Played = true;
        }

        if(Engine->Title_State->Sonichu_Forwards >= 200)
        {
          glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
//    glDepthMask(GL_FALSE);

    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
            Engine->Title_State->Current_State = STATE_PRESENT_TITLE;
        }
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(Engine->Title_State->Sonichu_Forwards,0,0);
        //glDisable(GL_LIGHT2);
        Render_SE3_Model(Engine->Title_State->Sonichu_Model_Skybox,0,0,0,0,0,0,0,false,1,1,1,false);
        Render_SE3_Model(Engine->Title_State->Sonichu_Model_Skybox_Light,0,0,0,0,0,0,0,false,1,1,1,false);
        //glEnable(GL_LIGHT2);
        Render_SE3_Model(Engine->Title_State->Sonichu_Model,0,0,0,0,0,0,0,false,1,1,1,false);
        //Render_SE3_Model(Engine->Title_State->Sonichu_Model_Engine_Trail,0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }



    else if(Engine->Title_State->Current_State == STATE_PRESENT_TITLE)
    {
    if(Engine->Title_State->Light < 1)
    {
        Render_Image_With_Vertex_Color(Engine->Title_State->Background, 0,0,1, Engine->Title_State->Light/2, Engine->Title_State->Light/2, Engine->Title_State->Light/2);
        //Render_Image_With_Vertex_Color(Engine->Title_State->Logo, 32,40,1, 0,0,Engine->Title_State->Light);
        Render_Image_At_Size(Engine->Title_State->Logo, 32,30, 576,184,1);
        Engine->Title_State->Light += 0.01;
    }
    else if(Engine->Title_State->Light >= 1)
    {
        if(!Engine->Title_State->Title_Music_Played)
        {
            Mix_PlayMusic(Engine->Title_State->Track, 0);
            Engine->Title_State->Title_Music_Played = true;
        }
        Render_Image_With_Vertex_Color(Engine->Title_State->Background, 0,0,1, 1,1,1);
        //Render_Image_With_Vertex_Color(Engine->Title_State->Text, 8,360-64-8,1, 1,1,1);
        Render_Image_At_Size(Engine->Title_State->Logo, 32,30, 576,184,1);

        Render_Image_With_Vertex_Color(Engine->Title_State->Demo, 640-240-32-32,360-128-8,1, 1,1,1);

        if(Engine->Title_State->Press_Banner_Visible)
        {
            if(Engine->Control_Mode == CONTROL_MODE_KEYBOARD_AND_MOUSE)
            {
                Render_Image_With_Vertex_Color(Engine->Title_State->Press_Enter, 320-112,360-64-32,1, 1,1,1);
            }

            else if(Engine->Control_Mode == CONTROL_MODE_CONTROLLER)
            {
                Render_Image_With_Vertex_Color(Engine->Title_State->Press_Enter, 320-112,360-64-32,1, 1,1,1);
            }
        }

        double x = Get_Ticks(Engine->Title_State->Cooldown_Timer);
        if(x > Engine->Title_State->Time_Press_Banner_Appeared + 0.5)
        {
            if(Engine->Title_State->Press_Banner_Visible == false)
            {
                Engine->Title_State->Press_Banner_Visible = true;
                ////printf("VISBLE!\n\n\n\n");
            }

            else if(Engine->Title_State->Press_Banner_Visible == true)
            {
                Engine->Title_State->Press_Banner_Visible = false;
            }
            Engine->Title_State->Time_Press_Banner_Appeared = x;
        }

    }
    }
}
void Input_Title_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Engine->Title_State->Current_State == STATE_PRESENT_TITLE)
    {
    if(Keypad.Keyboard_Start)
    {
        Resume_Timer(Engine->Title_State->Timer);
        if(!Engine->Title_State->Enter_Pressed)
        {
            Mix_HaltMusic();
            Mix_PlayChannel( -1, Engine->Title_State->Start, 0 );
        }
        Engine->Title_State->Enter_Pressed = true;
        Engine->Title_State->Decrease_Light = true;
    }
    glColor3f(1,1,1);
    }
}
void Process_Title_State(struct _Engine* Engine)
{
    if(Engine->Title_State->Current_State == STATE_PRETITLE)
    {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4,-4,-4,Engine->Title_State->Sonichu_Forwards,0,0,0,0,1);
    }

    else if(Engine->Title_State->Current_State == STATE_PRESENT_TITLE)
    {
    float x = Get_Ticks(Engine->Title_State->Timer);
    if(x > 5 && Engine->Title_State->Enter_Pressed)
    {
        Engine->Main_Menu_State = Create_Main_Menu_State();
        Initialize_Main_Menu_State(Engine);
        glColor3f(1,1,1);
        Engine->Current_State = STATE_MAIN_MENU;
    }

    if(Engine->Title_State->Decrease_Light && Engine->Title_State->Light > 0)
    {
        Engine->Title_State->Light -= 0.1;
    }
    }
}

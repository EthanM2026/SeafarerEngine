/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "title.h"
#include "stdlib.h"
#include "../../engine/graphics/image.h"
#include "../on_foot/on_foot_level.h"
#include "../../engine/emulator/new_opcodes.h"
//Intro needs to be on screen for 30 seconds.

struct _Title_State* Create_Title_State()
{
    struct _Title_State* I = (struct _Title_State*) calloc(1,sizeof (struct _Title_State));
    return I;
}

void Initialize_Title_State(struct _Engine* Engine)
{
    Write_Language("resources/data/languages/english_usa.lng");
    Load_Language(Engine->Text_Engine, "resources/data/languages/english_usa.lng");
    Create_English_Encoding_File();

    int TextTwo[256];
    TextTwo[0] = 'A';
    TextTwo[1] = 'B';
    TextTwo[2] = 'C';
    TextTwo[3] = '~';
    TextTwo[4] = 'D';
    TextTwo[5] = 'E';
    TextTwo[6] = 'F';
    TextTwo[7] = '`';
    TextTwo[8] = 'E';
    TextTwo[9] = 'F';


    Create_Encoded_Text(Engine->Text_Engine, "test_text.txt", "test_text.etx");

    Write_Example_Encoding("example.enc");
    Load_Image_And_Convert_To_Bitmap("percy.bmp", "tilemap.3bm");

    struct _x80_Programmer* Programmer = (struct _x80_Programmer*)calloc(1,sizeof(struct _x80_Programmer));


    New_Create_z80_Program(Programmer, "z802.rom");


    Engine->Title_State->Computer = Create_Computer();
    Initialize_Computer(Engine->Title_State->Computer);

    Turn_On_Computer(Engine->Title_State->Computer);

   // Load_Program_Into_RAM_And_Start(Engine->Title_State->Computer, "z802.rom");

   // Create_Cartridge(1, "cartridge.rom");

   // Compile_Source_Code("main.asm", "main.bin");

    Engine->Title_State->Sprite_Objects = (struct _TwoD_Sprite_Object*)calloc(1,sizeof(struct _TwoD_Sprite_Object));
    Load_TwoD_Sprite_Animation(Engine->Title_State->Sprite_Objects, "explosion.and", 0);
    for(int j = 0; j < 9; j++)
    {
        Engine->Title_State->Key_Buffer[j] = 0;
    }

    Engine->Title_State->Current_Key_Buffer_Position = 0;
    Engine->Title_State->Debug_Enabled = false;


    Engine->Title_State->Enter_Pressed = false;
    Engine->Title_State->Timer = Create_Timer();
    Set_Timer(Engine->Title_State->Timer);
    Pause_Timer(Engine->Title_State->Timer);

    Engine->Title_State->Background = Create_Image();
    Engine->Title_State->Logo = Create_Image();

    Load_Image(Engine->Title_State->Background,"resources/artwork/gui/title_background_3.bmp");
    Load_Image(Engine->Title_State->Logo, "resources/artwork/gui/title.bmp");

    Engine->Title_State->Decrease_Light = false;
    Engine->Title_State->Light = 0;

    //char Text[256];
    //sprintf(Text, "THE QUICK BROWN FOX JUMPED");
    //Text[26] = '\0';

    Engine->Title_State->Text = Create_Image();
    Initialize_Text_Image_From_Encoded_Text(Engine->Text_Engine, Engine->Title_State->Text, 24, 64, "test_text.etx");

    //Initialize_Text_Image(Engine->Text_Engine, Engine->Title_State->Text, Text,27,27*17,32);

    Engine->Title_State->Start = Mix_LoadWAV( "resources/tracks_and_sounds/general/start.wav" );

    Engine->Title_State->Percy = Create_Image();


    //Load_Bitmap_And_Convert_To_Image("tilemap.3bm", Engine->Title_State->Percy);

    Write_CSS_On_Foot_File_Header("resources/data/levels/final_arc/bay/bay_0.reg");
    Write_CSS_On_Foot_File_Header("resources/data/levels/final_arc/bay/bay_1.reg");
    Create_Level_List_Level_Container("resources/data/levels/final_arc/bay/bay.lvl");

//    Engine->Emulator = Create_Emulator();
//    Initialize_Emulator(Engine);

  //  Write_Conversation_File("resources/data/levels/final_arc/bay/cnv_jrh.cnv");

    Engine->Title_State->Track = Mix_LoadMUS( "resources/tracks_and_sounds/general/CSSTITLE3.mp3" );
    //Mix_PlayMusic(Engine->Title_State->Track, -1);

    Engine->Title_State->Title_Music_Played = false;
}

void Destroy_Title_State(struct _Title_State* Title_State)
{

}

void Render_Title_State(struct _Engine* Engine)
{
    if(Engine->Title_State->Light < 1)
    {
        Render_Image_With_Vertex_Color(Engine->Title_State->Background, 0,0,1, Engine->Title_State->Light/2, Engine->Title_State->Light/2, Engine->Title_State->Light/2);
        Render_Image_With_Vertex_Color(Engine->Title_State->Logo, 32,40,1, 0,0,Engine->Title_State->Light);
        Engine->Title_State->Light += 0.01;
    }
    else if(Engine->Title_State->Light >= 1)
    {
        if(!Engine->Title_State->Title_Music_Played)
        {
            Mix_PlayMusic(Engine->Title_State->Track, -1);
            Engine->Title_State->Title_Music_Played = true;
        }
        Render_Image_With_Vertex_Color(Engine->Title_State->Background, 0,0,1, 1,1,1);
        Render_Image_With_Vertex_Color(Engine->Title_State->Logo, 32,40,1, 1,1,1);
        Render_Image(Engine->Title_State->Text,16,128,1);
      //  Render_TwoD_Sprite_Animation(Engine->Title_State->Sprite_Objects,0,0);

        Process_Computer(Engine->Title_State->Computer,5);
        Update_Computer_Video_Card(Engine->Title_State->Computer);
        Render_Computer(Engine->Title_State->Computer, 0,0);

        //Render_Image(Engine->Title_State->Percy,0,0,0);
    }
}
void Input_Title_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Start)
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

    if(Keypad.Left)
    {
        Turn_Off_Computer(Engine->Title_State->Computer);
    }

    if(Keypad.Right)
    {
        Turn_On_Computer(Engine->Title_State->Computer);
    }
}
void Process_Title_State(struct _Engine* Engine)
{
    Process_TwoD_Sprite_Animation(Engine->Title_State->Sprite_Objects);
    //Process_Emulator(Engine);
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

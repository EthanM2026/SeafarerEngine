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

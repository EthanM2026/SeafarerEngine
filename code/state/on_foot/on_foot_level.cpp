/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "on_foot_level.h"
#include "stdio.h"
#include "string.h"
void Create_On_Foot_Level_File_Header(const char* Filename)
{
    struct _On_Foot_Level_File_Header Header;
    sprintf(Header.Name, "CSS TEST LEVEL 1");
    Header.Arc = 0;
    Header.Level_ID = 0;
    Header.Must_Be_Certain_Character = 0;
    Header.Default_Starting_Level_Spawnpoint = 0;
    Header.Possible_To_Fail_Level = 0;
    Header.Score_Kept = 0;
    Header.Time_Kept = 0;

    Header.Number_Of_Level_Segment_Files = 1;
    Header.Offset_To_Level_Segment_Files = sizeof(struct _On_Foot_Level_File_Header);


    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _On_Foot_Level_File_Header),f);
    rewind(f);

    fseek(f, Header.Offset_To_Level_Segment_Files, SEEK_SET);
    Write_Level_Segment_Filenames(f);
    rewind(f);

    fclose(f);
};
void Write_Level_Segment_Filenames(FILE* f)
{
    struct _Level_Filepath Filename;
    memset(Filename.Name, 0, 256);
    sprintf(Filename.Name, "resources/data/levels/final_arc/final_arc_test_level.dat");
    Filename.Name[57] = '\0';
    fwrite(&Filename, 1, sizeof(struct _Level_Filepath), f);
};







void Create_On_Foot_Level_Segment_File_Header(const char* Filename)
{
    struct _Level_Segment_File_Header Header;
    Header.Number_Of_Characters = 3; //Clyde, Rosechu, and BlueSpike
    Header.Number_Of_Spawnpoints = 1;

    int Size_Of_Spawnpoint_Area = Header.Number_Of_Characters * sizeof(struct _Level_Character_Entry);
    int Size_Of_Character_Area = Header.Number_Of_Spawnpoints * sizeof(struct _Level_Spawnpoint_Entry);

    int Offset_Location = sizeof(struct _Level_Segment_File_Header);
    Header.Offset_To_Spawnpoints = Offset_Location;
    Offset_Location += Size_Of_Spawnpoint_Area;

    Header.Offset_To_Characters = Offset_Location;

    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _Level_Segment_File_Header),f);
    rewind(f);

    fseek(f, Header.Offset_To_Spawnpoints, SEEK_SET);
    Write_On_Foot_Level_Segment_Spawnpoints(f);
    rewind(f);

    fseek(f, Header.Offset_To_Characters, SEEK_SET);
    Write_On_Foot_Level_Segment_Characters(f);
    rewind(f);

    fclose(f);


};
void Write_On_Foot_Level_Segment_Spawnpoints(FILE* f)
{
    struct _Level_Spawnpoint_Entry Startpoint;
    Startpoint.x = 0;
    Startpoint.y = 0;
    Startpoint.z = 0;

    fwrite(&Startpoint, 1, sizeof(struct _Level_Spawnpoint_Entry), f);

};
void Write_On_Foot_Level_Segment_Characters(FILE* f)
{
    struct _Level_Character_Entry Clyde;

    Clyde.Start_x = 20;
    Clyde.Start_y = 20;
    Clyde.Start_z = 0;

    Clyde.Start_Health = 10;
    Clyde.Start_Energy = 10;
    Clyde.Start_Phases_Through_Player = 1;
    Clyde.Start_Attacks_Phase_Through_Player = 1;
    Clyde.Start_Stays_Defeated = 1;
    Clyde.Start_Damage_Dealt_With_Primary_Attack = 0;
    Clyde.Start_Damage_Dealt_With_Secondary_Attack_A = 0;
    Clyde.Start_Damage_Dealt_With_Secondary_Attack_B = 0;
    Clyde.Start_Damage_Dealt_With_Super_Attack = 0;
    Clyde.Start_Damage_Dealt_With_Desperation_Attack = 0;

    Clyde.Start_Uses_Navigation_Mesh = 0;
    Clyde.Start_Number_Of_Navigation_Meshes = 0;
    Clyde.Start_Navigation_Mesh = 0;

    Clyde.Start_Current_State = 0; //State_T_Pose, Idle, Walk, Jog, Run, Defeated, Pick up Item, Opening Door, Stepping on Foot Switch, Flipping Wall Switch, Entering Vehicle, Starting Machine, Talking, Attacking
    Clyde.Start_Holding_Item = 0;
    Clyde.Start_Holding_Squirt_Gun = 0;
    Clyde.Start_Injured = 0;

    memset(Clyde.Filepath_To_Base_Sprite, 0, 128);
    memset(Clyde.Filepath_To_Base_Texture, 0, 128);
    sprintf(Clyde.Filepath_To_Base_Sprite, "resources/models/model_clyde.dat");
    sprintf(Clyde.Filepath_To_Base_Texture, "resources/models/texture_clyde.bmp");
    Clyde.Filepath_To_Base_Sprite[33] = '\0';
    Clyde.Filepath_To_Base_Texture[35] = '\0';

    Clyde.Can_Be_Idle = 1;
    Clyde.Idle_Frames[0] = 1; //1,2,3,4,5,6,7,8,9,10
    Clyde.Idle_Frames[1] = 10; //1,2,3,4,5,6,7,8,9,10
    Clyde.Idle_Loops = 1;












    struct _Level_Character_Entry Rosie;

    Rosie.Start_x = 60;
    Rosie.Start_y = 10;
    Rosie.Start_z = 0;

    Rosie.Start_Health = 10;
    Rosie.Start_Energy = 10;
    Rosie.Start_Phases_Through_Player = 1;
    Rosie.Start_Attacks_Phase_Through_Player = 1;
    Rosie.Start_Stays_Defeated = 1;
    Rosie.Start_Damage_Dealt_With_Primary_Attack = 0;
    Rosie.Start_Damage_Dealt_With_Secondary_Attack_A = 0;
    Rosie.Start_Damage_Dealt_With_Secondary_Attack_B = 0;
    Rosie.Start_Damage_Dealt_With_Super_Attack = 0;
    Rosie.Start_Damage_Dealt_With_Desperation_Attack = 0;

    Rosie.Start_Uses_Navigation_Mesh = 0;
    Rosie.Start_Number_Of_Navigation_Meshes = 0;
    Rosie.Start_Navigation_Mesh = 0;

    Rosie.Start_Current_State = 0; //State_T_Pose, Idle, Walk, Jog, Run, Defeated, Pick up Item, Opening Door, Stepping on Foot Switch, Flipping Wall Switch, Entering Vehicle, Starting Machine, Talking, Attacking
    Rosie.Start_Holding_Item = 0;
    Rosie.Start_Holding_Squirt_Gun = 0;
    Rosie.Start_Injured = 0;

    memset(Rosie.Filepath_To_Base_Sprite, 0, 128);
    memset(Rosie.Filepath_To_Base_Texture, 0, 128);
    sprintf(Rosie.Filepath_To_Base_Sprite, "resources/models/model_rosemary.dat");
    sprintf(Rosie.Filepath_To_Base_Texture, "resources/models/texture_rosemary.bmp");
    Rosie.Filepath_To_Base_Sprite[36] = '\0';
    Rosie.Filepath_To_Base_Texture[38] = '\0';

    Rosie.Can_Be_Idle = 1;
    Rosie.Idle_Frames[0] = 1; //1,2,3,4,5,6,7,8,9,10
    Rosie.Idle_Frames[1] = 10; //1,2,3,4,5,6,7,8,9,10
    Rosie.Idle_Loops = 1;










     struct _Level_Character_Entry BlueSpike;

    BlueSpike.Start_x = -40;
    BlueSpike.Start_y = 20;
    BlueSpike.Start_z = 0;

    BlueSpike.Start_Health = 10;
    BlueSpike.Start_Energy = 10;
    BlueSpike.Start_Phases_Through_Player = 1;
    BlueSpike.Start_Attacks_Phase_Through_Player = 1;
    BlueSpike.Start_Stays_Defeated = 1;
    BlueSpike.Start_Damage_Dealt_With_Primary_Attack = 0;
    BlueSpike.Start_Damage_Dealt_With_Secondary_Attack_A = 0;
    BlueSpike.Start_Damage_Dealt_With_Secondary_Attack_B = 0;
    BlueSpike.Start_Damage_Dealt_With_Super_Attack = 0;
    BlueSpike.Start_Damage_Dealt_With_Desperation_Attack = 0;

    BlueSpike.Start_Uses_Navigation_Mesh = 0;
    BlueSpike.Start_Number_Of_Navigation_Meshes = 0;
    BlueSpike.Start_Navigation_Mesh = 0;

    BlueSpike.Start_Current_State = 0; //State_T_Pose, Idle, Walk, Jog, Run, Defeated, Pick up Item, Opening Door, Stepping on Foot Switch, Flipping Wall Switch, Entering Vehicle, Starting Machine, Talking, Attacking
    BlueSpike.Start_Holding_Item = 0;
    BlueSpike.Start_Holding_Squirt_Gun = 0;
    BlueSpike.Start_Injured = 0;

    memset(BlueSpike.Filepath_To_Base_Sprite, 0, 128);
    memset(BlueSpike.Filepath_To_Base_Texture, 0, 128);
    sprintf(BlueSpike.Filepath_To_Base_Sprite, "resources/models/model_bluespike.dat");
    sprintf(BlueSpike.Filepath_To_Base_Texture, "resources/models/texture_bluespike.bmp");
    BlueSpike.Filepath_To_Base_Sprite[37] = '\0';
    BlueSpike.Filepath_To_Base_Texture[39] = '\0';

    BlueSpike.Can_Be_Idle = 1;
    BlueSpike.Idle_Frames[0] = 1; //1,2,3,4,5,6,7,8,9,10
    BlueSpike.Idle_Frames[1] = 10; //1,2,3,4,5,6,7,8,9,10
    BlueSpike.Idle_Loops = 1;






    fwrite(&Clyde, 1, sizeof(struct _Level_Character_Entry), f);
    fwrite(&Rosie, 1, sizeof(struct _Level_Character_Entry), f);
    fwrite(&BlueSpike, 1, sizeof(struct _Level_Character_Entry), f);
};


















void Load_On_Foot_Level_Segment_File_Header(struct _Engine* Engine, const char* Filename)
{
    FILE* f;
     f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

//    Engine->On_Foot_State->Header = (struct _Level_Segment_File_Header*)calloc(1,sizeof(struct _Level_Segment_File_Header));


//Engine->On_Foot_State->Header->Number_Of_Spawnpoints = *(int*)&(Buffer[0]);
//Engine->On_Foot_State->Header->Number_Of_Characters = *(int*)&(Buffer[4]);
//Engine->On_Foot_State->Header->Offset_To_Spawnpoints = *(int*)&(Buffer[8]);
//Engine->On_Foot_State->Header->Offset_To_Characters = *(int*)&(Buffer[12]);


//printf("CHARACTERS: %d\n", Engine->On_Foot_State->Header->Number_Of_Characters);

//printf("OFFSET TO CHARACTERS: %d\n", Engine->On_Foot_State->Header->Offset_To_Characters);


//Engine->On_Foot_State->Characters = (struct _On_Foot_Level_Character_Object*)calloc(1,Engine->On_Foot_State->Header->Number_Of_Characters*sizeof(struct _On_Foot_Level_Character_Object));


//for(int j = 0; j < Engine->On_Foot_State->Header->Number_Of_Characters; j++)
{
 //   printf("LOADING CHARACTER!\n");
 //   Buffer_Location = Engine->On_Foot_State->Header->Offset_To_Characters + (sizeof(struct _Level_Character_Entry)*j); //The int at 52
 //   Engine->On_Foot_State->Characters[j].Models = Create_MD2_Model();
  //  char Model_Filepath[128];
  //  char Texture_Filepath[128];
  //  printf("Model: \n");
  //  for(int x = 0; x < 128; x++)
    {
 //       Model_Filepath[x] = Buffer[Buffer_Location+x];
 //       printf("%c", Model_Filepath[x]);
    }

 //   printf("Texture: \n");
 //   for(int y = 0; y < 128; y++)
    {
 //       Texture_Filepath[y] = Buffer[Buffer_Location+y+128];
 //       printf("%c", Texture_Filepath[y]);
    }
//    Load_MD2_Model(Engine->On_Foot_State->Characters[j].Models, Model_Filepath);
//    Load_MD2_Texture(Engine->On_Foot_State->Characters[j].Models, Texture_Filepath);

//    Engine->On_Foot_State->Characters[j].x = *(float*)&(Buffer[Buffer_Location+512-256]);
//    Engine->On_Foot_State->Characters[j].y = *(float*)&(Buffer[Buffer_Location+516-256]);
 //   Engine->On_Foot_State->Characters[j].z = *(float*)&(Buffer[Buffer_Location+520-256]);
}
printf("Characters Loaded\n");


}



void Load_On_Foot_Level_File_Header(struct _Engine* Engine, const char* Filename)
{
     FILE* f;
     f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

    //Engine->On_Foot_State->File_Header = (struct _On_Foot_Level_File_Header*)calloc(1,sizeof(struct _On_Foot_Level_File_Header));


//Engine->On_Foot_State->File_Header->Number_Of_Level_Segment_Files = *(int*)&(Buffer[0]);
//Engine->On_Foot_State->File_Header->Offset_To_Level_Segment_Files = *(int*)&(Buffer[4]);




//Engine->On_Foot_State->Filepaths= (struct _Level_Filepath*)calloc(1,Engine->On_Foot_State->File_Header->Number_Of_Level_Segment_Files*sizeof(struct _Level_Filepath));


//for(int j = 0; j < Engine->On_Foot_State->File_Header->Number_Of_Level_Segment_Files; j++)
{
   // printf("LOADING LEVEL SEGMENT FILEPATH!\n");
    //Buffer_Location = Engine->On_Foot_State->File_Header->Offset_To_Level_Segment_Files + (sizeof(struct _Level_Filepath)*j); //The int at 52
    //Engine->On_Foot_State->Filepaths = (struct _Level_Filepath*) calloc(1,sizeof(struct _Level_Filepath));
   // for(int x = 0; x < 256; x++)
    {
   //    Engine->On_Foot_State->Filepaths[j].Name[x] = Buffer[Buffer_Location+x];
   //     printf("%c", Engine->On_Foot_State->Filepaths[j].Name[x]);
    }
}



};



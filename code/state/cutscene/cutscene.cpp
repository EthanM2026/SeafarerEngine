/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "cutscene.h"
#include <stdio.h>
#include <string.h>

struct _Cutscene_State* Create_Cutscene_State()
{
    struct _Cutscene_State* I = (struct _Cutscene_State*) calloc(1,sizeof (struct _Cutscene_State));
    return I;
};

void Destroy_Cutscene_State(struct _Engine* Engine)
{

};

void Initialize_Cutscene_State(struct _Engine* Engine)
{
    Write_Cutscene("scene");
    Engine->Cutscene_State->Render = true;
    Engine->Cutscene_State->Cleared_To_Animate = false;
    Engine->Cutscene_State->Animation_Interval_Window = 0;
    glLightf(1,1,1);

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
};

void Render_Cutscene_State(struct _Engine* Engine)
{
    if(Engine->Cutscene_State->Render)
    {
        for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Actors; i++)
        {
            glPushMatrix();
            glTranslatef(Engine->Cutscene_State->Cutscene_Actor_Object[i].x,Engine->Cutscene_State->Cutscene_Actor_Object[i].y,Engine->Cutscene_State->Cutscene_Actor_Object[i].z);
            Render_SE3_Model(Engine->Cutscene_State->Cutscene_Actor_Object[i].Model,Engine->Cutscene_State->Cutscene_Actor_Object[i].Current_Keyframe,0,0,0,0,0,0,false,1,1,1,false);
            glPopMatrix();
        }
    }
};

void Input_Cutscene_State(struct _Engine* Engine, struct _Keypad Keypad)
{

};

void Process_Cutscene_State(struct _Engine* Engine)
{
     double x = Get_Ticks(Engine->Cutscene_State->Timer);
     if(x > Engine->Cutscene_State->Animation_Interval_Window + 0.5)
     {
        if(!Engine->Cutscene_State->Cleared_To_Animate)
        {
            Engine->Cutscene_State->Cleared_To_Animate = true;
        }

        else if(Engine->Cutscene_State->Cleared_To_Animate)
        {
            Engine->Cutscene_State->Cleared_To_Animate = false;
        }

            Engine->Cutscene_State->Animation_Interval_Window = x;
     }
     if(x >= Engine->Cutscene_State->Header.Total_Duration_In_Seconds)
     {
            Mix_HaltMusic();
            Engine->Cutscene_State->Render = false;
     }

     if(x >= Engine->Cutscene_State->Header.Total_Duration_In_Seconds + 5)
     {
            printf("Executing NPS from cutscene State!\n");
            New_Process_Steps(Engine);
     }

     for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes; i++)
     {
        if(x >= Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].At_This_Time && Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Complete != 1)
        {
            Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Complete = 1;
            printf("Cam Action\n");

            int For_This_Camera = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Active_Camera;

            switch(Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Action)
            {
                case CAMERA_ACTION_MOVE_CAMERA:
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].x = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].New_x;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].y = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].New_y;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].z = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].New_z;

                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_x = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].New_Center_x;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_y = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].New_Center_y;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_z = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].New_Center_z;

                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Following_This_Actor = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Looking_At_This_Actor;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].x_Offset = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Look_Offset_x;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].y_Offset = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Look_Offset_y;
                    Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].z_Offset = Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[i].Look_Offset_z;

                    printf("New Coords: %f %f %f\n", Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].x,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].y,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].z);

                    printf("New Center: %f %f %f\n", Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_x,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_y,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_z);

                    if(Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Active)
                    {
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        gluLookAt(Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].x,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].y,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].z,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_x,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_y,Engine->Cutscene_State->Cutscene_Camera_Object[For_This_Camera].Center_z,0,0,1);
                    }
                break;
            }
        }
     }


     for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes; i++)
     {
        if(x >= Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].At_This_Time && Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].Complete != 1)
        {
            Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].Complete = 1;
            printf("Actor Motion\n");
            int ID = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].For_This_ID;

            printf("For Actor ID %d\n", ID);

            if(!Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].Teleport)
            {
                Engine->Cutscene_State->Cutscene_Actor_Object[ID].dx = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].dx;
                Engine->Cutscene_State->Cutscene_Actor_Object[ID].dy = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].dy;
                Engine->Cutscene_State->Cutscene_Actor_Object[ID].dz = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[i].dz;
            }
        }
     }

     for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes; i++)
     {
        if(x >= Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].At_This_Time && Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].Complete != 1)
        {
            Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].Complete = 1;
            printf("Actor Animation\n");
            int ID = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].For_This_ID;

            Engine->Cutscene_State->Cutscene_Actor_Object[ID].New_Keyframes[0] = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].New_Keyframes[0];
            Engine->Cutscene_State->Cutscene_Actor_Object[ID].New_Keyframes[1] = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].New_Keyframes[1];
            Engine->Cutscene_State->Cutscene_Actor_Object[ID].Current_Keyframe = Engine->Cutscene_State->Cutscene_Actor_Object[ID].New_Keyframes[0];
            Engine->Cutscene_State->Cutscene_Actor_Object[ID].Animate = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].Animate;
            Engine->Cutscene_State->Cutscene_Actor_Object[ID].Loop = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[i].Loop;

            printf("New Keyframes: %d %d\n", Engine->Cutscene_State->Cutscene_Actor_Object[ID].New_Keyframes[0],Engine->Cutscene_State->Cutscene_Actor_Object[ID].New_Keyframes[1]);
            printf("Current Keyframe: %d\n", Engine->Cutscene_State->Cutscene_Actor_Object[ID].Current_Keyframe);
            printf("Animate: %d\n", Engine->Cutscene_State->Cutscene_Actor_Object[ID].Animate);
        }
     }

     for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Actors; i++)
     {
            Engine->Cutscene_State->Cutscene_Actor_Object[i].x += Engine->Cutscene_State->Cutscene_Actor_Object[i].dx;
            Engine->Cutscene_State->Cutscene_Actor_Object[i].y += Engine->Cutscene_State->Cutscene_Actor_Object[i].dy;
            Engine->Cutscene_State->Cutscene_Actor_Object[i].z += Engine->Cutscene_State->Cutscene_Actor_Object[i].dz;
           // printf("Current Keyframe: %d\n", Engine->Cutscene_State->Cutscene_Actor_Object[i].Current_Keyframe);
            if(Engine->Cutscene_State->Cutscene_Actor_Object[i].Animate)
            {
            if(Engine->Cutscene_State->Cutscene_Actor_Object[i].Current_Keyframe < Engine->Cutscene_State->Cutscene_Actor_Object[i].New_Keyframes[1])
            {
                Engine->Cutscene_State->Cutscene_Actor_Object[i].Current_Keyframe += 1;
                if(Engine->Cutscene_State->Cutscene_Actor_Object[i].Current_Keyframe == Engine->Cutscene_State->Cutscene_Actor_Object[i].New_Keyframes[1] && Engine->Cutscene_State->Cutscene_Actor_Object[i].Loop)
                {
                    Engine->Cutscene_State->Cutscene_Actor_Object[i].Current_Keyframe = Engine->Cutscene_State->Cutscene_Actor_Object[i].New_Keyframes[0];
                }
            }
            }
     }


    for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Cameras; i++)
     {
     int Follow_Actor = Engine->Cutscene_State->Cutscene_Camera_Object[i].Following_This_Actor;

     if(Engine->Cutscene_State->Cutscene_Camera_Object[i].Active && Follow_Actor != -1)
                    {
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        gluLookAt(Engine->Cutscene_State->Cutscene_Camera_Object[i].x,Engine->Cutscene_State->Cutscene_Camera_Object[i].y,Engine->Cutscene_State->Cutscene_Camera_Object[i].z,Engine->Cutscene_State->Cutscene_Actor_Object[Follow_Actor].x,Engine->Cutscene_State->Cutscene_Actor_Object[Follow_Actor].y,Engine->Cutscene_State->Cutscene_Actor_Object[Follow_Actor].z,0,0,1);
                    }
    }







    for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes; i++)
     {
        if(x >= Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[i].At_This_Time && Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[i].Complete != 1)
        {
            Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[i].Complete = 1;
            Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[i].Playing = 1;
            printf("Track Action\n");

            Mix_PlayMusic(Engine->Cutscene_State->Tracks, 0);
        }
     }



     for(int i = 0; i < Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes; i++)
     {
        if(x >= Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[i].At_This_Time && Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[i].Complete != 1)
        {
            Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[i].Complete = 1;
            Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[i].Playing = 1;
            printf("SFX Action\n");

            Mix_PlayChannel(-1,Engine->Cutscene_State->SFX, 0);
        }
     }
};


void Write_Cutscene(const char* Filename)
{
    struct _Cutscene_Header H;

    memset(H.Name,0,256);

    H.Total_Duration_In_Seconds = 10;
    H.Number_Of_Actors = 1;
    H.Number_Of_Cameras = 1;
    H.Number_Of_Camera_Action_Keyframes = 1;
    H.Number_Of_Actor_Motion_Keyframes = 1;
    H.Number_Of_Actor_Animation_Keyframes = 1;
    H.Number_Of_Tracks = 1;
    H.Number_Of_SFX = 1;
    H.Number_Of_Track_Action_Keyframes = 1;
    H.Number_Of_SFX_Action_Keyframes = 1;

    int Size_Of_Actors = H.Number_Of_Actors * sizeof(struct _Cutscene_Actor);
    int Size_Of_Cameras = H.Number_Of_Cameras * sizeof(struct _Cutscene_Camera);
    int Size_Of_Camera_Action_Keyframes = H.Number_Of_Camera_Action_Keyframes * sizeof(struct _Cutscene_Camera_Action_Keyframe);
    int Size_Of_Actor_Motion_Keyframes = H.Number_Of_Actor_Motion_Keyframes * sizeof(struct _Cutscene_Actor_Motion_Keyframe);
    int Size_Of_Actor_Animation_Keyframes = H.Number_Of_Actor_Animation_Keyframes * sizeof(struct _Cutscene_Actor_Animation_Keyframe);
    int Size_Of_Tracks = H.Number_Of_Tracks * 256;
    int Size_Of_SFX = H.Number_Of_SFX * 256;
    int Size_Of_Track_Action_Keyframes = H.Number_Of_Track_Action_Keyframes * sizeof(struct _Cutscene_Track_Action_Keyframe);
    int Size_Of_SFX_Action_Keyframes = H.Number_Of_SFX_Action_Keyframes * sizeof(struct _Cutscene_SFX_Action_Keyframe);

    H.Offset_To_Actors = sizeof(struct _Cutscene_Header);
    H.Offset_To_Cameras = H.Offset_To_Actors + Size_Of_Actors;
    H.Offset_To_Camera_Action_Keyframes = H.Offset_To_Cameras + Size_Of_Cameras;
    H.Offset_To_Actor_Motion_Keyframes = H.Offset_To_Camera_Action_Keyframes + Size_Of_Camera_Action_Keyframes;
    H.Offset_To_Actor_Animation_Keyframes = H.Offset_To_Actor_Motion_Keyframes + Size_Of_Actor_Motion_Keyframes;
    H.Offset_To_Tracks = H.Offset_To_Actor_Animation_Keyframes + Size_Of_Actor_Animation_Keyframes;
    H.Offset_To_SFX = H.Offset_To_Tracks + Size_Of_Tracks;
    H.Offset_To_Track_Action_Keyframes = H.Offset_To_SFX + Size_Of_SFX;
    H.Offset_To_SFX_Action_Keyframes = H.Offset_To_Track_Action_Keyframes + Size_Of_Track_Action_Keyframes;

    printf("Offset to Actors is %d\n", H.Offset_To_Actors);
    printf("Offset to Cameras is %d\n", H.Offset_To_Cameras);
    printf("Offset to Camera Actions is %d\n", H.Offset_To_Camera_Action_Keyframes);

    printf("Offset to Actor Motion is %d\n", H.Offset_To_Actor_Motion_Keyframes);
    printf("Offset to Actor Animations is %d\n", H.Offset_To_Actor_Animation_Keyframes);

    printf("Offset to Tracks is %d\n", H.Offset_To_Tracks);


    struct _Cutscene_Actor CA;
    memset(CA.Filepath,0,256);
    memset(CA.Texture_Filepath,0,256);
    CA.Height = 40;
    CA.x = 10;
    CA.y = 10;
    CA.z = 10;

    struct _Cutscene_Camera CC;
    CC.x = 100;
    CC.y = 100;
    CC.z = 40;

    CC.Center_x = 0;
    CC.Center_y = 0;
    CC.Center_z = 40;

    CC.Active = 1;

    sprintf(CA.Filepath,"resources/data/characters/rosemary/rosemary.se3");
    sprintf(CA.Texture_Filepath,"resources/data/characters/rosemary/rosemary.sei");

    char Track[256];
    memset(Track,0,256);
    sprintf(Track, "track.mp3");

    struct _Cutscene_Track_Action_Keyframe CTAK;
    CTAK.At_This_Time = 5;
    CTAK.Complete = 0;
    CTAK.Playing = 1;
    CTAK.This_Track = 0;
    CTAK.Volume = 1;





    char SFX[256];
    memset(SFX,0,256);
    sprintf(SFX, "sfx.wav");

    struct _Cutscene_SFX_Action_Keyframe CSAK;
    CSAK.At_This_Time = 7;
    CSAK.Complete = 0;
    CSAK.Playing = 1;
    CSAK.This_Track = 0;
    CSAK.Volume = 1;

    struct _Cutscene_Camera_Action_Keyframe CCAK;
    CCAK.At_This_Time = 3;
    CCAK.Complete = 0;
    CCAK.Action = CAMERA_ACTION_MOVE_CAMERA;
    CCAK.New_x = 100;
    CCAK.New_y = 150;
    CCAK.New_z = 100;

    CCAK.New_Center_x = 0;
    CCAK.New_Center_y = 0;
    CCAK.New_Center_z = 0;

    struct _Cutscene_Actor_Motion_Keyframe CAMK;
    CAMK.At_This_Time = 5;
    CAMK.For_This_ID = 0;
    CAMK.Complete = 0;
    CAMK.Teleport = 0;
    CAMK.dx = 0.1;
    CAMK.dy = 0;
    CAMK.dz = 0;

    struct _Cutscene_Actor_Animation_Keyframe CAAK;
    CAAK.At_This_Time = 5;
    CAAK.Complete = 0;
    CAAK.For_This_ID = 0;
    CAAK.Loop = 1;
    CAAK.New_Keyframes[0] = 0;
    CAAK.New_Keyframes[1] = 9;


    FILE* f = fopen(Filename, "wb");
    fwrite(&H, 1, sizeof(struct _Cutscene_Header),f);
    rewind(f);

    fseek(f, H.Offset_To_Actors, SEEK_SET);
    fwrite(&CA, 1, sizeof(struct _Cutscene_Actor),f);
    rewind(f);

    fseek(f, H.Offset_To_Cameras, SEEK_SET);
    fwrite(&CC, 1, sizeof(struct _Cutscene_Camera),f);
    rewind(f);

    fseek(f, H.Offset_To_Tracks, SEEK_SET);
    fwrite(&Track, 1, 256,f);
    rewind(f);

    fseek(f, H.Offset_To_SFX, SEEK_SET);
    fwrite(&SFX, 1, 256,f);
    rewind(f);

    fseek(f, H.Offset_To_Track_Action_Keyframes, SEEK_SET);
    fwrite(&CTAK, 1, sizeof(struct _Cutscene_Track_Action_Keyframe),f);
    rewind(f);

    fseek(f, H.Offset_To_SFX_Action_Keyframes, SEEK_SET);
    fwrite(&CSAK, 1, sizeof(struct _Cutscene_SFX_Action_Keyframe),f);
    rewind(f);

    fseek(f, H.Offset_To_Camera_Action_Keyframes, SEEK_SET);
    fwrite(&CCAK, 1, sizeof(struct _Cutscene_Camera_Action_Keyframe),f);
    rewind(f);

    fseek(f, H.Offset_To_Actor_Animation_Keyframes, SEEK_SET);
    fwrite(&CAAK, 1, sizeof(struct _Cutscene_Actor_Animation_Keyframe),f);
    rewind(f);

    fseek(f, H.Offset_To_Actor_Motion_Keyframes, SEEK_SET);
    fwrite(&CAMK, 1, sizeof(struct _Cutscene_Actor_Motion_Keyframe),f);
    rewind(f);

    fclose(f);
};

void Load_Cutscene(struct _Engine* Engine, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(&Buffer, sizeof(unsigned char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;

    for(int i = 0; i < 256; i++)
    {
        Engine->Cutscene_State->Header.Name[i] = Buffer[i+Buffer_Location];
    }
    Buffer_Location += 256;

    Engine->Cutscene_State->Header.Total_Duration_In_Seconds = *(double*)&(Buffer[Buffer_Location]);
    Engine->Cutscene_State->Header.Number_Of_Actors = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->Cutscene_State->Header.Number_Of_Cameras = *(int*)&(Buffer[Buffer_Location+12]);
    Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes = *(int*)&(Buffer[Buffer_Location+16]);
    Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes = *(int*)&(Buffer[Buffer_Location+20]);
    Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes = *(int*)&(Buffer[Buffer_Location+24]);
    Engine->Cutscene_State->Header.Number_Of_Tracks = *(int*)&(Buffer[Buffer_Location+28]);
    Engine->Cutscene_State->Header.Number_Of_SFX = *(int*)&(Buffer[Buffer_Location+32]);
    Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes = *(int*)&(Buffer[Buffer_Location+36]);
    Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes = *(int*)&(Buffer[Buffer_Location+40]);

    printf("Number of Actors: %d\n", Engine->Cutscene_State->Header.Number_Of_Actors);
    printf("Number of Cameras: %d\n", Engine->Cutscene_State->Header.Number_Of_Cameras);
    printf("Number of Tracks: %d\n", Engine->Cutscene_State->Header.Number_Of_Tracks);

    Engine->Cutscene_State->Header.Offset_To_Actors = *(int*)&(Buffer[Buffer_Location+44]);
    Engine->Cutscene_State->Header.Offset_To_Cameras = *(int*)&(Buffer[Buffer_Location+48]);
    Engine->Cutscene_State->Header.Offset_To_Camera_Action_Keyframes = *(int*)&(Buffer[Buffer_Location+52]);
    Engine->Cutscene_State->Header.Offset_To_Actor_Motion_Keyframes = *(int*)&(Buffer[Buffer_Location+56]);
    Engine->Cutscene_State->Header.Offset_To_Actor_Animation_Keyframes = *(int*)&(Buffer[Buffer_Location+60]);
    Engine->Cutscene_State->Header.Offset_To_Tracks = *(int*)&(Buffer[Buffer_Location+64]);
    Engine->Cutscene_State->Header.Offset_To_SFX = *(int*)&(Buffer[Buffer_Location+68]);
    Engine->Cutscene_State->Header.Offset_To_Track_Action_Keyframes = *(int*)&(Buffer[Buffer_Location+72]);
    Engine->Cutscene_State->Header.Offset_To_SFX_Action_Keyframes = *(int*)&(Buffer[Buffer_Location+76]);

    Engine->Cutscene_State->Cutscene_Actor_Object = (struct _Cutscene_Actor_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Actors,sizeof(struct _Cutscene_Actor_Object));
    Engine->Cutscene_State->Cutscene_Camera_Object = (struct _Cutscene_Camera_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Cameras,sizeof(struct _Cutscene_Camera_Object));
    Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe = (struct _Cutscene_Camera_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes,sizeof(struct _Cutscene_Camera_Action_Keyframe));
    Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe = (struct _Cutscene_Actor_Motion_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes,sizeof(struct _Cutscene_Actor_Motion_Keyframe));
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe = (struct _Cutscene_Actor_Animation_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes,sizeof(struct _Cutscene_Actor_Animation_Keyframe));
    Engine->Cutscene_State->Cutscene_Track_Object = (struct _Cutscene_Track_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Tracks,sizeof(struct _Cutscene_Track_Object));
    Engine->Cutscene_State->Cutscene_SFX_Object = (struct _Cutscene_SFX_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_SFX,sizeof(struct _Cutscene_SFX_Object));
    Engine->Cutscene_State->Cutscene_Track_Action_Keyframe = (struct _Cutscene_Track_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes,sizeof(struct _Cutscene_Track_Action_Keyframe ));
    Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe = (struct _Cutscene_SFX_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes,sizeof(struct _Cutscene_SFX_Action_Keyframe));



    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actors; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));
        printf("Found Actor \n");

        char Actor_Model[256];
        char Actor_Texture[256];

        for(int l = 0; l < 256; l++)
        {
            Actor_Model[l] = Buffer[Buffer_Location+l];
            Actor_Texture[l] = Buffer[Buffer_Location+l+256];
        }
        printf("Model: ");

        for(int l = 0; l < 256; l++)
        {
            printf("%c",Actor_Model[l]);
        }

        printf("\nTexture: ");

        for(int l = 0; l < 256; l++)
        {
            printf("%c",Actor_Texture[l]);
        }
        printf("\n");

        Buffer_Location += 512;

        Engine->Cutscene_State->Cutscene_Actor_Object[j].Model = Create_SE3_Model();
        Load_SE3_Model(Engine->Cutscene_State->Cutscene_Actor_Object[j].Model,Actor_Model);
        Load_SE3_Texture(Engine->Cutscene_State->Cutscene_Actor_Object[j].Model,Actor_Texture);

        Engine->Cutscene_State->Cutscene_Actor_Object[j].Height = *(double*)&(Buffer[Buffer_Location]);
    Engine->Cutscene_State->Cutscene_Actor_Object[j].x = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->Cutscene_State->Cutscene_Actor_Object[j].y = *(double*)&(Buffer[Buffer_Location+16]);
    Engine->Cutscene_State->Cutscene_Actor_Object[j].z = *(double*)&(Buffer[Buffer_Location+24]);

        printf("Height %f x %f y %f z %f\n", Engine->Cutscene_State->Cutscene_Actor_Object[j].Height,Engine->Cutscene_State->Cutscene_Actor_Object[j].x,Engine->Cutscene_State->Cutscene_Actor_Object[j].y,Engine->Cutscene_State->Cutscene_Actor_Object[j].z);
    }




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
    gluPerspective(45, 1.7777,1.0f, METER_CONVERSION * 75000);
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




    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Cameras; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Cameras + (j*sizeof(struct _Cutscene_Camera));

        Engine->Cutscene_State->Cutscene_Camera_Object[j].x = *(double*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].z = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_x = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_y = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_z = *(double*)&(Buffer[Buffer_Location+40]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Active = *(int*)&(Buffer[Buffer_Location+48]);

        Engine->Cutscene_State->Cutscene_Camera_Object[j].Following_This_Actor= *(int*)&(Buffer[Buffer_Location+52]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].x_Offset = *(double*)&(Buffer[Buffer_Location+56]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].y_Offset = *(double*)&(Buffer[Buffer_Location+64]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].z_Offset = *(double*)&(Buffer[Buffer_Location+72]);

        printf("Camera x %f y %f z %f", Engine->Cutscene_State->Cutscene_Camera_Object[j].x,Engine->Cutscene_State->Cutscene_Camera_Object[j].y,Engine->Cutscene_State->Cutscene_Camera_Object[j].z);
        if(Engine->Cutscene_State->Cutscene_Camera_Object[j].Active)
        {
            gluLookAt(Engine->Cutscene_State->Cutscene_Camera_Object[j].x,Engine->Cutscene_State->Cutscene_Camera_Object[j].y,Engine->Cutscene_State->Cutscene_Camera_Object[j].z,Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_x,Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_y,Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_z,0,0,1);
        }
    }

    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Camera_Action_Keyframes + (j*sizeof(struct _Cutscene_Camera_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].At_This_Time = *(double*)&(Buffer[Buffer_Location]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+8]);

Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Action = *(int*)&(Buffer[Buffer_Location+12]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Active_Camera = *(int*)&(Buffer[Buffer_Location+16]);

Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_x = *(double*)&(Buffer[Buffer_Location+20]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_y = *(double*)&(Buffer[Buffer_Location+28]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_z = *(double*)&(Buffer[Buffer_Location+36]);

Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Center_x = *(double*)&(Buffer[Buffer_Location+44]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Center_y = *(double*)&(Buffer[Buffer_Location+52]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Center_z = *(double*)&(Buffer[Buffer_Location+60]);

Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Looking_At_This_Actor = *(int*)&(Buffer[Buffer_Location+68]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Look_Offset_x = *(double*)&(Buffer[Buffer_Location+72]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Look_Offset_y = *(double*)&(Buffer[Buffer_Location+80]);
Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Look_Offset_z = *(double*)&(Buffer[Buffer_Location+88]);

    printf("Keyframe new x y z %f %f %f\n", Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_x,Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_y,Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_z);

    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actor_Motion_Keyframes + (j*sizeof(struct _Cutscene_Actor_Motion_Keyframe));

        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].At_This_Time = *(double*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].For_This_ID = *(int*)&(Buffer[Buffer_Location+12]);

        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].dx = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].dy = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].dz = *(double*)&(Buffer[Buffer_Location+32]);

        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Teleport = *(int*)&(Buffer[Buffer_Location+40]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].New_x = *(double*)&(Buffer[Buffer_Location+48]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].New_y = *(double*)&(Buffer[Buffer_Location+56]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].New_z = *(double*)&(Buffer[Buffer_Location+64]);

        printf("At this time %f dx %f dy %f dz %f", Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].At_This_Time,Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].dx,Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].dy,Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].dz);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actor_Animation_Keyframes + (j*sizeof(struct _Cutscene_Actor_Animation_Keyframe));

        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].At_This_Time = *(double*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Visible = *(int*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+12]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].For_This_ID = *(int*)&(Buffer[Buffer_Location+16]);

        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Update_Animation = *(int*)&(Buffer[Buffer_Location+20]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[0] = *(int*)&(Buffer[Buffer_Location+24]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[1] = *(int*)&(Buffer[Buffer_Location+28]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Loop = *(int*)&(Buffer[Buffer_Location+32]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Rate = *(double*)&(Buffer[Buffer_Location+36]);
        Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Animate = *(int*)&(Buffer[Buffer_Location+44]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Tracks; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Tracks + (j*256);
        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
            printf("%c", Filepath[l]);
        }

        Engine->Cutscene_State->Tracks=Mix_LoadMUS(Filepath);
        //Mix_PlayMusic(Engine->Cutscene_State->Tracks, 0);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_SFX; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_SFX + (j*256);
        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
            printf("%c", Filepath[l]);
        }

        Engine->Cutscene_State->SFX =Mix_LoadWAV(Filepath);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Track_Action_Keyframes + (j*sizeof(struct _Cutscene_Track_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].At_This_Time = *(double*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].Playing = *(int*)&(Buffer[Buffer_Location+12]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].Volume = *(int*)&(Buffer[Buffer_Location+16]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].This_Track = *(int*)&(Buffer[Buffer_Location+20]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_SFX_Action_Keyframes + (j*sizeof(struct _Cutscene_SFX_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].At_This_Time = *(double*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].Playing = *(int*)&(Buffer[Buffer_Location+12]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].Volume = *(int*)&(Buffer[Buffer_Location+16]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].This_Track = *(int*)&(Buffer[Buffer_Location+20]);
    }

    printf("Cutscene loaded.\n");

    Engine->Cutscene_State->Timer = Create_Timer();
    Set_Timer(Engine->Cutscene_State->Timer);
};

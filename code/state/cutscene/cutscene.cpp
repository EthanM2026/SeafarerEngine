#include "cutscene.h"
#include <stdio.h>
#include <string.h>


struct _Cutscene_State* Create_Cutscene_State()
{
    struct _Cutscene_State* M = (struct _Cutscene_State*) calloc(1, sizeof(struct _Cutscene_State));
    return M;
}
void Destroy_Cutscene_State(struct _Engine* Engine)
{

}
void Initialize_Cutscene_State(struct _Engine* Engine)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,0.1f, 10000000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(80,80,80,0,0,20,0,0,1);

    //Write_Cutscene("TESTCUTSCENE");
    Engine->Cutscene_State->Timer = Create_Timer();
    Set_Timer(Engine->Cutscene_State->Timer);

    Engine->Cutscene_State->Laps = 0;
    Engine->Cutscene_State->Render = true;
}


void Render_Cutscene_State(struct _Engine* Engine)
{
    if(Engine->Cutscene_State->Render)
    {
    if(Engine->Cutscene_State->Header.Number_Of_Backgrounds > 0)
    {
        for(int j =0; j < Engine->Cutscene_State->Header.Number_Of_Backgrounds; j++)
        {
            glPushMatrix();
            glTranslatef(0,0,0);
            //if(Engine->Cutscene_State->Cutscene_Background_Object[j].Visible == 1)
            {
                Render_MD2_Model_GLDraw(Engine->Cutscene_State->Cutscene_Background_Object[j].Model,0,0,0,0,0,0,0,true,1,1,1);
            }
            glPopMatrix();
        }
    }



    if(Engine->Cutscene_State->Header.Number_Of_Actors > 0)
    {
     //printf("Actors: %d\n", Engine->Loaded_Level_Regions->Number_Of_Actors);
        for(int j =0; j < Engine->Cutscene_State->Header.Number_Of_Actors; j++) //272
        {

         glPushMatrix();
         glTranslatef(Engine->Cutscene_State->Cutscene_Actor_Object[j].x,Engine->Cutscene_State->Cutscene_Actor_Object[j].y,Engine->Cutscene_State->Cutscene_Actor_Object[j].z);
         //if(Engine->Cutscene_State->Cutscene_Actor_Object[j].Visible == 1)
            {
                //glRotatef(Face_Object(Engine->Cutscene_State->Cutscene_Actor_Object[j].x, Engine->Cutscene_State->Cutscene_Player->x, Engine->Cutscene_State->Cutscene_Actor_Object[j].y, Engine->Cutscene_State->Cutscene_Player->y),0,0,1);

             Render_MD2_Model_GLDraw(Engine->Cutscene_State->Cutscene_Actor_Object[j].Model,Engine->Cutscene_State->Cutscene_Actor_Object[j].Current_Keyframe,0,0,0,0,0,0,false,1,1,1);
            }
         glPopMatrix();
        }
    }


glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();



    if(Engine->Cutscene_State->Header.Number_Of_Captions > 0)
    {
        for(int j =0; j < Engine->Cutscene_State->Header.Number_Of_Captions; j++)
        {
            if(Engine->Cutscene_State->Cutscene_Caption_Object[j].Visible == 1)
            {
                Render_Image(Engine->Cutscene_State->Cutscene_Caption_Object[j].Image,Center_Coordinates(Engine->Cutscene_State->Cutscene_Caption_Object[j].Length*17,640),256+32,1);
            }
        }
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,0.1f, 10000000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(80,80,80,0,0,20,0,0,1);
}
}

void Input_Cutscene_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    double z = Get_Ticks(Engine->Cutscene_State->Timer);
    if(z > Engine->Cutscene_State->Header.Total_Duration_In_Minutes + 5)
    {
        printf("Cutscene Complete!\n");
        Engine->Current_Step += 1;
        Process_Engine_Steps(Engine);
    }
}


void Process_Cutscene_State(struct _Engine* Engine)
{
    float Current_Time = Get_Ticks(Engine->Cutscene_State->Timer);
    //printf("%f\n", Current_Time);



    if(Current_Time > Engine->Cutscene_State->Header.Total_Duration_In_Minutes)
    {
      //  Resume_Timer(Engine->Cutscene_State->Timer);
        Engine->Cutscene_State->Render = false;
    }


    if(Current_Time/Engine->Cutscene_State->Laps  >= 0.12 )
    {
            for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actors; j++)
    {
   // printf("First Frame: %d\n", Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[0]);
   // printf("Last Frame: %d\n", Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[1]);
    if(Engine->Cutscene_State->Cutscene_Actor_Object[j].Current_Keyframe + 1 > Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[1])
    {
        Engine->Cutscene_State->Cutscene_Actor_Object[j].Current_Keyframe = Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[0];
    }
    if(Engine->Cutscene_State->Cutscene_Actor_Object[j].Current_Keyframe + 1 <= Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[1])
    {
        Engine->Cutscene_State->Cutscene_Actor_Object[j].Current_Keyframe += 1;
    }
    }

        Engine->Cutscene_State->Laps +=1;
    }









    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes; j++)
    {
        if(Current_Time >= Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].At_This_Time)
        {
            if(!Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Complete)
            {
                Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Complete = 1;
                int For_This_ID = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].For_This_ID;
                if(Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Update_Heading)
                {

                }

                else if(Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Update_Location)
                {
                    Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].x = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].x;
                    Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].y = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].y;
                    Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].z = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].z;
                }
            }
        }
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Caption_Action_Keyframes; j++)
    {
        if(Current_Time >= Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].At_This_Time)
        {
            if(!Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].Complete)
            {
                Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].Complete = 1;
                int For_This_ID = Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].For_This_ID;
                Engine->Cutscene_State->Cutscene_Caption_Object[For_This_ID].Visible = Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].Visible;

            }
        }
    }



    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes; j++)
    {
        if(Current_Time >= Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].At_This_Time)
        {
            if(!Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Complete)
            {
                Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Complete = 1;
                int For_This_ID = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].For_This_ID;

                Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].Current_Keyframe = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[1];

                Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].New_Keyframes[0] = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[0];
                Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].New_Keyframes[1] = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[1];
                Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].Loop = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Loop;
                Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].Rate = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Rate;
                //Engine->Cutscene_State->Cutscene_Actor_Object[For_This_ID].Update_Animation = Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Update_Animation;
            }
        }
    }
}

void Create_Cutscene_File(const char* Filename)
{



}
void Load_Cutscene_File(struct _Engine* Engine, const char* Filename)
{

}




void Write_Cutscene(const char* Filename)
{
    struct _Cutscene_Header Header;

    Header.Total_Duration_In_Minutes = 10;
    Header.Number_Of_Actors = 1;
    Header.Number_Of_Backgrounds = 1;
    Header.Number_Of_Cameras = 0;
    Header.Number_Of_Tracks = 0;
    Header.Number_Of_SFX = 0;
    Header.Number_Of_Captions = 1;
    Header.Number_Of_Title_Cards = 0;
    Header.Number_Of_VFX = 0;
    Header.Number_Of_Billboard_Animations = 0;

    Header.Number_Of_Actor_Motion_Keyframes = 1;
    Header.Number_Of_Background_Motion_Keyframes = 0;
    Header.Number_Of_Camera_Action_Keyframes = 0;
    Header.Number_Of_Track_Action_Keyframes = 0;
    Header.Number_Of_SFX_Action_Keyframes = 0;
    Header.Number_Of_Caption_Action_Keyframes = 2;
    Header.Number_Of_Title_Card_Action_Keyframes = 0;
    Header.Number_Of_VFX_Motion_Keyframes = 0;
    Header.Number_Of_Billboard_Motion_Keyframes = 0;

    Header.Number_Of_Actor_Animation_Keyframes = 1;
    Header.Number_Of_Background_Animation_Keyframes = 0;
    Header.Number_Of_VFX_Animation_Keyframes = 0;
    Header.Number_Of_Billboard_Animation_Keyframes = 0;

    int Size_Of_Actors = Header.Number_Of_Actors* sizeof(struct _Cutscene_Actor);
    int Size_Of_Backgrounds = Header.Number_Of_Backgrounds* sizeof(struct _Cutscene_Backgrounds);
    int Size_Of_Cameras = Header.Number_Of_Cameras* sizeof(struct _Cutscene_Camera);
    int Size_Of_Tracks = Header.Number_Of_Tracks* sizeof(struct _Cutscene_Track);
    int Size_Of_SFX = Header.Number_Of_SFX* sizeof(struct _Cutscene_SFX);
    int Size_Of_Captions = Header.Number_Of_Captions* sizeof(struct _Cutscene_Caption);
    int Size_Of_Title_Cards = Header.Number_Of_Title_Cards* sizeof(struct _Cutscene_Title_Card);
    int Size_Of_VFX = Header.Number_Of_VFX* sizeof(struct _Cutscene_VFX);
    int Size_Of_Billboard_Animations = Header.Number_Of_Billboard_Animations* sizeof(struct _Cutscene_Billboard);

    int Size_Of_Actor_Motion_Keyframes = Header.Number_Of_Actor_Motion_Keyframes* sizeof(struct _Cutscene_Actor_Motion_Keyframe);
    int Size_Of_Background_Motion_Keyframes = Header.Number_Of_Background_Motion_Keyframes* sizeof(struct _Cutscene_Background_Motion_Keyframe);
    int Size_Of_Camera_Action_Keyframes = Header.Number_Of_Camera_Action_Keyframes* sizeof(struct _Cutscene_Camera_Action_Keyframe);
    int Size_Of_Track_Action_Keyframes = Header.Number_Of_Track_Action_Keyframes* sizeof(struct _Cutscene_Track_Action_Keyframe);
    int Size_Of_SFX_Action_Keyframes = Header.Number_Of_SFX_Action_Keyframes* sizeof(struct _Cutscene_SFX_Action_Keyframe);
    int Size_Of_Caption_Action_Keyframes = Header.Number_Of_Caption_Action_Keyframes* sizeof(struct _Cutscene_Caption_Action_Keyframe);
    int Size_Of_Title_Card_Action_Keyframes = Header.Number_Of_Title_Card_Action_Keyframes* sizeof(struct _Cutscene_Title_Card_Action_Keyframe);
    int Size_Of_VFX_Motion_Keyframes = Header.Number_Of_VFX_Motion_Keyframes* sizeof(struct _Cutscene_VFX_Motion_Keyframe);
    int Size_Of_Billboard_Motion_Keyframes = Header.Number_Of_Billboard_Motion_Keyframes* sizeof(struct _Cutscene_Billboard_Motion_Keyframe);

    int Size_Of_Actor_Animation_Keyframes = Header.Number_Of_Actor_Animation_Keyframes* sizeof(struct _Cutscene_Actor_Animation_Keyframe);
    int Size_Of_Background_Animation_Keyframes = Header.Number_Of_Background_Animation_Keyframes* sizeof(struct _Cutscene_Background_Animation_Keyframe);
    int Size_Of_VFX_Animation_Keyframes = Header.Number_Of_VFX_Animation_Keyframes* sizeof(struct _Cutscene_VFX_Animation_Keyframe);
    int Size_Of_Billboard_Animation_Keyframes = Header.Number_Of_Background_Animation_Keyframes* sizeof(struct _Cutscene_Background_Animation_Keyframe);






    int Current_Buffer_Location = sizeof(struct _Cutscene_Header);

    Header.Offset_To_Actors = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Actors;

    Header.Offset_To_Backgrounds = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Backgrounds;

    Header.Offset_To_Cameras = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Cameras;

    Header.Offset_To_Tracks = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Tracks;

    Header.Offset_To_SFX = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_SFX;

    Header.Offset_To_Captions = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Captions;

    Header.Offset_To_Title_Cards = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Title_Cards;

    Header.Offset_To_VFX = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_VFX;

    Header.Offset_To_Billboard_Animations = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Billboard_Animations;


    Header.Offset_To_Actor_Motion_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Actor_Motion_Keyframes;

    Header.Offset_To_Background_Motion_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Background_Motion_Keyframes;

    Header.Offset_To_Camera_Action_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Camera_Action_Keyframes;

    Header.Offset_To_Track_Action_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Track_Action_Keyframes;

    Header.Offset_To_SFX_Action_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_SFX_Action_Keyframes;

    Header.Offset_To_Caption_Action_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Caption_Action_Keyframes;

    Header.Offset_To_Title_Card_Action_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Title_Card_Action_Keyframes;

    Header.Offset_To_VFX_Motion_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_VFX_Motion_Keyframes;

    Header.Offset_To_Billboard_Motion_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Billboard_Motion_Keyframes;


    Header.Offset_To_Actor_Animation_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Actor_Animation_Keyframes;

    Header.Offset_To_Background_Animation_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Background_Animation_Keyframes;

    Header.Offset_To_VFX_Animation_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_VFX_Animation_Keyframes;

    Header.Offset_To_Billboard_Animation_Keyframes = Current_Buffer_Location;
    Current_Buffer_Location += Size_Of_Billboard_Animation_Keyframes;



    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _Cutscene_Header),f);
    rewind(f);

    fseek(f, Header.Offset_To_Actors, SEEK_SET);
    Write_Cutscene_Actor(f);
    rewind(f);

    fseek(f, Header.Offset_To_Backgrounds, SEEK_SET);
    Write_Cutscene_Background(f);
    rewind(f);

    fseek(f, Header.Offset_To_Cameras, SEEK_SET);
    Write_Cutscene_Camera(f);
    rewind(f);

    fseek(f, Header.Offset_To_Tracks, SEEK_SET);
    Write_Cutscene_Track(f);
    rewind(f);

    fseek(f, Header.Offset_To_SFX, SEEK_SET);
    Write_Cutscene_SFX(f);
    rewind(f);

    fseek(f, Header.Offset_To_Captions, SEEK_SET);
    Write_Cutscene_Caption(f);
    rewind(f);

    fseek(f, Header.Offset_To_Title_Cards, SEEK_SET);
    Write_Cutscene_Title_Card(f);
    rewind(f);

    fseek(f, Header.Offset_To_VFX, SEEK_SET);
    Write_Cutscene_VFX(f);
    rewind(f);

    fseek(f, Header.Offset_To_Billboard_Animations, SEEK_SET);
    Write_Cutscene_Billboard_Animation_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Actor_Motion_Keyframes, SEEK_SET);
    Write_Cutscene_Actor_Motion_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Background_Motion_Keyframes, SEEK_SET);
    Write_Cutscene_Background_Motion_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Camera_Action_Keyframes, SEEK_SET);
    Write_Cutscene_Camera_Action_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Track_Action_Keyframes, SEEK_SET);
    Write_Cutscene_Track_Action_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_SFX_Action_Keyframes, SEEK_SET);
    Write_Cutscene_SFX_Action_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Caption_Action_Keyframes, SEEK_SET);
    Write_Cutscene_Caption_Action_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Title_Card_Action_Keyframes, SEEK_SET);
    Write_Cutscene_Track_Action_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_VFX_Motion_Keyframes, SEEK_SET);
    Write_Cutscene_VFX_Animation_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Billboard_Motion_Keyframes, SEEK_SET);
    Write_Cutscene_Billboard_Motion_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Actor_Animation_Keyframes, SEEK_SET);
    Write_Cutscene_Actor_Animation_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Background_Animation_Keyframes, SEEK_SET);
    Write_Cutscene_Background_Animation_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_VFX_Animation_Keyframes, SEEK_SET);
    Write_Cutscene_VFX_Animation_Keyframe(f);
    rewind(f);

    fseek(f, Header.Offset_To_Billboard_Animation_Keyframes, SEEK_SET);
    Write_Cutscene_Billboard_Animation_Keyframe(f);
    rewind(f);

    fclose(f);
}


void Write_Cutscene_Actor(FILE* f)
{
    struct _Cutscene_Actor Actor;
    memset(Actor.Filepath,0,256);
    memset(Actor.Texture_Filepath,0,256);
    sprintf(Actor.Filepath, "resources/models/model_chris.dat");
    sprintf(Actor.Texture_Filepath, "resources/models/texture_chris.bmp");
    Actor.Filepath[32] = '\0';
    Actor.Texture_Filepath[34] = '\0';

    fwrite(&Actor,1,sizeof(struct _Cutscene_Actor),f);
}
void Write_Cutscene_Background(FILE* f)
{
    struct _Cutscene_Backgrounds BackOne;
    memset(BackOne.Filepath,0,256);
    memset(BackOne.Texture_Filepath,0,256);
    sprintf(BackOne.Filepath, "resources/models/model_skybox.dat");
    sprintf(BackOne.Texture_Filepath, "resources/models/texture_skybox.bmp");
    BackOne.Filepath[33] = '\0';
    BackOne.Texture_Filepath[35] = '\0';
    fwrite(&BackOne,1,sizeof(struct _Cutscene_Backgrounds),f);
}
void Write_Cutscene_Camera(FILE* f)
{

}
void Write_Cutscene_Track(FILE* f)
{

}
void Write_Cutscene_SFX(FILE* f)
{

}
void Write_Cutscene_Caption(FILE* f)
{
    struct _Cutscene_Caption CC;
    memset(CC.Caption,0,256);
    sprintf(CC.Caption, "TEST CAPTION!");
    CC.Caption[13] = '\0';
    CC.Length = 13;
    fwrite(&CC,1,sizeof(struct _Cutscene_Caption),f);
}
void Write_Cutscene_Title_Card(FILE* f)
{

}
void Write_Cutscene_VFX(FILE* f)
{

}
void Write_Cutscene_Billboard(FILE* f)
{

}

void Write_Cutscene_Actor_Motion_Keyframe(FILE* f)
{
    struct _Cutscene_Actor_Motion_Keyframe CAMK;
    CAMK.At_This_Time = 3;
    CAMK.Complete = 0;
    CAMK.New_Pitch = 0;
    CAMK.New_Throttle = 1;
    CAMK.New_Yaw = 0;
    CAMK.Update_Heading = 0;
    CAMK.Update_Location = 1;
    CAMK.x = 3;
    CAMK.y = 3;
    CAMK.z = 3;

    fwrite(&CAMK,1,sizeof(struct _Cutscene_Actor_Motion_Keyframe),f);
}
void Write_Cutscene_Background_Motion_Keyframe(FILE* f)
{

}
void Write_Cutscene_Camera_Action_Keyframe(FILE* f)
{

}
void Write_Cutscene_Track_Action_Keyframe(FILE* f)
{

}
void Write_Cutscene_SFX_Action_Keyframe(FILE* f)
{

}
void Write_Cutscene_Caption_Action_Keyframe(FILE* f)
{
    struct _Cutscene_Caption_Action_Keyframe CCAK;

    CCAK.At_This_Time = 1;
    CCAK.Complete = 0;
    CCAK.Visible = 1;

    fwrite(&CCAK,1,sizeof(struct _Cutscene_Caption_Action_Keyframe),f);



    struct _Cutscene_Caption_Action_Keyframe CCAKTwo;

    CCAKTwo.At_This_Time = 3;
    CCAKTwo.Complete = 0;
    CCAKTwo.Visible = 0;

    fwrite(&CCAKTwo,1,sizeof(struct _Cutscene_Caption_Action_Keyframe),f);
}
void Write_Cutscene_Title_Card_Action_Keyframe(FILE* f)
{

}
void Write_Cutscene_VFX_Motion_Keyframe(FILE* f)
{

}
void Write_Cutscene_Billboard_Motion_Keyframe(FILE* f)
{

}

void Write_Cutscene_Actor_Animation_Keyframe(FILE* f)
{
    struct _Cutscene_Actor_Animation_Keyframe CAAK;
    CAAK.At_This_Time = 3;
    CAAK.Complete = 0;
    CAAK.For_This_ID = 0;
    CAAK.Loop = 1;
    CAAK.New_Keyframes[0] = 0;
    CAAK.New_Keyframes[1] = 7;
    CAAK.Rate = 1;
    CAAK.Update_Animation = 1;
    CAAK.Visible = 1;

    fwrite(&CAAK,1,sizeof(struct _Cutscene_Actor_Animation_Keyframe),f);
}
void Write_Cutscene_Background_Animation_Keyframe(FILE* f)
{

}
void Write_Cutscene_VFX_Animation_Keyframe(FILE* f)
{

}
void Write_Cutscene_Billboard_Animation_Keyframe(FILE* f)
{

}

void Load_Cutscene(struct _Engine* Engine, const char* Filename)
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

    int Current_Buffer_Location = 0;

    Engine->Cutscene_State->Header.Total_Duration_In_Minutes = *(float*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Actors = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Backgrounds = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Cameras = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Tracks = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_SFX = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Captions = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    printf("Number of Captions: %d\n", Engine->Cutscene_State->Header.Number_Of_Captions);

    Engine->Cutscene_State->Header.Number_Of_Title_Cards = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_VFX = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Billboard_Animations = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;


    Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Background_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Caption_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Title_Card_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_VFX_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Billboard_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;


    Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Background_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_VFX_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Number_Of_Billboard_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;




    Engine->Cutscene_State->Header.Offset_To_Actors = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Backgrounds = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Cameras = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Tracks = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_SFX = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Captions = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Title_Cards = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_VFX = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Billboard_Animations = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;


    Engine->Cutscene_State->Header.Offset_To_Actor_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Background_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Camera_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Track_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_SFX_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Caption_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Title_Card_Action_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_VFX_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Billboard_Motion_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;


    Engine->Cutscene_State->Header.Offset_To_Actor_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Background_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_VFX_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;

    Engine->Cutscene_State->Header.Offset_To_Billboard_Animation_Keyframes = *(int*)&(Buffer[Current_Buffer_Location]);
    Current_Buffer_Location += 4;


    Engine->Cutscene_State->Cutscene_Actor_Object = (struct _Cutscene_Actor_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Actors, sizeof(struct _Cutscene_Actor_Object));
    Engine->Cutscene_State->Cutscene_Background_Object = (struct _Cutscene_Background_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Backgrounds, sizeof(struct _Cutscene_Background_Object));
    Engine->Cutscene_State->Cutscene_Camera_Object = (struct _Cutscene_Camera_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Cameras, sizeof(struct _Cutscene_Camera_Object));
    Engine->Cutscene_State->Cutscene_Track_Object = (struct _Cutscene_Track_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Tracks, sizeof(struct _Cutscene_Track_Object));
    Engine->Cutscene_State->Cutscene_SFX_Object = (struct _Cutscene_SFX_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_SFX, sizeof(struct _Cutscene_SFX_Object));
    Engine->Cutscene_State->Cutscene_Caption_Object = (struct _Cutscene_Caption_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Captions, sizeof(struct _Cutscene_Caption_Object));
    Engine->Cutscene_State->Cutscene_Title_Card_Object = (struct _Cutscene_Title_Card_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Title_Cards, sizeof(struct _Cutscene_Title_Card_Object));
    Engine->Cutscene_State->Cutscene_VFX_Object = (struct _Cutscene_VFX_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_VFX, sizeof(struct _Cutscene_VFX_Object));
    Engine->Cutscene_State->Cutscene_Billboard_Animation_Object = (struct _Cutscene_Billboard_Animation_Object*)calloc(Engine->Cutscene_State->Header.Number_Of_Billboard_Animations, sizeof(struct _Cutscene_Billboard_Animation_Object));
    Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe = (struct _Cutscene_Actor_Motion_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes, sizeof(struct _Cutscene_Actor_Motion_Keyframe));
    Engine->Cutscene_State->Cutscene_Background_Motion_Keyframe = (struct _Cutscene_Background_Motion_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Background_Motion_Keyframes, sizeof(struct _Cutscene_Background_Motion_Keyframe));
    Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe = (struct _Cutscene_Camera_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes, sizeof(struct _Cutscene_Camera_Action_Keyframe));
    Engine->Cutscene_State->Cutscene_Track_Action_Keyframe = (struct _Cutscene_Track_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes, sizeof(struct _Cutscene_Track_Action_Keyframe));
    Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe = (struct _Cutscene_SFX_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes, sizeof(struct _Cutscene_SFX_Action_Keyframe));
    Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe = (struct _Cutscene_Caption_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Caption_Action_Keyframes, sizeof(struct _Cutscene_Caption_Action_Keyframe));
    Engine->Cutscene_State->Cutscene_Title_Card_Action_Keyframe = (struct _Cutscene_Title_Card_Action_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Title_Card_Action_Keyframes, sizeof(struct _Cutscene_Title_Card_Action_Keyframe));
    Engine->Cutscene_State->Cutscene_VFX_Motion_Keyframe = (struct _Cutscene_VFX_Motion_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_VFX_Motion_Keyframes, sizeof(struct _Cutscene_VFX_Motion_Keyframe));
    Engine->Cutscene_State->Cutscene_Billboard_Motion_Keyframe = (struct _Cutscene_Billboard_Motion_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Billboard_Motion_Keyframes, sizeof(struct _Cutscene_Billboard_Motion_Keyframe));
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe = (struct _Cutscene_Actor_Animation_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes, sizeof(struct _Cutscene_Actor_Animation_Keyframe));
    Engine->Cutscene_State->Cutscene_Background_Animation_Keyframe = (struct _Cutscene_Background_Animation_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Background_Animation_Keyframes, sizeof(struct _Cutscene_Background_Animation_Keyframe));
    Engine->Cutscene_State->Cutscene_VFX_Animation_Keyframe = (struct _Cutscene_VFX_Animation_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_VFX_Animation_Keyframes, sizeof(struct _Cutscene_VFX_Animation_Keyframe));
    Engine->Cutscene_State->Cutscene_Billboard_Animation_Keyframe = (struct _Cutscene_Billboard_Animation_Keyframe*)calloc(Engine->Cutscene_State->Header.Number_Of_Billboard_Animation_Keyframes, sizeof(struct _Cutscene_Billboard_Animation_Keyframe));
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actors; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));
        char Filepath[256];
        char Texture_Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] =  Buffer[Buffer_Location+l];
            Texture_Filepath[l] =  Buffer[Buffer_Location+l+256];
        }
        Buffer_Location += 512;
        Engine->Cutscene_State->Cutscene_Actor_Object[j].Height = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Actor_Object[j].Model = Create_MD2_Model();
        Load_MD2_Model(Engine->Cutscene_State->Cutscene_Actor_Object[j].Model, Filepath);
        Load_MD2_Texture(Engine->Cutscene_State->Cutscene_Actor_Object[j].Model, Texture_Filepath);

        Engine->Cutscene_State->Cutscene_Actor_Object[j].Current_Keyframe = 0;
        Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[0] = 0;
        Engine->Cutscene_State->Cutscene_Actor_Object[j].New_Keyframes[1] = 0;
    }


    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Backgrounds; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Backgrounds+ (j*sizeof(struct _Cutscene_Backgrounds));
        char Filepath[256];
        char Texture_Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] =  Buffer[Buffer_Location+l];
            Texture_Filepath[l] =  Buffer[Buffer_Location+l+256];
        }
        Buffer_Location += 512;

        Engine->Cutscene_State->Cutscene_Background_Object[j].Model = Create_MD2_Model();
        Load_MD2_Model(Engine->Cutscene_State->Cutscene_Background_Object[j].Model, Filepath);
        Load_MD2_Texture(Engine->Cutscene_State->Cutscene_Background_Object[j].Model, Texture_Filepath);
    }


    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Cameras; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Cameras + (j*sizeof(struct _Cutscene_Camera));

        Engine->Cutscene_State->Cutscene_Camera_Object[j].x = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].y = *(float*)&(Buffer[Buffer_Location+4]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].z = *(float*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_x = *(float*)&(Buffer[Buffer_Location+12]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_y = *(float*)&(Buffer[Buffer_Location+16]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_z = *(float*)&(Buffer[Buffer_Location+20]);
        Engine->Cutscene_State->Cutscene_Camera_Object[j].Active = *(int*)&(Buffer[Buffer_Location+24]);

        Engine->Cutscene_State->Cutscene_Camera_Object[j].Action = -1;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_x = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_y = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_z = 0;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Center_x = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Center_y = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Center_z = 0;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Horizontal_Angles[0] = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Horizontal_Angles[1] = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Rotational_Velocity = 0;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Vertical_Angles[0] = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Vertical_Angles[1] = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Vertical_Velocity = 0;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].Following_This_Actor = -1;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].x_Offset = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].y_Offset = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].z_Offset = 0;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].Actor_Portion_To_Look_At = 0;

        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Yaw = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Pitch = 0;
        Engine->Cutscene_State->Cutscene_Camera_Object[j].New_Throttle = 0;

        if(Engine->Cutscene_State->Cutscene_Camera_Object[j].Active == 1)
        {
            gluLookAt(Engine->Cutscene_State->Cutscene_Camera_Object[j].x,Engine->Cutscene_State->Cutscene_Camera_Object[j].y,Engine->Cutscene_State->Cutscene_Camera_Object[j].z,Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_x,Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_y,Engine->Cutscene_State->Cutscene_Camera_Object[j].Center_z,0,0,1);
        }
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Tracks; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Tracks + (j*sizeof(struct _Cutscene_Track));

        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
        }
        Engine->Cutscene_State->Cutscene_Track_Object[j].Audio = Mix_LoadMUS(Filepath);
        Engine->Cutscene_State->Cutscene_Track_Object[j].Playing = -1;
        Engine->Cutscene_State->Cutscene_Track_Object[j].Volume = 0;
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_SFX; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_SFX + (j*sizeof(struct _Cutscene_SFX));

        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
        }
        Engine->Cutscene_State->Cutscene_SFX_Object[j].Audio = Mix_LoadWAV(Filepath);
        Engine->Cutscene_State->Cutscene_SFX_Object[j].Playing = -1;
        Engine->Cutscene_State->Cutscene_SFX_Object[j].Volume = 0;
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Captions; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Captions + (j*sizeof(struct _Cutscene_Caption));
        char Filepath[256];
        printf("Caption: ");
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
            printf("%c", Filepath[l]);
        }
        printf("\n");

        int Length = *(int*)&(Buffer[Buffer_Location+256]);
        printf("Length: %d\n", Length);

        Engine->Cutscene_State->Cutscene_Caption_Object[j].Image = Create_Image();
        Engine->Cutscene_State->Cutscene_Caption_Object[j].Length = Length;
        Initialize_Text_Image(Engine->Text_Engine, Engine->Cutscene_State->Cutscene_Caption_Object[j].Image, Filepath,Length,17*Length,24);
        Engine->Cutscene_State->Cutscene_Caption_Object[j].Visible = -1;
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Title_Cards; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Title_Cards + (j*sizeof(struct _Cutscene_Title_Card));
        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
        }

        Engine->Cutscene_State->Cutscene_Title_Card_Object[j].Image = Create_Image();
        Load_Image(Engine->Cutscene_State->Cutscene_Title_Card_Object[j].Image, Filepath);
        Engine->Cutscene_State->Cutscene_Title_Card_Object[j].Visible = -1;
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_VFX; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_VFX + (j*sizeof(struct _Cutscene_VFX));
        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
        }
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Image = Create_Image();
        Load_Image(Engine->Cutscene_State->Cutscene_VFX_Object[j].Image, Filepath);

        Engine->Cutscene_State->Cutscene_VFX_Object[j].Visible = -1;

        Engine->Cutscene_State->Cutscene_VFX_Object[j].Update_Animation = -1;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].New_Keyframes[0] = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].New_Keyframes[1] = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Loop = -1;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Rate = -1;

        Engine->Cutscene_State->Cutscene_VFX_Object[j].New_x = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].New_y = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].New_z = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Update_Location = -1;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Update_Heading = -1;

        Engine->Cutscene_State->Cutscene_VFX_Object[j].Throttle = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Pitch = 0;
        Engine->Cutscene_State->Cutscene_VFX_Object[j].Yaw = 0;
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Billboard_Animations; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Billboard_Animations + (j*sizeof(struct _Cutscene_Billboard));
        char Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
        }

        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Image = Create_Image();
        Load_Image(Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Image, Filepath);

        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Visible = -1;

        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Update_Animation = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].New_Keyframes[0] = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].New_Keyframes[1] = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Loop = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Rate = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Distance = -1;

        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].New_x = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].New_y = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].New_z = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Update_Location = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Update_Heading = -1;

        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Throttle = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Pitch = -1;
        Engine->Cutscene_State->Cutscene_Billboard_Animation_Object[j].Yaw = -1;
    }




    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actor_Motion_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actor_Motion_Keyframes + (j*sizeof(struct _Cutscene_Actor_Motion_Keyframe));

        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+4]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].For_This_ID = *(int*)&(Buffer[Buffer_Location+8]);

        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].New_Throttle = *(float*)&(Buffer[Buffer_Location+12]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].New_Pitch = *(float*)&(Buffer[Buffer_Location+16]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].New_Yaw = *(float*)&(Buffer[Buffer_Location+20]);

        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Update_Location = *(int*)&(Buffer[Buffer_Location+24]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].Update_Heading = *(int*)&(Buffer[Buffer_Location+28]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].x = *(float*)&(Buffer[Buffer_Location+32]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].y = *(float*)&(Buffer[Buffer_Location+36]);
        Engine->Cutscene_State->Cutscene_Actor_Motion_Keyframe[j].z = *(float*)&(Buffer[Buffer_Location+40]);
    }


    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Background_Motion_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Background_Motion_Keyframes + (j*sizeof(struct _Cutscene_Background_Motion_Keyframe));

        Engine->Cutscene_State->Cutscene_Background_Motion_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Background_Motion_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+4]);
        Engine->Cutscene_State->Cutscene_Background_Motion_Keyframe[j].New_Rotational_Velocity = *(float*)&(Buffer[Buffer_Location+8]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Camera_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Camera_Action_Keyframes + (j*sizeof(struct _Cutscene_Camera_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Complete = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Action = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Active_Camera = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_x = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_y = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_z = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Center_x = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Center_y = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Center_z = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Horizontal_Angles[0] = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Horizontal_Angles[1] = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Rotational_Velocity = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Vertical_Angles[0] = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Vertical_Angles[1] = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Vertical_Velocity = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Following_This_Actor = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].x_Offset = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].y_Offset = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].z_Offset = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Actor_Portion_To_Look_At = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Update_Location = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].Update_Heading = *(float*)&(Buffer[Buffer_Location]);

        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Yaw = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Pitch = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Camera_Action_Keyframe[j].New_Throttle = *(float*)&(Buffer[Buffer_Location]);
    }







    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Track_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Track_Action_Keyframes + (j*sizeof(struct _Cutscene_Track_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].Complete = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].Playing = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Track_Action_Keyframe[j].Volume = *(float*)&(Buffer[Buffer_Location]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_SFX_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_SFX_Action_Keyframes + (j*sizeof(struct _Cutscene_SFX_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].Complete = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].Playing = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_SFX_Action_Keyframe[j].Volume = *(float*)&(Buffer[Buffer_Location]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Caption_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Caption_Action_Keyframes + (j*sizeof(struct _Cutscene_Caption_Action_Keyframe));

        Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+4]);
        Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].For_This_ID = *(int*)&(Buffer[Buffer_Location+8]);
        Engine->Cutscene_State->Cutscene_Caption_Action_Keyframe[j].Visible = *(int*)&(Buffer[Buffer_Location+12]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Title_Card_Action_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));

        float At_This_Time = *(float*)&(Buffer[Buffer_Location]);
        int Complete = *(float*)&(Buffer[Buffer_Location]);
        int Visible = *(float*)&(Buffer[Buffer_Location]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_VFX_Motion_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));

        float At_This_Time = *(float*)&(Buffer[Buffer_Location]);
    int Complete = *(float*)&(Buffer[Buffer_Location]);
    float New_x = *(float*)&(Buffer[Buffer_Location]);
    float New_y = *(float*)&(Buffer[Buffer_Location]);
    float New_z = *(float*)&(Buffer[Buffer_Location]);
    int Update_Location = *(float*)&(Buffer[Buffer_Location]);
    int Update_Heading = *(float*)&(Buffer[Buffer_Location]);

    float Throttle = *(float*)&(Buffer[Buffer_Location]);
    float Pitch = *(float*)&(Buffer[Buffer_Location]);
    float Yaw = *(float*)&(Buffer[Buffer_Location]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Billboard_Motion_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));

        float At_This_Time = *(float*)&(Buffer[Buffer_Location]);
    int Complete = *(float*)&(Buffer[Buffer_Location]);
    float New_x = *(float*)&(Buffer[Buffer_Location]);
    float New_y = *(float*)&(Buffer[Buffer_Location]);
    float New_z = *(float*)&(Buffer[Buffer_Location]);
    int Update_Location = *(float*)&(Buffer[Buffer_Location]);
    int Update_Heading = *(float*)&(Buffer[Buffer_Location]);

    float Throttle = *(float*)&(Buffer[Buffer_Location]);
    float Pitch = *(float*)&(Buffer[Buffer_Location]);
    float Yaw = *(float*)&(Buffer[Buffer_Location]);
    }

    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Actor_Animation_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actor_Animation_Keyframes + (j*sizeof(struct _Cutscene_Actor_Animation_Keyframe));

    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].At_This_Time = *(float*)&(Buffer[Buffer_Location]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Visible = *(int*)&(Buffer[Buffer_Location+4]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Complete = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].For_This_ID = *(int*)&(Buffer[Buffer_Location+12]);

    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Update_Animation = *(int*)&(Buffer[Buffer_Location+16]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[0] = *(int*)&(Buffer[Buffer_Location+20]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].New_Keyframes[1] = *(int*)&(Buffer[Buffer_Location+24]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Loop = *(int*)&(Buffer[Buffer_Location+28]);
    Engine->Cutscene_State->Cutscene_Actor_Animation_Keyframe[j].Rate = *(float*)&(Buffer[Buffer_Location+32]);

    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Background_Animation_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));

        float At_This_Time = *(float*)&(Buffer[Buffer_Location]);
    int Visible = *(float*)&(Buffer[Buffer_Location]);
    int Complete = *(float*)&(Buffer[Buffer_Location]);

    int Update_Animation = *(float*)&(Buffer[Buffer_Location]);
   // int New_Keyframes[2] = *(float*)&(Buffer[Buffer_Location]);
    int Loop = *(float*)&(Buffer[Buffer_Location]);
    float Rate = *(float*)&(Buffer[Buffer_Location]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_VFX_Animation_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));

        float At_This_Time = *(float*)&(Buffer[Buffer_Location]);
    int Visible = *(float*)&(Buffer[Buffer_Location]);
    int Complete = *(float*)&(Buffer[Buffer_Location]);

    int Update_Animation = *(float*)&(Buffer[Buffer_Location]);
   // int New_Keyframes[2] = *(float*)&(Buffer[Buffer_Location]);
    int Loop = *(float*)&(Buffer[Buffer_Location]);
    float Rate = *(float*)&(Buffer[Buffer_Location]);
    }
    for(int j = 0; j < Engine->Cutscene_State->Header.Number_Of_Billboard_Animation_Keyframes; j++)
    {
        Buffer_Location = Engine->Cutscene_State->Header.Offset_To_Actors + (j*sizeof(struct _Cutscene_Actor));

        float At_This_Time = *(float*)&(Buffer[Buffer_Location]);
    int Visible = *(float*)&(Buffer[Buffer_Location]);
    int Complete = *(float*)&(Buffer[Buffer_Location]);

    int Update_Animation = *(float*)&(Buffer[Buffer_Location]);
   // int New_Keyframes[2] = *(float*)&(Buffer[Buffer_Location]);
    int Loop = *(float*)&(Buffer[Buffer_Location]);
    float Rate = *(float*)&(Buffer[Buffer_Location]);
    float Distance = *(float*)&(Buffer[Buffer_Location]);
    }

}

/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef CUTSCENE_H
#define CUTSCENE_H
#include "../../engine/engine.h"
#include <stdio.h>
#include "endian.h"
#include "../../engine/timer/timer.h"

struct _Cutscene_Actor
{
    char Filepath[256];
    char Texture_Filepath[256];
    float Height;
};
struct _Cutscene_Backgrounds
{
    char Filepath[256];
    char Texture_Filepath[256];
};
struct _Cutscene_Camera
{
    float x;
    float y;
    float z;
    float Center_x;
    float Center_y;
    float Center_z;
    int Active;
};
struct _Cutscene_Track
{
    char Filepath[256];
};
struct _Cutscene_SFX
{
    char Filepath[256];
};
struct _Cutscene_Caption
{
    char Caption[256];
    int Length;
};
struct _Cutscene_Title_Card
{
    char Filepath[256];
};
struct _Cutscene_VFX
{
    char Filepath[256];
};
struct _Cutscene_Billboard
{
    char Filepath[256];
};









struct _Cutscene_Actor_Motion_Keyframe
{
    float At_This_Time;
    int Complete;
    int For_This_ID;

    float New_Throttle;
    float New_Pitch;
    float New_Yaw;

    int Update_Location;
    int Update_Heading;
    float x;
    float y;
    float z;
};
struct _Cutscene_Background_Motion_Keyframe
{
    float At_This_Time;
    int Complete;
    float New_Rotational_Velocity;
};
struct _Cutscene_Camera_Action_Keyframe
{
    float At_This_Time;
    int Complete;

    int Action;
    int Active_Camera;

    float New_x;
    float New_y;
    float New_z;

    float New_Center_x;
    float New_Center_y;
    float New_Center_z;

    float New_Horizontal_Angles[2];
    float New_Rotational_Velocity;

    float New_Vertical_Angles[2];
    float New_Vertical_Velocity;

    int Following_This_Actor;
    float x_Offset;
    float y_Offset;
    float z_Offset;

    int Actor_Portion_To_Look_At;
    int Update_Location;
    int Update_Heading;

    float New_Yaw;
    float New_Pitch;
    float New_Throttle;

};
struct _Cutscene_Track_Action_Keyframe
{
    float At_This_Time;
    int Complete;
    int Playing;
    int Volume;
};
struct _Cutscene_SFX_Action_Keyframe
{
    float At_This_Time;
    int Complete;
    int Playing;
    int Volume;
};
struct _Cutscene_Caption_Action_Keyframe
{
    float At_This_Time;
    int Complete;
    int For_This_ID;
    int Visible;
};
struct _Cutscene_Title_Card_Action_Keyframe
{
    float At_This_Time;
    int Complete;
    int Visible;
};
struct _Cutscene_VFX_Motion_Keyframe
{
    float At_This_Time;
    int Complete;
    float New_x;
    float New_y;
    float New_z;
    int Update_Location;
    int Update_Heading;

    float Throttle;
    float Pitch;
    float Yaw;
};
struct _Cutscene_Billboard_Motion_Keyframe
{
    float At_This_Time;
    int Complete;
    float New_x;
    float New_y;
    float New_z;
    int Update_Location;
    int Update_Heading;

    float Throttle;
    float Pitch;
    float Yaw;
};










struct _Cutscene_Actor_Animation_Keyframe
{
    float At_This_Time;
    int Visible;
    int Complete;
    int For_This_ID;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;
};
struct _Cutscene_Background_Animation_Keyframe
{
    float At_This_Time;
    int Visible;
    int Complete;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;
};
struct _Cutscene_VFX_Animation_Keyframe
{
    float At_This_Time;
    int Visible;
    int Complete;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;
};
struct _Cutscene_Billboard_Animation_Keyframe
{
    float At_This_Time;
    int Visible;
    int Complete;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;
    float Distance;
};




struct _Cutscene_Header
{
    float Total_Duration_In_Minutes;
    int Number_Of_Actors;
    int Number_Of_Backgrounds;
    int Number_Of_Cameras;
    int Number_Of_Tracks;
    int Number_Of_SFX;
    int Number_Of_Captions;
    int Number_Of_Title_Cards;
    int Number_Of_VFX;
    int Number_Of_Billboard_Animations;

    int Number_Of_Actor_Motion_Keyframes;
    int Number_Of_Background_Motion_Keyframes;
    int Number_Of_Camera_Action_Keyframes;
    int Number_Of_Track_Action_Keyframes;
    int Number_Of_SFX_Action_Keyframes;
    int Number_Of_Caption_Action_Keyframes;
    int Number_Of_Title_Card_Action_Keyframes;
    int Number_Of_VFX_Motion_Keyframes;
    int Number_Of_Billboard_Motion_Keyframes;

    int Number_Of_Actor_Animation_Keyframes;
    int Number_Of_Background_Animation_Keyframes;
    int Number_Of_VFX_Animation_Keyframes;
    int Number_Of_Billboard_Animation_Keyframes;



    int Offset_To_Actors;
    int Offset_To_Backgrounds;
    int Offset_To_Cameras;
    int Offset_To_Tracks;
    int Offset_To_SFX;
    int Offset_To_Captions;
    int Offset_To_Title_Cards;
    int Offset_To_VFX;
    int Offset_To_Billboard_Animations;

    int Offset_To_Actor_Motion_Keyframes;
    int Offset_To_Background_Motion_Keyframes;
    int Offset_To_Camera_Action_Keyframes;
    int Offset_To_Track_Action_Keyframes;
    int Offset_To_SFX_Action_Keyframes;
    int Offset_To_Caption_Action_Keyframes;
    int Offset_To_Title_Card_Action_Keyframes;
    int Offset_To_VFX_Motion_Keyframes;
    int Offset_To_Billboard_Motion_Keyframes;

    int Offset_To_Actor_Animation_Keyframes;
    int Offset_To_Background_Animation_Keyframes;
    int Offset_To_VFX_Animation_Keyframes;
    int Offset_To_Billboard_Animation_Keyframes;
};





struct _Cutscene_Actor_Object
{
    struct _SE3_Model* Model;
    float Height;

    int Visible;

    int Update_Animation;
    int Current_Keyframe;
    int New_Keyframes[2];
    int Loop;
    float Rate;

    float New_Throttle;
    float New_Pitch;
    float New_Yaw;

    int Update_Location;
    int Update_Heading;
    float x;
    float y;
    float z;
};
struct _Cutscene_Background_Object
{
    struct _SE3_Model* Model;

    int Visible;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;

    float New_Rotational_Velocity;
};
struct _Cutscene_Camera_Object
{
    float x;
    float y;
    float z;
    float Center_x;
    float Center_y;
    float Center_z;
    int Active;

    int Action;

    float New_x;
    float New_y;
    float New_z;

    float New_Center_x;
    float New_Center_y;
    float New_Center_z;

    float New_Horizontal_Angles[2];
    float New_Rotational_Velocity;

    float New_Vertical_Angles[2];
    float New_Vertical_Velocity;

    int Following_This_Actor;
    float x_Offset;
    float y_Offset;
    float z_Offset;

    int Actor_Portion_To_Look_At;

    float New_Yaw;
    float New_Pitch;
    float New_Throttle;
};
struct _Cutscene_Track_Object
{
    Mix_Music* Audio;
    int Playing;
    int Volume;
};
struct _Cutscene_SFX_Object
{
    Mix_Chunk* Audio;
    int Playing;
    int Volume;
};
struct _Cutscene_Caption_Object
{
    struct _Image* Image;
    int Visible;
    int Length;
};
struct _Cutscene_Title_Card_Object
{
    struct _Image* Image;
    int Visible;
};
struct _Cutscene_VFX_Object
{
    struct _Image* Image;

    int Visible;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;

    float New_x;
    float New_y;
    float New_z;
    int Update_Location;
    int Update_Heading;

    float Throttle;
    float Pitch;
    float Yaw;
};
struct _Cutscene_Billboard_Animation_Object
{
    struct _Image* Image;

    int Visible;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    float Rate;
    float Distance;

    float New_x;
    float New_y;
    float New_z;
    int Update_Location;
    int Update_Heading;

    float Throttle;
    float Pitch;
    float Yaw;
};






struct _Cutscene_State
{
    struct _Cutscene_Header Header;

    struct _Cutscene_Actor_Object* Cutscene_Actor_Object;
    struct _Cutscene_Background_Object* Cutscene_Background_Object;
    struct _Cutscene_Camera_Object* Cutscene_Camera_Object;
    struct _Cutscene_Track_Object* Cutscene_Track_Object;
    struct _Cutscene_SFX_Object* Cutscene_SFX_Object;
    struct _Cutscene_Caption_Object* Cutscene_Caption_Object;
    struct _Cutscene_Title_Card_Object* Cutscene_Title_Card_Object;
    struct _Cutscene_VFX_Object* Cutscene_VFX_Object;
    struct _Cutscene_Billboard_Animation_Object* Cutscene_Billboard_Animation_Object;

    struct _Cutscene_Actor_Motion_Keyframe* Cutscene_Actor_Motion_Keyframe;
    struct _Cutscene_Background_Motion_Keyframe* Cutscene_Background_Motion_Keyframe;
    struct _Cutscene_Camera_Action_Keyframe* Cutscene_Camera_Action_Keyframe;
    struct _Cutscene_Track_Action_Keyframe* Cutscene_Track_Action_Keyframe;
    struct _Cutscene_SFX_Action_Keyframe* Cutscene_SFX_Action_Keyframe;
    struct _Cutscene_Caption_Action_Keyframe* Cutscene_Caption_Action_Keyframe;
    struct _Cutscene_Title_Card_Action_Keyframe* Cutscene_Title_Card_Action_Keyframe;
    struct _Cutscene_VFX_Motion_Keyframe* Cutscene_VFX_Motion_Keyframe;
    struct _Cutscene_Billboard_Motion_Keyframe* Cutscene_Billboard_Motion_Keyframe;

    struct _Cutscene_Actor_Animation_Keyframe* Cutscene_Actor_Animation_Keyframe;
    struct _Cutscene_Background_Animation_Keyframe* Cutscene_Background_Animation_Keyframe;
    struct _Cutscene_VFX_Animation_Keyframe* Cutscene_VFX_Animation_Keyframe;
    struct _Cutscene_Billboard_Animation_Keyframe* Cutscene_Billboard_Animation_Keyframe;

    struct _Timer* Timer;
    int Laps;

    bool Render = true;
};




struct _Cutscene_State* Create_Cutscene_State();
void Destroy_Cutscene_State(struct _Engine* Engine);

void Initialize_Cutscene_State(struct _Engine* Engine);

void Render_Cutscene_State(struct _Engine* Engine);
void Input_Cutscene_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Cutscene_State(struct _Engine* Engine);

void Write_Cutscene(const char* Filename);


void Write_Cutscene_Actor(FILE* f);
void Write_Cutscene_Background(FILE* f);
void Write_Cutscene_Camera(FILE* f);
void Write_Cutscene_Track(FILE* f);
void Write_Cutscene_SFX(FILE* f);
void Write_Cutscene_Caption(FILE* f);
void Write_Cutscene_Title_Card(FILE* f);
void Write_Cutscene_VFX(FILE* f);
void Write_Cutscene_Billboard(FILE* f);

void Write_Cutscene_Actor_Motion_Keyframe(FILE* f);
void Write_Cutscene_Background_Motion_Keyframe(FILE* f);
void Write_Cutscene_Camera_Action_Keyframe(FILE* f);
void Write_Cutscene_Track_Action_Keyframe(FILE* f);
void Write_Cutscene_SFX_Action_Keyframe(FILE* f);
void Write_Cutscene_Caption_Action_Keyframe(FILE* f);
void Write_Cutscene_Title_Card_Action_Keyframe(FILE* f);
void Write_Cutscene_VFX_Motion_Keyframe(FILE* f);
void Write_Cutscene_Billboard_Motion_Keyframe(FILE* f);

void Write_Cutscene_Actor_Animation_Keyframe(FILE* f);
void Write_Cutscene_Background_Animation_Keyframe(FILE* f);
void Write_Cutscene_VFX_Animation_Keyframe(FILE* f);
void Write_Cutscene_Billboard_Animation_Keyframe(FILE* f);

void Load_Cutscene(struct _Engine* Engine, const char* Filename);

#endif /* CUTSCENE_H */

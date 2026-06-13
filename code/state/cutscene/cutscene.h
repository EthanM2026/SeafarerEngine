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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "endian.h"
#include "../../engine/timer/timer.h"

#define CAMERA_ACTION_MOVE_CAMERA 1

#pragma pack(push, 1)
struct _Cutscene_Actor
{
    char Filepath[256];
    char Texture_Filepath[256];
    double Height; //For face shot
    double x;
    double y;
    double z;
};
struct _Cutscene_Backgrounds
{
    char Filepath[256];
    char Texture_Filepath[256];
};
struct _Cutscene_Camera
{
    double x;
    double y;
    double z;
    double Center_x;
    double Center_y;
    double Center_z;
    int Active;

    int Looking_At_This_Actor;
    double Look_Offset_x;
    double Look_Offset_y;
    double Look_Offset_z;
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
    double At_This_Time;
    int Complete;
    int For_This_ID;

    double dx;
    double dy;
    double dz;

    int Teleport;
    double New_x;
    double New_y;
    double New_z;
};
struct _Cutscene_Background_Motion_Keyframe
{
    double At_This_Time;
    int Complete;
    double New_Rotational_Velocity;
};

struct _Cutscene_Camera_Action_Keyframe
{
    double At_This_Time;
    int Complete;

    int Action;
    int Active_Camera;

    double New_x;
    double New_y;
    double New_z;

    double New_Center_x;
    double New_Center_y;
    double New_Center_z;

    int Looking_At_This_Actor;
    double Look_Offset_x;
    double Look_Offset_y;
    double Look_Offset_z;
};
struct _Cutscene_Track_Action_Keyframe
{
    double At_This_Time;
    int Complete;
    int Playing;
    int Volume;
    int This_Track;
};
struct _Cutscene_SFX_Action_Keyframe
{
    double At_This_Time;
    int Complete;
    int Playing;
    int Volume;
    int This_Track;
};
struct _Cutscene_Caption_Action_Keyframe
{
    double At_This_Time;
    int Complete;
    int For_This_ID;
    int Visible;
};
struct _Cutscene_Title_Card_Action_Keyframe
{
    double At_This_Time;
    int Complete;
    int Visible;
};
struct _Cutscene_VFX_Motion_Keyframe
{
    double At_This_Time;
    int Complete;
    double New_x;
    double New_y;
    double New_z;
    int Update_Location;
    int Update_Heading;

    double Throttle;
    double Pitch;
    double Yaw;
};
struct _Cutscene_Billboard_Motion_Keyframe
{
    double At_This_Time;
    int Complete;
    double New_x;
    double New_y;
    double New_z;
    int Update_Location;
    int Update_Heading;

    double Throttle;
    double Pitch;
    double Yaw;
};










struct _Cutscene_Actor_Animation_Keyframe
{
    double At_This_Time;
    int Visible;
    int Complete;
    int For_This_ID;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    double Rate;
    int Animate;
};
struct _Cutscene_Background_Animation_Keyframe
{
    double At_This_Time;
    int Visible;
    int Complete;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    double Rate;
};
struct _Cutscene_VFX_Animation_Keyframe
{
    double At_This_Time;
    int Visible;
    int Complete;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    double Rate;
};
struct _Cutscene_Billboard_Animation_Keyframe
{
    double At_This_Time;
    int Visible;
    int Complete;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    double Rate;
    double Distance;
};




struct _Cutscene_Header
{
    char Name[256];
    double Total_Duration_In_Seconds;
    int Number_Of_Actors;
    int Number_Of_Cameras;
    int Number_Of_Camera_Action_Keyframes;
    int Number_Of_Actor_Motion_Keyframes;
    int Number_Of_Actor_Animation_Keyframes;
    int Number_Of_Tracks;
    int Number_Of_SFX;
    int Number_Of_Track_Action_Keyframes;
    int Number_Of_SFX_Action_Keyframes;

    int Offset_To_Actors;
    int Offset_To_Cameras;
    int Offset_To_Camera_Action_Keyframes;
    int Offset_To_Actor_Motion_Keyframes;
    int Offset_To_Actor_Animation_Keyframes;
    int Offset_To_Tracks;
    int Offset_To_SFX;
    int Offset_To_Track_Action_Keyframes;
    int Offset_To_SFX_Action_Keyframes;
};





struct _Cutscene_Actor_Object
{
    struct _SE3_Model* Model;
    double Height;

    int Visible;
    int Animate;

    int Update_Animation;
    int Current_Keyframe;
    int New_Keyframes[2];
    int Loop;
    double Rate;

    double dx;
    double dy;
    double dz;

    int Update_Location;
    int Update_Heading;
    double x;
    double y;
    double z;
};
struct _Cutscene_Background_Object
{
    struct _SE3_Model* Model;

    int Visible;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    double Rate;

    double New_Rotational_Velocity;
};
struct _Cutscene_Camera_Object
{
    double x;
    double y;
    double z;
    double Center_x;
    double Center_y;
    double Center_z;
    int Active;

    int Action;

    double New_x;
    double New_y;
    double New_z;

    double New_Center_x;
    double New_Center_y;
    double New_Center_z;

    double New_Horizontal_Angles[2];
    double New_Rotational_Velocity;

    double New_Vertical_Angles[2];
    double New_Vertical_Velocity;

    int Following_This_Actor;
    double x_Offset;
    double y_Offset;
    double z_Offset;

    int Actor_Portion_To_Look_At;

    double New_Yaw;
    double New_Pitch;
    double New_Throttle;
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
    double Rate;

    double New_x;
    double New_y;
    double New_z;
    int Update_Location;
    int Update_Heading;

    double Throttle;
    double Pitch;
    double Yaw;
};
struct _Cutscene_Billboard_Animation_Object
{
    struct _Image* Image;

    int Visible;

    int Update_Animation;
    int New_Keyframes[2];
    int Loop;
    double Rate;
    double Distance;

    double New_x;
    double New_y;
    double New_z;
    int Update_Location;
    int Update_Heading;

    double Throttle;
    double Pitch;
    double Yaw;
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

    Mix_Music* Tracks;
    Mix_Chunk* SFX;

    int Laps;

    double Animation_Interval_Window = true;
    bool Cleared_To_Animate = true;

    bool Render = true;
};



#pragma pack(pop)

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

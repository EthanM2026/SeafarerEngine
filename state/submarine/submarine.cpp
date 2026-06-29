/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "submarine.h"
#include "math.h"
#include "submarine.h"
#include "math.h"

double Get_Gravity_Accel(double radius)
{
    double Result;
    Result = ((6.674e-11)*5.972e24/(radius*radius));
    return Result;
}


struct _Player_Submarine* Create_Player()
{
    struct _Player_Submarine* Player_Submarine = (struct _Player_Submarine*) calloc(1,sizeof(struct _Player_Submarine));
    return Player_Submarine;
};
void Initialize_On_Foot_Player(struct _Engine* Engine, const char* Character_Filepath, float x, float y, float z)
{
    Engine->On_Foot_State->On_Foot_Player->AffectedByGravity = true;
    Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair = false;

    Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.z = 0;
   // Engine->On_Foot_State->On_Foot_Player->Player_Type = PLAYER_TYPE_ON_FOOT;

    //Engine->On_Foot_State->On_Foot_Player->Models[0] = Create_SE3_Model();
    //Load_SE3_Model(Engine->On_Foot_State->On_Foot_Player->Models[0], "resources/models/model_chris.dat");
    //Load_SE3_Texture(Engine->On_Foot_State->On_Foot_Player->Models[0], "resources/models/texture_chris.sei");

    Load_Character(Engine, "resources/data/characters/rosemary/character_rosemary.dat");

    Engine->On_Foot_State->On_Foot_Player->Jump_Not_Pressed = true;

    Engine->On_Foot_State->On_Foot_Player->On_The_Ground = false;

    Engine->On_Foot_State->On_Foot_Player->x = 0;
    Engine->On_Foot_State->On_Foot_Player->y = 0;
    Engine->On_Foot_State->On_Foot_Player->z = 100;

    Engine->On_Foot_State->On_Foot_Player->Start_Frame = 0;
    Engine->On_Foot_State->On_Foot_Player->Current_Frame = 0;
    Engine->On_Foot_State->On_Foot_Player->End_Frame = 0;
    Engine->On_Foot_State->On_Foot_Player->Current_Model = 0;

    Engine->On_Foot_State->On_Foot_Player->Current_State = 0;

    Engine->On_Foot_State->On_Foot_Player->Forward_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Player->Forward_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Player->Forward_Vector.z = 0;


    Engine->On_Foot_State->On_Foot_Player->Right_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Player->Right_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Player->Right_Vector.z = 0;


    Engine->On_Foot_State->On_Foot_Player->Up_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Player->Up_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Player->Up_Vector.z = 0;



    Engine->On_Foot_State->On_Foot_Player->Ray_Sensor_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Player->Ray_Sensor_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Player->Ray_Sensor_Vector.z = 0;

    Engine->On_Foot_State->On_Foot_Player->Yaw_Angle = 0;
    Engine->On_Foot_State->On_Foot_Player->Pitch_Angle = 0;

    if(Engine->On_Foot_State->On_Foot_Player->Player_Type == PLAYER_TYPE_CAR || Engine->On_Foot_State->On_Foot_Player->Player_Type == PLAYER_TYPE_ON_FOOT)
    {
        Engine->On_Foot_State->On_Foot_Player->Throttle = 0;
    }


    else if(Engine->On_Foot_State->On_Foot_Player->Player_Type == PLAYER_TYPE_SUBMARINE)
    {
        Engine->On_Foot_State->On_Foot_Player->Throttle = 3;
    }


    Engine->On_Foot_State->On_Foot_Player->Floor_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Wall_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Ceiling_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Object_Collision = false;

    Engine->On_Foot_State->On_Foot_Player->On_Top_Of_An_AABB_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Underneath_An_AABB_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Colliding_With_An_AABB_Collision = false;

    Engine->On_Foot_State->On_Foot_Player->On_Top_Of_Mesh_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Underneath_Mesh_Collision = false;
    Engine->On_Foot_State->On_Foot_Player->Colliding_With_Mesh_Collision = false;

    Engine->On_Foot_State->On_Foot_Player->Able_To_Move_Away = true;

    Engine->On_Foot_State->On_Foot_Player->Able_To_Move_Off_Platform = true;


    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.min.z = 0;

    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.max.z = 20.5;


    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.min.z = 20.5;

    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.max.z = 40.5;


    Engine->On_Foot_State->On_Foot_Player->Do_Not_Alter_Velocity = false;

    Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
    Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;
    Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;

    Engine->On_Foot_State->On_Foot_Player->Dead = false;
    Engine->On_Foot_State->On_Foot_Player->Playing_Dead_Animation = false;
};

void Render_Player(struct _Engine* Engine)
{

  //  double z = Get_Ticks(Engine->On_Foot_State->Timer);
   // ////printf("Ticks: %f\n", z);
  //  int x = z * 100;
   // if(x % 12 == 0)
   // {
   //     Engine->On_Foot_State->On_Foot_Player->Current_Frame += 1;
   //         if(Engine->On_Foot_State->On_Foot_Player->Current_Frame >= 8)
   //         {
   //             Engine->On_Foot_State->On_Foot_Player->Current_Frame = 0;
   //         }
   // }

    // Get the current time
    float currentTime = (float)glfwGetTime(); // GLFW time in seconds

    // Check if enough time has passed to update the frame
    if(Engine->On_Foot_State->On_Foot_Player->Current_State == CURRENT_STATE_WALKING)
    {
    while (currentTime - Engine->On_Foot_State->On_Foot_Player->lastTime >= 0.1) {
        Engine->On_Foot_State->On_Foot_Player->Current_Frame = (Engine->On_Foot_State->On_Foot_Player->Current_Frame + 1) % 8;
        Engine->On_Foot_State->On_Foot_Player->lastTime += 0.1;
    }
    }

    else if(!Engine->On_Foot_State->On_Foot_Player->Current_State == CURRENT_STATE_WALKING)
    {
        Engine->On_Foot_State->On_Foot_Player->Current_Frame = 0;
    }

    if(!Engine->On_Foot_State->FPS_Mode)
    {
    Render_SE3_Model(Engine->On_Foot_State->On_Foot_Player->Models[0],Engine->On_Foot_State->On_Foot_Player->Current_Frame,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
    }
};
void Process_Player(struct _Engine* Engine)
{

    if(!Engine->On_Foot_State->On_Foot_Player->Apply_External_Velocity)
    {
        Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.x = 0;
        Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.y = 0;
        Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.z = 0;
    }




    if(Engine->On_Foot_State->On_Foot_Player->z <= -100)
    {
        //Engine->On_Foot_State->On_Foot_Player->Dead = true;
    }

    if(Engine->On_Foot_State->On_Foot_Player->Current_Health <= 0)
    {
        Engine->On_Foot_State->On_Foot_Player->Dead = true;
        ////printf("YOU DIED!\n");
    }

     if(Engine->On_Foot_State->On_Foot_Player->z < 0)
    {
        Engine->On_Foot_State->Current_State = STATE_UNDER_WATER;
    }

    else if(Engine->On_Foot_State->On_Foot_Player->z >= 0)
    {
        Engine->On_Foot_State->Current_State = STATE_ABOVE_WATER;
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Vortexes; j++)
    {
    double x = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,Engine->On_Foot_State->Vortexes[j]->x,Engine->On_Foot_State->Vortexes[j]->y,Engine->On_Foot_State->Vortexes[j]->z);
    if(x <= Engine->On_Foot_State->Vortexes[j]->Gravity_Radius && !Engine->On_Foot_State->Is_In_Subspace)
    {
        Engine->On_Foot_State->On_Foot_Player->AffectedByGravity = false;
        Engine->On_Foot_State->On_Foot_Player->Apply_External_Velocity = true;
        if (!Mix_Playing(3))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(3, Engine->USND_Vortex, 0); // Start the sound
                    }
        if(Engine->On_Foot_State->On_Foot_Player->x > Engine->On_Foot_State->Vortexes[j]->x)
        {
            Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.x = -Engine->On_Foot_State->Vortexes[j]->Suction_Force;
        }

        if(Engine->On_Foot_State->On_Foot_Player->x < Engine->On_Foot_State->Vortexes[j]->x)
        {
            Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.x = Engine->On_Foot_State->Vortexes[j]->Suction_Force;
        }



        if(Engine->On_Foot_State->On_Foot_Player->y > Engine->On_Foot_State->Vortexes[j]->y)
        {
            Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.y = -Engine->On_Foot_State->Vortexes[j]->Suction_Force;
        }

        if(Engine->On_Foot_State->On_Foot_Player->y < Engine->On_Foot_State->Vortexes[j]->y)
        {
            Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.y = Engine->On_Foot_State->Vortexes[j]->Suction_Force;
        }


        if(Engine->On_Foot_State->On_Foot_Player->z > Engine->On_Foot_State->Vortexes[j]->z)
        {
            Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.z = -Engine->On_Foot_State->Vortexes[j]->Suction_Force;
        }

        if(Engine->On_Foot_State->On_Foot_Player->z < Engine->On_Foot_State->Vortexes[j]->z)
        {
            Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.z = Engine->On_Foot_State->Vortexes[j]->Suction_Force;
        }
    }

    if(x > Engine->On_Foot_State->Vortexes[j]->Gravity_Radius)
    {
        Engine->On_Foot_State->On_Foot_Player->Apply_External_Velocity = false;
        Engine->On_Foot_State->On_Foot_Player->AffectedByGravity = true;
    }

    if(x <= 40 && !Engine->On_Foot_State->Is_In_Subspace)
    {
        //Enter_Subspace(Engine);
        //Engine->On_Foot_State->Time_Entered_Subspace = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
         //Engine->On_Foot_State->On_Foot_Player->Apply_External_Velocity = false;
    }
    }

    if(Engine->On_Foot_State->Is_In_Subspace)
    {
        double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        if(x > Engine->On_Foot_State->Time_Entered_Subspace + 10)
        {
            Exit_Subspace(Engine);
        }
    }

    if(Engine->On_Foot_State->On_Foot_Player->Dead && !Engine->On_Foot_State->On_Foot_Player->Playing_Dead_Animation)
    {
        Resume_Timer(Engine->On_Foot_State->Death_Timer);
        Engine->On_Foot_State->On_Foot_Player->Playing_Dead_Animation = true;
    }

    float pitch_rad = Engine->On_Foot_State->On_Foot_Player->Yaw_Angle * M_PI / 180.0f;
    float yaw_rad = Engine->On_Foot_State->On_Foot_Player->Pitch_Angle * M_PI / 180.0f;

    Engine->On_Foot_State->On_Foot_Player->Forward_Vector.x = cos(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->On_Foot_Player->Forward_Vector.y = sin(pitch_rad);
    Engine->On_Foot_State->On_Foot_Player->Forward_Vector.z = sin(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->On_Foot_Player->Forward_Vector = vec3_normalize(Engine->On_Foot_State->On_Foot_Player->Forward_Vector);
    Vec3 global_up = {0.0f, 0.0f, 1.0f};
    Engine->On_Foot_State->On_Foot_Player->Right_Vector = vec3_cross(Engine->On_Foot_State->On_Foot_Player->Forward_Vector, global_up);
    Engine->On_Foot_State->On_Foot_Player->Right_Vector = vec3_normalize(Engine->On_Foot_State->On_Foot_Player->Right_Vector);
    Engine->On_Foot_State->On_Foot_Player->Up_Vector = vec3_cross(Engine->On_Foot_State->On_Foot_Player->Right_Vector, Engine->On_Foot_State->On_Foot_Player->Forward_Vector);
    Engine->On_Foot_State->On_Foot_Player->Up_Vector = vec3_normalize(Engine->On_Foot_State->On_Foot_Player->Up_Vector);


    Vec3 displacement = vec3_scale(Engine->On_Foot_State->On_Foot_Player->Forward_Vector, Engine->On_Foot_State->On_Foot_Player->Throttle * 0.16);

    Engine->On_Foot_State->On_Foot_Player->displacement.x = displacement.x;
    Engine->On_Foot_State->On_Foot_Player->displacement.y = displacement.y;
    //Engine->On_Foot_State->On_Foot_Player->displacement.z = displacement.z;




    Vec3 ray_sensor = vec3_scale(Engine->On_Foot_State->On_Foot_Player->Forward_Vector, 10 * 0.16);

    Engine->On_Foot_State->On_Foot_Player->Ray_Sensor_Vector.x = (displacement.x*16);
    Engine->On_Foot_State->On_Foot_Player->Ray_Sensor_Vector.y = (displacement.y*16);

    Engine->On_Foot_State->On_Foot_Player->Ray_Sensor_Vector.z = Engine->On_Foot_State->On_Foot_Player->z+25;


    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.min.x = Engine->On_Foot_State->On_Foot_Player->x-4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.min.y = Engine->On_Foot_State->On_Foot_Player->y-4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.min.z = Engine->On_Foot_State->On_Foot_Player->z-2;

    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.max.x = Engine->On_Foot_State->On_Foot_Player->x+4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.max.y = Engine->On_Foot_State->On_Foot_Player->y+4;
    Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry.max.z = Engine->On_Foot_State->On_Foot_Player->z+20.5;


    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.min.x = Engine->On_Foot_State->On_Foot_Player->x+-4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.min.y = Engine->On_Foot_State->On_Foot_Player->y+-4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.min.z = Engine->On_Foot_State->On_Foot_Player->z+20.5;

    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.max.x = Engine->On_Foot_State->On_Foot_Player->x+4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.max.y = Engine->On_Foot_State->On_Foot_Player->y+4;
    Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry.max.z = Engine->On_Foot_State->On_Foot_Player->z+40.5;




    bool Mesh_Already_Detected = false;


    //FOOT COLLISION DETECTION
    if(Engine->On_Foot_State->On_Foot_Player->AffectedByGravity)
    {
    Engine->On_Foot_State->On_Foot_Player->displacement.z -= GRAVITY;
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Slope_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Slope_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            Engine->On_Foot_State->On_Foot_Player->On_The_Ground = true;

        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+20,Engine->On_Foot_State->On_Foot_Player->x+Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->y+Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->z+20,Engine->On_Foot_State->Slope_Collision_Meshes[j]))
        {
        //If my displacement takes me into the slope, increase my z.
            Engine->On_Foot_State->On_Foot_Player->z += Engine->On_Foot_State->On_Foot_Player->Throttle+5;

        }

        }

       // else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Slope_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
       // {
            ////printf("No Collision with floor!\n");
       //     Engine->On_Foot_State->On_Foot_Player->On_The_Ground = false;
       // }
    }








    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Player->displacement.z < 0 && !Engine->On_Foot_State->On_Foot_Player->On_The_Ground)
            {
                    if (!Mix_Playing(2))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(2, Engine->USND_Thud, 0); // Start the sound
                    }
            }
            Engine->On_Foot_State->On_Foot_Player->On_The_Ground = true;
            Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with floor!\n");
            Engine->On_Foot_State->On_Foot_Player->On_The_Ground = false;
        }


        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
            ////printf("Colliding with Ceiling!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with Ceiling!\n");
        }
    }


    //FOOT COLLISION DETECTION
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
            Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with wall!\n");
            if(Engine->On_Foot_State->On_Foot_Player->displacement.x != 0 && Engine->On_Foot_State->On_Foot_Player->displacement.y != 0)
            {
                if(Engine->On_Foot_State->On_Foot_Player->On_The_Ground)
                {
                    if (!Mix_Playing(1))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(1, Engine->USND_Walking, 0); // Start the sound
                    }
                }
            }
        }


        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
            Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;
            ////printf("Colliding with Wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with Wall!\n");
        }
    }

     for(int Ship_Index = 0; Ship_Index < Engine->On_Foot_State->Region.Number_Of_Ships; Ship_Index++)
    {
        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Ceiling_Collision_Meshes; j++)
        {
        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
            ////printf("Colliding with floor!\n");
        }
        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with floor!\n");
        }
        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
            ////printf("Colliding with Ceiling!\n");
        }
        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with Ceiling!\n");
        }
        }


        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Wall_Collision_Meshes; j++)
        {
            if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
            Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with wall!\n");
        }


        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
            Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;
            ////printf("Colliding with Wall!\n");
        }

        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with Wall!\n");
        }
        }
    }



    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Goalposts; j++)
    {
        double x = Get_Distance(Engine->On_Foot_State->Goalposts[j]->x,Engine->On_Foot_State->Goalposts[j]->y,Engine->On_Foot_State->Goalposts[j]->z,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
        ////printf("distance: %f\n", x);
        if(x <= 1)
        {
            Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
            Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
            if(Engine->On_Foot_State->Goalposts[j]->Enable_This_Exit == LEVEL_CLEARED_YELLOW_FORWARDS)
            {
                for(int j = 0; j < Engine->Number_Of_Levels; j++)
                {
                if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Easy_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                    Engine->Loaded_Levels[j+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Medium_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                    Engine->Loaded_Levels[j+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Hard_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                    Engine->Loaded_Levels[j+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                }
                }
            }





            if(Engine->On_Foot_State->Goalposts[j]->Enable_This_Exit == LEVEL_CLEARED_YELLOW_UPWARDS)
            {
                for(int j = 0; j < Engine->Number_Of_Levels; j++)
                {
                if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Easy_Level_Exit_Status = LEVEL_CLEARED_YELLOW_UPWARDS;
                    Engine->Loaded_Levels[j+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Medium_Level_Exit_Status = LEVEL_CLEARED_YELLOW_UPWARDS;
                    Engine->Loaded_Levels[j+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Hard_Level_Exit_Status = LEVEL_CLEARED_YELLOW_UPWARDS;
                    Engine->Loaded_Levels[j+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                }
                }
            }
        }
    }


    if(Engine->On_Foot_State->On_Foot_Player->displacement.z > 0)
    {
        Engine->On_Foot_State->On_Foot_Player->Current_State = JUMPING;
       // ////printf("Jumping \n");
    }

    if(Engine->On_Foot_State->On_Foot_Player->displacement.z < 0)
    {
        Engine->On_Foot_State->On_Foot_Player->Current_State = FALLING;
      //  ////printf("Falling \n");
    }


    if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
    {
        Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
        Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
        Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;

        Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.z = 0;
        Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.x = 0;
        Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.y = 0;
    }


    Engine->On_Foot_State->On_Foot_Player->z += Engine->On_Foot_State->On_Foot_Player->displacement.z;

    Engine->On_Foot_State->On_Foot_Player->x += Engine->On_Foot_State->On_Foot_Player->displacement.x;
    Engine->On_Foot_State->On_Foot_Player->y += Engine->On_Foot_State->On_Foot_Player->displacement.y;

    Engine->On_Foot_State->On_Foot_Player->OldDisX = Engine->On_Foot_State->On_Foot_Player->displacement.x;
    Engine->On_Foot_State->On_Foot_Player->OldDisY = Engine->On_Foot_State->On_Foot_Player->displacement.y;
    Engine->On_Foot_State->On_Foot_Player->OldDisZ = Engine->On_Foot_State->On_Foot_Player->displacement.z;



    Engine->On_Foot_State->On_Foot_Player->z += Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.z;

    Engine->On_Foot_State->On_Foot_Player->x += Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.x;
    Engine->On_Foot_State->On_Foot_Player->y += Engine->On_Foot_State->On_Foot_Player->External_Velocity_Vector.y;

    //printf("x %f y %f z %f\n",Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
};



void Handle_Player_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{
    float rotation_speed = 90.0f; // degrees per second
    float acceleration = 10.0f;   // units per second squared
    float delta_time =0.016;

    double Current_Time = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
    if(Current_Time > Engine->On_Foot_State->Time_Weapons_Menu_Was_Born +5)
    {
        Engine->On_Foot_State->Show_Weapons_Menu = false;
    }

    if(Keypad.Mouse_Is_Clicked)
    {
        Engine->On_Foot_State->Show_Weapons_Menu = false;
        Resume_Timer(Engine->On_Foot_State->Cooldown_Timer);
        Engine->On_Foot_State->Cooldown_Complete = false;
    }

    if(Keypad.Mouse_Scroll_Up && Engine->On_Foot_State->On_Foot_Player->Current_Weapon < Engine->On_Foot_State->On_Foot_Player->Number_Of_Weapons-1)
    {
        if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
        {
        Engine->On_Foot_State->On_Foot_Player->Current_Weapon += 1;
        if(!Engine->On_Foot_State->Show_Weapons_Menu)
        {
            Engine->On_Foot_State->Show_Weapons_Menu = true;
            Engine->On_Foot_State->Time_Weapons_Menu_Was_Born = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        }

        else if(Engine->On_Foot_State->Show_Weapons_Menu)
        {
            Engine->On_Foot_State->Time_Weapons_Menu_Was_Born = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        }
        }
    }

    else if(Keypad.Mouse_Scroll_Down && Engine->On_Foot_State->On_Foot_Player->Current_Weapon > 0)
    {
        if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
        {
        Engine->On_Foot_State->On_Foot_Player->Current_Weapon -= 1;
        if(!Engine->On_Foot_State->Show_Weapons_Menu)
        {
            Engine->On_Foot_State->Show_Weapons_Menu = true;
            Engine->On_Foot_State->Time_Weapons_Menu_Was_Born = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        }
        else if(Engine->On_Foot_State->Show_Weapons_Menu)
        {
            Engine->On_Foot_State->Time_Weapons_Menu_Was_Born = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        }
        }
    }

        if(Keypad.Keyboard_W && Engine->On_Foot_State->FPS_Mode && Keypad.Controller_Mode != CONTROL_MODE_CONTROLLER) // Up Arrow (accelerate forward)
        {

                Engine->On_Foot_State->On_Foot_Player->Current_State = CURRENT_STATE_WALKING;
                Engine->On_Foot_State->On_Foot_Player->Throttle = 9.5;
                //1.3 meters per second.
                //0.05909090909 units per 60 frames
        }

        if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
        {
            if(Keypad.Controller.Controller_Primary_Thumbpad[0] != 0 || Keypad.Controller.Controller_Primary_Thumbpad[1] != 0)
            {
                if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                {
                Engine->On_Foot_State->On_Foot_Player->Throttle = 9.5 * Keypad.Controller.Controller_Primary_Thumbpad[1] * -1;
                }
            }

        }

        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Seats; j++)
        {
            double x = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,Engine->On_Foot_State->Seat_Objects[j]->x,Engine->On_Foot_State->Seat_Objects[j]->y,Engine->On_Foot_State->Seat_Objects[j]->z);
            if(x < 30)
            {
                if(Keypad.Controller.Controller_Y || Keypad.Keyboard_E)
                {
                    if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                    {
                    Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair = true;
                    Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->Seat_Objects[j]->x;
                    Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->Seat_Objects[j]->y;
                    Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->Seat_Objects[j]->z;
                    }
                }


            }

        }

        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ships; j++)
        {
        for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_Seats; m++)
        {
        	double x = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->z);
            if(x < 30)
            {
                if(Keypad.Controller.Controller_Y || Keypad.Keyboard_E)
                {
                    if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                    {
                    Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair = true;
                    Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->x;
                    Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->y;
                    Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->z;
                    }
                }
            }
        }
        }



            if(Keypad.Controller.Controller_A || Keypad.Keyboard_A)
            {
                if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                {
                Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair = false;
                }
            }



        if(!Keypad.Keyboard_W && Engine->On_Foot_State->FPS_Mode && Keypad.Controller_Mode != CONTROL_MODE_CONTROLLER) // Down Arrow (decelerate/accelerate backward)
        {

            Engine->On_Foot_State->On_Foot_Player->Current_State = 0;
            Engine->On_Foot_State->On_Foot_Player->Throttle = 0;
        }





        if(Keypad.Keyboard_Up && !Engine->On_Foot_State->FPS_Mode && Keypad.Controller_Mode != CONTROL_MODE_CONTROLLER) // Up Arrow (accelerate forward)
        {

                Engine->On_Foot_State->On_Foot_Player->Current_State = CURRENT_STATE_WALKING;
                Engine->On_Foot_State->On_Foot_Player->Throttle = 9.5;

                //1.3 meters per second.
                //0.05909090909 units per 60 frames

        }



        if(!Keypad.Keyboard_Up && !Engine->On_Foot_State->FPS_Mode && Keypad.Controller_Mode != CONTROL_MODE_CONTROLLER) // Down Arrow (decelerate/accelerate backward)
        {

            Engine->On_Foot_State->On_Foot_Player->Current_State = 0;

                Engine->On_Foot_State->On_Foot_Player->Throttle = 0;


        }


        if(Keypad.Keyboard_Left && !Engine->On_Foot_State->FPS_Mode) // Left Arrow (turn left / yaw decrease)
        {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle += rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->On_Foot_Player->Yaw_Angle < 0.0f)
            {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle += 360.0f;
            }
            }
        if(Keypad.Keyboard_Right && !Engine->On_Foot_State->FPS_Mode) // Right Arrow (turn right / yaw increase)
        {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle -= rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->On_Foot_Player->Yaw_Angle >= 360.0f)
            {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle -= 360.0f;
            }
            }
        if(Keypad.Keyboard_A && Engine->On_Foot_State->On_Foot_Player->Jump_Not_Pressed && Engine->On_Foot_State->On_Foot_Player->On_The_Ground) // Spacebar (brake/stop)
        {
            Engine->On_Foot_State->On_Foot_Player->Jump_Not_Pressed = false;
            Engine->On_Foot_State->On_Foot_Player->displacement.z = 3;

            Engine->On_Foot_State->NPC_Character_Objects[0]->displacement.z=5;
        }

        if(!Keypad.Keyboard_A)
        {
            Engine->On_Foot_State->On_Foot_Player->Jump_Not_Pressed = true;
        }

        for(int Ship_Index = 0; Ship_Index < Engine->On_Foot_State->Region.Number_Of_Ships; Ship_Index++)
    {
        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Docking_Ports; j++)
        {
            double x = Get_Distance(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->x,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->y,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->z,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
           // //printf("Docking Port xyz %f %f %f\n", Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->x,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->y,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->z);

       // //printf("docking port distance: %f\n", x);
            if(x <= 30)
            {
                if(Keypad.Controller.Controller_Y || Keypad.Keyboard_E)
                {
                Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_X;
                Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Y;
                Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Z;
                //printf("Teleported to %f %f %f\n", Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_X,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Y,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Z);

                Engine->On_Foot_State->On_Foot_Player->Is_On_Board = true;
                Engine->On_Foot_State->On_Foot_Player->Ship_ID = Ship_Index;
                }

            }
        }
    }




        if(Keypad.Keyboard_B) // Spacebar (brake/stop)
        {
           // Engine->On_Foot_State->On_Foot_Player->z -= 10;
           // if(Engine->On_Foot_State->On_Foot_Player->On_The_Ground)
           // {
             //   Engine->On_Foot_State->On_Foot_Player->Current_State = PREPARING_TO_JUMP;
               // //////printf("Preparing to Jump \n");

            //}
        }

        if(!Keypad.Keyboard_B) // Spacebar (brake/stop)
        {
            if(Engine->On_Foot_State->On_Foot_Player->On_The_Ground)
            {
                Engine->On_Foot_State->On_Foot_Player->Current_State = NOT_PREPARING_TO_JUMP;
               // //////printf("Not Preparing to Jump \n");

            }
        }


    // After changing yaw or pitch, always re-calculate the orientation vectors
    Process_Player(Engine);
};


void Write_Character(const char* Filename)
{
    struct _Character_Header Header;

    memset(Header.Name,0,256);
    memset(Header.Description,0,256);
    sprintf(Header.Name, "Chris");
    sprintf(Header.Description, "Chris");
    Header.Name[5] = '\0';
    Header.Description[5] = '\0';

    Header.AABB_Height = 40;
    Header.AABB_Length = 20;
    Header.AABB_Width = 20;

    Header.Number_Of_Models_Used = 1;
    Header.Number_Of_Voice_Lines = 0;
    Header.Number_Of_Animations = 0;

    Header.Attack = 100;
    Header.Defense = 50;
    Header.Speed = 10;
    Header.Health = 100;

    Header.Override_Loadout = 0; //Use the CSS loadout instead of this filepath's.

    memset(Header.Loadout_Filepath,0,256);
    sprintf(Header.Loadout_Filepath, "none");
    Header.Name[4] = '\0';

    int Size_Of_Model_Filepaths = Header.Number_Of_Models_Used * 256;
    int Size_Of_Voice_Line_Filepaths = Header.Number_Of_Voice_Lines * 256;
    int Size_Of_Animations = Header.Number_Of_Animations * 256;

    Header.Offset_To_Model_Filepaths = sizeof(struct _Character_Header);
    Header.Offset_To_Voice_Lines = Header.Offset_To_Model_Filepaths + Size_Of_Model_Filepaths;
    Header.Offset_To_Animations = Header.Offset_To_Voice_Lines + Size_Of_Voice_Line_Filepaths;

    char Model_Filepath[256];
    memset(Model_Filepath,0,256);
    sprintf(Model_Filepath,"resources/data/characters/rosemary/model_rosemary.dat");
    Model_Filepath[53] = '\0';

    char Texture_Filepath[256];
    memset(Texture_Filepath,0,256);
    sprintf(Texture_Filepath,"resources/data/characters/rosemary/texture_rosemary.sei");
    Texture_Filepath[55] = '\0';

    FILE* f = fopen(Filename, "wb");
    fwrite(&Header,1,sizeof(struct _Character_Header),f);
    rewind(f);

    fseek(f, Header.Offset_To_Model_Filepaths, SEEK_SET);
    fwrite(&Model_Filepath,1,256,f);
    fwrite(&Texture_Filepath,1,256,f);
    //fwrite
    fclose(f);
};

void Load_Character(struct _Engine* Engine, const char* Filename)
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

    for(int j = 0; j < 256; j++)
    {
        Engine->On_Foot_State->On_Foot_Player->Header.Name[j] = Buffer[Buffer_Location+j];
        Engine->On_Foot_State->On_Foot_Player->Header.Description[j] = Buffer[Buffer_Location+j+256];
    }
    Buffer_Location += 512;

    Engine->On_Foot_State->On_Foot_Player->Header.AABB_Height = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->On_Foot_Player->Header.AABB_Length = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->On_Foot_Player->Header.AABB_Width = *(double*)&(Buffer[Buffer_Location+16]);
    Buffer_Location += 24;

    Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Models_Used = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Voice_Lines = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Animations = *(int*)&(Buffer[Buffer_Location+12]);
    Buffer_Location += 16;

    Engine->On_Foot_State->On_Foot_Player->Header.Attack = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->On_Foot_Player->Header.Defense = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->On_Foot_Player->Header.Speed = *(double*)&(Buffer[Buffer_Location+16]);
    Engine->On_Foot_State->On_Foot_Player->Header.Health = *(double*)&(Buffer[Buffer_Location+24]);
    Buffer_Location += 32;

    Engine->On_Foot_State->On_Foot_Player->Header.Override_Loadout = *(int*)&(Buffer[Buffer_Location]); //Use the CSS loadout instead of this filepath's.
    Buffer_Location += 4;

    for(int j = 0; j < 256; j++)
    {
        Engine->On_Foot_State->On_Foot_Player->Header.Loadout_Filepath[j] = Buffer[Buffer_Location+j];
    }
    Buffer_Location += 256;

    Engine->On_Foot_State->On_Foot_Player->Header.Offset_To_Model_Filepaths = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->On_Foot_Player->Header.Offset_To_Voice_Lines = *(int*)&(Buffer[Buffer_Location+4]);
    Engine->On_Foot_State->On_Foot_Player->Header.Offset_To_Animations = *(int*)&(Buffer[Buffer_Location+8]);

    for(int m = 0; m < Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Models_Used; m++)
    {
        Engine->On_Foot_State->On_Foot_Player->Models[m] = (struct _SE3_Model*)calloc(1,sizeof(struct _SE3_Model));
    }

    for(int m = 0; m < Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Animations; m++)
    {
        Engine->On_Foot_State->On_Foot_Player->Animations[m] = (struct _Character_Animation*)calloc(1,sizeof(struct _Character_Animation));
    }


    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Models_Used; j++)
    {
        Buffer_Location = Engine->On_Foot_State->On_Foot_Player->Header.Offset_To_Model_Filepaths + (j*256);
        char Model_Filepath[256];
        char Texture_Filepath[256];

        for(int m = 0; m < 256; m++)
        {
            Model_Filepath[m] = Buffer[Buffer_Location+m];
            Texture_Filepath[m] = Buffer[Buffer_Location+m+256];
        }
        Load_SE3_Model(Engine->On_Foot_State->On_Foot_Player->Models[j], Model_Filepath);
        Load_SE3_Texture(Engine->On_Foot_State->On_Foot_Player->Models[j], Texture_Filepath);
    }

    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Voice_Lines; j++)
    {
        Buffer_Location = Engine->On_Foot_State->On_Foot_Player->Header.Offset_To_Voice_Lines + (j*256);
    }

    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Player->Header.Number_Of_Animations; j++)
    {
        Buffer_Location = Engine->On_Foot_State->On_Foot_Player->Header.Offset_To_Animations+ (j*sizeof(struct _Character_Animation));
    }


    Engine->On_Foot_State->On_Foot_Player->Current_Attack =Engine->On_Foot_State->On_Foot_Player->Header.Attack;
    Engine->On_Foot_State->On_Foot_Player->Current_Defense =Engine->On_Foot_State->On_Foot_Player->Header.Defense;
    Engine->On_Foot_State->On_Foot_Player->Current_Speed =Engine->On_Foot_State->On_Foot_Player->Header.Speed;
    Engine->On_Foot_State->On_Foot_Player->Current_Health =Engine->On_Foot_State->On_Foot_Player->Header.Health;

    Engine->On_Foot_State->On_Foot_Player->Dead = false;
    Engine->On_Foot_State->On_Foot_Player->Playing_Dead_Animation = false;
};






struct _New_Player_Car* Create_New_Player_Car()
{

}

void Initialize_New_Player_Car(struct _Engine* Engine, int Car_ID)
{

}

void Process_Player_Car(struct _Engine* Engine, int Car_ID)
{
    Engine->On_Foot_State->Cars[Car_ID]->Throttle += Engine->On_Foot_State->Cars[Car_ID]->dThrottle;
    if(Engine->On_Foot_State->Cars[Car_ID]->Throttle - 0.25 >= 0)
    {
        Engine->On_Foot_State->Cars[Car_ID]->Throttle -= 0.25;
    }
    float pitch_rad = Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle * M_PI / 180.0f;
    float yaw_rad = Engine->On_Foot_State->Cars[Car_ID]->Pitch_Angle * M_PI / 180.0f;

    Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector.x = cos(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector.y = sin(pitch_rad);
    Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector.z = sin(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector = vec3_normalize(Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector);
    Vec3 global_up = {0.0f, 0.0f, 1.0f};
    Engine->On_Foot_State->Cars[Car_ID]->Right_Vector = vec3_cross(Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector, global_up);
    Engine->On_Foot_State->Cars[Car_ID]->Right_Vector = vec3_normalize(Engine->On_Foot_State->Cars[Car_ID]->Right_Vector);
    Engine->On_Foot_State->Cars[Car_ID]->Up_Vector = vec3_cross(Engine->On_Foot_State->Cars[Car_ID]->Right_Vector, Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector);
    Engine->On_Foot_State->Cars[Car_ID]->Up_Vector = vec3_normalize(Engine->On_Foot_State->Cars[Car_ID]->Up_Vector);


    Vec3 displacement = vec3_scale(Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector, Engine->On_Foot_State->Cars[Car_ID]->Throttle * 0.16);

    Engine->On_Foot_State->Cars[Car_ID]->displacement.x = displacement.x;
    Engine->On_Foot_State->Cars[Car_ID]->displacement.y = displacement.y;
    //Engine->On_Foot_State->Cars[Car_ID]->displacement.z = displacement.z;




    Vec3 ray_sensor = vec3_scale(Engine->On_Foot_State->Cars[Car_ID]->Forward_Vector, 10 * 0.16);

    Engine->On_Foot_State->Cars[Car_ID]->Ray_Sensor_Vector.x = (displacement.x*16);
    Engine->On_Foot_State->Cars[Car_ID]->Ray_Sensor_Vector.y = (displacement.y*16);

    Engine->On_Foot_State->Cars[Car_ID]->Ray_Sensor_Vector.z = Engine->On_Foot_State->Cars[Car_ID]->z+25;


    Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry.min.x = Engine->On_Foot_State->Cars[Car_ID]->x-4;
    Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry.min.y = Engine->On_Foot_State->Cars[Car_ID]->y-4;
    Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry.min.z = Engine->On_Foot_State->Cars[Car_ID]->z-2;

    Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry.max.x = Engine->On_Foot_State->Cars[Car_ID]->x+4;
    Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry.max.y = Engine->On_Foot_State->Cars[Car_ID]->y+4;
    Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry.max.z = Engine->On_Foot_State->Cars[Car_ID]->z+20.5;


    Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry.min.x = Engine->On_Foot_State->Cars[Car_ID]->x+-4;
    Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry.min.y = Engine->On_Foot_State->Cars[Car_ID]->y+-4;
    Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry.min.z = Engine->On_Foot_State->Cars[Car_ID]->z+20.5;

    Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry.max.x = Engine->On_Foot_State->Cars[Car_ID]->x+4;
    Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry.max.y = Engine->On_Foot_State->Cars[Car_ID]->y+4;
    Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry.max.z = Engine->On_Foot_State->Cars[Car_ID]->z+40.5;




    bool Mesh_Already_Detected = false;


    //FOOT COLLISION DETECTION
    Engine->On_Foot_State->Cars[Car_ID]->displacement.z -= 0.05;

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            Engine->On_Foot_State->Cars[Car_ID]->displacement.z = 0;
            ////printf("Colliding with floor!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            ////printf("No Collision with floor!\n");
        }


        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            Engine->On_Foot_State->Cars[Car_ID]->displacement.z = 0;
            ////printf("Colliding with Ceiling!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            ////printf("No Collision with Ceiling!\n");
        }
    }


    //FOOT COLLISION DETECTION
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            Engine->On_Foot_State->Cars[Car_ID]->displacement.x = 0;
            Engine->On_Foot_State->Cars[Car_ID]->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Feet_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            ////printf("No Collision with wall!\n");
        }


        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            Engine->On_Foot_State->Cars[Car_ID]->displacement.x = 0;
            Engine->On_Foot_State->Cars[Car_ID]->displacement.y = 0;
            ////printf("Colliding with Wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Cars[Car_ID]->Head_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->Cars[Car_ID]->displacement.x,Engine->On_Foot_State->Cars[Car_ID]->displacement.y,Engine->On_Foot_State->Cars[Car_ID]->displacement.z))
        {
            ////printf("No Collision with Wall!\n");
        }
    }


    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Goalposts; j++)
    {
        double x = Get_Distance(Engine->On_Foot_State->Goalposts[j]->x,Engine->On_Foot_State->Goalposts[j]->y,Engine->On_Foot_State->Goalposts[j]->z,Engine->On_Foot_State->Cars[Car_ID]->x,Engine->On_Foot_State->Cars[Car_ID]->y,Engine->On_Foot_State->Cars[Car_ID]->z);
        ////printf("distance: %f\n", x);
        if(x <= 40)
        {
            Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
            Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
            if(Engine->On_Foot_State->Goalposts[j]->Enable_This_Exit == LEVEL_CLEARED_YELLOW_FORWARDS)
            {
                for(int j = 0; j < Engine->Number_Of_Levels; j++)
                {
                if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Easy_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                    Engine->Loaded_Levels[j+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Medium_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                    Engine->Loaded_Levels[j+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Hard_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                    Engine->Loaded_Levels[j+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                }
                }
            }





            if(Engine->On_Foot_State->Goalposts[j]->Enable_This_Exit == LEVEL_CLEARED_YELLOW_UPWARDS)
            {
                for(int j = 0; j < Engine->Number_Of_Levels; j++)
                {
                if(Engine->Loaded_Levels[j].Easy_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Easy_Level_Exit_Status = LEVEL_CLEARED_YELLOW_UPWARDS;
                    Engine->Loaded_Levels[j+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Medium_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Medium_Level_Exit_Status = LEVEL_CLEARED_YELLOW_UPWARDS;
                    Engine->Loaded_Levels[j+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                }

                if(Engine->Loaded_Levels[j].Hard_Level_ID == Engine->Overworld_State->Current_Choice)
                {
                    Engine->Loaded_Levels[j].Hard_Level_Exit_Status = LEVEL_CLEARED_YELLOW_UPWARDS;
                    Engine->Loaded_Levels[j+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                }
                }
            }
        }
    }


    if(Engine->On_Foot_State->Cars[Car_ID]->displacement.z > 0)
    {
        Engine->On_Foot_State->Cars[Car_ID]->Current_State = JUMPING;
       // ////printf("Jumping \n");
    }

    if(Engine->On_Foot_State->Cars[Car_ID]->displacement.z < 0)
    {
        Engine->On_Foot_State->Cars[Car_ID]->Current_State = FALLING;
      //  ////printf("Falling \n");
    }



    Engine->On_Foot_State->Cars[Car_ID]->z += Engine->On_Foot_State->Cars[Car_ID]->displacement.z;

    Engine->On_Foot_State->Cars[Car_ID]->x += Engine->On_Foot_State->Cars[Car_ID]->displacement.x;
    Engine->On_Foot_State->Cars[Car_ID]->y += Engine->On_Foot_State->Cars[Car_ID]->displacement.y;

    float disx = Engine->On_Foot_State->Cars[Car_ID]->displacement.x;
    float disy = Engine->On_Foot_State->Cars[Car_ID]->displacement.y;
    float disz = Engine->On_Foot_State->Cars[Car_ID]->displacement.z;

    ////printf("x %f y %f z %f\n",Engine->On_Foot_State->Cars[Car_ID]->x,Engine->On_Foot_State->Cars[Car_ID]->y,Engine->On_Foot_State->Cars[Car_ID]->z);
}


void Handle_Player_Car_Inputs(struct _Engine* Engine, struct _Keypad Keypad, int Car_ID)
{
    float rotation_speed = 90.0f; // degrees per second
    float acceleration = 10.0f;   // units per second squared
    float delta_time =0.016;

        if(Keypad.Keyboard_Up) // Up Arrow (accelerate forward)
        {

                Engine->On_Foot_State->Cars[Car_ID]->dThrottle = 0.5;

        }
        if(Keypad.Keyboard_Down || !Keypad.Keyboard_Up) // Down Arrow (decelerate/accelerate backward)
        {

            Engine->On_Foot_State->Cars[Car_ID]->Current_State = 0;
                Engine->On_Foot_State->Cars[Car_ID]->dThrottle = 0;
        }
        if(Keypad.Keyboard_Left) // Left Arrow (turn left / yaw decrease)
        {
            Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle += rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle < 0.0f)
            {
            Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle += 360.0f;
            }
            }
        if(Keypad.Keyboard_Right) // Right Arrow (turn right / yaw increase)
        {
            Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle -= rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle >= 360.0f)
            {
            Engine->On_Foot_State->Cars[Car_ID]->Yaw_Angle -= 360.0f;
            }
            }
        if(Keypad.Keyboard_A)
        {
            Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->Cars[Car_ID]->x;
            Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->Cars[Car_ID]->y;
            Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->Cars[Car_ID]->z;

            Engine->On_Foot_State->Player_Is_In_Car = false;
            Engine->On_Foot_State->In_This_Car = -1;
        }
    // After changing yaw or pitch, always re-calculate the orientation vectors
    Process_Player_Car(Engine, Car_ID);
}

void Render_Player_Car(struct _Engine* Engine, int Car_ID)
{

}










struct _Submarine_Object* Create_Submarine_Object()
{
    struct _Submarine_Object* SO = (struct _Submarine_Object*)calloc(1,sizeof(struct _Submarine_Object));
    return SO;
}

void Initialize_Submarine_Object(struct _Engine* Engine, int Submarine_ID)
{
    Engine->Neural_Network_Training_State->Submarine->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Engine->Neural_Network_Training_State->Submarine->Models[0], "resources/data/submarines/car.se3");
        Load_SE3_Texture(Engine->Neural_Network_Training_State->Submarine->Models[0], "sub.sei");

        Engine->Neural_Network_Training_State->Submarine->View_Mode = TPS_MODE;

        Engine->Neural_Network_Training_State->Submarine->plane_x = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_y = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_z = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_yaw = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_pitch = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_roll = 0;
        Engine->Neural_Network_Training_State->Submarine->Single_Pilot = 0;

        Engine->Neural_Network_Training_State->Submarine->On_The_Ground = false;


    Engine->Neural_Network_Training_State->Submarine->Start_Frame = 0;
    Engine->Neural_Network_Training_State->Submarine->Current_Frame = 0;
    Engine->Neural_Network_Training_State->Submarine->End_Frame = 0;
    Engine->Neural_Network_Training_State->Submarine->Current_Model = 0;

    Engine->Neural_Network_Training_State->Submarine->Current_State = 0;

    Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x = 0;
    Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y = -1;
    Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z = 0;


    Engine->Neural_Network_Training_State->Submarine->Right_Vector.x = 1;
    Engine->Neural_Network_Training_State->Submarine->Right_Vector.y = 0;
    Engine->Neural_Network_Training_State->Submarine->Right_Vector.z = 0;


    Engine->Neural_Network_Training_State->Submarine->Up_Vector.x = 0;
    Engine->Neural_Network_Training_State->Submarine->Up_Vector.y = 0;
    Engine->Neural_Network_Training_State->Submarine->Up_Vector.z = 1;



    Engine->Neural_Network_Training_State->Submarine->Ray_Sensor_Vector.x = 0;
    Engine->Neural_Network_Training_State->Submarine->Ray_Sensor_Vector.y = 0;
    Engine->Neural_Network_Training_State->Submarine->Ray_Sensor_Vector.z = 0;


        Engine->Neural_Network_Training_State->Submarine->Throttle = 0;

    Engine->Neural_Network_Training_State->Submarine->Floor_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Wall_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Ceiling_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Object_Collision = false;

    Engine->Neural_Network_Training_State->Submarine->On_Top_Of_An_AABB_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Underneath_An_AABB_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Colliding_With_An_AABB_Collision = false;

    Engine->Neural_Network_Training_State->Submarine->On_Top_Of_Mesh_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Underneath_Mesh_Collision = false;
    Engine->Neural_Network_Training_State->Submarine->Colliding_With_Mesh_Collision = false;

    Engine->Neural_Network_Training_State->Submarine->Able_To_Move_Away = true;

    Engine->Neural_Network_Training_State->Submarine->Able_To_Move_Off_Platform = true;


    Engine->Neural_Network_Training_State->Submarine->Feet_Collision_Geometry.min.x = -4;
    Engine->Neural_Network_Training_State->Submarine->Feet_Collision_Geometry.min.y = -4;
    Engine->Neural_Network_Training_State->Submarine->Feet_Collision_Geometry.min.z = 0;

    Engine->Neural_Network_Training_State->Submarine->Feet_Collision_Geometry.max.x = 4;
    Engine->Neural_Network_Training_State->Submarine->Feet_Collision_Geometry.max.y = 4;
    Engine->Neural_Network_Training_State->Submarine->Feet_Collision_Geometry.max.z = 20.5;


    Engine->Neural_Network_Training_State->Submarine->Head_Collision_Geometry.min.x = -4;
    Engine->Neural_Network_Training_State->Submarine->Head_Collision_Geometry.min.y = -4;
    Engine->Neural_Network_Training_State->Submarine->Head_Collision_Geometry.min.z = 20.5;

    Engine->Neural_Network_Training_State->Submarine->Head_Collision_Geometry.max.x = 4;
    Engine->Neural_Network_Training_State->Submarine->Head_Collision_Geometry.max.y = 4;
    Engine->Neural_Network_Training_State->Submarine->Head_Collision_Geometry.max.z = 40.5;


    Engine->Neural_Network_Training_State->Submarine->Do_Not_Alter_Velocity = false;

    Engine->Neural_Network_Training_State->Submarine->displacement.x = 0;
    Engine->Neural_Network_Training_State->Submarine->displacement.y = 0;
    Engine->Neural_Network_Training_State->Submarine->displacement.z = 0;




      Engine->Neural_Network_Training_State->Submarine->SPEED_STEP = 0.005f;
    Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP = 1.0f;
    Engine->Neural_Network_Training_State->Submarine->TIMER_MS = 16; // ~60 FPS update rate

// --- Global State Variables ---
// Position (World Coordinates)

    Engine->Neural_Network_Training_State->Submarine->plane_roll = 0.0f;  // Rotation around Y-axis (Forward/Backward)

// Speed (Magnitude of movement)
    Engine->Neural_Network_Training_State->Submarine->plane_speed = 0.5;

}

double Inverse_Square_Law(double Intensity, double Distance)
{
    double Result;
    if(Distance <= 1)
    {
        Distance = 0.001;
    }
    Result = (Intensity)/(4*3.14*(Distance*Distance));
    return Result;
}

void Process_Submarine_Object(struct _Engine* Engine, int Submarine_ID)
{
    if(!Engine->On_Foot_State->Submarines[Submarine_ID]->Apply_External_Velocity)
    {
        Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.x = 0;
        Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.y = 0;
        Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.z = 0;
    }

    double x = Get_Distance(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z,Engine->On_Foot_State->Subspace_Hole_x,Engine->On_Foot_State->Subspace_Hole_y,Engine->On_Foot_State->Subspace_Hole_z);
    if(x == 0)
    {
        x = 0.001;
    }
    if(x <= 1000 && !Engine->On_Foot_State->Is_In_Subspace)
    {
        Engine->On_Foot_State->Submarines[Submarine_ID]->Apply_External_Velocity = true;

        if(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x > Engine->On_Foot_State->Subspace_Hole_x)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.x = -600/x;
        }

        if(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x < Engine->On_Foot_State->Subspace_Hole_x)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.x = 600/x;
        }



        if(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y > Engine->On_Foot_State->Subspace_Hole_y)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.y = -600/x;
        }

        if(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y < Engine->On_Foot_State->Subspace_Hole_y)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.y = 600/x;
        }


        if(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z > Engine->On_Foot_State->Subspace_Hole_z)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.z = -600/x;
        }

        if(Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z < Engine->On_Foot_State->Subspace_Hole_z)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.z = 600/x;
        }
    }

    if(x > 1000)
    {
        Engine->On_Foot_State->Submarines[Submarine_ID]->Apply_External_Velocity = false;
    }

    if(x <= 40 && !Engine->On_Foot_State->Is_In_Subspace)
    {
       // Enter_Subspace(Engine);
      //  Engine->On_Foot_State->Time_Entered_Subspace = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
      //   Engine->On_Foot_State->Submarines[Submarine_ID]->Apply_External_Velocity = false;
    }

    if(Engine->On_Foot_State->Is_In_Subspace)
    {
        double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        if(x > Engine->On_Foot_State->Time_Entered_Subspace + 10)
        {
            Exit_Subspace(Engine);
        }
    }




    Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x += Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.x * Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle;
    Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y += Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.y * Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle;
    Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z += Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.z * Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle;



    Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z += Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.z;
    Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x += Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.x;
    Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y += Engine->On_Foot_State->Submarines[Submarine_ID]->External_Velocity_Vector.y;

    ////printf("Forward Vector: %f %f %f\n", Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.x,Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.y,Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.z);

    ////printf("xyz: %f %f %f\n", Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z);
}


void Handle_Submarine_Object_Inputs(struct _Engine* Engine, struct _Keypad Keypad, int Submarine_ID)
{
    //100 meters/second
    //100m / 60 frames
    //1.6m a frame


    if(Keypad.Controller.Controller_RTrigger && Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle < METER_CONVERSION*20)
    {
        Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle += 1;
    }

    if(Keypad.Controller.Controller_LTrigger && Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle > 0)
    {
        Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle -= 1;
    }

     //if(Keypad.Keyboard_Up)

       // Engine->On_Foot_State->Submarines[Submarine_ID]->plane_pitch -= Engine->On_Foot_State->Submarines[Submarine_ID]->ANGLE_STEP;
         rotate_axes(&Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector,  &Engine->On_Foot_State->Submarines[Submarine_ID]->Up_Vector, Engine->On_Foot_State->Submarines[Submarine_ID]->Right_Vector,   Keypad.Controller.Controller_Primary_Thumbpad[1]);



   // if(Keypad.Keyboard_Down)

        //Engine->On_Foot_State->Submarines[Submarine_ID]->plane_pitch += Engine->On_Foot_State->Submarines[Submarine_ID]->ANGLE_STEP;
         //rotate_axes(&Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector,  &Engine->On_Foot_State->Submarines[Submarine_ID]->Up_Vector, Engine->On_Foot_State->Submarines[Submarine_ID]->Right_Vector,   Keypad.Controller.Controller_Primary_Thumbpad[0]*10);
        //Process_Submarine_Object(Engine, Submarine_ID);


   // if(Keypad.Keyboard_Left)

       // Engine->On_Foot_State->Submarines[Submarine_ID]->plane_yaw += Engine->On_Foot_State->Submarines[Submarine_ID]->ANGLE_STEP;
         rotate_axes(&Engine->On_Foot_State->Submarines[Submarine_ID]->Up_Vector,  &Engine->On_Foot_State->Submarines[Submarine_ID]->Right_Vector, Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector,   -Keypad.Controller.Controller_Primary_Thumbpad[0]);
       // Process_Submarine_Object(Engine, Submarine_ID);


   // if(Keypad.Keyboard_Right)

       // Engine->On_Foot_State->Submarines[Submarine_ID]->plane_yaw -= Engine->On_Foot_State->Submarines[Submarine_ID]->ANGLE_STEP;
        //rotate_axes(&Engine->On_Foot_State->Submarines[Submarine_ID]->Up_Vector,  &Engine->On_Foot_State->Submarines[Submarine_ID]->Right_Vector, Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector,   -Keypad.Controller.Controller_Primary_Thumbpad[1]*10);
        //Process_Submarine_Object(Engine, Submarine_ID);


    if(Keypad.Controller.Controller_A && Engine->On_Foot_State->Player_Is_In_Submarine)
        {
            Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x;
            Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y;
            Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z;

            Engine->On_Foot_State->Player_Is_In_Submarine = false;
            Engine->On_Foot_State->In_This_Submarine = -1;
        }

    if(Keypad.Keyboard_C)
        {
            Engine->On_Foot_State->Submarines[Submarine_ID]->Throttle = 0;
        }


        if(Keypad.Controller.Controller_B && Engine->On_Foot_State->Submarines[Submarine_ID]->Fire_Button_Not_Pressed)
    {
         Initialize_Torpedo(Engine,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_x,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_y,Engine->On_Foot_State->Submarines[Submarine_ID]->plane_z,0,0,0,128,TORPEDO_MODE_PROJECTILE,Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.x,Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.y,Engine->On_Foot_State->Submarines[Submarine_ID]->Forward_Vector.z);
         Engine->On_Foot_State->Submarines[Submarine_ID]->Fire_Button_Not_Pressed = false;
    }

    if(!Keypad.Controller.Controller_B)
    {
        Engine->On_Foot_State->Submarines[Submarine_ID]->Fire_Button_Not_Pressed = true;
    }
}

void Render_Submarine_Object(struct _Engine* Engine, int Submarine_ID)
{
    Render_SE3_Model(Engine->On_Foot_State->Submarines[Submarine_ID]->Models[0],0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
}





struct _Torpedo* Create_Torpedo()
{
    struct _Torpedo* Torpedo = (struct _Torpedo*) calloc(1,sizeof(struct _Torpedo));
    return Torpedo;
}

void Initialize_Torpedo(struct _Engine* Engine, float x, float y, float z, float DestX, float DestY, float DestZ, float Velocity, unsigned char Mode, float xVector, float yVector, float zVector)
{
    Engine->On_Foot_State->Number_Of_Torpedoes += 1;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1] = Create_Torpedo();

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Origin_X = x;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Origin_Y = y;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Origin_Z = z; //If it's too far away, just explode so the space can be overwritten.


    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->x = x;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->y = y;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->z = z;

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Velocity = Velocity;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Explosive_Yield_In_TNT = 32;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Proximity_To_Explode = 32;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Exploded = false;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Dead = false;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Visible = true;

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->xVector = xVector*Velocity;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->yVector = yVector*Velocity;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->zVector = zVector*Velocity;

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->displacement.x = xVector;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->displacement.y = yVector;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->displacement.z = zVector;

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Destination_X = 0;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Destination_Y = 0;
    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Destination_Z = 0;

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Mode = Mode;

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Time_Of_Birth = Get_Ticks(Engine->On_Foot_State->Universal_Timer);

    Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Current_Frame = 0;
}

void Detonate_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{
    Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Exploded = true;
    Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Mode = TORPEDO_MODE_EXPLODED;
    Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Death = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
    if (!Mix_Playing(4))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(4, Engine->USND_Explosion, 0); // Start the sound
                    }
    Engine->On_Foot_State->Shaky_Camera = true;
   // glEnable(GL_LIGHT3);
   // GLfloat gnr_position[] = { Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z+20, 0.0 };
   // glLightfv(GL_LIGHT3, GL_POSITION, gnr_position);
}

void Process_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{
    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_GRENADE)
    {

    }

    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_EXPLODED)
    {
        double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);

        if(x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Death + 0.05)
        {
            Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Current_Frame = 1;
        }

        if(x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Death + 0.1)
        {
            Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Current_Frame = 2;
        }

        if(x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Death + 0.15)
        {
            Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Current_Frame = 3;
        }

        if(x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Death + 0.20)
        {
            Engine->On_Foot_State->Torpedoes[Engine->On_Foot_State->Number_Of_Torpedoes-1]->Current_Frame = 4;
        }

        if(x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Death + 0.25)
        {
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Visible = false;
     //       GLfloat gnr_position[] = { -200,-200,-200, 0.0 };
  //  glLightfv(GL_LIGHT3, GL_POSITION, gnr_position);
  //  glDisable(GL_LIGHT3);
        }
    }

    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_HOMING)
    {
    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_X)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x -= 1;
    }

    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x <= Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_X)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x += 1;
    }



    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_Y)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y -= 1;
    }

    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y <= Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_Y)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y += 1;
    }


    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_Z)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z -= 1;
    }

    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z <= Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_Z)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z += 1;
    }

    double x = Get_Distance(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_X,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_Y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Destination_Z);

    if(x <= 30)
    {
        Detonate_Torpedo(Engine, Torpedo_ID);
    }
    }

    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_PROJECTILE)
    {
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x += Engine->On_Foot_State->Torpedoes[Torpedo_ID]->xVector;
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y += Engine->On_Foot_State->Torpedoes[Torpedo_ID]->yVector;
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z += Engine->On_Foot_State->Torpedoes[Torpedo_ID]->zVector;

        double x = Get_Distance(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
        if(x >= METER_CONVERSION * 50)
        {
          //  Detonate_Torpedo(Engine, Torpedo_ID);
        }
    }





    if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_GRENADE)//Grenade
    {

        double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        if(x > Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Time_Of_Birth + 2 && !Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Exploded)
        {
            Detonate_Torpedo(Engine, Torpedo_ID);
        }

        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry.min.x = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x-4;
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry.min.y = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y-4;
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry.min.z = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z-4;

        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry.max.x = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x+4;
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry.max.y = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y+4;
        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry.max.z = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z+4;

        //Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->xVector*Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Velocity;
       // Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->yVector*Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Velocity;
        //Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z = Engine->On_Foot_State->Torpedoes[Torpedo_ID]->zVector*Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Velocity;

        Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z -= GRAVITY/10;

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z))
        {
            if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z < 0 && !Engine->On_Foot_State->Torpedoes[Torpedo_ID]->On_The_Ground)
            {
                    if (!Mix_Playing(2))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(2, Engine->USND_Glass_Thud, 0); // Start the sound
                    }
            }
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->On_The_Ground = true;
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z = 0;
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z))
        {
            ////printf("No Collision with floor!\n");
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->On_The_Ground = false;
        }
    }


    //FOOT COLLISION DETECTION
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z))
        {
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x = 0;
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z))
        {
            ////printf("No Collision with wall!\n");
            if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x != 0 && Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y != 0)
            {
                if(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->On_The_Ground)
                {
                    if (!Mix_Playing(1))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(1, Engine->USND_Glass_Roll, 0); // Start the sound
                    }
                }
            }
        }
    }



     for(int Ship_Index = 0; Ship_Index < Engine->On_Foot_State->Region.Number_Of_Ships; Ship_Index++)
    {
        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Ceiling_Collision_Meshes; j++)
        {
        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z = 0;
            ////printf("Colliding with floor!\n");
        }
        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with floor!\n");
        }
        }


        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Wall_Collision_Meshes; j++)
        {
            if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x = 0;
            Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->Torpedoes[Torpedo_ID]->Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y,Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with wall!\n");
        }
        }
    }

    Engine->On_Foot_State->Torpedoes[Torpedo_ID]->z += Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.z;

    Engine->On_Foot_State->Torpedoes[Torpedo_ID]->x += Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.x;
    Engine->On_Foot_State->Torpedoes[Torpedo_ID]->y += Engine->On_Foot_State->Torpedoes[Torpedo_ID]->displacement.y;
    }
}

void Render_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{

}

// --- Vector Math Helpers ---
void normalize(Vec3* v) {
    float d = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    if (d > 0) { v->x /= d; v->y /= d; v->z /= d; }
}

// --- Rotation Logic (Pitch, Yaw, Roll) ---
void rotate_axes(Vec3* v1, Vec3* v2, Vec3 axis, float angle) {
    // Basic rotation of two vectors around a third axis
    Vec3 old_v1 = *v1;
    Vec3 old_v2 = *v2;

    v1->x = old_v1.x * cos(angle*0.01745329251) + old_v2.x * sin(angle*0.01745329251);
    v1->y = old_v1.y * cos(angle*0.01745329251) + old_v2.y * sin(angle*0.01745329251);
    v1->z = old_v1.z * cos(angle*0.01745329251) + old_v2.z * sin(angle*0.01745329251);

    v2->x = old_v2.x * cos(angle*0.01745329251) - old_v1.x * sin(angle*0.01745329251);
    v2->y = old_v2.y * cos(angle*0.01745329251) - old_v1.y * sin(angle*0.01745329251);
    v2->z = old_v2.z * cos(angle*0.01745329251) - old_v1.z * sin(angle*0.01745329251);

    normalize(v1);
    normalize(v2);
}


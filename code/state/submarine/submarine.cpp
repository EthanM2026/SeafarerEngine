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

//Maybe no jumping is a good thing.

struct _New_Player_Submarine* Create_New_Player_Submarine()
{
     struct _New_Player_Submarine* Player_Submarine = (struct _New_Player_Submarine*) calloc(1,sizeof(struct _New_Player_Submarine));
    return Player_Submarine;
}
void Initialize_New_Player_Submarine(struct _Engine* Engine)
{
    Engine->On_Foot_State->On_Foot_Submarine_Player->SPEED_STEP = 0.005f;
    Engine->On_Foot_State->On_Foot_Submarine_Player->ANGLE_STEP = 2.0f;
    Engine->On_Foot_State->On_Foot_Submarine_Player->TIMER_MS = 16; // ~60 FPS update rate

// --- Global State Variables ---
// Position (World Coordinates)
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x = 0.0f;
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y = 0.0f; // +Y is FORWARDS
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z = 0.0f; // +Z is UPWARDS

// Orientation Angles (Degrees)
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_pitch = 0.0f; // Rotation around X-axis (Right/Left)
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_roll = 0.0f;  // Rotation around Y-axis (Forward/Backward)
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_yaw = 0.0f;   // Rotation around Z-axis (Up/Down)

// Speed (Magnitude of movement)
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed = 0.5;
    Engine->On_Foot_State->On_Foot_Submarine_Player->Model = Create_MD2_Model();
    Load_MD2_Model(Engine->On_Foot_State->On_Foot_Submarine_Player->Model, "resources/models/model_chris.dat");
    Load_Transparent_MD2_Texture(Engine->On_Foot_State->On_Foot_Submarine_Player->Model, "resources/models/texture_chris.bmp","resources/models/texture_chris.trm");
};



void Handle_Player_Submarine_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Up)
    {
        Engine->On_Foot_State->On_Foot_Submarine_Player->plane_pitch -= Engine->On_Foot_State->On_Foot_Submarine_Player->ANGLE_STEP;
        Process_Player(Engine);
    }

    if(Keypad.Down)
    {
        Engine->On_Foot_State->On_Foot_Submarine_Player->plane_pitch += Engine->On_Foot_State->On_Foot_Submarine_Player->ANGLE_STEP;
        Process_Player(Engine);
    }

    if(Keypad.Left)
    {
        Engine->On_Foot_State->On_Foot_Submarine_Player->plane_yaw += Engine->On_Foot_State->On_Foot_Submarine_Player->ANGLE_STEP;
        Process_Player(Engine);
    }

    if(Keypad.Right)
    {
        Engine->On_Foot_State->On_Foot_Submarine_Player->plane_yaw -= Engine->On_Foot_State->On_Foot_Submarine_Player->ANGLE_STEP;
        Process_Player(Engine);
    }

    if(Keypad.A)
    {
    Initialize_Torpedo(Engine, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z, 0,0,0,10, TORPEDO_MODE_HOMING, Engine->On_Foot_State->On_Foot_Submarine_Player->xVector,Engine->On_Foot_State->On_Foot_Submarine_Player->yVector,Engine->On_Foot_State->On_Foot_Submarine_Player->zVector);
    }

    if(Keypad.B)
    {
    Initialize_Torpedo(Engine, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z, 0,0,0,10, TORPEDO_MODE_VECTOR, Engine->On_Foot_State->On_Foot_Submarine_Player->xVector,Engine->On_Foot_State->On_Foot_Submarine_Player->yVector,Engine->On_Foot_State->On_Foot_Submarine_Player->zVector);
    }
};

void Render_Player_Submarine(struct _Engine* Engine)
{
        Render_MD2_Model(Engine->On_Foot_State->On_Foot_Submarine_Player->Model,0,Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x,Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y,Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z,0,0,0,false,Engine->On_Foot_State->Red,Engine->On_Foot_State->Green,Engine->On_Foot_State->Blue);
};






struct _Player_Submarine* Create_Player()
{
    struct _Player_Submarine* Player_Submarine = (struct _Player_Submarine*) calloc(1,sizeof(struct _Player_Submarine));
    return Player_Submarine;
};
void Initialize_On_Foot_Player(struct _Engine* Engine, const char* Character_Filepath, float x, float y, float z)
{
    Engine->On_Foot_State->On_Foot_Player->Player_Type = PLAYER_TYPE_CAR;

    Engine->On_Foot_State->On_Foot_Player->Models[0] = Create_MD2_Model();
    Load_MD2_Model(Engine->On_Foot_State->On_Foot_Player->Models[0], "resources/models/model_chris.dat");
    Load_MD2_Texture(Engine->On_Foot_State->On_Foot_Player->Models[0], "resources/models/texture_chris.bmp");

    Engine->On_Foot_State->On_Foot_Player->On_The_Ground = false;

    Engine->On_Foot_State->On_Foot_Player->x = -64;
    Engine->On_Foot_State->On_Foot_Player->y = -37;
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
};

void Process_Player_Submarine(struct _Engine* Engine)
{
    GLfloat m[16];

    // 1. Calculate the plane's rotation matrix M
    glPushMatrix();
    glLoadIdentity();

    // Rotation Order: Yaw (Z) -> Pitch (X) -> Roll (Y)
    glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_yaw, 0.0f, 0.0f, 1.0f);     // Z-axis (Yaw)
    glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_pitch, 1.0f, 0.0f, 0.0f);   // X-axis (Pitch) - Applied to the yawed frame
    glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_roll, 0.0f, 1.0f, 0.0f);    // Y-axis (Roll) - Applied to the pitched frame

    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    glPopMatrix(); // Restore the ModelView matrix state

    // 2. Extract the Forward Vector (which starts as +Y local axis)
    // The forward vector is stored in the 2nd column of the 4x4 matrix (indices 4, 5, 6)
    float fwdX = m[4];
    float fwdY = m[5];
    float fwdZ = m[6];

    // 3. Apply movement using the calculated forward vector and speed
    Engine->On_Foot_State->On_Foot_Submarine_Player->xVector = 10*fwdX * Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed;
    Engine->On_Foot_State->On_Foot_Submarine_Player->yVector = 10*fwdY * Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed;
    Engine->On_Foot_State->On_Foot_Submarine_Player->zVector = 10*fwdZ * Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed;

    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x += fwdX * Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed;
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y += fwdY * Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed;
    Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z += fwdZ * Engine->On_Foot_State->On_Foot_Submarine_Player->plane_speed;



    // 4. Clamp Z position (prevent flying below the ground)
    if (Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z < 0.0f) {
        Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z = 0.0f;
    }
}

void Render_Player(struct _Engine* Engine)
{

  //  double z = Get_Ticks(Engine->On_Foot_State->Timer);
   // printf("Ticks: %f\n", z);
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
    while (currentTime - Engine->On_Foot_State->On_Foot_Player->lastTime >= 0.1) {
        Engine->On_Foot_State->On_Foot_Player->currentFrame = (Engine->On_Foot_State->On_Foot_Player->currentFrame + 1) % 8;
        Engine->On_Foot_State->On_Foot_Player->lastTime += 0.1;
    }


    Render_MD2_Model(Engine->On_Foot_State->On_Foot_Player->Models[0],Engine->On_Foot_State->On_Foot_Player->currentFrame,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,0,0,0,false,Engine->On_Foot_State->Red,Engine->On_Foot_State->Green,Engine->On_Foot_State->Blue);
};
void Process_Player(struct _Engine* Engine)
{
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

    if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_CAR)
    {
                Engine->On_Foot_State->On_Foot_Player->Throttle += Engine->On_Foot_State->On_Foot_Player->dThrottle;
                if(Engine->On_Foot_State->On_Foot_Player->Throttle -0.2 >= 0)
                {
                Engine->On_Foot_State->On_Foot_Player->Throttle -= 0.2;
                }
                if(Engine->On_Foot_State->On_Foot_Player->Throttle -0.2 < 0)
                {
                Engine->On_Foot_State->On_Foot_Player->Throttle = 0;
                }
    }

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





    if(Engine->On_Foot_State->On_Foot_Player->z <= -500)
    {
        Engine->On_Foot_State->On_Foot_Player->z = 0;
        Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
    }

    bool Mesh_Already_Detected = false;

    //FOOT COLLISION DETECTION
    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {
        //printf("j = %d\n",j);
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall == IS_FLOOR_CEILING)
            {
               // printf("Mesh detected");
                Engine->On_Foot_State->On_Foot_Player->On_The_Ground = true;

                if(Engine->On_Foot_State->On_Foot_Player->Current_State != PREPARING_TO_JUMP)
                {
                    Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;

                    if(Engine->On_Foot_State->On_Foot_Player->displacement.z + Engine->On_Foot_State->On_Foot_Player->z < Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z)
                    {
                        float Teleport_Distance =  Engine->On_Foot_State->On_Foot_Player->z - Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z;
                        Engine->On_Foot_State->On_Foot_Player->z -= Teleport_Distance;
                    }
                    else if(Engine->On_Foot_State->On_Foot_Player->displacement.z + Engine->On_Foot_State->On_Foot_Player->z >= Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z)
                    {
                        Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z;
                    }
                }

                else if(Engine->On_Foot_State->On_Foot_Player->Current_State == PREPARING_TO_JUMP)
                {
                    Engine->On_Foot_State->On_Foot_Player->displacement.z = 2;
                }
                break;
            }
        }

        if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
                //printf("Mesh not detected");
                Engine->On_Foot_State->On_Foot_Player->On_The_Ground = false;
                if(Engine->On_Foot_State->On_Foot_Player->displacement.z -0.05 > -0.06)
                {
                Engine->On_Foot_State->On_Foot_Player->displacement.z -= 0.05;
                }
                Engine->On_Foot_State->On_Foot_Player->z += Engine->On_Foot_State->On_Foot_Player->displacement.z;

        }
    }

    //HEAD COLLISION GEOMETRY
    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {
      //  printf("j = %d\n",j);
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,Engine->On_Foot_State->On_Foot_Player->displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall == IS_FLOOR_CEILING)
            {
                //printf("Mesh detected");
                Engine->On_Foot_State->On_Foot_Player->displacement.z = 0;
                Engine->On_Foot_State->On_Foot_Player->On_The_Ground = true;
                Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Minimum_Z;
                break;
            }
        }
    }


    //WALL COLLISION
    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {
        //printf("j = %d\n",j);
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->displacement.y,displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall == IS_FLOOR_CEILING)
            {
                Engine->On_Foot_State->On_Foot_Player->displacement.x = 0;
                Engine->On_Foot_State->On_Foot_Player->displacement.y = 0;
                if(Engine->On_Foot_State->On_Foot_Player->Player_Type == PLAYER_TYPE_CAR)
                {
                    Engine->On_Foot_State->On_Foot_Player->Throttle = 0;
                    Engine->On_Foot_State->On_Foot_Player->dThrottle = 0;
                }
                break;
            }
        }
    }



    if(Engine->On_Foot_State->On_Foot_Player->displacement.z > 0)
    {
        Engine->On_Foot_State->On_Foot_Player->Current_State = JUMPING;
       // printf("Jumping \n");
    }

    if(Engine->On_Foot_State->On_Foot_Player->displacement.z < 0)
    {
        Engine->On_Foot_State->On_Foot_Player->Current_State = FALLING;
      //  printf("Falling \n");
    }


    Engine->On_Foot_State->On_Foot_Player->x += Engine->On_Foot_State->On_Foot_Player->displacement.x;
    Engine->On_Foot_State->On_Foot_Player->y += Engine->On_Foot_State->On_Foot_Player->displacement.y;


    //printf("x %f y %f z %f\n",Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
};


void Handle_Player_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{
    float rotation_speed = 90.0f; // degrees per second
    float acceleration = 10.0f;   // units per second squared
    float delta_time =0.016;

        if(Keypad.Up) // Up Arrow (accelerate forward)
        {

                Engine->On_Foot_State->On_Foot_Player->Current_State = CURRENT_STATE_WALKING;
                Engine->On_Foot_State->On_Foot_Player->Throttle = 7;

        }
        if(Keypad.Down || !Keypad.Up) // Down Arrow (decelerate/accelerate backward)
        {

            Engine->On_Foot_State->On_Foot_Player->Current_State = 0;

                Engine->On_Foot_State->On_Foot_Player->Throttle = 0;


        }
        if(Keypad.Left) // Left Arrow (turn left / yaw decrease)
        {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle += rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->On_Foot_Player->Yaw_Angle < 0.0f)
            {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle += 360.0f;
            }
            }
        if(Keypad.Right) // Right Arrow (turn right / yaw increase)
        {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle -= rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->On_Foot_Player->Yaw_Angle >= 360.0f)
            {
            Engine->On_Foot_State->On_Foot_Player->Yaw_Angle -= 360.0f;
            }
            }
        if(Keypad.B) // Spacebar (brake/stop)
        {
            if(Engine->On_Foot_State->On_Foot_Player->On_The_Ground)
            {
                Engine->On_Foot_State->On_Foot_Player->Current_State = PREPARING_TO_JUMP;
               // printf("Preparing to Jump \n");

            }
        }

        if(!Keypad.B) // Spacebar (brake/stop)
        {
            if(Engine->On_Foot_State->On_Foot_Player->On_The_Ground)
            {
                Engine->On_Foot_State->On_Foot_Player->Current_State = NOT_PREPARING_TO_JUMP;
               // printf("Not Preparing to Jump \n");

            }
        }


    // After changing yaw or pitch, always re-calculate the orientation vectors
    Process_Player(Engine);
};





void Run_Collision_Test(struct _Engine* Engine)
{
    //Decide what DX, DY, DZ to give
    //Four Conditions:

    //Floor- Yes
    //Wall- Yes
    //Ceiling- Yes

    //Floor- No
    //Wall- No
    //Ceiling- No

    //Floor- Yes
    //Wall- Np
    //Ceiling- No

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

    //Floor-
    //Wall-
    //Ceiling-

}


void Write_Character(const char* Filename)
{
    struct _Character_Header Header;

    sprintf(Header.Name, "Chris");
    sprintf(Header.Description, "None");

    Header.AABB_Height = 0;
    Header.AABB_Length = 0;
    Header.AABB_Width = 0;

    Header.Number_Of_Models_Used = 0;
    Header.Number_Of_Voice_Lines = 0;
    Header.Number_Of_Animations = 0;

    Header.Attack_Power_Percentage = 0;
    Header.Defense_Power_Percentage = 0;
    Header.Speed_Percentage = 0;

    //Chris and Clyde: Explorer Class. Medium Speed, Medium Attack, Medium Defense.
    //BlueSpike and Rosie: Nymph Class. Max Speed, lowest attack and defense.
    //Bob and Wagner: Power Class. Low Speed, High Defense and Attack.

    Header.Override_Loadout = 0; //Use the CSS loadout instead of this filepath's.
    sprintf(Header.Loadout_Filepath, "None");

    int Size_Of_Model_Filepaths = Header.Number_Of_Models_Used * 256;
    int Size_Of_Voice_Lines = Header.Number_Of_Voice_Lines * 256;
    int Size_Of_Animations = Header.Number_Of_Animations * sizeof(struct _Character_Animation);

    Header.Offset_To_Model_Filepaths = sizeof(struct _Character_Header);
    Header.Offset_To_Voice_Lines = sizeof(struct _Character_Header) + Size_Of_Model_Filepaths;
    Header.Offset_To_Animations = sizeof(struct _Character_Header) + Size_Of_Model_Filepaths + Size_Of_Voice_Lines;




    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _Character_Header),f);
    rewind(f);

    fseek(f, Header.Offset_To_Model_Filepaths, SEEK_SET);
    Write_Model_Filepaths(f); //268
    rewind(f);

    fseek(f, Header.Offset_To_Voice_Lines, SEEK_SET);
    Write_Voice_Lines(f); //268
    rewind(f);

    fseek(f, Header.Offset_To_Animations, SEEK_SET);
    Write_Animations(f); //268
    rewind(f);

    fclose(f);
}

void Write_Model_Filepaths(FILE* f)
{
    struct _Filepath F;

    //fwrite(&L,1,sizeof(struct _On_Foot_Light),f);
}

void Write_Voice_Lines(FILE* f)
{
    struct _Filepath F;

    //fwrite(&L,1,sizeof(struct _On_Foot_Light),f);
}

void Write_Animations(FILE* f)
{
    struct _Character_Animation CA;

    memset(CA.Name,0,256);
    sprintf(CA.Name, "IDLE");
    CA.Name[4] = '\0';
    CA.ID = 0;
    CA.Model_Number = 0;
    CA.Start_Keyframe = 0;
    CA.End_Keyframe = 0;
    CA.Speed = 0;
    CA.Loop = 0;

    struct _Character_Animation CATwo;

    memset(CATwo.Name,0,256);
    sprintf(CATwo.Name, "WALK");
    CATwo.Name[4] = '\0';
    CATwo.ID = 1;
    CATwo.Model_Number = 0;
    CATwo.Start_Keyframe = 1;
    CATwo.End_Keyframe = 8;
    CATwo.Speed = 0;
    CATwo.Loop = 1;



    memset(CATwo.Name,0,256);
    sprintf(CATwo.Name, "JUMP");
    CATwo.Name[4] = '\0';
    CATwo.ID = 1;
    CATwo.Model_Number = 0;
    CATwo.Start_Keyframe = 1;
    CATwo.End_Keyframe = 8;
    CATwo.Speed = 0;
    CATwo.Loop = 1;

    //fwrite(&L,1,sizeof(struct _On_Foot_Light),f);
}

void Load_Character(struct _Engine* Engine, const char* Filename)
{

}

struct _Torpedo* Create_Torpedo()
{
    struct _Torpedo* Torpedo = (struct _Torpedo*)calloc(1,sizeof(struct _Torpedo));
    return Torpedo;
}

void Initialize_Torpedo(struct _Engine* Engine, float x, float y, float z, float DestX, float DestY, float DestZ, float Velocity, unsigned char Mode, float xVector, float yVector, float zVector)
{
    Engine->On_Foot_State->Number_Of_Torpedoes += 1;
    Engine->On_Foot_State->Current_Torpedo += 1;

    if(Engine->On_Foot_State->Current_Torpedo < 256)
    {
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo] = Create_Torpedo();

    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Origin_X = x;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Origin_Y = y;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Origin_Z = z; //If it's too far away, just explode so the space can be overwritten.

    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->x = x;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->y = y;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->z = z;

    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Velocity = Velocity;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Explosive_Yield_In_TNT = 32;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Proximity_To_Explode = 100;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Exploded = false;

    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Destination_X = DestX;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Destination_Y=DestY;
    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Destination_Z=DestZ;

    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Model = Create_MD2_Model();
    Load_MD2_Model(Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Model, "resources/models/torpedo.dat");
    Load_MD2_Texture(Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Model, "resources/models/model_chris.bmp");

    Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Mode = Mode;

    if(Mode == TORPEDO_MODE_VECTOR)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->xVector = xVector;
        Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->yVector = yVector;
        Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->zVector = zVector;
    }

    }
}

void Process_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{
    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_HOMING)
    {
    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x < Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_X)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x += Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Velocity;
    }
    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y < Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_Y)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y += Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Velocity;
    }
    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z < Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_Z)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z += Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Velocity;
    }





    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x >= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_X)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x -= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Velocity;
    }
    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y >= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_Y)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y -= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Velocity;
    }
    if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z >= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_Z)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z -= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Velocity;
    }

    float Distance = Get_Distance(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_X,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_Y,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Destination_Z);
    if(Distance <= Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Proximity_To_Explode)
    {
        printf("Torpedo has EXPLODED!\n");
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Exploded = true;
    }
    }


    else if(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Mode == TORPEDO_MODE_VECTOR)
    {
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x += Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->xVector;
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y += Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->yVector;
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z += Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->zVector;


        float Distance = Get_Distance(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Origin_X,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Origin_Y,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Origin_Z);
    if(Distance >= 1000)
    {
        printf("Torpedo has EXPLODED!\n");
        Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Exploded = true;
    }

    }
}

void Render_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{
    if(!Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Exploded)
    {
     //Render_MD2_Model_GLDraw(Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->Model,0,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->x,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->y,Engine->On_Foot_State->On_Foot_Torpedoes[Torpedo_ID]->z,0,0,0,false,Engine->On_Foot_State->Red,Engine->On_Foot_State->Green,Engine->On_Foot_State->Blue);
     }
}


struct _Circular_Shadow* Create_Circular_Shadow()
{
    struct _Circular_Shadow* Circular_Shadow = (struct _Circular_Shadow*) calloc(1,sizeof(struct _Circular_Shadow));
    return Circular_Shadow;
}

void Initialize_Circular_Shadow(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Z, float Parent_Radius)
{
    Engine->On_Foot_State->Universal_Circular_Shadow->Shadow = Create_MD2_Model();
    Load_MD2_Model(Engine->On_Foot_State->Universal_Circular_Shadow->Shadow, "resources/models/model_chris.dat");
    Load_MD2_Texture(Engine->On_Foot_State->Universal_Circular_Shadow->Shadow, "resources/models/texture_chris.bmp");
    Engine->On_Foot_State->Universal_Circular_Shadow->Lifetime = 320;

    Engine->On_Foot_State->Universal_Circular_Shadow->x = 0;
    Engine->On_Foot_State->Universal_Circular_Shadow->y = 0;
    Engine->On_Foot_State->Universal_Circular_Shadow->z = 0;
}


void Recalculate_Circular_Shadow_XY_Coordinates(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Radius)
{
    Engine->On_Foot_State->Universal_Circular_Shadow->x = Parent_X;
    Engine->On_Foot_State->Universal_Circular_Shadow->y = Parent_Y;
}




void Recalculate_Circular_Shadow_Z_Coordinate(struct _Engine* Engine, float Parent_X, float Parent_Y, float Parent_Z, float Parent_Radius)
{
    float Start_Z_Coordinate = Parent_Z;
    float Candidate_Z_Coordinate = Start_Z_Coordinate;
    Engine->On_Foot_State->Universal_Circular_Shadow->z = 0;
    bool Found_The_Floor = false;
    while(!Found_The_Floor)
    {
        for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
        {
            Candidate_Z_Coordinate -= 1;
            if(Check_If_Ray_Sensor_Detects_Mesh(Parent_X,Parent_Y,Start_Z_Coordinate, Parent_X, Parent_Y, Candidate_Z_Coordinate,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh))
            {
                Engine->On_Foot_State->Universal_Circular_Shadow->z = Candidate_Z_Coordinate;
                printf("Detected \n");
                Found_The_Floor = true;
            }
        }
        Candidate_Z_Coordinate = Start_Z_Coordinate;
        if(abs(Start_Z_Coordinate - Candidate_Z_Coordinate) >= Engine->On_Foot_State->Universal_Circular_Shadow->Lifetime)
        {
            Found_The_Floor = true;
        }
    }
}

void Render_Shadow(struct _Engine* Engine)
{
    Render_MD2_Model(Engine->On_Foot_State->Universal_Circular_Shadow->Shadow,0,Engine->On_Foot_State->Universal_Circular_Shadow->x,Engine->On_Foot_State->Universal_Circular_Shadow->y,Engine->On_Foot_State->Universal_Circular_Shadow->z,0,0,0,false,1,1,1);
}




struct _New_Player_Car* Create_New_Player_Car()
{
    struct _New_Player_Car* New_Player_Car = (struct _New_Player_Car*) calloc(1,sizeof(struct _New_Player_Car));
    return New_Player_Car;
}

void Initialize_New_Player_Car(struct _Engine* Engine)
{
//    Engine->On_Foot_State->On_Foot_Car_Player->Car_Player_Type = PLAYER_TYPE_CAR;

    Engine->On_Foot_State->On_Foot_Car_Player->Models[0] = Create_MD2_Model();
    Load_MD2_Model(Engine->On_Foot_State->On_Foot_Car_Player->Models[0], "resources/models/model_chris.dat");
    Load_MD2_Texture(Engine->On_Foot_State->On_Foot_Car_Player->Models[0], "resources/models/texture_chris.bmp");

    Engine->On_Foot_State->On_Foot_Car_Player->On_The_Ground = false;

    Engine->On_Foot_State->On_Foot_Car_Player->x = -64;
    Engine->On_Foot_State->On_Foot_Car_Player->y = -37;
    Engine->On_Foot_State->On_Foot_Car_Player->z = 100;

    Engine->On_Foot_State->On_Foot_Car_Player->Start_Frame = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Current_Frame = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->End_Frame = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Current_Model = 0;

    Engine->On_Foot_State->On_Foot_Car_Player->Current_State = 0;

    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector.z = 0;


    Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector.z = 0;


    Engine->On_Foot_State->On_Foot_Car_Player->Up_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Up_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Up_Vector.z = 0;



    Engine->On_Foot_State->On_Foot_Car_Player->Ray_Sensor_Vector.x = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Ray_Sensor_Vector.y = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Ray_Sensor_Vector.z = 0;

    Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->Pitch_Angle = 0;

        Engine->On_Foot_State->On_Foot_Car_Player->Throttle = 0;



    Engine->On_Foot_State->On_Foot_Car_Player->Floor_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Wall_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Ceiling_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Object_Collision = false;

    Engine->On_Foot_State->On_Foot_Car_Player->On_Top_Of_An_AABB_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Underneath_An_AABB_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Colliding_With_An_AABB_Collision = false;

    Engine->On_Foot_State->On_Foot_Car_Player->On_Top_Of_Mesh_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Underneath_Mesh_Collision = false;
    Engine->On_Foot_State->On_Foot_Car_Player->Colliding_With_Mesh_Collision = false;

    Engine->On_Foot_State->On_Foot_Car_Player->Able_To_Move_Away = true;

    Engine->On_Foot_State->On_Foot_Car_Player->Able_To_Move_Off_Platform = true;


    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.min.z = 0;

    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.max.z = 20.5;


    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.min.z = 20.5;

    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.max.z = 40.5;


    Engine->On_Foot_State->On_Foot_Car_Player->Do_Not_Alter_Velocity = false;

    Engine->On_Foot_State->On_Foot_Car_Player->displacement.x = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->displacement.y = 0;
    Engine->On_Foot_State->On_Foot_Car_Player->displacement.z = 0;
}

void Process_Player_Car(struct _Engine* Engine)
{
    float pitch_rad = Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle * M_PI / 180.0f;
    float yaw_rad = Engine->On_Foot_State->On_Foot_Car_Player->Pitch_Angle * M_PI / 180.0f;

    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector.x = cos(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector.y = sin(pitch_rad);
    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector.z = sin(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector = vec3_normalize(Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector);
    Vec3 global_up = {0.0f, 0.0f, 1.0f};
    Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector = vec3_cross(Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector, global_up);
    Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector = vec3_normalize(Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector);
    Engine->On_Foot_State->On_Foot_Car_Player->Up_Vector = vec3_cross(Engine->On_Foot_State->On_Foot_Car_Player->Right_Vector, Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector);
    Engine->On_Foot_State->On_Foot_Car_Player->Up_Vector = vec3_normalize(Engine->On_Foot_State->On_Foot_Car_Player->Up_Vector);

                Engine->On_Foot_State->On_Foot_Car_Player->Throttle += Engine->On_Foot_State->On_Foot_Car_Player->dThrottle;
                if(Engine->On_Foot_State->On_Foot_Car_Player->Throttle -0.2 >= 0)
                {
                Engine->On_Foot_State->On_Foot_Car_Player->Throttle -= 0.2;
                }
                if(Engine->On_Foot_State->On_Foot_Car_Player->Throttle -0.2 < 0)
                {
                Engine->On_Foot_State->On_Foot_Car_Player->Throttle = 0;
                }


    Vec3 displacement = vec3_scale(Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector, Engine->On_Foot_State->On_Foot_Car_Player->Throttle * 0.16);

    Engine->On_Foot_State->On_Foot_Car_Player->displacement.x = displacement.x;
    Engine->On_Foot_State->On_Foot_Car_Player->displacement.y = displacement.y;
    //Engine->On_Foot_State->On_Foot_Car_Player->displacement.z = displacement.z;




    Vec3 ray_sensor = vec3_scale(Engine->On_Foot_State->On_Foot_Car_Player->Forward_Vector, 10 * 0.16);

    Engine->On_Foot_State->On_Foot_Car_Player->Ray_Sensor_Vector.x = (displacement.x*16);
    Engine->On_Foot_State->On_Foot_Car_Player->Ray_Sensor_Vector.y = (displacement.y*16);

    Engine->On_Foot_State->On_Foot_Car_Player->Ray_Sensor_Vector.z = Engine->On_Foot_State->On_Foot_Car_Player->z+25;


    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.min.x = Engine->On_Foot_State->On_Foot_Car_Player->x-4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.min.y = Engine->On_Foot_State->On_Foot_Car_Player->y-4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.min.z = Engine->On_Foot_State->On_Foot_Car_Player->z-2;

    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.max.x = Engine->On_Foot_State->On_Foot_Car_Player->x+4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.max.y = Engine->On_Foot_State->On_Foot_Car_Player->y+4;
    Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry.max.z = Engine->On_Foot_State->On_Foot_Car_Player->z+20.5;


    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.min.x = Engine->On_Foot_State->On_Foot_Car_Player->x+-4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.min.y = Engine->On_Foot_State->On_Foot_Car_Player->y+-4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.min.z = Engine->On_Foot_State->On_Foot_Car_Player->z+20.5;

    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.max.x = Engine->On_Foot_State->On_Foot_Car_Player->x+4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.max.y = Engine->On_Foot_State->On_Foot_Car_Player->y+4;
    Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry.max.z = Engine->On_Foot_State->On_Foot_Car_Player->z+40.5;





    if(Engine->On_Foot_State->On_Foot_Car_Player->z <= -500)
    {
        Engine->On_Foot_State->On_Foot_Car_Player->z = 0;
        Engine->On_Foot_State->On_Foot_Car_Player->displacement.z = 0;
    }

    bool Mesh_Already_Detected = false;

    //FOOT COLLISION DETECTION
    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {
        //printf("j = %d\n",j);
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,Engine->On_Foot_State->On_Foot_Car_Player->displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall == IS_FLOOR_CEILING)
            {
               // printf("Mesh detected");
                Engine->On_Foot_State->On_Foot_Car_Player->On_The_Ground = true;

                if(Engine->On_Foot_State->On_Foot_Car_Player->Current_State != PREPARING_TO_JUMP)
                {
                    Engine->On_Foot_State->On_Foot_Car_Player->displacement.z = 0;

                    if(Engine->On_Foot_State->On_Foot_Car_Player->displacement.z + Engine->On_Foot_State->On_Foot_Car_Player->z < Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z)
                    {
                        float Teleport_Distance =  Engine->On_Foot_State->On_Foot_Car_Player->z - Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z;
                        Engine->On_Foot_State->On_Foot_Car_Player->z -= Teleport_Distance;
                    }
                    else if(Engine->On_Foot_State->On_Foot_Car_Player->displacement.z + Engine->On_Foot_State->On_Foot_Car_Player->z >= Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z)
                    {
                        Engine->On_Foot_State->On_Foot_Car_Player->z = Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Maximum_Z;
                    }
                }

                else if(Engine->On_Foot_State->On_Foot_Car_Player->Current_State == PREPARING_TO_JUMP)
                {
                    Engine->On_Foot_State->On_Foot_Car_Player->displacement.z = 2;
                }
                break;
            }
        }

        if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Car_Player->Feet_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,Engine->On_Foot_State->On_Foot_Car_Player->displacement.z))
        {
                //printf("Mesh not detected");
                Engine->On_Foot_State->On_Foot_Car_Player->On_The_Ground = false;
                if(Engine->On_Foot_State->On_Foot_Car_Player->displacement.z -0.05 > -0.06)
                {
                Engine->On_Foot_State->On_Foot_Car_Player->displacement.z -= 0.05;
                }
                Engine->On_Foot_State->On_Foot_Car_Player->z += Engine->On_Foot_State->On_Foot_Car_Player->displacement.z;

        }
    }

    //HEAD COLLISION GEOMETRY
    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {
      //  printf("j = %d\n",j);
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,Engine->On_Foot_State->On_Foot_Car_Player->displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall == IS_FLOOR_CEILING)
            {
                //printf("Mesh detected");
                Engine->On_Foot_State->On_Foot_Car_Player->displacement.z = 0;
                Engine->On_Foot_State->On_Foot_Car_Player->On_The_Ground = true;
                Engine->On_Foot_State->On_Foot_Car_Player->z = Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh->Minimum_Z;
                break;
            }
        }
    }


    //WALL COLLISION
    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {
        //printf("j = %d\n",j);
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Car_Player->Head_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,Engine->On_Foot_State->On_Foot_Car_Player->displacement.x,Engine->On_Foot_State->On_Foot_Car_Player->displacement.y,displacement.z))
        {
            if(Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall == IS_FLOOR_CEILING)
            {
                Engine->On_Foot_State->On_Foot_Car_Player->displacement.x = 0;
                Engine->On_Foot_State->On_Foot_Car_Player->displacement.y = 0;

                    Engine->On_Foot_State->On_Foot_Car_Player->Throttle = 0;
                    Engine->On_Foot_State->On_Foot_Car_Player->dThrottle = 0;

                break;
            }
        }
    }



    if(Engine->On_Foot_State->On_Foot_Car_Player->displacement.z > 0)
    {
        Engine->On_Foot_State->On_Foot_Car_Player->Current_State = JUMPING;
       // printf("Jumping \n");
    }

    if(Engine->On_Foot_State->On_Foot_Car_Player->displacement.z < 0)
    {
        Engine->On_Foot_State->On_Foot_Car_Player->Current_State = FALLING;
      //  printf("Falling \n");
    }


    Engine->On_Foot_State->On_Foot_Car_Player->x += Engine->On_Foot_State->On_Foot_Car_Player->displacement.x;
    Engine->On_Foot_State->On_Foot_Car_Player->y += Engine->On_Foot_State->On_Foot_Car_Player->displacement.y;


   // printf("x %f y %f z%f",Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
}


void Handle_Player_Car_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{
    float rotation_speed = 90.0f; // degrees per second
    float acceleration = 10.0f;   // units per second squared
    float delta_time =0.016;

        if(Keypad.Up) // Up Arrow (accelerate forward)
        {

                Engine->On_Foot_State->On_Foot_Car_Player->dThrottle = 0.5;

        }
        if(Keypad.Down || !Keypad.Up) // Down Arrow (decelerate/accelerate backward)
        {

            Engine->On_Foot_State->On_Foot_Car_Player->Current_State = 0;
                Engine->On_Foot_State->On_Foot_Car_Player->dThrottle = 0;
        }
        if(Keypad.Left) // Left Arrow (turn left / yaw decrease)
        {
            Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle += rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle < 0.0f)
            {
            Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle += 360.0f;
            }
            }
        if(Keypad.Right) // Right Arrow (turn right / yaw increase)
        {
            Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle -= rotation_speed * delta_time;
            // Keep yaw within 0-360 range
            if (Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle >= 360.0f)
            {
            Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle -= 360.0f;
            }
            }
    // After changing yaw or pitch, always re-calculate the orientation vectors
    Process_Player_Car(Engine);
}

void Render_Player_Car(struct _Engine* Engine)
{
    Render_MD2_Model(Engine->On_Foot_State->On_Foot_Car_Player->Models[0],0,Engine->On_Foot_State->On_Foot_Car_Player->x,Engine->On_Foot_State->On_Foot_Car_Player->y,Engine->On_Foot_State->On_Foot_Car_Player->z,0,0,0,false,Engine->On_Foot_State->Red,Engine->On_Foot_State->Green,Engine->On_Foot_State->Blue);
}


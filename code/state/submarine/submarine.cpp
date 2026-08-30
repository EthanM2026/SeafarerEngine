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
void Initialize_On_Foot_Player(struct _Player_Submarine* On_Foot_Player, const char* Character_Filepath, float x, float y, float z)
{
    On_Foot_Player->Is_On_A_Ship = false;
    On_Foot_Player->On_This_Ship = -1;

    On_Foot_Player->Current_Location = LOCATION_ON_FOOT;
    On_Foot_Player->Visible = true;
    On_Foot_Player->Injurable = true;

    On_Foot_Player->Current_Health = 100;

    On_Foot_Player->AffectedByGravity = true;
    On_Foot_Player->Is_Locked_Into_Chair = false;

    On_Foot_Player->External_Velocity_Vector.x = 0;
    On_Foot_Player->External_Velocity_Vector.y = 0;
    On_Foot_Player->External_Velocity_Vector.z = 0;
   // On_Foot_Player->Player_Type = PLAYER_TYPE_ON_FOOT;

    On_Foot_Player->Models[0] = Create_SE3_Model();
    Load_SE3_Model(On_Foot_Player->Models[0], "rosemary.se3");
    Load_SE3_Texture(On_Foot_Player->Models[0], "rosemary.sei");

    //Load_Character(Engine, "resources/data/characters/rosemary/character_rosemary.dat");

    On_Foot_Player->Jump_Not_Pressed = true;

    On_Foot_Player->On_The_Ground = false;

    On_Foot_Player->x = 0;
    On_Foot_Player->y = 0;
    On_Foot_Player->z = 100;

    On_Foot_Player->Start_Frame = 0;
    On_Foot_Player->Current_Frame = 0;
    On_Foot_Player->End_Frame = 0;
    On_Foot_Player->Current_Model = 0;

    On_Foot_Player->Current_State = 0;

    On_Foot_Player->Forward_Vector.x = 0;
    On_Foot_Player->Forward_Vector.y = 0;
    On_Foot_Player->Forward_Vector.z = 0;


    On_Foot_Player->Right_Vector.x = 0;
    On_Foot_Player->Right_Vector.y = 0;
    On_Foot_Player->Right_Vector.z = 0;


    On_Foot_Player->Up_Vector.x = 0;
    On_Foot_Player->Up_Vector.y = 0;
    On_Foot_Player->Up_Vector.z = 0;



    On_Foot_Player->Ray_Sensor_Vector.x = 0;
    On_Foot_Player->Ray_Sensor_Vector.y = 0;
    On_Foot_Player->Ray_Sensor_Vector.z = 0;

    On_Foot_Player->Yaw_Angle = 0;
    On_Foot_Player->Pitch_Angle = 0;

    if(On_Foot_Player->Player_Type == PLAYER_TYPE_CAR || On_Foot_Player->Player_Type == PLAYER_TYPE_ON_FOOT)
    {
        On_Foot_Player->Throttle = 0;
    }


    else if(On_Foot_Player->Player_Type == PLAYER_TYPE_SUBMARINE)
    {
        On_Foot_Player->Throttle = 3;
    }


    On_Foot_Player->Floor_Collision = false;
    On_Foot_Player->Wall_Collision = false;
    On_Foot_Player->Ceiling_Collision = false;
    On_Foot_Player->Object_Collision = false;

    On_Foot_Player->On_Top_Of_An_AABB_Collision = false;
    On_Foot_Player->Underneath_An_AABB_Collision = false;
    On_Foot_Player->Colliding_With_An_AABB_Collision = false;

    On_Foot_Player->On_Top_Of_Mesh_Collision = false;
    On_Foot_Player->Underneath_Mesh_Collision = false;
    On_Foot_Player->Colliding_With_Mesh_Collision = false;

    On_Foot_Player->Able_To_Move_Away = true;

    On_Foot_Player->Able_To_Move_Off_Platform = true;


    On_Foot_Player->Feet_Collision_Geometry.min.x = -4;
    On_Foot_Player->Feet_Collision_Geometry.min.y = -4;
    On_Foot_Player->Feet_Collision_Geometry.min.z = 0;

    On_Foot_Player->Feet_Collision_Geometry.max.x = 4;
    On_Foot_Player->Feet_Collision_Geometry.max.y = 4;
    On_Foot_Player->Feet_Collision_Geometry.max.z = 20.5;


    On_Foot_Player->Head_Collision_Geometry.min.x = -4;
    On_Foot_Player->Head_Collision_Geometry.min.y = -4;
    On_Foot_Player->Head_Collision_Geometry.min.z = 20.5;

    On_Foot_Player->Head_Collision_Geometry.max.x = 4;
    On_Foot_Player->Head_Collision_Geometry.max.y = 4;
    On_Foot_Player->Head_Collision_Geometry.max.z = 40.5;


    On_Foot_Player->Do_Not_Alter_Velocity = false;

    On_Foot_Player->displacement.x = 0;
    On_Foot_Player->displacement.y = 0;
    On_Foot_Player->displacement.z = 0;

    On_Foot_Player->Dead = false;
    On_Foot_Player->Playing_Dead_Animation = false;
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

    if(Engine->On_Foot_State->Third_Person_Mode)
    {
        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Player->Models[0],Engine->On_Foot_State->On_Foot_Player->Current_Frame,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,0,0,0,false,1,1,1,false);
    }
};




void Weapon_Round_Collision_Detection(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{

}


void Handle_Collision_Detection(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{
    On_Foot_Player->Apply_Motion = true;
    On_Foot_Player->Apply_Gravity = true;

    bool Colliding_With_A_Slope_Ground = false;

    float slopeThreshold = 0.70710678118; // cos(45 degrees)
    float flatThreshold  = 0.96592582628; // cos(15 degrees)

    for(int j = 0; j < On_Foot_Region_File->Number_Of_Slope_Collision_Geometries; j++)
    {
        if(Check_If_AABB_Detects_Mesh(&On_Foot_Player->Feet_Collision_Geometry,On_Foot_Region->Slopes[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            // Displace the AABB out of the triangle
                On_Foot_Player->x += On_Foot_Player->Feet_Collision_Geometry.collisionNormal.x * On_Foot_Player->Feet_Collision_Geometry.Penetration_Depth;
                On_Foot_Player->y += On_Foot_Player->Feet_Collision_Geometry.collisionNormal.y * On_Foot_Player->Feet_Collision_Geometry.Penetration_Depth;
                On_Foot_Player->z += On_Foot_Player->Feet_Collision_Geometry.collisionNormal.z * GRAVITY*20;



if (On_Foot_Player->Feet_Collision_Geometry.collisionNormal.z >= flatThreshold) {
    // FLAT FLOOR
    // Action: Reset vertical velocity, enable jumping, apply ground friction.
   // printf("Collided with Floor\n");
    On_Foot_Player->On_The_Ground = true;
    Colliding_With_A_Slope_Ground = true;
    On_Foot_Player->displacement.z = 0;
}
else if (On_Foot_Player->Feet_Collision_Geometry.collisionNormal.z < flatThreshold && On_Foot_Player->Feet_Collision_Geometry.collisionNormal.z >= slopeThreshold) {
    // SLOPE / RAMP
    // Action: Allow the player to slide up/down, slightly reduce speed based on steepness.
    //printf("Collided with Slope\n");
    float dotProd = vec3_dot(On_Foot_Player->displacement,  On_Foot_Player->Feet_Collision_Geometry.collisionNormal);

// Only respond if moving TOWARD the surface
if (dotProd < 0.0f) {
    On_Foot_Player->displacement.x -= On_Foot_Player->Feet_Collision_Geometry.collisionNormal.x * dotProd;
    On_Foot_Player->displacement.y -= On_Foot_Player->Feet_Collision_Geometry.collisionNormal.y * dotProd;
    On_Foot_Player->displacement.z -= On_Foot_Player->Feet_Collision_Geometry.collisionNormal.z * dotProd;
}
}
else {
    // WALL (or very steep slope that should act like a wall)
    // Action: Block horizontal movement entirely in that direction.
    //printf("Collided with Wall\n");
    On_Foot_Player->displacement.x = 0;
    On_Foot_Player->displacement.y = 0;
}

        }
    }




    //FIX: There is an oversight where Player is always off the ground if not colliding with a vertical only!
for(int j = 0; j < On_Foot_Region_File->Number_Of_Vertical_Collision_Geometries; j++)
    {
        if(Check_If_AABB_Detects_Mesh(&On_Foot_Player->Feet_Collision_Geometry,On_Foot_Region->Ceilings[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            if(On_Foot_Player->displacement.z < 0 && !On_Foot_Player->On_The_Ground)
            {
                    if (!Mix_Playing(2))
                    { // If channel 1 is not playing anything
                        //Mix_PlayChannel(2, Engine->USND_Thud, 0); // Start the sound
                    }
            }
            On_Foot_Player->On_The_Ground = true;
            On_Foot_Player->displacement.z = 0;
            On_Foot_Player->Apply_Gravity = false;
        }

        else if(!Check_If_AABB_Detects_Mesh(&On_Foot_Player->Feet_Collision_Geometry,On_Foot_Region->Ceilings[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with floor!\n");
            if(!Colliding_With_A_Slope_Ground)
            {
                On_Foot_Player->On_The_Ground = false;
            }
        }


        if(Check_If_AABB_Detects_Mesh(&On_Foot_Player->Head_Collision_Geometry,On_Foot_Region->Ceilings[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            On_Foot_Player->displacement.z = 0; //Need test for ceiling collision
            ////printf("Colliding with Ceiling!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(&On_Foot_Player->Head_Collision_Geometry,On_Foot_Region->Ceilings[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with Ceiling!\n");
        }
    }


    //FOOT COLLISION DETECTION
    for(int j = 0; j < On_Foot_Region_File->Number_Of_Horizontal_Collision_Geometries; j++)
    {
        if(Check_If_AABB_Detects_Mesh(&On_Foot_Player->Feet_Collision_Geometry,On_Foot_Region->Walls[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            On_Foot_Player->displacement.x = 0;
            On_Foot_Player->displacement.y = 0;
            On_Foot_Player->Apply_Motion = false;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(&On_Foot_Player->Feet_Collision_Geometry,On_Foot_Region->Walls[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with wall!\n");
            if(On_Foot_Player->displacement.x != 0 && On_Foot_Player->displacement.y != 0)
            {
                if(On_Foot_Player->On_The_Ground)
                {
                    if (!Mix_Playing(1))
                    { // If channel 1 is not playing anything
                        //Mix_PlayChannel(1, Engine->USND_Walking, 0); // Start the sound
                    }
                }
            }
        }


        if(Check_If_AABB_Detects_Mesh(&On_Foot_Player->Head_Collision_Geometry,On_Foot_Region->Walls[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            On_Foot_Player->displacement.x = 0;
            On_Foot_Player->displacement.y = 0;
            On_Foot_Player->Apply_Motion = false;
            ////printf("Colliding with Wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(&On_Foot_Player->Head_Collision_Geometry,On_Foot_Region->Walls[j]->Collision_Mesh,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            ////printf("No Collision with Wall!\n");
        }
    }

Submarine_Vehicle_Collision_Detection(On_Foot_Player, On_Foot_Region, On_Foot_Region_File);
   Door_Collision_Detection(On_Foot_Player, On_Foot_Region, On_Foot_Region_File);
   Weapon_Round_Collision_Detection(On_Foot_Player, On_Foot_Region, On_Foot_Region_File);
}

void Process_Player(struct _Seafarer_Engine_Network_Client* SENC, int Connection_Status, struct _Engine* Engine, struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{
    float pitch_rad = On_Foot_Player->Yaw_Angle * M_PI / 180.0f;
    float yaw_rad = On_Foot_Player->Pitch_Angle * M_PI / 180.0f;

    On_Foot_Player->Forward_Vector.x = cos(yaw_rad) * cos(pitch_rad);
    On_Foot_Player->Forward_Vector.y = sin(pitch_rad);
    On_Foot_Player->Forward_Vector.z = sin(yaw_rad) * cos(pitch_rad);
    On_Foot_Player->Forward_Vector = vec3_normalize(On_Foot_Player->Forward_Vector);
    Vec3 global_up = {0.0f, 0.0f, 1.0f};
    On_Foot_Player->Right_Vector = vec3_cross(On_Foot_Player->Forward_Vector, global_up);
    On_Foot_Player->Right_Vector = vec3_normalize(On_Foot_Player->Right_Vector);
    On_Foot_Player->Up_Vector = vec3_cross(On_Foot_Player->Right_Vector, On_Foot_Player->Forward_Vector);
    On_Foot_Player->Up_Vector = vec3_normalize(On_Foot_Player->Up_Vector);


    Vec3 displacement = vec3_scale(On_Foot_Player->Forward_Vector, On_Foot_Player->Throttle * 0.16);

    On_Foot_Player->displacement.x = displacement.x;
    On_Foot_Player->displacement.y = displacement.y;
    //On_Foot_Player->displacement.z = displacement.z;


    Vec3 ray_sensor = vec3_scale(On_Foot_Player->Forward_Vector, 10 * 0.16);

    On_Foot_Player->Ray_Sensor_Vector.x = (displacement.x*16);
    On_Foot_Player->Ray_Sensor_Vector.y = (displacement.y*16);

    On_Foot_Player->Ray_Sensor_Vector.z = On_Foot_Player->z+25;


    On_Foot_Player->Feet_Collision_Geometry.min.x = On_Foot_Player->x-4;
    On_Foot_Player->Feet_Collision_Geometry.min.y = On_Foot_Player->y-4;
    On_Foot_Player->Feet_Collision_Geometry.min.z = On_Foot_Player->z-2;

    On_Foot_Player->Feet_Collision_Geometry.max.x = On_Foot_Player->x+4;
    On_Foot_Player->Feet_Collision_Geometry.max.y = On_Foot_Player->y+4;
    On_Foot_Player->Feet_Collision_Geometry.max.z = On_Foot_Player->z+20.5;


    On_Foot_Player->Head_Collision_Geometry.min.x = On_Foot_Player->x+-4;
    On_Foot_Player->Head_Collision_Geometry.min.y = On_Foot_Player->y+-4;
    On_Foot_Player->Head_Collision_Geometry.min.z = On_Foot_Player->z+20.5;

    On_Foot_Player->Head_Collision_Geometry.max.x = On_Foot_Player->x+4;
    On_Foot_Player->Head_Collision_Geometry.max.y = On_Foot_Player->y+4;
    On_Foot_Player->Head_Collision_Geometry.max.z = On_Foot_Player->z+40.5;




    if(On_Foot_Player->displacement.z > 0)
    {
        On_Foot_Player->Current_State = JUMPING;
    }

    if(On_Foot_Player->displacement.z < 0)
    {
        On_Foot_Player->Current_State = FALLING;
    }

    On_Foot_Player->displacement.z -= GRAVITY;

    if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
    {
        Handle_Collision_Detection(On_Foot_Player, On_Foot_Region, On_Foot_Region_File);

        if(On_Foot_Player->Is_On_A_Ship)
        {
            Handle_Collision_Detection(On_Foot_Player, &Engine->On_Foot_State->Ship_Objects[On_Foot_Player->On_This_Ship]->Ship_Interior, &Engine->On_Foot_State->Ship_Objects[On_Foot_Player->On_This_Ship]->Ship_Interior_File);
        }
    }


    if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
    {
        On_Foot_Player->z += On_Foot_Player->displacement.z;
    }


    if(!On_Foot_Player->Dead && On_Foot_Player->Current_Location == LOCATION_ON_FOOT && !Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
    {


                On_Foot_Player->x += On_Foot_Player->displacement.x;
                On_Foot_Player->y += On_Foot_Player->displacement.y;


    }


    if(Connection_Status == 1)
    {
        Packet move_pkt;
        move_pkt.type = MSG_MOVE;
        move_pkt.x = 0.0f;
        move_pkt.y = 0.0f;
        move_pkt.z = 0.0f;
        move_pkt.pitch = On_Foot_Player->Pitch_Angle;
        move_pkt.yaw = On_Foot_Player->Yaw_Angle;

        move_pkt.x += On_Foot_Player->displacement.x;
        move_pkt.y += On_Foot_Player->displacement.y;
        move_pkt.z += On_Foot_Player->displacement.z;

        // Send input request to the server instead of moving locally
        if (move_pkt.x != 0.0f || move_pkt.y != 0.0f || move_pkt.z != 0.0f)
        {
            send(SENC->sock_fd, &move_pkt, sizeof(Packet), 0);
        }
    }



    //printf("x %f y %f z %f\n",On_Foot_Player->x,On_Foot_Player->y,On_Foot_Player->z);
   // printf("Throttle: %f\n", On_Foot_Player->Throttle);

   if(On_Foot_Player->Current_Health <= 0 && !On_Foot_Player->Dead)
   {
        On_Foot_Player->Dead = true;
        printf("Player died!\n");
        On_Foot_Player->Throttle = 0;
   }
};

void Generate_Player_Inputs(struct _Seafarer_Engine_Network_Client* SENC, int Connection_Status, struct _Engine* Engine, struct _Player_Submarine* On_Foot_Player, struct _Keypad Keypad, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{
   // if(Keypad.Keyboard_W) // Up Arrow (accelerate forward)
  //  {
  //      On_Foot_Player->Current_State = CURRENT_STATE_WALKING;
     //  On_Foot_Player->Throttle = 0.15;
  //  }

  //  if(!Keypad.Keyboard_W) // Down Arrow (decelerate/accelerate backward)
  //  {
  //      On_Foot_Player->Current_State = 0;
 //       On_Foot_Player->Throttle = 0;
 //  }

  //  if(Keypad.Keyboard_A && On_Foot_Player->Jump_Not_Pressed && On_Foot_Player->On_The_Ground) // Spacebar (brake/stop)
 //   {
  //      On_Foot_Player->Jump_Not_Pressed = false;
 //       On_Foot_Player->displacement.z = 3;
 //   }
//
 //   if(!Keypad.Keyboard_A)
 //   {
 //      On_Foot_Player->Jump_Not_Pressed = true;
 //   }
    Process_Player(SENC, Connection_Status, Engine, On_Foot_Player, On_Foot_Region, On_Foot_Region_File);
};



void Handle_Player_Inputs(struct _Engine* Engine, struct _Seafarer_Engine_Network_Client* SENC, int Connection_Status, struct _Player_Submarine* On_Foot_Player, struct _Keypad Keypad, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{
    if(!On_Foot_Player->Dead && On_Foot_Player->Current_Location == LOCATION_ON_FOOT)
    {
    if(Keypad.Keyboard_W && !On_Foot_Player->Is_Locked_Into_Chair) // Up Arrow (accelerate forward)
    {
        On_Foot_Player->Current_State = CURRENT_STATE_WALKING;
        On_Foot_Player->Throttle = 9.5;
    }



    if(Keypad.Keyboard_Y && !Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode_Switch_Was_Toggled) // Down Arrow (decelerate/accelerate backward)
    {
        Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode_Switch_Was_Toggled = true;

        if(Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode)
        {
            Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode = false;
        }

        else if(!Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode)
        {
            Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode = true;
        }
    }

    if(!Keypad.Keyboard_Y) // Down Arrow (decelerate/accelerate backward)
    {
        Engine->On_Foot_State->On_Foot_Region.Seats[0]->TV_Mode_Switch_Was_Toggled = false;
    }

    if(!Keypad.Keyboard_W) // Down Arrow (decelerate/accelerate backward)
    {
        On_Foot_Player->Current_State = 0;
        On_Foot_Player->Throttle = 0;
    }

    if(Keypad.Keyboard_A && On_Foot_Player->Jump_Not_Pressed && On_Foot_Player->On_The_Ground) // Spacebar (brake/stop)
    {
        On_Foot_Player->Jump_Not_Pressed = false;
        On_Foot_Player->displacement.z = 3;
    }

    if(!Keypad.Keyboard_A)
    {
        On_Foot_Player->Jump_Not_Pressed = true;
    }

    if(Keypad.Keyboard_R && On_Foot_Player->Is_Locked_Into_Chair)
    {
        if(On_Foot_Region->Seats[On_Foot_Player->Chair_ID]->New_Computer->Power_State == false)
        {
            Turn_Computer_On(Engine, On_Foot_Region->Seats[On_Foot_Player->Chair_ID]);
        }

        else if(On_Foot_Region->Seats[On_Foot_Player->Chair_ID]->New_Computer->Power_State == true) //Reboot
        {
            Turn_Computer_Off(Engine, On_Foot_Region->Seats[On_Foot_Player->Chair_ID]);
            Turn_Computer_On(Engine, On_Foot_Region->Seats[On_Foot_Player->Chair_ID]);
        }
    }

    if(Keypad.Keyboard_T && On_Foot_Player->Is_Locked_Into_Chair && On_Foot_Region->Seats[On_Foot_Player->Chair_ID]->New_Computer->Power_State == true)
    {
        Turn_Computer_Off(Engine, On_Foot_Region->Seats[On_Foot_Player->Chair_ID]);
    }

    if(Keypad.Keyboard_E) // Up Arrow (accelerate forward)
    {
        for(int j = 0; j < On_Foot_Region_File->Number_Of_Seats; j++)
        {
            double x = Get_Distance(On_Foot_Player->x,On_Foot_Player->y,On_Foot_Player->z,On_Foot_Region->Seats[j]->x,On_Foot_Region->Seats[j]->y,On_Foot_Region->Seats[j]->z);
            if((x/METER_CONVERSION) <= 1 && !On_Foot_Player->Is_Locked_Into_Chair)
            {
                printf("Seat!\n");
                On_Foot_Player->Is_Locked_Into_Chair = true;
                On_Foot_Player->Chair_ID = j;

                On_Foot_Player->x = On_Foot_Region->Seats[j]->x;
                On_Foot_Player->y = On_Foot_Region->Seats[j]->y;
                On_Foot_Player->z = On_Foot_Region->Seats[j]->z;
            }
        }



        for(int j = 0; j < On_Foot_Region_File->Number_Of_Vehicles; j++)
        {
            double x = Get_Distance(On_Foot_Player->x,On_Foot_Player->y,On_Foot_Player->z,On_Foot_Region->Submarine_Vehicles[j]->plane_x,On_Foot_Region->Submarine_Vehicles[j]->plane_y,On_Foot_Region->Submarine_Vehicles[j]->plane_z);


            if((x/METER_CONVERSION) <= 1)
            {
                if(!On_Foot_Region->Submarine_Vehicles[j]->Is_A_Proxy)
                {
                    printf("Number of Passengers: %d\n", On_Foot_Region->Submarine_Vehicles[j]->Number_Of_Passengers);
                    printf("Max Number of Passengers: %d\n", On_Foot_Region->Submarine_Vehicles[j]->Maximum_Passengers);
                    if(On_Foot_Region->Submarine_Vehicles[j]->Number_Of_Passengers + 1 <= On_Foot_Region->Submarine_Vehicles[j]->Maximum_Passengers)
                    {
                        if(On_Foot_Player->Current_Location == LOCATION_ON_FOOT)
                        {
                            printf("Getting in the SUB!\n");
                            On_Foot_Player->Current_Location = LOCATION_SUBMARINE_VEHICLE;
                            On_Foot_Player->Inside_This_Vehicle = j;

                            for(int s = 0; s < On_Foot_Region->Submarine_Vehicles[j]->Maximum_Passengers; s++)
                            {
                                if(On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[s].Category == 0 && On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[s].Subcategory == 0 && On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[s].Variant == 0 && On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[s].ID == 0)
                                {
                                    On_Foot_Player->Am_This_Passenger = s;
                                }
                            }
                            //On_Foot_Player->Vehicle_Belongs_To_This_Region = Region_ID;
                            On_Foot_Player->Visible = false;
                            On_Foot_Player->Injurable = false;

                            for(int n = 0; n < 256; n++)
                            {
                                On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[On_Foot_Player->Am_This_Passenger].Name[n] = On_Foot_Player->ID.Name[n];
                            }
                            On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[On_Foot_Player->Am_This_Passenger].Category = On_Foot_Player->ID.Category;
                            On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[On_Foot_Player->Am_This_Passenger].Subcategory = On_Foot_Player->ID.Subcategory;
                            On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[On_Foot_Player->Am_This_Passenger].Variant = On_Foot_Player->ID.Variant;
                            On_Foot_Region->Submarine_Vehicles[j]->Passenger_IDs[On_Foot_Player->Am_This_Passenger].ID = On_Foot_Player->ID.ID;

                            On_Foot_Region->Submarine_Vehicles[j]->Number_Of_Passengers += 1;

                            On_Foot_Region->Submarine_Vehicles[j]->Throttle = 1;

                            printf("You are passenger %d\n", On_Foot_Player->Am_This_Passenger);
                        }
                    }
                }

                else if(On_Foot_Region->Submarine_Vehicles[j]->Is_A_Proxy)
                {
                    On_Foot_Player->x = Engine->On_Foot_State->Ship_Objects[On_Foot_Region->Submarine_Vehicles[j]->Proxy_ID]->Ship_Interior.Doors[0]->x;
                    On_Foot_Player->y = Engine->On_Foot_State->Ship_Objects[On_Foot_Region->Submarine_Vehicles[j]->Proxy_ID]->Ship_Interior.Doors[0]->y;
                    On_Foot_Player->z = Engine->On_Foot_State->Ship_Objects[On_Foot_Region->Submarine_Vehicles[j]->Proxy_ID]->Ship_Interior.Doors[0]->z+400;

                    On_Foot_Player->Is_On_A_Ship = true;
                    On_Foot_Player->On_This_Ship = On_Foot_Region->Submarine_Vehicles[j]->Proxy_ID;
                }
            }
        }
    }





    }
    Process_Player(SENC, Connection_Status, Engine, On_Foot_Player, On_Foot_Region, On_Foot_Region_File);
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
    Engine->On_Foot_State->On_Foot_Player->Current_Health = 100;

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
}


void Handle_Player_Car_Inputs(struct _Engine* Engine, struct _Keypad Keypad, int Car_ID)
{

}

void Render_Player_Car(struct _Engine* Engine, int Car_ID)
{

}










struct _Submarine_Object* Create_Submarine_Object()
{
    struct _Submarine_Object* SO = (struct _Submarine_Object*)calloc(1,sizeof(struct _Submarine_Object));
    return SO;
}



void Initialize_On_Foot_Submarine_Object(struct _Submarine_Object* Submarine_Object, double x, double y, double z, double yaw, double pitch, double roll)
{
    Submarine_Object->Port_Me_To_A_Display = false;
    Submarine_Object->Use_This_Seats_Display = -1;


    Submarine_Object->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Submarine_Object->Models[0], "resources/models/test_plane.se3");
        Load_SE3_Texture(Submarine_Object->Models[0], "resources/models/ray_drone.sei");

        Submarine_Object->View_Mode = TPS_MODE;

        Submarine_Object->plane_x = x;
        Submarine_Object->plane_y = y;
        Submarine_Object->plane_z = z;
        Submarine_Object->plane_yaw = yaw;
        Submarine_Object->plane_pitch = pitch;
        Submarine_Object->plane_roll = roll;
        Submarine_Object->Single_Pilot = 0;

        Submarine_Object->On_The_Ground = false;


    Submarine_Object->Start_Frame = 0;
    Submarine_Object->Current_Frame = 0;
    Submarine_Object->End_Frame = 0;
    Submarine_Object->Current_Model = 0;

    Submarine_Object->Current_State = 0;

    Submarine_Object->Forward_Vector.x = 0;
    Submarine_Object->Forward_Vector.y = -1;
    Submarine_Object->Forward_Vector.z = 0;


    Submarine_Object->Right_Vector.x = 1;
    Submarine_Object->Right_Vector.y = 0;
    Submarine_Object->Right_Vector.z = 0;


    Submarine_Object->Up_Vector.x = 0;
    Submarine_Object->Up_Vector.y = 0;
    Submarine_Object->Up_Vector.z = 1;



    Submarine_Object->Ray_Sensor_Vector.x = 0;
    Submarine_Object->Ray_Sensor_Vector.y = 0;
    Submarine_Object->Ray_Sensor_Vector.z = 0;


        Submarine_Object->Throttle = 1;

    Submarine_Object->Floor_Collision = false;
    Submarine_Object->Wall_Collision = false;
    Submarine_Object->Ceiling_Collision = false;
    Submarine_Object->Object_Collision = false;

    Submarine_Object->On_Top_Of_An_AABB_Collision = false;
    Submarine_Object->Underneath_An_AABB_Collision = false;
    Submarine_Object->Colliding_With_An_AABB_Collision = false;

    Submarine_Object->On_Top_Of_Mesh_Collision = false;
    Submarine_Object->Underneath_Mesh_Collision = false;
    Submarine_Object->Colliding_With_Mesh_Collision = false;

    Submarine_Object->Able_To_Move_Away = true;

    Submarine_Object->Able_To_Move_Off_Platform = true;


    Submarine_Object->Feet_Collision_Geometry.min.x = -4;
    Submarine_Object->Feet_Collision_Geometry.min.y = -4;
    Submarine_Object->Feet_Collision_Geometry.min.z = 0;

    Submarine_Object->Feet_Collision_Geometry.max.x = 4;
    Submarine_Object->Feet_Collision_Geometry.max.y = 4;
    Submarine_Object->Feet_Collision_Geometry.max.z = 20.5;


    Submarine_Object->Head_Collision_Geometry.min.x = -4;
    Submarine_Object->Head_Collision_Geometry.min.y = -4;
    Submarine_Object->Head_Collision_Geometry.min.z = 20.5;

    Submarine_Object->Head_Collision_Geometry.max.x = 4;
    Submarine_Object->Head_Collision_Geometry.max.y = 4;
    Submarine_Object->Head_Collision_Geometry.max.z = 40.5;


    Submarine_Object->Do_Not_Alter_Velocity = false;

    Submarine_Object->displacement.x = 0;
    Submarine_Object->displacement.y = 0;
    Submarine_Object->displacement.z = 0;




      Submarine_Object->SPEED_STEP = 0.005f;
    Submarine_Object->ANGLE_STEP = 1.0f;
    Submarine_Object->TIMER_MS = 16; // ~60 FPS update rate

// --- Global State Variables ---
// Position (World Coordinates)

    Submarine_Object->plane_roll = 0.0f;  // Rotation around Y-axis (Forward/Backward)

// Speed (Magnitude of movement)
    Submarine_Object->plane_speed = 0.5;

}







void Initialize_Submarine_Object(struct _Engine* Engine, int Submarine_ID)
{
    Engine->Neural_Network_Training_State->Submarine->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Engine->Neural_Network_Training_State->Submarine->Models[0], "resources/models/test_plane.se3");
        Load_SE3_Texture(Engine->Neural_Network_Training_State->Submarine->Models[0], "texture_skybox.sei");

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
    Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->plane_x += Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->Forward_Vector.x * Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->Throttle;
    Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->plane_y += Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->Forward_Vector.y * Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->Throttle;
    Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->plane_z += Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->Forward_Vector.z * Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[Submarine_ID]->Throttle;
}


void Handle_Submarine_Object_Inputs(struct _Engine* Engine, struct _Keypad Keypad, int ID)
{
        if(Keypad.Keyboard_Up)
        {
            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_pitch -= Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP;
            if(Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_pitch < 0)
            {
                Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_pitch += 360;
            }
            rotate_axes(&Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Forward_Vector,  &Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Up_Vector, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Right_Vector,   -Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP);
        }

        if(Keypad.Keyboard_Down)
        {
            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_pitch += Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP;
            if(Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_pitch > 359)
            {
                Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_pitch -= 360;
            }
            rotate_axes(&Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Forward_Vector,  &Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Up_Vector, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Right_Vector,   Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP);
        }

        if(Keypad.Keyboard_Left)
        {
            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_yaw += Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP;
            if(Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_yaw > 359)
            {
                Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_yaw -= 360;
            }
            rotate_axes(&Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Up_Vector,  &Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Right_Vector, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Forward_Vector,   Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP);
        }

        if(Keypad.Keyboard_Right)
        {
            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_yaw -= Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP;
            if(Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_yaw < 0)
            {
                Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->plane_yaw += 360;
            }
            rotate_axes(&Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Up_Vector,  &Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Right_Vector, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->Forward_Vector,   -Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[ID]->ANGLE_STEP);
        }
};

void Render_Submarine_Object(struct _Engine* Engine, int Submarine_ID)
{

}





struct _Torpedo* Create_Torpedo()
{
    struct _Torpedo* Torpedo = (struct _Torpedo*) calloc(1,sizeof(struct _Torpedo));
    return Torpedo;
}

void Initialize_Torpedo(struct _Engine* Engine, float x, float y, float z, float DestX, float DestY, float DestZ, float Velocity, unsigned char Mode, float xVector, float yVector, float zVector)
{

}

void Detonate_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{

}

void Process_Torpedo(struct _Engine* Engine, int Torpedo_ID)
{

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

/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "on_foot.h"
#include <stdio.h>
#include <string.h>

void Spawn_Character(struct _On_Foot_Region* On_Foot_Region, int ID, double x, double y, double z)
{
    On_Foot_Region->Characters[ID] = Create_Player();
    Initialize_On_Foot_Player(On_Foot_Region->Characters[ID], "none",x,y,z);
}


void Submarine_Vehicle_Collision_Detection(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{

}


void Door_Collision_Detection(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{
    //FOOT COLLISION DETECTION
    for(int j = 0; j < On_Foot_Region_File->Number_Of_Doors; j++)
    {
        if(checkAABBCollision(On_Foot_Region->Doors[j]->Entrance_AABB,On_Foot_Player->Feet_Collision_Geometry,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            if(checkAABBCollision(On_Foot_Region->Doors[j]->Entrance_AABB,On_Foot_Player->Head_Collision_Geometry,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
            {
                if(!On_Foot_Region->Doors[j]->Entrance_Open)
                {
                    On_Foot_Player->displacement.x = 0;
                    On_Foot_Player->displacement.y = 0;
                    On_Foot_Player->x -= On_Foot_Player->Forward_Vector.x;
                    On_Foot_Player->y -= On_Foot_Player->Forward_Vector.y;
                }
            }
            printf("Colliding with wall!\n");
        }

        if(checkAABBCollision(On_Foot_Region->Doors[j]->Exit_AABB,On_Foot_Player->Feet_Collision_Geometry,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
        {
            if(checkAABBCollision(On_Foot_Region->Doors[j]->Exit_AABB,On_Foot_Player->Head_Collision_Geometry,On_Foot_Player->displacement.x,On_Foot_Player->displacement.y,On_Foot_Player->displacement.z))
            {
                if(!On_Foot_Region->Doors[j]->Exit_Open)
                {
                    On_Foot_Player->displacement.x = 0;
                    On_Foot_Player->displacement.y = 0;
                    On_Foot_Player->x -= On_Foot_Player->Forward_Vector.x;
                    On_Foot_Player->y -= On_Foot_Player->Forward_Vector.y;
                }
            }
        }
    }
}

void Render_Player_Subroutine(struct _Engine* Engine)
{
    //glPushMatrix();
   // glTranslatef(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
    //glRotatef(Engine->On_Foot_State->On_Foot_Player->Yaw_Angle,0,0,1);
    //Render_Player(Engine);
    //glPopMatrix();

    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Camera_X+10*(Engine->On_Foot_State->Camera_Front_X),Engine->On_Foot_State->Camera_Y+10*(Engine->On_Foot_State->Camera_Front_Y),Engine->On_Foot_State->Camera_Z+10*(Engine->On_Foot_State->Camera_Front_Z));
    glRotatef(Engine->On_Foot_State->On_Foot_Player->Yaw_Angle,0,0,1);
    glRotatef(Engine->On_Foot_State->Camera_Pitch,0,-1,0);
    Render_SE3_Model(Engine->On_Foot_State->Hand,0,0,0,0,0,0,0,false,1,1,1,false);
    Render_SE3_Model(Engine->On_Foot_State->Weapon_Objects[Engine->On_Foot_State->On_Foot_Player->Current_Weapon]->Weapon_Model,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
}


void Handle_Unpausing_Logic(struct _Engine* Engine, struct _Keypad Keypad)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //Mix_PlayChannel( -1, Engine->On_Foot_State->Pause_Confirm, 0 );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.7777,1.0f, METER_CONVERSION*10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40,40,40,0,0,20,0,0,1);
    glEnable(GL_LIGHTING);
    glfwSetInputMode(Engine->W, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Handle_Pause_Nav_Subroutine(struct _Engine* Engine, struct _Keypad Keypad)
{
    ////printf("Current Choice: %d\n", Engine->On_Foot_State->Current_Choice);
    if(Keypad.Keyboard_Down && Engine->On_Foot_State->Current_Pause_Choice < 2 && Engine->On_Foot_State->Down_Not_Pressed)
    {
         Engine->On_Foot_State->Pause_Selector_Y += 96;
         Engine->On_Foot_State->Current_Pause_Choice += 1;
         Engine->On_Foot_State->Down_Not_Pressed = false;
         //Mix_PlayChannel( -1, Engine->On_Foot_State->Pause_Select, 0 );
    }

    if(Keypad.Keyboard_Up && Engine->On_Foot_State->Current_Pause_Choice > 0 && Engine->On_Foot_State->Up_Not_Pressed)
    {
         Engine->On_Foot_State->Pause_Selector_Y -= 96;
         Engine->On_Foot_State->Current_Pause_Choice -= 1;
         Engine->On_Foot_State->Up_Not_Pressed = false;
        // Mix_PlayChannel( -1, Engine->On_Foot_State->Pause_Select, 0 );
    }

    if(Keypad.Keyboard_Start && !Engine->On_Foot_State->Pause_Button_Pressed)
    {
        if(Engine->On_Foot_State->Current_Pause_Choice == 2)
        {
            Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
            Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
            Engine->On_Foot_State->Render = false;
           // Mix_PlayChannel( -1, Engine->On_Foot_State->Pause_Confirm, 0 );
        }

        else if(Engine->On_Foot_State->Current_Pause_Choice == 1)
        {
           // Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
           // Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
           // Mix_PlayChannel( -1, Engine->On_Foot_State->Pause_Confirm, 0 );
            Engine->On_Foot_State->Pause_Button_Pressed = true;
            Engine->On_Foot_State->Current_State = STATE_UNPAUSED;
            Handle_Unpausing_Logic(Engine, Keypad);

        }
    }

    if(!Keypad.Keyboard_Up)
    {
        Engine->On_Foot_State->Up_Not_Pressed = true;
    }

    if(!Keypad.Keyboard_Down)
    {
        Engine->On_Foot_State->Down_Not_Pressed = true;
    }

}



void Handle_Pause_Rendering_Pass(struct _Engine* Engine)
{
    Render_Image(Engine->On_Foot_State->Pause_Background,0,0,0);
    if(Engine->On_Foot_State->Current_Pause_Choice == 0)
    {
        Render_Image(Engine->On_Foot_State->Pause_Inventory_Highlighted,176,32+16,0);
    }
    if(Engine->On_Foot_State->Current_Pause_Choice == 1 || Engine->On_Foot_State->Current_Pause_Choice == 2)
    {
        Render_Image(Engine->On_Foot_State->Pause_Inventory,176,32+16,0);
    }

    if(Engine->On_Foot_State->Current_Pause_Choice == 1)
    {
        Render_Image(Engine->On_Foot_State->Pause_Resume_Highlighted,176,128+16,1);
    }
    if(Engine->On_Foot_State->Current_Pause_Choice == 0 || Engine->On_Foot_State->Current_Pause_Choice == 2)
    {
        Render_Image(Engine->On_Foot_State->Pause_Resume,176,128+16,1);
    }

    if(Engine->On_Foot_State->Current_Pause_Choice == 2)
    {
        Render_Image(Engine->On_Foot_State->Pause_Exit_Highlighted,176,224+16,1);
    }
    if(Engine->On_Foot_State->Current_Pause_Choice == 0 || Engine->On_Foot_State->Current_Pause_Choice == 1)
    {
        Render_Image(Engine->On_Foot_State->Pause_Exit,176,224+16,1);
    }
}


void Write_Ship(const char* Filename)
{

    struct _On_Foot_Region_File OFR;
    memset(OFR.Name,0,256);
    sprintf(OFR.Name, "TEST REGION!");

    OFR.Number_Of_Vertical_Collision_Geometries = 1;
    OFR.Number_Of_Horizontal_Collision_Geometries = 1;
    OFR.Number_Of_Slope_Collision_Geometries = 1;
    OFR.Number_Of_Terrains = 2;
    OFR.Number_Of_Doors = 1;
    OFR.Number_Of_Ships = 0;
    OFR.Number_Of_Viewscreens = 0;
    OFR.Number_Of_Characters = 0;
    OFR.Number_Of_Goalposts = 1;
    OFR.Number_Of_Vehicles = 0;
    OFR.Number_Of_Seats = 3;

    int Size_Of_Vertical_Collision_Geometries = OFR.Number_Of_Vertical_Collision_Geometries * sizeof(struct _Vertical_Collision_Geometry);
    int Size_Of_Horizontal_Collision_Geometries = OFR.Number_Of_Horizontal_Collision_Geometries * sizeof(struct _Horizontal_Collision_Geometry);
    int Size_Of_Slope_Collision_Geometries = OFR.Number_Of_Slope_Collision_Geometries * sizeof(struct _Slope_Collision_Geometry);
    int Size_Of_Terrains = OFR.Number_Of_Terrains * sizeof(struct _Terrain);
    int Size_Of_Doors = OFR.Number_Of_Doors * sizeof(struct _Door);
    int Size_Of_Ships = OFR.Number_Of_Ships * sizeof(struct _Ship);
    int Size_Of_Viewscreens = OFR.Number_Of_Viewscreens * sizeof(struct _Viewscreen);
    int Size_Of_Characters = OFR.Number_Of_Characters * sizeof(struct _Character);
    int Size_Of_Goalposts = OFR.Number_Of_Goalposts * sizeof(struct _Goalpost);
    int Size_Of_Vehicles = OFR.Number_Of_Vehicles * sizeof(struct _Vehicle);
    int Size_Of_Seats = OFR.Number_Of_Seats * sizeof(struct _Seat);

    OFR.Offset_To_Vertical_Collision_Geometries = sizeof(struct _On_Foot_Region_File);
    OFR.Offset_To_Horizontal_Collision_Geometries = OFR.Offset_To_Vertical_Collision_Geometries + Size_Of_Vertical_Collision_Geometries;
    OFR.Offset_To_Slope_Collision_Geometries = OFR.Offset_To_Horizontal_Collision_Geometries + Size_Of_Horizontal_Collision_Geometries;
    OFR.Offset_To_Terrains = OFR.Offset_To_Slope_Collision_Geometries + Size_Of_Slope_Collision_Geometries;
    OFR.Offset_To_Doors = OFR.Offset_To_Terrains + Size_Of_Terrains;
    OFR.Offset_To_Ships = OFR.Offset_To_Doors + Size_Of_Doors;
    OFR.Offset_To_Viewscreens = OFR.Offset_To_Ships + Size_Of_Ships;
    OFR.Offset_To_Characters = OFR.Offset_To_Viewscreens + Size_Of_Viewscreens;
    OFR.Offset_To_Goalposts = OFR.Offset_To_Characters + Size_Of_Characters;
    OFR.Offset_To_Vehicles = OFR.Offset_To_Goalposts + Size_Of_Goalposts;
    OFR.Offset_To_Seats = OFR.Offset_To_Vehicles + Size_Of_Vehicles;

    struct _Seat S;
    memset(S.Model,0,256);
    memset(S.Texture,0,256);

    sprintf(S.Model, "resources/data/seats/seat.se3");
    sprintf(S.Texture, "resources/data/seats/seat.sei");

    S.x = 0;
    S.y = 0;
    S.z = 0;

    S.x1 = 0;
    S.y1 = 0;
    S.z1 = 0;

    S.x2 = 0;
    S.y2 = 0;
    S.z2 = 0;

    S.Eye_Offset_x = 0;
    S.Eye_Offset_y = 0;
    S.Eye_Offset_z = 0;

    S.Is_A_Drivers_Seat = 0;
    S.Seat_Contains_Console = 0;

    S.Red_Color_Balance = 0;
    S.Green_Color_Balance = 0;
    S.Blue_Color_Balance = 0;

    S.Color_Mode = 0;

    S.Power_State = 1;



    S.Screen_y3=6.24936;
    S.Screen_x3=-20.31796;
    S.Screen_z3=27.15369;

    S.Screen_y2=6.24936;
    S.Screen_x2=-22.14693;
    S.Screen_z2=39.65241;

    S.Screen_y1=-6.24936;
    S.Screen_x1=-22.14693;
    S.Screen_z1=39.65241;

    S.Screen_y4=-6.24936;
    S.Screen_x4=-20.31796;
    S.Screen_z4=27.15369;




    struct _Seat Se;
    memset(Se.Model,0,256);
    memset(Se.Texture,0,256);

    sprintf(Se.Model, "resources/data/seats/seat.se3");
    sprintf(Se.Texture, "resources/data/seats/seat.sei");

    Se.x = 0;
    Se.y = 60;
    Se.z = 0;

    Se.x1 = 0;
    Se.y1 = 0;
    Se.z1 = 0;

    Se.x2 = 0;
    Se.y2 = 0;
    Se.z2 = 0;

    Se.Eye_Offset_x = 0;
    Se.Eye_Offset_y = 0;
    Se.Eye_Offset_z = 0;

    Se.Is_A_Drivers_Seat = 0;
    Se.Seat_Contains_Console = 0;

    Se.Red_Color_Balance = 0;
    Se.Green_Color_Balance = 0;
    Se.Blue_Color_Balance = 0;

    Se.Color_Mode = 0;

    Se.Power_State = 1;



    Se.Screen_y3=6.24936;
    Se.Screen_x3=-20.31796;
    Se.Screen_z3=27.15369;

    Se.Screen_y2=6.24936;
    Se.Screen_x2=-22.14693;
    Se.Screen_z2=39.65241;

    Se.Screen_y1=-6.24936;
    Se.Screen_x1=-22.14693;
    Se.Screen_z1=39.65241;

    Se.Screen_y4=-6.24936;
    Se.Screen_x4=-20.31796;
    Se.Screen_z4=27.15369;

    struct _Seat Sea;
    memset(Sea.Model,0,256);
    memset(Sea.Texture,0,256);

    sprintf(Sea.Model, "resources/data/seats/seat.se3");
    sprintf(Sea.Texture, "resources/data/seats/seat.sei");

    Sea.x = 0;
    Sea.y = 120;
    Sea.z = 0;

    Sea.x1 = 0;
    Sea.y1 = 0;
    Sea.z1 = 0;

    Sea.x2 = 0;
    Sea.y2 = 0;
    Sea.z2 = 0;

    Sea.Eye_Offset_x = 0;
    Sea.Eye_Offset_y = 0;
    Sea.Eye_Offset_z = 0;

    Sea.Is_A_Drivers_Seat = 0;
    Sea.Seat_Contains_Console = 0;

    Sea.Red_Color_Balance = 0;
    Sea.Green_Color_Balance = 0;
    Sea.Blue_Color_Balance = 0;

    Sea.Color_Mode = 0;

    Sea.Power_State = 1;



    Sea.Screen_y3=6.24936;
    Sea.Screen_x3=-20.31796;
    Sea.Screen_z3=27.15369;

    Sea.Screen_y2=6.24936;
    Sea.Screen_x2=-22.14693;
    Sea.Screen_z2=39.65241;

    Sea.Screen_y1=-6.24936;
    Sea.Screen_x1=-22.14693;
    Sea.Screen_z1=39.65241;

    Sea.Screen_y4=-6.24936;
    Sea.Screen_x4=-20.31796;
    Sea.Screen_z4=27.15369;


    struct _Vehicle V;

    memset(V.Model,0,256);
    memset(V.Texture,0,256);

    sprintf(V.Model, "resources/data/submarines/car.se3");
    sprintf(V.Texture, "resources/data/submarines/car.sei");

    V.x = 20;
    V.y = 0;
    V.z = 0;
    V.Roll = 0;
    V.Yaw = 0;
    V.Pitch = 0;
    V.Damage = 0;
    V.Entrance_Radius = 1;
    V.Is_A_Proxy = false;
    V.Proxy_ID = -1;
    V.Maximum_Passengers = 1;
    V.Number_Of_Passengers = 0;

    for(int m = 0; m < 256; m++)
    {
        for(int n = 0; n < 256; n++)
        {
                V.Passenger_IDs[m].Name[n] = 0;
        }
        V.Passenger_IDs[m].Category = 0;
        V.Passenger_IDs[m].Subcategory = 0;
        V.Passenger_IDs[m].Variant = 0;
        V.Passenger_IDs[m].ID = 0;
    }





    struct _Vehicle Ve;

    memset(Ve.Model,0,256);
    memset(Ve.Texture,0,256);

    sprintf(Ve.Model, "resources/data/submarines/car.se3");
    sprintf(Ve.Texture, "resources/data/submarines/car.sei");

    Ve.x = 40;
    Ve.y = 0;
    Ve.z = 0;
    Ve.Roll = 0;
    Ve.Yaw = 0;
    Ve.Pitch = 0;
    Ve.Damage = 0;
    Ve.Entrance_Radius = 1;
    Ve.Is_A_Proxy = false;
    Ve.Proxy_ID = 0;
    Ve.Maximum_Passengers = 1;
    Ve.Number_Of_Passengers = 0;

    for(int m = 0; m < 256; m++)
    {
        for(int n = 0; n < 256; n++)
        {
                Ve.Passenger_IDs[m].Name[n] = 0;
        }
        Ve.Passenger_IDs[m].Category = 0;
        Ve.Passenger_IDs[m].Subcategory = 0;
        Ve.Passenger_IDs[m].Variant = 0;
        Ve.Passenger_IDs[m].ID = 0;
    }


    struct _Vertical_Collision_Geometry VCG;
    struct _Horizontal_Collision_Geometry HCG;
    struct _Slope_Collision_Geometry SCG;
    struct _Terrain Terrain;
    struct _Door Door;
    struct _Ship Ship;
    struct _Character C;
    struct _Goalpost Goal;

    memset(Goal.Model,0,256);
    memset(Goal.Texture,0,256);

    sprintf(Goal.Model, "resources/models/ray_drone.se3");
    sprintf(Goal.Texture, "resources/models/ray_drone.sei");

    Goal.x = 150;
    Goal.y = 0;
    Goal.z = 0;

    Goal.Radius = 40;
    Goal.Clear_Category = DIFFICULTY_MEDIUM;

    memset(C.Filepath,0,256);
    sprintf(C.Filepath,"testship");
    C.x = 30;
    C.y = 30;
    C.z = 30;


    memset(Ship.Filepath,0,256);
    sprintf(Ship.Filepath,"testship");

    memset(VCG.Filepath,0,256);
    memset(HCG.Filepath,0,256);
    memset(SCG.Filepath,0,256);
    memset(Terrain.Model,0,256);
    memset(Terrain.Texture,0,256);

    memset(Door.Model,0,256);
    memset(Door.Texture,0,256);
    memset(Door.Ceiling_Geometry_File,0,256);
    memset(Door.Wall_Geometry_File,0,256);

    Door.x=0;
    Door.y=0;
    Door.z=0;

    Door.Axis_Orientation = 0;

    sprintf(Door.Model, "resources/models/door.se3");
    sprintf(Door.Texture, "rosemary.sei");

    sprintf(Door.Ceiling_Geometry_File, "none");
    sprintf(Door.Wall_Geometry_File, "none");

    Door.Use_Custom_Collision_Geometry = false;

    Door.Entrance_AABB.min.x = -39.44;
    Door.Entrance_AABB.min.y = -68.10824;
    Door.Entrance_AABB.min.z = 0.32830;

    Door.Entrance_AABB.max.x = 39.44;
    Door.Entrance_AABB.max.y = -65.35680;
    Door.Entrance_AABB.max.z = 79.20830;


    Door.Exit_AABB.min.x = -39.44;
    Door.Exit_AABB.min.y = 65.33680;
    Door.Exit_AABB.min.z = 0.32830;

    Door.Exit_AABB.max.x = 39.44;
    Door.Exit_AABB.max.y = 68.10824;
    Door.Exit_AABB.max.z = 79.20830;


    Door.Entrance_Open = false;
    Door.Exit_Open = false;

    Door.Number_Of_Frames = 10;
    Door.Current_Frame = 0;

    Door.Teleport_Destination_X = 0;
    Door.Teleport_Destination_Y = 0;
    Door.Teleport_Destination_Z = 6000;

    Door.Connected_To_Type = 1;
    Door.Connection_Destination_Door = 0;




    sprintf(VCG.Filepath,"resources/models/vtestgeometry.se3");
    sprintf(HCG.Filepath,"resources/models/htestgeometry.se3");
    sprintf(SCG.Filepath,"resources/models/stestgeometry.se3");
    sprintf(Terrain.Model,"resources/models/testgeometry.se3");
    sprintf(Terrain.Texture,"rosemary.sei");
    Terrain.Visible = true;
    Terrain.x = 0;
    Terrain.y = 0;
    Terrain.z = 0;

    VCG.x = 0;
    HCG.x = 0;
    SCG.x = 0;

    VCG.y = 0;
    HCG.y = 0;
    SCG.y = 0;

    VCG.z = 0;
    HCG.z = 0;
    SCG.z = 0;


    FILE* f = fopen(Filename, "wb");
    fwrite(&OFR, 1, sizeof(struct _On_Foot_Region_File),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Vertical_Collision_Geometries, SEEK_SET);
    fwrite(&VCG, 1, sizeof(struct _Vertical_Collision_Geometry),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Horizontal_Collision_Geometries, SEEK_SET);
    fwrite(&HCG, 1, sizeof(struct _Horizontal_Collision_Geometry),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Slope_Collision_Geometries, SEEK_SET);
    fwrite(&SCG, 1, sizeof(struct _Slope_Collision_Geometry),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Terrains, SEEK_SET);
    fwrite(&Terrain, 1, sizeof(struct _Terrain),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Goalposts, SEEK_SET);
    fwrite(&Goal, 1, sizeof(struct _Goalpost),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Doors, SEEK_SET);
    fwrite(&Door, 1, sizeof(struct _Door),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Seats, SEEK_SET);
    fwrite(&S, 1, sizeof(struct _Seat),f);
    fwrite(&Se, 1, sizeof(struct _Seat),f);
    fwrite(&Sea, 1, sizeof(struct _Seat),f);
    fclose(f);
}


void Write_On_Foot_Region(const char* Filename)
{
    struct _On_Foot_Region_File OFR;
    memset(OFR.Name,0,256);
    sprintf(OFR.Name, "TEST REGION!");

    OFR.Number_Of_Vertical_Collision_Geometries = 1;
    OFR.Number_Of_Horizontal_Collision_Geometries = 1;
    OFR.Number_Of_Slope_Collision_Geometries = 1;
    OFR.Number_Of_Terrains = 2;
    OFR.Number_Of_Doors = 1;
    OFR.Number_Of_Ships = 0;
    OFR.Number_Of_Viewscreens = 0;
    OFR.Number_Of_Characters = 12;
    OFR.Number_Of_Goalposts = 1;
    OFR.Number_Of_Vehicles = 2;
    OFR.Number_Of_Seats = 3;

    int Size_Of_Vertical_Collision_Geometries = OFR.Number_Of_Vertical_Collision_Geometries * sizeof(struct _Vertical_Collision_Geometry);
    int Size_Of_Horizontal_Collision_Geometries = OFR.Number_Of_Horizontal_Collision_Geometries * sizeof(struct _Horizontal_Collision_Geometry);
    int Size_Of_Slope_Collision_Geometries = OFR.Number_Of_Slope_Collision_Geometries * sizeof(struct _Slope_Collision_Geometry);
    int Size_Of_Terrains = OFR.Number_Of_Terrains * sizeof(struct _Terrain);
    int Size_Of_Doors = OFR.Number_Of_Doors * sizeof(struct _Door);
    int Size_Of_Ships = OFR.Number_Of_Ships * sizeof(struct _Ship);
    int Size_Of_Viewscreens = OFR.Number_Of_Viewscreens * sizeof(struct _Viewscreen);
    int Size_Of_Characters = OFR.Number_Of_Characters * sizeof(struct _Character);
    int Size_Of_Goalposts = OFR.Number_Of_Goalposts * sizeof(struct _Goalpost);
    int Size_Of_Vehicles = OFR.Number_Of_Vehicles * sizeof(struct _Vehicle);
    int Size_Of_Seats = OFR.Number_Of_Seats * sizeof(struct _Seat);

    OFR.Offset_To_Vertical_Collision_Geometries = sizeof(struct _On_Foot_Region_File);
    OFR.Offset_To_Horizontal_Collision_Geometries = OFR.Offset_To_Vertical_Collision_Geometries + Size_Of_Vertical_Collision_Geometries;
    OFR.Offset_To_Slope_Collision_Geometries = OFR.Offset_To_Horizontal_Collision_Geometries + Size_Of_Horizontal_Collision_Geometries;
    OFR.Offset_To_Terrains = OFR.Offset_To_Slope_Collision_Geometries + Size_Of_Slope_Collision_Geometries;
    OFR.Offset_To_Doors = OFR.Offset_To_Terrains + Size_Of_Terrains;
    OFR.Offset_To_Ships = OFR.Offset_To_Doors + Size_Of_Doors;
    OFR.Offset_To_Viewscreens = OFR.Offset_To_Ships + Size_Of_Ships;
    OFR.Offset_To_Characters = OFR.Offset_To_Viewscreens + Size_Of_Viewscreens;
    OFR.Offset_To_Goalposts = OFR.Offset_To_Characters + Size_Of_Characters;
    OFR.Offset_To_Vehicles = OFR.Offset_To_Goalposts + Size_Of_Goalposts;
    OFR.Offset_To_Seats = OFR.Offset_To_Vehicles + Size_Of_Vehicles;

    struct _Seat S;
    memset(S.Model,0,256);
    memset(S.Texture,0,256);

    sprintf(S.Model, "resources/data/seats/seat.se3");
    sprintf(S.Texture, "resources/data/seats/seat.sei");

    S.x = 0;
    S.y = 0;
    S.z = 0;

    S.x1 = 0;
    S.y1 = 0;
    S.z1 = 0;

    S.x2 = 0;
    S.y2 = 0;
    S.z2 = 0;

    S.Eye_Offset_x = 0;
    S.Eye_Offset_y = 0;
    S.Eye_Offset_z = 0;

    S.Is_A_Drivers_Seat = 0;
    S.Seat_Contains_Console = 0;

    S.Red_Color_Balance = 0;
    S.Green_Color_Balance = 0;
    S.Blue_Color_Balance = 0;

    S.Color_Mode = 0;

    S.Power_State = 1;



    S.Screen_y3=6.24936;
    S.Screen_x3=-20.31796;
    S.Screen_z3=27.15369;

    S.Screen_y2=6.24936;
    S.Screen_x2=-22.14693;
    S.Screen_z2=39.65241;

    S.Screen_y1=-6.24936;
    S.Screen_x1=-22.14693;
    S.Screen_z1=39.65241;

    S.Screen_y4=-6.24936;
    S.Screen_x4=-20.31796;
    S.Screen_z4=27.15369;




    struct _Seat Se;
    memset(Se.Model,0,256);
    memset(Se.Texture,0,256);

    sprintf(Se.Model, "resources/data/seats/seat.se3");
    sprintf(Se.Texture, "resources/data/seats/seat.sei");

    Se.x = 0;
    Se.y = 60;
    Se.z = 0;

    Se.x1 = 0;
    Se.y1 = 0;
    Se.z1 = 0;

    Se.x2 = 0;
    Se.y2 = 0;
    Se.z2 = 0;

    Se.Eye_Offset_x = 0;
    Se.Eye_Offset_y = 0;
    Se.Eye_Offset_z = 0;

    Se.Is_A_Drivers_Seat = 0;
    Se.Seat_Contains_Console = 0;

    Se.Red_Color_Balance = 0;
    Se.Green_Color_Balance = 0;
    Se.Blue_Color_Balance = 0;

    Se.Color_Mode = 0;

    Se.Power_State = 1;



    Se.Screen_y3=6.24936;
    Se.Screen_x3=-20.31796;
    Se.Screen_z3=27.15369;

    Se.Screen_y2=6.24936;
    Se.Screen_x2=-22.14693;
    Se.Screen_z2=39.65241;

    Se.Screen_y1=-6.24936;
    Se.Screen_x1=-22.14693;
    Se.Screen_z1=39.65241;

    Se.Screen_y4=-6.24936;
    Se.Screen_x4=-20.31796;
    Se.Screen_z4=27.15369;

    struct _Seat Sea;
    memset(Sea.Model,0,256);
    memset(Sea.Texture,0,256);

    sprintf(Sea.Model, "resources/data/seats/seat.se3");
    sprintf(Sea.Texture, "resources/data/seats/seat.sei");

    Sea.x = 0;
    Sea.y = 120;
    Sea.z = 0;

    Sea.x1 = 0;
    Sea.y1 = 0;
    Sea.z1 = 0;

    Sea.x2 = 0;
    Sea.y2 = 0;
    Sea.z2 = 0;

    Sea.Eye_Offset_x = 0;
    Sea.Eye_Offset_y = 0;
    Sea.Eye_Offset_z = 0;

    Sea.Is_A_Drivers_Seat = 0;
    Sea.Seat_Contains_Console = 0;

    Sea.Red_Color_Balance = 0;
    Sea.Green_Color_Balance = 0;
    Sea.Blue_Color_Balance = 0;

    Sea.Color_Mode = 0;

    Sea.Power_State = 1;



    Sea.Screen_y3=6.24936;
    Sea.Screen_x3=-20.31796;
    Sea.Screen_z3=27.15369;

    Sea.Screen_y2=6.24936;
    Sea.Screen_x2=-22.14693;
    Sea.Screen_z2=39.65241;

    Sea.Screen_y1=-6.24936;
    Sea.Screen_x1=-22.14693;
    Sea.Screen_z1=39.65241;

    Sea.Screen_y4=-6.24936;
    Sea.Screen_x4=-20.31796;
    Sea.Screen_z4=27.15369;


    struct _Vehicle V;

    memset(V.Model,0,256);
    memset(V.Texture,0,256);

    sprintf(V.Model, "resources/data/submarines/car.se3");
    sprintf(V.Texture, "resources/data/submarines/car.sei");

    V.x = 0;
    V.y = 1000;
    V.z = 0;
    V.Roll = 0;
    V.Yaw = 0;
    V.Pitch = 0;
    V.Damage = 0;
    V.Entrance_Radius = 1;
    V.Is_A_Proxy = false;
    V.Proxy_ID = -1;
    V.Maximum_Passengers = 1;
    V.Number_Of_Passengers = 0;

    for(int m = 0; m < 256; m++)
    {
        for(int n = 0; n < 256; n++)
        {
                V.Passenger_IDs[m].Name[n] = 0;
        }
        V.Passenger_IDs[m].Category = 0;
        V.Passenger_IDs[m].Subcategory = 0;
        V.Passenger_IDs[m].Variant = 0;
        V.Passenger_IDs[m].ID = 0;
    }





    struct _Vehicle Ve;

    memset(Ve.Model,0,256);
    memset(Ve.Texture,0,256);

    sprintf(Ve.Model, "resources/data/submarines/car.se3");
    sprintf(Ve.Texture, "resources/data/submarines/car.sei");

    Ve.x = 400;
    Ve.y = 0;
    Ve.z = 0;
    Ve.Roll = 0;
    Ve.Yaw = 0;
    Ve.Pitch = 0;
    Ve.Damage = 0;
    Ve.Entrance_Radius = 1;
    Ve.Is_A_Proxy = false;
    Ve.Proxy_ID = 0;
    Ve.Maximum_Passengers = 1;
    Ve.Number_Of_Passengers = 0;

    for(int m = 0; m < 256; m++)
    {
        for(int n = 0; n < 256; n++)
        {
                Ve.Passenger_IDs[m].Name[n] = 0;
        }
        Ve.Passenger_IDs[m].Category = 0;
        Ve.Passenger_IDs[m].Subcategory = 0;
        Ve.Passenger_IDs[m].Variant = 0;
        Ve.Passenger_IDs[m].ID = 0;
    }


    struct _Vertical_Collision_Geometry VCG;
    struct _Horizontal_Collision_Geometry HCG;
    struct _Slope_Collision_Geometry SCG;
    struct _Terrain Terrain;
    struct _Door Door;
    struct _Ship Ship;
    struct _Character C;
    struct _Goalpost Goal;

    memset(Goal.Model,0,256);
    memset(Goal.Texture,0,256);

    sprintf(Goal.Model, "resources/models/ray_drone.se3");
    sprintf(Goal.Texture, "resources/models/ray_drone.sei");

    Goal.x = 150;
    Goal.y = 0;
    Goal.z = 0;

    Goal.Radius = 40;
    Goal.Clear_Category = DIFFICULTY_MEDIUM;

    memset(C.Filepath,0,256);
    sprintf(C.Filepath,"testship");
    C.x = 30;
    C.y = 30;
    C.z = 30;


    memset(Ship.Filepath,0,256);
    sprintf(Ship.Filepath,"testship");

    memset(VCG.Filepath,0,256);
    memset(HCG.Filepath,0,256);
    memset(SCG.Filepath,0,256);
    memset(Terrain.Model,0,256);
    memset(Terrain.Texture,0,256);

    memset(Door.Model,0,256);
    memset(Door.Texture,0,256);
    memset(Door.Ceiling_Geometry_File,0,256);
    memset(Door.Wall_Geometry_File,0,256);

    Door.x=0;
    Door.y=0;
    Door.z=0;

    Door.Axis_Orientation = 0;

    sprintf(Door.Model, "resources/models/door.se3");
    sprintf(Door.Texture, "rosemary.sei");

    sprintf(Door.Ceiling_Geometry_File, "none");
    sprintf(Door.Wall_Geometry_File, "none");

    Door.Use_Custom_Collision_Geometry = false;

    Door.Entrance_AABB.min.x = -39.44;
    Door.Entrance_AABB.min.y = -68.10824;
    Door.Entrance_AABB.min.z = 0.32830;

    Door.Entrance_AABB.max.x = 39.44;
    Door.Entrance_AABB.max.y = -65.35680;
    Door.Entrance_AABB.max.z = 79.20830;


    Door.Exit_AABB.min.x = -39.44;
    Door.Exit_AABB.min.y = 65.33680;
    Door.Exit_AABB.min.z = 0.32830;

    Door.Exit_AABB.max.x = 39.44;
    Door.Exit_AABB.max.y = 68.10824;
    Door.Exit_AABB.max.z = 79.20830;


    Door.Entrance_Open = false;
    Door.Exit_Open = false;

    Door.Number_Of_Frames = 10;
    Door.Current_Frame = 0;

    Door.Teleport_Destination_X = 0;
    Door.Teleport_Destination_Y = 0;
    Door.Teleport_Destination_Z = 6000;

    Door.Connected_To_Type = 1;
    Door.Connection_Destination_Door = 0;




    sprintf(VCG.Filepath,"resources/models/vtestgeometry.se3");
    sprintf(HCG.Filepath,"resources/models/htestgeometry.se3");
    sprintf(SCG.Filepath,"resources/models/stestgeometry.se3");
    sprintf(Terrain.Model,"resources/models/testgeometry.se3");
    sprintf(Terrain.Texture,"rosemary.sei");
    Terrain.Visible = true;
    Terrain.x = 0;
    Terrain.y = 0;
    Terrain.z = 0;

    VCG.x = 0;
    HCG.x = 0;
    SCG.x = 0;

    VCG.y = 0;
    HCG.y = 0;
    SCG.y = 0;

    VCG.z = 0;
    HCG.z = 0;
    SCG.z = 0;


    FILE* f = fopen(Filename, "wb");
    fwrite(&OFR, 1, sizeof(struct _On_Foot_Region_File),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Vertical_Collision_Geometries, SEEK_SET);
    fwrite(&VCG, 1, sizeof(struct _Vertical_Collision_Geometry),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Horizontal_Collision_Geometries, SEEK_SET);
    fwrite(&HCG, 1, sizeof(struct _Horizontal_Collision_Geometry),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Slope_Collision_Geometries, SEEK_SET);
    fwrite(&SCG, 1, sizeof(struct _Slope_Collision_Geometry),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Terrains, SEEK_SET);
    fwrite(&Terrain, 1, sizeof(struct _Terrain),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Ships, SEEK_SET);
    //fwrite(&Ship, 1, sizeof(struct _Ship),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Goalposts, SEEK_SET);
    fwrite(&Goal, 1, sizeof(struct _Goalpost),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Characters, SEEK_SET);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    fwrite(&C, 1, sizeof(struct _Character),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Doors, SEEK_SET);
    fwrite(&Door, 1, sizeof(struct _Door),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Vehicles, SEEK_SET);
    fwrite(&V, 1, sizeof(struct _Vehicle),f);
    fwrite(&Ve, 1, sizeof(struct _Vehicle),f);
    rewind(f);

    fseek(f, OFR.Offset_To_Seats, SEEK_SET);
    fwrite(&S, 1, sizeof(struct _Seat),f);
    fwrite(&Se, 1, sizeof(struct _Seat),f);
    fwrite(&Sea, 1, sizeof(struct _Seat),f);
    fclose(f);
}

void Load_On_Foot_Region(struct _Engine* Engine, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File, const char* Filename, double Z_Offset)
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
        On_Foot_Region_File->Name[j] = Buffer[Buffer_Location+j];
    }
    Buffer_Location += 256;

    On_Foot_Region_File->Number_Of_Vertical_Collision_Geometries = *(int*)&(Buffer[Buffer_Location]);
    On_Foot_Region_File->Number_Of_Horizontal_Collision_Geometries = *(int*)&(Buffer[Buffer_Location+4]);
    On_Foot_Region_File->Number_Of_Slope_Collision_Geometries = *(int*)&(Buffer[Buffer_Location+8]);
    On_Foot_Region_File->Number_Of_Terrains = *(int*)&(Buffer[Buffer_Location+12]);
    On_Foot_Region_File->Number_Of_Doors = *(int*)&(Buffer[Buffer_Location+16]);
    On_Foot_Region_File->Number_Of_Ships = *(int*)&(Buffer[Buffer_Location+20]);
    On_Foot_Region_File->Number_Of_Viewscreens = *(int*)&(Buffer[Buffer_Location+24]);
    On_Foot_Region_File->Number_Of_Characters = *(int*)&(Buffer[Buffer_Location+28]);
    On_Foot_Region_File->Number_Of_Goalposts = *(int*)&(Buffer[Buffer_Location+32]);
    On_Foot_Region_File->Number_Of_Vehicles = *(int*)&(Buffer[Buffer_Location+36]);
    On_Foot_Region_File->Number_Of_Seats = *(int*)&(Buffer[Buffer_Location+40]);

    On_Foot_Region_File->Offset_To_Vertical_Collision_Geometries = *(int*)&(Buffer[Buffer_Location+44]);
    On_Foot_Region_File->Offset_To_Horizontal_Collision_Geometries = *(int*)&(Buffer[Buffer_Location+48]);
    On_Foot_Region_File->Offset_To_Slope_Collision_Geometries = *(int*)&(Buffer[Buffer_Location+52]);
    On_Foot_Region_File->Offset_To_Terrains = *(int*)&(Buffer[Buffer_Location+56]);
    On_Foot_Region_File->Offset_To_Doors = *(int*)&(Buffer[Buffer_Location+60]);
    On_Foot_Region_File->Offset_To_Ships = *(int*)&(Buffer[Buffer_Location+64]);
    On_Foot_Region_File->Offset_To_Viewscreens = *(int*)&(Buffer[Buffer_Location+68]);
    On_Foot_Region_File->Offset_To_Characters = *(int*)&(Buffer[Buffer_Location+72]);
    On_Foot_Region_File->Offset_To_Goalposts = *(int*)&(Buffer[Buffer_Location+76]);
    On_Foot_Region_File->Offset_To_Vehicles = *(int*)&(Buffer[Buffer_Location+80]);
    On_Foot_Region_File->Offset_To_Seats = *(int*)&(Buffer[Buffer_Location+84]);


    for(int j = 0; j <On_Foot_Region_File->Number_Of_Seats; j++)
    {
        On_Foot_Region->Seats[j] = (struct _Seat_Object*)calloc(1,sizeof(struct _Seat_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Seats + (j * sizeof(struct _Seat));
        char Mesh_Filename[256];
        char Texture_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            Texture_Filename[x] = Buffer[Buffer_Location+x+256];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 512;

        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);

        double x1 = *(double*)&(Buffer[Buffer_Location+24]);
        double y1 = *(double*)&(Buffer[Buffer_Location+32]);
        double z1 = *(double*)&(Buffer[Buffer_Location+40]);

        double x2 = *(double*)&(Buffer[Buffer_Location+48]);
        double y2 = *(double*)&(Buffer[Buffer_Location+56]);
        double z2 = *(double*)&(Buffer[Buffer_Location+64]);

        double Eye_Offset_x = *(double*)&(Buffer[Buffer_Location+72]);
        double Eye_Offset_y = *(double*)&(Buffer[Buffer_Location+80]);
        double Eye_Offset_z = *(double*)&(Buffer[Buffer_Location+88]);

        Buffer_Location += 8 * 12;

        int Is_A_Drivers_Seat = *(int*)&(Buffer[Buffer_Location]);
        int Seat_Contains_Console = *(int*)&(Buffer[Buffer_Location+4]);

        Buffer_Location += 8;

        double Screen_x1 = *(double*)&(Buffer[Buffer_Location]);
        double Screen_y1 = *(double*)&(Buffer[Buffer_Location+8]);
        double Screen_z1 = *(double*)&(Buffer[Buffer_Location+16]);

        double Screen_x2 = *(double*)&(Buffer[Buffer_Location+24]);
        double Screen_y2 = *(double*)&(Buffer[Buffer_Location+32]);
        double Screen_z2 = *(double*)&(Buffer[Buffer_Location+40]);

        double Screen_x3 = *(double*)&(Buffer[Buffer_Location+48]);
        double Screen_y3 = *(double*)&(Buffer[Buffer_Location+56]);
        double Screen_z3 = *(double*)&(Buffer[Buffer_Location+64]);

        double Screen_x4 = *(double*)&(Buffer[Buffer_Location+72]);
        double Screen_y4 = *(double*)&(Buffer[Buffer_Location+80]);
        double Screen_z4 = *(double*)&(Buffer[Buffer_Location+88]);

        Buffer_Location += 8 * 12;

        unsigned char Red_Color_Balance = *(unsigned char*)&(Buffer[Buffer_Location]);
        unsigned char Green_Color_Balance = *(unsigned char*)&(Buffer[Buffer_Location+1]);
        unsigned char Blue_Color_Balance = *(unsigned char*)&(Buffer[Buffer_Location+2]);
        bool Color_Mode = *(bool*)&(Buffer[Buffer_Location+3]);
        bool Power_State = *(bool*)&(Buffer[Buffer_Location+4]);



        On_Foot_Region->Seats[j]->Model = Create_SE3_Model();
        Load_SE3_Model(On_Foot_Region->Seats[j]->Model, Mesh_Filename);
        Load_SE3_Texture(On_Foot_Region->Seats[j]->Model, Texture_Filename);

        On_Foot_Region->Seats[j]->x = x;
        On_Foot_Region->Seats[j]->y = y;
        On_Foot_Region->Seats[j]->z = z+Z_Offset;
        On_Foot_Region->Seats[j]->Collision_Box.min.x = x1 + x;
        On_Foot_Region->Seats[j]->Collision_Box.min.y = y1 + y;
        On_Foot_Region->Seats[j]->Collision_Box.min.z = z1 + z+Z_Offset;

        On_Foot_Region->Seats[j]->Collision_Box.max.x = x2 + x;
        On_Foot_Region->Seats[j]->Collision_Box.max.y = y2 + y;
        On_Foot_Region->Seats[j]->Collision_Box.max.z = z2 + z+Z_Offset;

        On_Foot_Region->Seats[j]->Eye_Offset_x = Eye_Offset_x + x;
        On_Foot_Region->Seats[j]->Eye_Offset_y = Eye_Offset_y + y;
        On_Foot_Region->Seats[j]->Eye_Offset_z = Eye_Offset_z + z+Z_Offset;

        On_Foot_Region->Seats[j]->Seat_Contains_Console = Seat_Contains_Console;
        On_Foot_Region->Seats[j]->Is_A_Drivers_Seat = Is_A_Drivers_Seat;


        On_Foot_Region->Seats[j]->Screen_x1 = Screen_x1;
        On_Foot_Region->Seats[j]->Screen_y1 = Screen_y1;
        On_Foot_Region->Seats[j]->Screen_z1 = Screen_z1+Z_Offset;

        On_Foot_Region->Seats[j]->Screen_x2 = Screen_x2;
        On_Foot_Region->Seats[j]->Screen_y2 = Screen_y2;
        On_Foot_Region->Seats[j]->Screen_z2 = Screen_z2+Z_Offset;

        On_Foot_Region->Seats[j]->Screen_x3 = Screen_x3;
        On_Foot_Region->Seats[j]->Screen_y3 = Screen_y3;
        On_Foot_Region->Seats[j]->Screen_z3 = Screen_z3+Z_Offset;

        On_Foot_Region->Seats[j]->Screen_x4 = Screen_x4;
        On_Foot_Region->Seats[j]->Screen_y4 = Screen_y4;
        On_Foot_Region->Seats[j]->Screen_z4 = Screen_z4+Z_Offset;

        On_Foot_Region->Seats[j]->New_Computer = Create_New_Computer();
        Initialize_New_Computer(Engine, On_Foot_Region->Seats[j]);
        Turn_Computer_Off(Engine, On_Foot_Region->Seats[j]);

        On_Foot_Region->Seats[j]->TV_Mode = true;
        On_Foot_Region->Seats[j]->TV_Mode_Switch_Was_Toggled = false;
    }


    for(int j = 0; j < On_Foot_Region_File->Number_Of_Vehicles; j++)
    {
        Buffer_Location = On_Foot_Region_File->Offset_To_Vehicles + (j * sizeof(struct _Vehicle));
        On_Foot_Region->Submarine_Vehicles[j] = Create_Submarine_Object();

        char Mesh_Filename[256];
        char Texture_Filename[256];

        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            Texture_Filename[x] = Buffer[Buffer_Location+x+256];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 512;

        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);
        double Roll = *(double*)&(Buffer[Buffer_Location+24]);
        double Yaw = *(double*)&(Buffer[Buffer_Location+32]);
        double Pitch = *(double*)&(Buffer[Buffer_Location+40]);
        double Damage = *(double*)&(Buffer[Buffer_Location+48]);
        double Entrance_Radius = *(double*)&(Buffer[Buffer_Location+56]);
        Buffer_Location += 8 * 8;

        bool Is_A_Proxy = Buffer[Buffer_Location+0];
        int Proxy_ID = *(int*)&(Buffer[Buffer_Location+1]);
        Buffer_Location += 5;

        int Maximum_Passengers = *(int*)&(Buffer[Buffer_Location+0]);
        int Number_Of_Passengers = *(int*)&(Buffer[Buffer_Location+4]);
        Buffer_Location += 8;

        struct _Universal_ID Passenger_IDs[256];

        Initialize_On_Foot_Submarine_Object(On_Foot_Region->Submarine_Vehicles[j],x,y,z,Yaw,Pitch,Roll);
    }

    for(int j = 0; j < On_Foot_Region_File->Number_Of_Goalposts; j++)
    {
        Buffer_Location = On_Foot_Region_File->Offset_To_Goalposts + (j * sizeof(struct _Goalpost));
        On_Foot_Region->Goalposts[j] = (struct _Goalpost_Object*)calloc(1,sizeof(struct _Goalpost_Object));

        char Mesh_Filename[256];
        char Texture_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            Texture_Filename[x] = Buffer[Buffer_Location+x+256];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 512;
        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);
        double Radius = *(double*)&(Buffer[Buffer_Location+24]);
        int Clear_Category = *(int*)&(Buffer[Buffer_Location+32]);

        On_Foot_Region->Goalposts[j]->Model = Create_SE3_Model();
        Load_SE3_Model(On_Foot_Region->Goalposts[j]->Model, Mesh_Filename);
        Load_SE3_Texture(On_Foot_Region->Goalposts[j]->Model, Texture_Filename);

        On_Foot_Region->Goalposts[j]->x = x;
        On_Foot_Region->Goalposts[j]->y = y;
        On_Foot_Region->Goalposts[j]->z = z+Z_Offset;
        On_Foot_Region->Goalposts[j]->Radius = Radius;
        On_Foot_Region->Goalposts[j]->Clear_Category = Clear_Category;
    }


    for(int j = 0; j < On_Foot_Region_File->Number_Of_Characters; j++)
    {
        Buffer_Location = On_Foot_Region_File->Offset_To_Characters + (j * sizeof(struct _Character));
        char Mesh_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 256;
        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);

        Spawn_Character(On_Foot_Region, j, x,y,z+Z_Offset);
    }



    for(int j = 0; j <On_Foot_Region_File->Number_Of_Viewscreens; j++)
    {
        On_Foot_Region->Viewscreens[j] = (struct _Viewscreen_Object*)calloc(1,sizeof(struct _Viewscreen_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Viewscreens + (j * sizeof(struct _Viewscreen));
        char Mesh_Filename[256];
        char Texture_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            Texture_Filename[x] = Buffer[Buffer_Location+x+256];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 512;

        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);

        double xTwo = *(double*)&(Buffer[Buffer_Location+24]);
        double yTwo = *(double*)&(Buffer[Buffer_Location+32]);
        double zTwo = *(double*)&(Buffer[Buffer_Location+40]);

        bool Viewscreen_On = Buffer[Buffer_Location+48];
        bool Viewscreen_Receiving_Power = Buffer[Buffer_Location+49];

        On_Foot_Region->Viewscreens[j]->Viewscreen_Box = Create_SE3_Model();
        Load_SE3_Model(On_Foot_Region->Viewscreens[j]->Viewscreen_Box,Mesh_Filename);
        Load_SE3_Texture(On_Foot_Region->Viewscreens[j]->Viewscreen_Box,Texture_Filename);

        On_Foot_Region->Viewscreens[j]->x = x;
        On_Foot_Region->Viewscreens[j]->y = y;
        On_Foot_Region->Viewscreens[j]->z = z+Z_Offset;

        On_Foot_Region->Viewscreens[j]->xTwo = xTwo;
        On_Foot_Region->Viewscreens[j]->yTwo = yTwo;
        On_Foot_Region->Viewscreens[j]->zTwo = zTwo+Z_Offset;

        On_Foot_Region->Viewscreens[j]->Viewscreen_On = Viewscreen_On;
        On_Foot_Region->Viewscreens[j]->Viewscreen_Receiving_Power = Viewscreen_Receiving_Power;
    }







    for(int j = 0; j < On_Foot_Region_File->Number_Of_Ships; j++)
    {
        Buffer_Location = On_Foot_Region_File->Offset_To_Ships + (j * sizeof(struct _Ship));
        Engine->On_Foot_State->Ship_Objects[j] = (struct _Ship_Object*)calloc(1,sizeof(struct _Ship_Object));
        Engine->On_Foot_State->Current_Safe_Ship_Altitude += 5000;
        printf("Making Ship at altitude %f\n", Engine->On_Foot_State->Current_Safe_Ship_Altitude);

        char Ship_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Ship_Filename[x] = Buffer[Buffer_Location+x];
            printf("%c", Ship_Filename[x]);
        }
        Load_On_Foot_Region(Engine, &Engine->On_Foot_State->Ship_Objects[j]->Ship_Interior, &Engine->On_Foot_State->Ship_Objects[j]->Ship_Interior_File, Ship_Filename,Engine->On_Foot_State->Current_Safe_Ship_Altitude);
    }

    for(int j = 0; j < On_Foot_Region_File->Number_Of_Vertical_Collision_Geometries; j++)
    {
        On_Foot_Region->Ceilings[j] = (struct _Vertical_Collision_Geometry_Object*)calloc(1,sizeof(struct _Vertical_Collision_Geometry_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Vertical_Collision_Geometries + (j * sizeof(struct _Vertical_Collision_Geometry));
        char Mesh_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 256;
        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);

        On_Foot_Region->Ceilings[j]->Collision_Mesh = Create_Preloaded_Collision_Mesh(Mesh_Filename,0,0,Z_Offset);
    }

    for(int j = 0; j <On_Foot_Region_File->Number_Of_Horizontal_Collision_Geometries; j++)
    {
        On_Foot_Region->Walls[j] = (struct _Horizontal_Collision_Geometry_Object*)calloc(1,sizeof(struct _Horizontal_Collision_Geometry_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Horizontal_Collision_Geometries + (j * sizeof(struct _Horizontal_Collision_Geometry));
        char Mesh_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 256;
        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);

        On_Foot_Region->Walls[j]->Collision_Mesh = Create_Preloaded_Collision_Mesh(Mesh_Filename,0,0,Z_Offset);
    }

    for(int j = 0; j <On_Foot_Region_File->Number_Of_Slope_Collision_Geometries; j++)
    {
        On_Foot_Region->Slopes[j] = (struct _Slope_Collision_Geometry_Object*)calloc(1,sizeof(struct _Slope_Collision_Geometry_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Slope_Collision_Geometries + (j * sizeof(struct _Slope_Collision_Geometry));
        char Mesh_Filename[256];
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 256;
        double x = *(double*)&(Buffer[Buffer_Location+0]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);

        On_Foot_Region->Slopes[j]->Collision_Mesh = Create_Preloaded_Collision_Mesh(Mesh_Filename,0,0,Z_Offset);
    }

    for(int j = 0; j <On_Foot_Region_File->Number_Of_Terrains; j++)
    {
        On_Foot_Region->Terrains[j] = (struct _Terrain_Object*)calloc(1,sizeof(struct _Terrain_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Terrains + (j * sizeof(struct _Terrain));
        char Mesh_Filename[256];
        char Texture_Filename[256];
        bool Visible;
        for(int x = 0; x < 256; x++)
        {
            Mesh_Filename[x] = Buffer[Buffer_Location+x];
            Texture_Filename[x] = Buffer[Buffer_Location+x+256];
            printf("%c", Mesh_Filename[x]);
        }
        Buffer_Location += 512;
        Visible = Buffer[Buffer_Location];
        double x = *(double*)&(Buffer[Buffer_Location+1]);
        double y = *(double*)&(Buffer[Buffer_Location+9]);
        double z = *(double*)&(Buffer[Buffer_Location+17]);

        On_Foot_Region->Terrains[j]->Terrain_Model = Create_SE3_Model();
        Load_SE3_Model(On_Foot_Region->Terrains[j]->Terrain_Model, Mesh_Filename);
        Load_SE3_Texture(On_Foot_Region->Terrains[j]->Terrain_Model, Texture_Filename);

        On_Foot_Region->Terrains[j]->x = x;
        On_Foot_Region->Terrains[j]->y = y;
        On_Foot_Region->Terrains[j]->z = z+Z_Offset;
    }

    for(int j = 0; j <On_Foot_Region_File->Number_Of_Doors; j++)
    {
        On_Foot_Region->Doors[j] = (struct _Door_Object*)calloc(1,sizeof(struct _Door_Object));
        Buffer_Location = On_Foot_Region_File->Offset_To_Doors + (j * sizeof(struct _Door));

        double x = *(double*)&(Buffer[Buffer_Location]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);
        int Axis_Orientation = *(int*)&(Buffer[Buffer_Location+24]);
        Buffer_Location += 28;

        printf("Door coordinates: %f %f %f\n", x,y,z);

        char Model[256];
        char Texture[256];
        char Ceiling_Geometry_File[256];
        char Wall_Geometry_File[256];

        for(int x = 0; x < 256; x++)
        {
            Model[x] = Buffer[Buffer_Location+x];
            Texture[x] = Buffer[Buffer_Location+x+256];
            Ceiling_Geometry_File[x] = Buffer[Buffer_Location+x+256+256];
            Wall_Geometry_File[x] = Buffer[Buffer_Location+x+256+256+256];
        }
        Buffer_Location += 256 * 4;

        bool Use_Custom_Collision_Geometry = Buffer[Buffer_Location];
        Buffer_Location += 1;

        AABB Entrance_AABB;
        AABB Exit_AABB;

        Entrance_AABB.min.x = *(double*)&(Buffer[Buffer_Location]);
        Entrance_AABB.min.y = *(double*)&(Buffer[Buffer_Location+8]);
        Entrance_AABB.min.z = *(double*)&(Buffer[Buffer_Location+16]);

        Entrance_AABB.max.x = *(double*)&(Buffer[Buffer_Location+24]);
        Entrance_AABB.max.y = *(double*)&(Buffer[Buffer_Location+32]);
        Entrance_AABB.max.z = *(double*)&(Buffer[Buffer_Location+40]);
        Buffer_Location += sizeof(AABB);


        Exit_AABB.min.x = *(double*)&(Buffer[Buffer_Location]);
        Exit_AABB.min.y = *(double*)&(Buffer[Buffer_Location+8]);
        Exit_AABB.min.z = *(double*)&(Buffer[Buffer_Location+16]);

        Exit_AABB.max.x = *(double*)&(Buffer[Buffer_Location+24]);
        Exit_AABB.max.y = *(double*)&(Buffer[Buffer_Location+32]);
        Exit_AABB.max.z = *(double*)&(Buffer[Buffer_Location+40]);
        Buffer_Location += sizeof(AABB);


        bool Entrance_Open = Buffer[Buffer_Location];
        bool Exit_Open = Buffer[Buffer_Location+1];

        int Number_Of_Frames = Buffer[Buffer_Location+2];
        int Current_Frame = Buffer[Buffer_Location+6];

        double Teleport_Destination_X = *(double*)&(Buffer[Buffer_Location+10]);
        double Teleport_Destination_Y = *(double*)&(Buffer[Buffer_Location+18]);
        double Teleport_Destination_Z = *(double*)&(Buffer[Buffer_Location+26]);

        int Connected_To_Type = *(int*)&(Buffer[Buffer_Location+34]);
        int Connection_Destination_Door = *(int*)&(Buffer[Buffer_Location+38]);




        On_Foot_Region->Doors[j]->x = x;
        On_Foot_Region->Doors[j]->y = y;
        On_Foot_Region->Doors[j]->z = z+Z_Offset;

        On_Foot_Region->Doors[j]->Axis_Orientation = Axis_Orientation;

        On_Foot_Region->Doors[j]->Door_Model = Create_SE3_Model();
        Load_SE3_Model(On_Foot_Region->Doors[j]->Door_Model, Model);
        Load_SE3_Texture(On_Foot_Region->Doors[j]->Door_Model, Texture);

        if(Use_Custom_Collision_Geometry)
        {
            On_Foot_Region->Doors[j]->Custom_Ceiling_Collision_Geometry = Create_Preloaded_Collision_Mesh(Ceiling_Geometry_File,x,y,z+Z_Offset);
            On_Foot_Region->Doors[j]->Custom_Wall_Collision_Geometry = Create_Preloaded_Collision_Mesh(Wall_Geometry_File,x,y,z+Z_Offset);
        }

        On_Foot_Region->Doors[j]->Entrance_AABB.max.x = Entrance_AABB.max.x + On_Foot_Region->Doors[j]->x;
        On_Foot_Region->Doors[j]->Entrance_AABB.max.y = Entrance_AABB.max.y + On_Foot_Region->Doors[j]->y;
        On_Foot_Region->Doors[j]->Entrance_AABB.max.z = Entrance_AABB.max.z + On_Foot_Region->Doors[j]->z;

        On_Foot_Region->Doors[j]->Entrance_AABB.min.x = Entrance_AABB.min.x + On_Foot_Region->Doors[j]->x;
        On_Foot_Region->Doors[j]->Entrance_AABB.min.y = Entrance_AABB.min.y + On_Foot_Region->Doors[j]->y;
        On_Foot_Region->Doors[j]->Entrance_AABB.min.z = Entrance_AABB.min.z + On_Foot_Region->Doors[j]->z;


        On_Foot_Region->Doors[j]->Exit_AABB.max.x = Exit_AABB.max.x + On_Foot_Region->Doors[j]->x;
        On_Foot_Region->Doors[j]->Exit_AABB.max.y = Exit_AABB.max.y + On_Foot_Region->Doors[j]->y;
        On_Foot_Region->Doors[j]->Exit_AABB.max.z = Exit_AABB.max.z + On_Foot_Region->Doors[j]->z;

        On_Foot_Region->Doors[j]->Exit_AABB.min.x = Exit_AABB.min.x + On_Foot_Region->Doors[j]->x;
        On_Foot_Region->Doors[j]->Exit_AABB.min.y = Exit_AABB.min.y + On_Foot_Region->Doors[j]->y;
        On_Foot_Region->Doors[j]->Exit_AABB.min.z = Exit_AABB.min.z + On_Foot_Region->Doors[j]->z;



        On_Foot_Region->Doors[j]->Entrance_Open = Entrance_Open;
        On_Foot_Region->Doors[j]->Exit_Open = Exit_Open;

        On_Foot_Region->Doors[j]->Number_Of_Frames = Number_Of_Frames;
        On_Foot_Region->Doors[j]->Current_Frame = Current_Frame;

        On_Foot_Region->Doors[j]->Teleport_Destination_X = Teleport_Destination_X;
        On_Foot_Region->Doors[j]->Teleport_Destination_Y = Teleport_Destination_Y;
        On_Foot_Region->Doors[j]->Teleport_Destination_Z = Teleport_Destination_Z+Z_Offset;

        On_Foot_Region->Doors[j]->Connected_To_Type = Connected_To_Type;
        On_Foot_Region->Doors[j]->Connection_Destination_Door = Connection_Destination_Door;
    }
}

void numberToCharArray(int num, char result[3]) {
    // Extract each digit and convert to char by adding '0'
    result[2] = (num % 10) + '0';       // Ones place
    result[1] = ((num / 10) % 10) + '0'; // Tens place
    result[0] = (num / 100) + '0';       // Hundreds place
}

struct _On_Foot_State* Create_On_Foot_State()
{
    struct _On_Foot_State* On_Foot_State = (struct _On_Foot_State*) calloc(1,sizeof (struct _On_Foot_State));
    return On_Foot_State;
}

void Destroy_On_Foot_State(struct _Engine* Engine)
{
    free(Engine->On_Foot_State);
}

void Assign_Weapons_Properties_To_Character(struct _Player_Submarine* On_Foot_Player,
int Current_Weapon,
int Current_Ammo,
double Current_Reload_Time,
double Current_Cooldown_Time,
int Current_Ammo_Per_Clip,
int Current_Stored_Ammo, int Ammo_Mode)
{
    On_Foot_Player->Current_Weapon = Current_Weapon;
    On_Foot_Player->Current_Ammo = Current_Ammo;

   On_Foot_Player->Current_Reload_Time = Current_Reload_Time;
    On_Foot_Player->Current_Cooldown_Time = Current_Cooldown_Time;

    On_Foot_Player->Current_Ammo_Per_Clip = Current_Ammo_Per_Clip;
    On_Foot_Player->Current_Stored_Ammo = Current_Stored_Ammo;

    On_Foot_Player->Ammo_Mode = Ammo_Mode;

    On_Foot_Player->Current_Stored_Ammo -= On_Foot_Player->Current_Ammo_Per_Clip;

}


void Initialize_On_Foot_State(struct _Engine* Engine)
{
    setupFBO(Engine);

    Engine->On_Foot_State->Total_Cycles = 0;

    Engine->On_Foot_State->Go_Back_Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Go_Back_Timer);
    Pause_Timer(Engine->On_Foot_State->Go_Back_Timer);
    Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = false;

    Engine->On_Foot_State->Interact_Not_Pressed = true;
    Engine->On_Foot_State->Render = true;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Engine->On_Foot_State->Hand = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Hand, "resources/models/shooting_hand.se3");
    Load_SE3_Texture(Engine->On_Foot_State->Hand, "notexture");

    Write_Weapon("weapon.wpn");
    Write_Weapon("weapon2.wpn");
    Load_Weapon(Engine, 0, "weapon.wpn");
    Load_Weapon(Engine, 1, "weapon2.wpn");

    Engine->On_Foot_State->Pause_Background = Create_Image();
    Load_Image(Engine->On_Foot_State->Pause_Background, "resources/artwork/gui/title_background.sei");

    Engine->On_Foot_State->Pause_Resume = Create_Image();
    Engine->On_Foot_State->Pause_Exit = Create_Image();
    Engine->On_Foot_State->Pause_Inventory = Create_Image();

    Load_Image(Engine->On_Foot_State->Pause_Resume, "resources/artwork/gui/english/resume.sei");
    Load_Image(Engine->On_Foot_State->Pause_Exit, "resources/artwork/gui/english/exit_to_level_select.sei");
    Load_Image(Engine->On_Foot_State->Pause_Inventory, "resources/artwork/gui/english/exit_to_title.sei");

    Engine->On_Foot_State->Pause_Resume_Highlighted = Create_Image();
    Engine->On_Foot_State->Pause_Exit_Highlighted = Create_Image();
    Engine->On_Foot_State->Pause_Inventory_Highlighted = Create_Image();

    Load_Image(Engine->On_Foot_State->Pause_Resume_Highlighted, "resources/artwork/gui/english/resume_highlighted.sei");
    Load_Image(Engine->On_Foot_State->Pause_Exit_Highlighted, "resources/artwork/gui/english/exit_to_level_select_highlighted.sei");
    Load_Image(Engine->On_Foot_State->Pause_Inventory_Highlighted, "resources/artwork/gui/english/exit_to_title_highlighted.sei");

    Engine->On_Foot_State->Current_Pause_Choice = 2;

    Engine->On_Foot_State->Up_Not_Pressed = false;
    Engine->On_Foot_State->Down_Not_Pressed = false;

    //Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = false;

    Engine->On_Foot_State->Pause_Selector_Y = 52;




    Engine->On_Foot_State->Health = 99;

    Engine->USND_Thud = Mix_LoadWAV( "resources/tracks_and_sounds/general/thud.wav" );
    Engine->USND_Walking = Mix_LoadWAV( "resources/tracks_and_sounds/general/carpet.wav" );
    Engine->USND_Fire = Mix_LoadWAV( "resources/tracks_and_sounds/general/thud.wav" );
    Engine->USND_Flashlight = Mix_LoadWAV( "resources/tracks_and_sounds/general/select.wav" );
    Engine->USND_Explosion = Mix_LoadWAV( "resources/tracks_and_sounds/general/anime_explosion.wav" );
    Engine->USND_Glass_Thud = Mix_LoadWAV( "resources/tracks_and_sounds/general/glass_thud.wav" );
    Engine->USND_Glass_Roll = Mix_LoadWAV( "resources/tracks_and_sounds/general/glass_roll.wav" );

    Engine->USND_Vortex = Mix_LoadWAV( "resources/tracks_and_sounds/general/vortex.wav" );

    Engine->USND_Computer_Beep = Mix_LoadWAV( "resources/tracks_and_sounds/general/computer_keyboard_value_adjustment.wav");

    Engine->USND_Power_On = Mix_LoadWAV( "resources/tracks_and_sounds/general/power_on.wav");

    Engine->USND_Powered = Mix_LoadWAV( "resources/tracks_and_sounds/general/powered.wav");

    Write_Ship("testship");
    Write_On_Foot_Region("testregion");
    Load_On_Foot_Region(Engine,&Engine->On_Foot_State->On_Foot_Region, &Engine->On_Foot_State->On_Foot_Region_File, "testregion",0);


    Engine->On_Foot_State->Ammo = 8;

    Engine->On_Foot_State->Universal_Timer = Create_Timer();
    Engine->On_Foot_State->Fire_Timer = Create_Timer();
    Engine->On_Foot_State->Cooldown_Timer = Create_Timer();
    Engine->On_Foot_State->Reload_Timer = Create_Timer();

    Set_Timer(Engine->On_Foot_State->Universal_Timer);
    Set_Timer(Engine->On_Foot_State->Fire_Timer);
    Set_Timer(Engine->On_Foot_State->Cooldown_Timer);
    Set_Timer(Engine->On_Foot_State->Reload_Timer);

    Pause_Timer(Engine->On_Foot_State->Fire_Timer);
    Pause_Timer(Engine->On_Foot_State->Cooldown_Timer);
    Pause_Timer(Engine->On_Foot_State->Reload_Timer);

    Engine->On_Foot_State->Animation_Fired = 0;
    Engine->On_Foot_State->Weapon_Fired = false;

    Engine->On_Foot_State->Reload_Fired = 0;
    Engine->On_Foot_State->Need_To_Reload = false;

    Engine->On_Foot_State->Mode = 0;

    Engine->On_Foot_State->Maximum_X_Coordinate = AREA_OF_OPERATIONS;
    Engine->On_Foot_State->Maximum_Y_Coordinate = AREA_OF_OPERATIONS;
    Engine->On_Foot_State->Maximum_Z_Coordinate = AREA_OF_OPERATIONS;

    Engine->On_Foot_State->Minimum_X_Coordinate = AREA_OF_OPERATIONS*-1;
    Engine->On_Foot_State->Minimum_Y_Coordinate = AREA_OF_OPERATIONS*-1;
    Engine->On_Foot_State->Minimum_Z_Coordinate = AREA_OF_OPERATIONS*-1;

    Engine->On_Foot_State->Kill_Ceiling_Z = AREA_OF_OPERATIONS; //Either stop me or kill me here.
    Engine->On_Foot_State->Kill_Floor_Z = AREA_OF_OPERATIONS*-1; //Either stop me or kill me here.

    Engine->On_Foot_State->Current_Player_State = PLAYER_IS_ON_FOOT;

    Engine->On_Foot_State->Third_Person_Mode = 0;
    Engine->On_Foot_State->Debug_Mode = 0;
    Engine->On_Foot_State->Current_State = STATE_UNPAUSED;
    Engine->On_Foot_State->Current_Location = CURRENT_LOCATION_SEA;

    Engine->On_Foot_State->Camera_X = 0;
    Engine->On_Foot_State->Camera_Y = 0;
    Engine->On_Foot_State->Camera_Z = 0;
    Engine->On_Foot_State->Camera_Yaw = 0;
    Engine->On_Foot_State->Camera_Pitch = 0;
    Engine->On_Foot_State->Camera_Front_X = 0;
    Engine->On_Foot_State->Camera_Front_Y = 0;
    Engine->On_Foot_State->Camera_Front_Z = 0;
    Engine->On_Foot_State->Camera_Sensitivity = 0.01;

    Engine->On_Foot_State->Pause_Button_Pressed = false;

//    Engine->On_Foot_State->Client = (struct _Seafarer_Engine_Multiplayer_Client*)calloc(1,sizeof(struct _Seafarer_Engine_Multiplayer_Client));

    Engine->On_Foot_State->Skybox = Create_SE3_Model();
    Engine->On_Foot_State->Seabox = Create_SE3_Model();
    Engine->On_Foot_State->Seasurfacebox = Create_SE3_Model();
    Engine->On_Foot_State->Hyperbox = Create_SE3_Model();

    Engine->On_Foot_State->Rosechu = Create_SE3_Model();

    Load_SE3_Model(Engine->On_Foot_State->Skybox,"resources/models/model_skybox.se3");
    Load_SE3_Model(Engine->On_Foot_State->Seabox, "resources/models/model_skybox.se3");
    Load_SE3_Model(Engine->On_Foot_State->Seasurfacebox,"resources/models/model_ocean.se3");
    Load_SE3_Model(Engine->On_Foot_State->Hyperbox, "resources/models/model_hyperspace.se3");

    Load_SE3_Model(Engine->On_Foot_State->Rosechu,"resources/models/rosemary.se3");

    BMPtoSEI("resources/models/texture_ocean.bmp", "resources/models/texture_ocean.sei");
    BMPtoSEI("resources/models/texture_skybox.bmp", "resources/models/texture_skybox.sei");
    BMPtoSEI("resources/models/texture_skybox_ocean.bmp", "resources/models/texture_skybox_ocean.sei");

    Load_SE3_Texture(Engine->On_Foot_State->Skybox,"resources/models/texture_skybox.sei");
    Load_SE3_Texture(Engine->On_Foot_State->Seabox, "resources/models/texture_skybox_ocean.sei");

    Load_SE3_Texture(Engine->On_Foot_State->Seasurfacebox,"resources/models/texture_ocean.sei");
    Load_SE3_Texture(Engine->On_Foot_State->Hyperbox, "resources/models/rosemary.sei");

    Load_SE3_Texture(Engine->On_Foot_State->Rosechu, "resources/models/rosemary.sei");

    Engine->On_Foot_State->On_Foot_Player = Create_Player();
    Initialize_On_Foot_Player(Engine->On_Foot_State->On_Foot_Player, "none", 0,0,0);

    Assign_Weapons_Properties_To_Character(Engine->On_Foot_State->On_Foot_Player,1,1,1,1,1,100, TORPEDO_MODE_KNIFE);
    Assign_Weapons_Properties_To_Character(Engine->On_Foot_State->On_Foot_Region.Characters[0],1,1,1,1,1,1, TORPEDO_MODE_KNIFE);

    Engine->On_Foot_State->SENC = (struct _Seafarer_Engine_Network_Client*)calloc(1,sizeof(struct _Seafarer_Engine_Network_Client));

    Engine->On_Foot_State->Connection_Status = Connect_IPv4(Engine->On_Foot_State->SENC, "127.0.0.1", 8888, "Chris");

    if(Engine->On_Foot_State->Connection_Status == 1)
    {
        printf("Connected to server.\n");
        network_update(Engine->On_Foot_State->SENC);
        printf("My Id is %d\n", Engine->On_Foot_State->SENC->my_id);

    }

    else if(Engine->On_Foot_State->Connection_Status == -1)
    {
        printf("Not connected to server.\n");
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
    gluPerspective(60, 1.7777,1.0f, METER_CONVERSION*10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4,-4,-4,0,0,0,0,0,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat gn_ambient[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat gn_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat gn_specular[] = { 0.6, 0.6, 0.6, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, gn_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, gn_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, gn_specular);
    GLfloat gn_position[] = { 1, 1, 1, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, gn_position);

    glEnable(GL_LIGHT1);
    GLfloat flashlight_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat flashlight_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat flashlight_specular[] = { 1, 1, 1, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, flashlight_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, flashlight_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, flashlight_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 5.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);


    Engine->On_Foot_State->Numbers[0] = Create_Image();
    Engine->On_Foot_State->Numbers[1] = Create_Image();
    Engine->On_Foot_State->Numbers[2] = Create_Image();
    Engine->On_Foot_State->Numbers[3] = Create_Image();
    Engine->On_Foot_State->Numbers[4] = Create_Image();
    Engine->On_Foot_State->Numbers[5] = Create_Image();
    Engine->On_Foot_State->Numbers[6] = Create_Image();
    Engine->On_Foot_State->Numbers[7] = Create_Image();
    Engine->On_Foot_State->Numbers[8] = Create_Image();
    Engine->On_Foot_State->Numbers[9] = Create_Image();

    BMPtoSEI("zero.bmp", "zero.sei");
    BMPtoSEI("one.bmp", "one.sei");
    BMPtoSEI("two.bmp", "two.sei");
    BMPtoSEI("three.bmp", "three.sei");
    BMPtoSEI("four.bmp", "four.sei");
    BMPtoSEI("five.bmp", "five.sei");
    BMPtoSEI("six.bmp", "six.sei");
    BMPtoSEI("seven.bmp", "seven.sei");
    BMPtoSEI("eight.bmp", "eight.sei");
    BMPtoSEI("nine.bmp", "nine.sei");

    Load_Image(Engine->On_Foot_State->Numbers[0], "zero.sei");
    Load_Image(Engine->On_Foot_State->Numbers[1], "one.sei");
    Load_Image(Engine->On_Foot_State->Numbers[2], "two.sei");
    Load_Image(Engine->On_Foot_State->Numbers[3], "three.sei");
    Load_Image(Engine->On_Foot_State->Numbers[4], "four.sei");
    Load_Image(Engine->On_Foot_State->Numbers[5], "five.sei");
    Load_Image(Engine->On_Foot_State->Numbers[6], "six.sei");
    Load_Image(Engine->On_Foot_State->Numbers[7], "seven.sei");
    Load_Image(Engine->On_Foot_State->Numbers[8], "eight.sei");
    Load_Image(Engine->On_Foot_State->Numbers[9], "nine.sei");

    Engine->On_Foot_State->Number_Of_Weapon_Rounds = 0;
    Engine->On_Foot_State->Current_Weapon_Round = -1;

    Engine->On_Foot_State->Current_Safe_Ship_Altitude = 0;


    if(Engine->On_Foot_State->Connection_Status == 1)
    {
        Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->SENC->local_players[Engine->On_Foot_State->SENC->my_id].x;
        Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->SENC->local_players[Engine->On_Foot_State->SENC->my_id].y;
    }

    printf("My x is %f and my y is %f\n", Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y);
}

void Handle_Location_Rendering_Logic(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->On_Foot_Player->z < 0)
    {
        Engine->On_Foot_State->Current_Location = CURRENT_LOCATION_UNDERWATER;
    }

    else if(Engine->On_Foot_State->On_Foot_Player->z >= 0)
    {
        Engine->On_Foot_State->Current_Location = CURRENT_LOCATION_SEA;
    }


    switch(Engine->On_Foot_State->Current_Location)
        {
            case CURRENT_LOCATION_SEA:
                glPushMatrix();
                glTranslatef(0,0,0);
                Render_SE3_Model(Engine->On_Foot_State->Skybox, 0,0,0,0,0,0,0,false,1,1,1,false);
                //Render_SE3_Model(Engine->On_Foot_State->Seasurfacebox, 0,0,0,0,0,0,0,false,1,1,1,false);
                glPopMatrix();
            break;

            case CURRENT_LOCATION_UNDERWATER:
                glPushMatrix();
                glTranslatef(0,0,0);
                Render_SE3_Model(Engine->On_Foot_State->Seabox, 0,0,0,0,0,0,0,false,1,1,1,false);
                glPopMatrix();
            break;

            case CURRENT_LOCATION_HYPERSPACE:
                glPushMatrix();
                glTranslatef(0,0,0);
                Render_SE3_Model(Engine->On_Foot_State->Hyperbox, 0,0,0,0,0,0,0,false,1,1,1,false);
                glPopMatrix();
            break;
        }
}

void Handle_TwoD_HUD_Logic(struct _Engine* Engine)
{
     glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);

        int Render_x;

        if(Engine->On_Foot_State->On_Foot_Player->Ammo_Mode != TORPEDO_MODE_KNIFE)
        {
    char result[3];
    numberToCharArray(Engine->On_Foot_State->On_Foot_Player->Current_Ammo, result);

    switch(result[0])
    {
        case '0':
            Render_Image(Engine->On_Foot_State->Numbers[0],Render_x,360-48,1);
        break;
        case '1':
            Render_Image(Engine->On_Foot_State->Numbers[1],Render_x,360-48,1);
        break;
        case '2':
            Render_Image(Engine->On_Foot_State->Numbers[2],Render_x,360-48,1);
        break;
        case '3':
            Render_Image(Engine->On_Foot_State->Numbers[3],Render_x,360-48,1);
        break;
        case '4':
            Render_Image(Engine->On_Foot_State->Numbers[4],Render_x,360-48,1);
        break;
        case '5':
            Render_Image(Engine->On_Foot_State->Numbers[5],Render_x,360-48,1);
        break;
        case '6':
            Render_Image(Engine->On_Foot_State->Numbers[6],Render_x,360-48,1);
        break;
        case '7':
            Render_Image(Engine->On_Foot_State->Numbers[7],Render_x,360-48,1);
        break;
        case '8':
            Render_Image(Engine->On_Foot_State->Numbers[8],Render_x,360-48,1);
        break;
        case '9':
            Render_Image(Engine->On_Foot_State->Numbers[9],Render_x,360-48,1);
        break;
    }

    Render_x += 16;
    switch(result[1])
    {
        case '0':
            Render_Image(Engine->On_Foot_State->Numbers[0],Render_x,360-48,1);
        break;
        case '1':
            Render_Image(Engine->On_Foot_State->Numbers[1],Render_x,360-48,1);
        break;
        case '2':
            Render_Image(Engine->On_Foot_State->Numbers[2],Render_x,360-48,1);
        break;
        case '3':
            Render_Image(Engine->On_Foot_State->Numbers[3],Render_x,360-48,1);
        break;
        case '4':
            Render_Image(Engine->On_Foot_State->Numbers[4],Render_x,360-48,1);
        break;
        case '5':
            Render_Image(Engine->On_Foot_State->Numbers[5],Render_x,360-48,1);
        break;
        case '6':
            Render_Image(Engine->On_Foot_State->Numbers[6],Render_x,360-48,1);
        break;
        case '7':
            Render_Image(Engine->On_Foot_State->Numbers[7],Render_x,360-48,1);
        break;
        case '8':
            Render_Image(Engine->On_Foot_State->Numbers[8],Render_x,360-48,1);
        break;
        case '9':
            Render_Image(Engine->On_Foot_State->Numbers[9],Render_x,360-48,1);
        break;
    }

    Render_x += 16;
    switch(result[2])
    {
        case '0':
            Render_Image(Engine->On_Foot_State->Numbers[0],Render_x,360-48,1);
        break;
        case '1':
            Render_Image(Engine->On_Foot_State->Numbers[1],Render_x,360-48,1);
        break;
        case '2':
            Render_Image(Engine->On_Foot_State->Numbers[2],Render_x,360-48,1);
        break;
        case '3':
            Render_Image(Engine->On_Foot_State->Numbers[3],Render_x,360-48,1);
        break;
        case '4':
            Render_Image(Engine->On_Foot_State->Numbers[4],Render_x,360-48,1);
        break;
        case '5':
            Render_Image(Engine->On_Foot_State->Numbers[5],Render_x,360-48,1);
        break;
        case '6':
            Render_Image(Engine->On_Foot_State->Numbers[6],Render_x,360-48,1);
        break;
        case '7':
            Render_Image(Engine->On_Foot_State->Numbers[7],Render_x,360-48,1);
        break;
        case '8':
            Render_Image(Engine->On_Foot_State->Numbers[8],Render_x,360-48,1);
        break;
        case '9':
            Render_Image(Engine->On_Foot_State->Numbers[9],Render_x,360-48,1);
        break;
    }
    }






    char healthresult[3];
    numberToCharArray(Engine->On_Foot_State->Health, healthresult);

    Render_x = 16;
    switch(healthresult[0])
    {
        case '0':
            Render_Image(Engine->On_Foot_State->Numbers[0],Render_x,360-48,1);
        break;
        case '1':
            Render_Image(Engine->On_Foot_State->Numbers[1],Render_x,360-48,1);
        break;
        case '2':
            Render_Image(Engine->On_Foot_State->Numbers[2],Render_x,360-48,1);
        break;
        case '3':
            Render_Image(Engine->On_Foot_State->Numbers[3],Render_x,360-48,1);
        break;
        case '4':
            Render_Image(Engine->On_Foot_State->Numbers[4],Render_x,360-48,1);
        break;
        case '5':
            Render_Image(Engine->On_Foot_State->Numbers[5],Render_x,360-48,1);
        break;
        case '6':
            Render_Image(Engine->On_Foot_State->Numbers[6],Render_x,360-48,1);
        break;
        case '7':
            Render_Image(Engine->On_Foot_State->Numbers[7],Render_x,360-48,1);
        break;
        case '8':
            Render_Image(Engine->On_Foot_State->Numbers[8],Render_x,360-48,1);
        break;
        case '9':
            Render_Image(Engine->On_Foot_State->Numbers[9],Render_x,360-48,1);
        break;
    }

    Render_x += 16;
    switch(healthresult[1])
    {
        case '0':
            Render_Image(Engine->On_Foot_State->Numbers[0],Render_x,360-48,1);
        break;
        case '1':
            Render_Image(Engine->On_Foot_State->Numbers[1],Render_x,360-48,1);
        break;
        case '2':
            Render_Image(Engine->On_Foot_State->Numbers[2],Render_x,360-48,1);
        break;
        case '3':
            Render_Image(Engine->On_Foot_State->Numbers[3],Render_x,360-48,1);
        break;
        case '4':
            Render_Image(Engine->On_Foot_State->Numbers[4],Render_x,360-48,1);
        break;
        case '5':
            Render_Image(Engine->On_Foot_State->Numbers[5],Render_x,360-48,1);
        break;
        case '6':
            Render_Image(Engine->On_Foot_State->Numbers[6],Render_x,360-48,1);
        break;
        case '7':
            Render_Image(Engine->On_Foot_State->Numbers[7],Render_x,360-48,1);
        break;
        case '8':
            Render_Image(Engine->On_Foot_State->Numbers[8],Render_x,360-48,1);
        break;
        case '9':
            Render_Image(Engine->On_Foot_State->Numbers[9],Render_x,360-48,1);
        break;
    }

    Render_x += 16;
    switch(healthresult[2])
    {
        case '0':
            Render_Image(Engine->On_Foot_State->Numbers[0],Render_x,360-48,1);
        break;
        case '1':
            Render_Image(Engine->On_Foot_State->Numbers[1],Render_x,360-48,1);
        break;
        case '2':
            Render_Image(Engine->On_Foot_State->Numbers[2],Render_x,360-48,1);
        break;
        case '3':
            Render_Image(Engine->On_Foot_State->Numbers[3],Render_x,360-48,1);
        break;
        case '4':
            Render_Image(Engine->On_Foot_State->Numbers[4],Render_x,360-48,1);
        break;
        case '5':
            Render_Image(Engine->On_Foot_State->Numbers[5],Render_x,360-48,1);
        break;
        case '6':
            Render_Image(Engine->On_Foot_State->Numbers[6],Render_x,360-48,1);
        break;
        case '7':
            Render_Image(Engine->On_Foot_State->Numbers[7],Render_x,360-48,1);
        break;
        case '8':
            Render_Image(Engine->On_Foot_State->Numbers[8],Render_x,360-48,1);
        break;
        case '9':
            Render_Image(Engine->On_Foot_State->Numbers[9],Render_x,360-48,1);
        break;
    }


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.7777,1.0f, METER_CONVERSION*10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
}


void Render_Submarine_Vehicles(struct _Engine* Engine)
{
    printf("There are %d vehicles\n", Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Vehicles);
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Vehicles; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->plane_x,Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->plane_y,Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->plane_z);

        float mat[16] = {
        (float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Right_Vector.x, (float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Right_Vector.y, (float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Right_Vector.z, 0,
        (float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Up_Vector.x,    (float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Up_Vector.y,    (float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Up_Vector.z,    0,
        -(float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Forward_Vector.x,  -(float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Forward_Vector.y,  -(float)Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Forward_Vector.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);

        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[j]->Models[0], 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Viewscreens(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Viewscreens; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->On_Foot_Region.Viewscreens[j]->x,Engine->On_Foot_State->On_Foot_Region.Viewscreens[j]->y,Engine->On_Foot_State->On_Foot_Region.Viewscreens[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Viewscreens[j]->Viewscreen_Box, 0,0,0,0,0,0,0,false,1,1,1,false);

        Draw_TV_Vertices(-1,0,0,0,0,0,0,0,0,0,0,0,0);
        glPopMatrix();
    }
}


void Render_Goalposts(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Goalposts; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->On_Foot_Region.Goalposts[j]->x,Engine->On_Foot_State->On_Foot_Region.Goalposts[j]->y,Engine->On_Foot_State->On_Foot_Region.Goalposts[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Goalposts[j]->Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Seats(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Seats; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->On_Foot_Region.Seats[j]->x,Engine->On_Foot_State->On_Foot_Region.Seats[j]->y,Engine->On_Foot_State->On_Foot_Region.Seats[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Seats[j]->Model, 0,0,0,0,0,0,0,false,1,1,1,false);

        if(Engine->On_Foot_State->On_Foot_Region.Seats[j]->TV_Mode)
        {
            Draw_TV_Vertices(Engine->On_Foot_State->textureColorBuffer,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x1,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y1,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z1,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x2,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y2,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z2,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x3,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y3,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z3,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x4,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y4,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z4);
        }

        else if(!Engine->On_Foot_State->On_Foot_Region.Seats[j]->TV_Mode)
        {
            Draw_TV_Vertices(Engine->On_Foot_State->On_Foot_Region.Seats[j]->New_Computer->Texture_ID,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x1,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y1,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z1,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x2,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y2,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z2,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x3,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y3,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z3,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_x4,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_y4,Engine->On_Foot_State->On_Foot_Region.Seats[j]->Screen_z4);
        }

        glPopMatrix();
    }
}


void Render_Terrains(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Terrains; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->On_Foot_Region.Terrains[j]->x,Engine->On_Foot_State->On_Foot_Region.Terrains[j]->y,Engine->On_Foot_State->On_Foot_Region.Terrains[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Terrains[j]->Terrain_Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Ship_Terrains(struct _Engine* Engine, int ID)
{
    for(int j = 0; j < Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior_File.Number_Of_Terrains; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Terrains[j]->x,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Terrains[j]->y,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Terrains[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Terrains[j]->Terrain_Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Ship_Viewscreens(struct _Engine* Engine, int ID)
{
    for(int j = 0; j < Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior_File.Number_Of_Viewscreens; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Viewscreens[j]->x,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Viewscreens[j]->y,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Viewscreens[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Viewscreens[j]->Viewscreen_Box, 0,0,0,0,0,0,0,false,1,1,1,false);

        Draw_TV_Vertices(-1,0,0,0,0,0,0,0,0,0,0,0,0);
        glPopMatrix();
    }
}

void Render_Ship_Seats(struct _Engine* Engine, int ID)
{
    for(int j = 0; j < Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior_File.Number_Of_Seats; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Seats[j]->x,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Seats[j]->y,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Seats[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Seats[j]->Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Ship_Goalposts(struct _Engine* Engine, int ID)
{
    for(int j = 0; j < Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior_File.Number_Of_Goalposts; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Goalposts[j]->x,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Goalposts[j]->y,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Goalposts[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Goalposts[j]->Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Characters(struct _Engine* Engine, int ID)
{
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->On_Foot_Region.Characters[ID]->x,Engine->On_Foot_State->On_Foot_Region.Characters[ID]->y,Engine->On_Foot_State->On_Foot_Region.Characters[ID]->z);
    glRotatef(Engine->On_Foot_State->On_Foot_Region.Characters[ID]->Yaw_Angle,0,0,1);
    Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Characters[ID]->Models[0], 0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
}

void Render_Ship_Doors(struct _Engine* Engine, int ID)
{
    for(int j = 0; j < Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior_File.Number_Of_Doors; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Doors[j]->x,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Doors[j]->y,Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Doors[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[ID]->Ship_Interior.Doors[j]->Door_Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Doors(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Doors; j++)
    {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->On_Foot_Region.Doors[j]->x,Engine->On_Foot_State->On_Foot_Region.Doors[j]->y,Engine->On_Foot_State->On_Foot_Region.Doors[j]->z);
       //printf("Door coordinates: %f %f %f\n", Engine->On_Foot_State->On_Foot_Region.Doors[j]->x,Engine->On_Foot_State->On_Foot_Region.Doors[j]->y,Engine->On_Foot_State->On_Foot_Region.Doors[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->On_Foot_Region.Doors[j]->Door_Model, 0,0,0,0,0,0,0,false,1,1,1,false);
        glPopMatrix();
    }
}

void Render_Weapon_Rounds(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->Number_Of_Weapon_Rounds; j++)
    {
        Process_Weapon_Round(Engine, j);
        Render_Weapon_Round(Engine,j);
    }
    //printf("My Current Weapon is %d and My Cooldown is %f and My Reload is %f\n", Engine->On_Foot_State->On_Foot_Player->Current_Weapon,Engine->On_Foot_State->On_Foot_Player->Current_Cooldown_Time,Engine->On_Foot_State->On_Foot_Player->Current_Reload_Time);
}


void Render_Second_Pass(struct _Engine* Engine)
{
    Render_Terrains(Engine);
    Render_Doors(Engine);
    Render_Viewscreens(Engine);
    Render_Goalposts(Engine);
    //Render_Submarine_Vehicles(Engine);
    //Render_Seats(Engine);

    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Characters; j++)
            {
                if(Engine->On_Foot_State->SENC->local_players[j].active == 1)
                {
                    Render_Characters(Engine, j);
                }
            }

    Handle_Location_Rendering_Logic(Engine);
   Render_Weapon_Rounds(Engine);
}


void Render_On_Foot_State(struct _Engine* Engine, struct _Audio_Chip* Audio_Chip)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Seats; j++)
    {
        if(Engine->On_Foot_State->On_Foot_Region.Seats[j]->New_Computer->Power_State)
        {
            Process_New_Computer_Cycles(Engine, Engine->On_Foot_State->On_Foot_Region.Seats[j], 10, Audio_Chip);
        }
    }

    if(Engine->On_Foot_State->Render)
    {
        if(Engine->On_Foot_State->Current_State == STATE_UNPAUSED)
        {
            if(!Engine->On_Foot_State->On_Foot_Player->Is_On_A_Ship)
            {
                Render_Terrains(Engine);
                Render_Doors(Engine);
                Render_Viewscreens(Engine);
                Render_Goalposts(Engine);
                Render_Seats(Engine);
            }

            Render_Submarine_Vehicles(Engine);

            for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Characters; j++)
            {
                if(Engine->On_Foot_State->SENC->local_players[j].active == 1)
                {
                    Render_Characters(Engine, j);
                }
            }

            if(Engine->On_Foot_State->On_Foot_Player->Is_On_A_Ship)
            {
                Render_Ship_Terrains(Engine,Engine->On_Foot_State->On_Foot_Player->On_This_Ship);
                Render_Ship_Doors(Engine,Engine->On_Foot_State->On_Foot_Player->On_This_Ship);
                Render_Ship_Viewscreens(Engine,Engine->On_Foot_State->On_Foot_Player->On_This_Ship);
                Render_Ship_Goalposts(Engine,Engine->On_Foot_State->On_Foot_Player->On_This_Ship);
               // Render_Ship_Submarine_Vehicles(Engine,j);
                Render_Ship_Seats(Engine,Engine->On_Foot_State->On_Foot_Player->On_This_Ship);
            }

            Handle_Location_Rendering_Logic(Engine);
            Render_Player_Subroutine(Engine);
            Render_Weapon_Rounds(Engine);

            glBindFramebuffer(GL_FRAMEBUFFER, Engine->On_Foot_State->fbo);
            glViewport(0, 0, 256,256);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60, 1.0f,1.0f, METER_CONVERSION*10000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            Vec3 cam;
            cam.x = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_x - Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Forward_Vector.x * 2000 + Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.x * 30.0f;
            cam.y = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_y - Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Forward_Vector.y * 2000 + Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.y * 30.0f;
            cam.z = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_z - Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Forward_Vector.z * 2000 + Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.z * 30.0f;

            gluLookAt(cam.x, cam.y, cam.z, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_x, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_y, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_z, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.x, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.y, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.z);

            Render_Second_Pass(Engine);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, 1920,1080);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION*10000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();


            Handle_TwoD_HUD_Logic(Engine);
        }

    if(Engine->On_Foot_State->Current_State == STATE_PAUSED)
    {
        Handle_Pause_Rendering_Pass(Engine);
    }
    }
}

void Check_If_Door_Should_Be_Opened(struct _Engine* Engine)
{
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Doors; j++)
    {
        if(Check_If_Ray_Sensor_Detects_AABB(Engine->On_Foot_State->On_Foot_Region.Doors[j]->Entrance_AABB,Engine->On_Foot_State->Camera_X,Engine->On_Foot_State->Camera_Y,Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Camera_X+Engine->On_Foot_State->Camera_Front_X*100,Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->Camera_Front_Y*100,Engine->On_Foot_State->Camera_Z+Engine->On_Foot_State->Camera_Front_Z*100))
        {
            printf("Door!\n");
            Mix_PlayChannel(-1, Engine->USND_Vortex, 0); // Start the sound
            if(Engine->On_Foot_State->On_Foot_Region.Doors[j]->Entrance_Open)
            {
                Engine->On_Foot_State->On_Foot_Region.Doors[j]->Entrance_Open = false;
                printf("Door locked\n");
            }

            else if(!Engine->On_Foot_State->On_Foot_Region.Doors[j]->Entrance_Open)
            {
                Engine->On_Foot_State->On_Foot_Region.Doors[j]->Entrance_Open = true;
                printf("Door unlocked\n");
            }
        }

        if(Check_If_Ray_Sensor_Detects_AABB(Engine->On_Foot_State->On_Foot_Region.Doors[j]->Exit_AABB,Engine->On_Foot_State->Camera_X,Engine->On_Foot_State->Camera_Y,Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Camera_X+Engine->On_Foot_State->Camera_Front_X*100,Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->Camera_Front_Y*100,Engine->On_Foot_State->Camera_Z+Engine->On_Foot_State->Camera_Front_Z*100))
        {
            printf("Door!\n");
            Mix_PlayChannel(-1, Engine->USND_Vortex, 0); // Start the sound
            if(Engine->On_Foot_State->On_Foot_Region.Doors[j]->Exit_Open)
            {
                Engine->On_Foot_State->On_Foot_Region.Doors[j]->Exit_Open = false;
                printf("Door locked\n");
            }

            else if(!Engine->On_Foot_State->On_Foot_Region.Doors[j]->Exit_Open)
            {
                Engine->On_Foot_State->On_Foot_Region.Doors[j]->Exit_Open = true;
                printf("Door unlocked\n");
            }
        }
    }
}

void Check_If_Player_Forward_Vector_Connected(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File, double Camera_X,double Camera_Y,double Camera_Z,double Camera_Front_X,double Camera_Front_Y,double Camera_Front_Z)
{
    for(int j = 0; j < On_Foot_Region_File->Number_Of_Characters; j++)
    {
        if(Check_If_Ray_Sensor_Detects_AABB(On_Foot_Region->Characters[j]->Head_Collision_Geometry,Camera_X,Camera_Y,Camera_Z,Camera_X+Camera_Front_X*10,Camera_Y+Camera_Front_Y*10,Camera_Z+Camera_Front_Z*10))
        {
            printf("Stab connected!\n");
        }
    }
}

void Handle_Item_Logic(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Mouse_Is_Clicked)
    {
        if(!Engine->On_Foot_State->Weapon_Fired && Engine->On_Foot_State->On_Foot_Player->Current_Ammo > 0 && !Engine->On_Foot_State->Need_To_Reload)
        {
            if(Engine->On_Foot_State->On_Foot_Player->Ammo_Mode != TORPEDO_MODE_KNIFE)
            {
                Engine->On_Foot_State->Weapon_Fired = true;
                Mix_PlayChannel(-1, Engine->USND_Computer_Beep, 0);
                Engine->On_Foot_State->On_Foot_Player->Current_Ammo -= 1;
                printf("Fired Weapon!\n");
                Create_Weapon_Round(Engine, Engine->On_Foot_State->On_Foot_Player->Right_Vector,Engine->On_Foot_State->On_Foot_Player->Up_Vector,Engine->On_Foot_State->On_Foot_Player->Forward_Vector);
                Engine->On_Foot_State->Cooldown_Fired = Get_Ticks(Engine->On_Foot_State->Universal_Timer);

                if(Engine->On_Foot_State->On_Foot_Player->Current_Ammo == 0)
                {
                    Engine->On_Foot_State->Need_To_Reload = true;
                    printf("Need to reload!");
                    Engine->On_Foot_State->Reload_Fired = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
                }
            }

            else if(Engine->On_Foot_State->On_Foot_Player->Ammo_Mode == TORPEDO_MODE_KNIFE)
            {
                Engine->On_Foot_State->Weapon_Fired = true;
                Mix_PlayChannel(-1, Engine->USND_Computer_Beep, 0);
                printf("Fired Knife!\n");
                Check_If_Player_Forward_Vector_Connected(Engine->On_Foot_State->On_Foot_Player, &Engine->On_Foot_State->On_Foot_Region,&Engine->On_Foot_State->On_Foot_Region_File,Engine->On_Foot_State->Camera_X,Engine->On_Foot_State->Camera_Y,Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Camera_Front_X,Engine->On_Foot_State->Camera_Front_Y,Engine->On_Foot_State->Camera_Front_Z);

                for(int m = 0; m < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Ships; m++)
                {
                    Check_If_Player_Forward_Vector_Connected(Engine->On_Foot_State->On_Foot_Player, &Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior,&Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior_File,Engine->On_Foot_State->Camera_X,Engine->On_Foot_State->Camera_Y,Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Camera_Front_X,Engine->On_Foot_State->Camera_Front_Y,Engine->On_Foot_State->Camera_Front_Z);
                }

                Engine->On_Foot_State->Cooldown_Fired = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
            }
        }
    }

    if(Keypad.Keyboard_E && Engine->On_Foot_State->Interact_Not_Pressed)
    {
        Engine->On_Foot_State->Interact_Not_Pressed = false;
        printf("Interact!\n");
        //FOOT COLLISION DETECTION
        Check_If_Door_Should_Be_Opened(Engine);
    }
}

void Handle_Pausing_Logic(struct _Engine* Engine, struct _Keypad Keypad)
{
    //Mix_PlayChannel( -1, Engine->On_Foot_State->Pause, 0 );
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glfwSetInputMode(Engine->W, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Handle_Cutscene_Start_Logic(struct _Engine* Engine, struct _Keypad Keypad)
{

}



void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad, struct _Audio_Chip* Audio_Chip)
{

    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Seats; j++)
    {
        if(Engine->On_Foot_State->On_Foot_Region.Seats[j]->New_Computer->Power_State)
        {
            Process_New_Computer_Cycles(Engine, Engine->On_Foot_State->On_Foot_Region.Seats[j], 10, Audio_Chip);
        }
    }

    if(Engine->On_Foot_State->Render)
    {

        for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Seats; j++)
        {
            if(Engine->On_Foot_State->On_Foot_Region.Seats[j]->New_Computer->Power_State)
            {
                Render_New_Computer(Engine, Engine->On_Foot_State->On_Foot_Region.Seats[j]);
                if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                {
                    Handle_New_Computer_Inputs(Engine, Engine->On_Foot_State->On_Foot_Region.Seats[Engine->On_Foot_State->On_Foot_Player->Chair_ID], Keypad);
                }
            }
        }

    if(Engine->On_Foot_State->Current_State == STATE_UNPAUSED)
    {
        for(int r = 0; r < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Goalposts; r++)
        {
            double o = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z, Engine->On_Foot_State->On_Foot_Region.Goalposts[r]->x,Engine->On_Foot_State->On_Foot_Region.Goalposts[r]->y,Engine->On_Foot_State->On_Foot_Region.Goalposts[r]->z);
            if(o <= 60)
            {
                printf("level cleared!\n");
                Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
                Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
                Engine->On_Foot_State->Render = false;

                printf("Levels: %d\n", Engine->Number_Of_Levels);
                printf("Triple: %d\n", Engine->This_Is_The_Current_Loaded_Level_Triple);

                if(Engine->This_Is_The_Current_Loaded_Level_Triple < Engine->Number_Of_Levels-1)
                {
                switch(Engine->This_Is_The_Current_Loaded_Level_Difficulty)
                {
                    case DIFFICULTY_EASY:
                        Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple].Easy_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                        Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple].Easy_Level_Exists = LEVEL_NOT_ACCESSIBLE;
                        switch(Engine->On_Foot_State->On_Foot_Region.Goalposts[r]->Clear_Category)
                        {
                            case DIFFICULTY_EASY:
                                //Open the next Easy Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Easy_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;

                            case DIFFICULTY_MEDIUM:
                                //Open the next Medium Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Medium_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;

                            case DIFFICULTY_HARD:
                                //Open the next Hard Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Hard_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;
                        }
                    break;

                    case DIFFICULTY_MEDIUM:
                        Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple].Medium_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                        Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple].Medium_Level_Exists = LEVEL_NOT_ACCESSIBLE;
                        switch(Engine->On_Foot_State->On_Foot_Region.Goalposts[r]->Clear_Category)
                        {
                            case DIFFICULTY_EASY:
                                //Open the next Easy Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Easy_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;

                            case DIFFICULTY_MEDIUM:
                                //Open the next Medium Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Medium_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;

                            case DIFFICULTY_HARD:
                                //Open the next Hard Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Hard_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;
                        }
                    break;

                    case DIFFICULTY_HARD:
                        Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple].Hard_Level_Exit_Status = LEVEL_CLEARED_YELLOW_FORWARDS;
                        Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple].Hard_Level_Exists = LEVEL_NOT_ACCESSIBLE;
                        switch(Engine->On_Foot_State->On_Foot_Region.Goalposts[r]->Clear_Category)
                        {
                            case DIFFICULTY_EASY:
                                //Open the next Easy Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Easy_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Easy_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;

                            case DIFFICULTY_MEDIUM:
                                //Open the next Medium Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Medium_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Medium_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;

                            case DIFFICULTY_HARD:
                                //Open the next Hard Level!
                                if(Engine->This_Is_The_Current_Loaded_Level_Triple + 1 < Engine->Number_Of_Levels)
                                {
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Hard_Level_Exit_Status = LEVEL_ACCESSIBLE;
                                    Engine->Loaded_Levels[Engine->This_Is_The_Current_Loaded_Level_Triple+1].Hard_Level_Exists = LEVEL_ACCESSIBLE;
                                }
                            break;
                        }
                    break;
                }
                }
            }
        }


        if(Engine->On_Foot_State->Current_Player_State == PLAYER_IS_ON_FOOT || Engine->On_Foot_State->Current_Player_State == PLAYER_IS_ONBOARD)
        {
            mouseMotion(Engine, Keypad.Mouse_X, Keypad.Mouse_Y, Keypad);
            Handle_Item_Logic(Engine, Keypad);

            if(Engine->On_Foot_State->On_Foot_Player->Current_Location == LOCATION_ON_FOOT)
            {
            Handle_Player_Inputs(Engine, Engine->On_Foot_State->SENC, Engine->On_Foot_State->Connection_Status, Engine->On_Foot_State->On_Foot_Player, Keypad, &Engine->On_Foot_State->On_Foot_Region, &Engine->On_Foot_State->On_Foot_Region_File);
            }

            if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
            {
                Handle_Submarine_Object_Inputs(Engine, Keypad, 0);
            }

            else if(Engine->On_Foot_State->On_Foot_Player->Current_Location == LOCATION_SUBMARINE_VEHICLE)
            {
                Handle_Submarine_Object_Inputs(Engine, Keypad, 0);
                if(Keypad.Keyboard_F) //Dismount
                    {
                            printf("Dismounted Vehicle\n");
                            Engine->On_Foot_State->On_Foot_Player->Current_Location = LOCATION_ON_FOOT;
                            Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_x;
                            Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_y;
                            Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_z;

                            Engine->On_Foot_State->On_Foot_Player->Visible = true;
                            Engine->On_Foot_State->On_Foot_Player->Injurable = true;

                            for(int n = 0; n < 256; n++)
                            {
                                Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Passenger_IDs[Engine->On_Foot_State->On_Foot_Player->Am_This_Passenger].Name[n] = 0;
                            }
                            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Passenger_IDs[Engine->On_Foot_State->On_Foot_Player->Am_This_Passenger].Category = 0;
                            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Passenger_IDs[Engine->On_Foot_State->On_Foot_Player->Am_This_Passenger].Subcategory = 0;
                            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Passenger_IDs[Engine->On_Foot_State->On_Foot_Player->Am_This_Passenger].Variant = 0;
                            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Passenger_IDs[Engine->On_Foot_State->On_Foot_Player->Am_This_Passenger].ID = 0;

                            Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Number_Of_Passengers -= 1;
                    }
            }
        }

        if(Keypad.Keyboard_Start && !Engine->On_Foot_State->Pause_Button_Pressed)
        {
            if(Engine->On_Foot_State->Current_Player_State != PLAYER_IS_IN_CUTSCENE)
            {
                Engine->On_Foot_State->Pause_Button_Pressed = true;
                Engine->On_Foot_State->Current_State = STATE_PAUSED;
                Handle_Pausing_Logic(Engine, Keypad);
            }
        }

        if(Keypad.Keyboard_A)
        {
            if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
            {
                    Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair = false;
                    Engine->On_Foot_State->On_Foot_Player->Chair_ID = -1;
                    Engine->On_Foot_State->On_Foot_Player->Jump_Not_Pressed = false;
                    Engine->On_Foot_State->On_Foot_Player->displacement.z = 3;
            }
        }

        for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Characters; j++)
        {
            Generate_Player_Inputs(Engine->On_Foot_State->SENC, Engine->On_Foot_State->Connection_Status, Engine, Engine->On_Foot_State->On_Foot_Region.Characters[j], Keypad, &Engine->On_Foot_State->On_Foot_Region, &Engine->On_Foot_State->On_Foot_Region_File);
        }
    }

    else if(Engine->On_Foot_State->Current_State == STATE_PAUSED)
    {
        Handle_Pause_Nav_Subroutine(Engine, Keypad);
    }

    if(!Keypad.Keyboard_Start)
    {
        Engine->On_Foot_State->Pause_Button_Pressed = false;
    }

    if(!Keypad.Keyboard_E)
    {
        Engine->On_Foot_State->Interact_Not_Pressed = true;
    }
    }
}


void Process_Doors(struct _Player_Submarine* On_Foot_Player, struct _On_Foot_Region* On_Foot_Region, struct _On_Foot_Region_File* On_Foot_Region_File)
{
    for(int j = 0; j < On_Foot_Region_File->Number_Of_Doors; j++)
    {
        double x = Get_Distance(On_Foot_Region->Doors[j]->x,On_Foot_Region->Doors[j]->y,On_Foot_Region->Doors[j]->z+33,On_Foot_Player->x,On_Foot_Player->y,On_Foot_Player->z);
        //printf("%f\n", x);
        if(x < 35)
        {
            //Then teleport me to destination door.
        }
    }
}

void Handle_Unpaused_Processing(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Connection_Status == 1)
    {

    }

    Process_Doors(Engine->On_Foot_State->On_Foot_Player, &Engine->On_Foot_State->On_Foot_Region, &Engine->On_Foot_State->On_Foot_Region_File);
    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Ships; j++)
    {
        Process_Doors(Engine->On_Foot_State->On_Foot_Player, &Engine->On_Foot_State->Ship_Objects[j]->Ship_Interior, &Engine->On_Foot_State->Ship_Objects[j]->Ship_Interior_File);
    }


    double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        if(x > Engine->On_Foot_State->Animation_Fired + 0.1) //10 fps
        {
            // printf("Animation fired!\n");
            Engine->On_Foot_State->Animation_Fired = x;
        }

        if(x > Engine->On_Foot_State->Cooldown_Fired + Engine->On_Foot_State->On_Foot_Player->Current_Cooldown_Time) //10 fps
        {
            if(Engine->On_Foot_State->Weapon_Fired)
            {
                printf("Cooldown fired!\n");
                Engine->On_Foot_State->Weapon_Fired = false;
                Engine->On_Foot_State->Cooldown_Fired = 0;
            }
        }

        if(x > Engine->On_Foot_State->Reload_Fired + Engine->On_Foot_State->On_Foot_Player->Current_Reload_Time) //10 fps
        {
            if(Engine->On_Foot_State->Need_To_Reload)
            {
                printf("Reload fired!\n");
                Engine->On_Foot_State->Need_To_Reload = false;
                Engine->On_Foot_State->Cooldown_Fired = 0;

                //Determine how much ammo to reload.
                if(Engine->On_Foot_State->On_Foot_Player->Current_Stored_Ammo > 0)
                {
                    if(Engine->On_Foot_State->On_Foot_Player->Current_Ammo_Per_Clip <= Engine->On_Foot_State->On_Foot_Player->Current_Stored_Ammo)
                    {
                        //Then give me Current_Ammo_Per_Clip.
                        Engine->On_Foot_State->On_Foot_Player->Current_Ammo += Engine->On_Foot_State->On_Foot_Player->Current_Ammo_Per_Clip;
                        Engine->On_Foot_State->On_Foot_Player->Current_Stored_Ammo -= Engine->On_Foot_State->On_Foot_Player->Current_Ammo_Per_Clip;
                    }

                    else if(Engine->On_Foot_State->On_Foot_Player->Current_Ammo_Per_Clip > Engine->On_Foot_State->On_Foot_Player->Current_Stored_Ammo)
                    {
                        //Then give me Current_Ammo_Per_Clip.
                        Engine->On_Foot_State->On_Foot_Player->Current_Ammo += Engine->On_Foot_State->On_Foot_Player->Current_Stored_Ammo;
                        Engine->On_Foot_State->On_Foot_Player->Current_Stored_Ammo = 0;
                    }
                }
            }
        }

        for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Vehicles; j++)
    {
        Process_Submarine_Object(Engine,j);
    }

        if(Engine->On_Foot_State->On_Foot_Player->Current_Location == LOCATION_ON_FOOT)
        {

            Engine->On_Foot_State->Camera_X = Engine->On_Foot_State->On_Foot_Player->x;
            Engine->On_Foot_State->Camera_Y = Engine->On_Foot_State->On_Foot_Player->y;
            Engine->On_Foot_State->Camera_Z = Engine->On_Foot_State->On_Foot_Player->z+40;

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(Engine->On_Foot_State->Camera_X, Engine->On_Foot_State->Camera_Y, Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Camera_X + Engine->On_Foot_State->Camera_Front_X, Engine->On_Foot_State->Camera_Y + Engine->On_Foot_State->Camera_Front_Y, Engine->On_Foot_State->Camera_Z + Engine->On_Foot_State->Camera_Front_Z,0,0,1);

            GLfloat light_position[] = { (float)Engine->On_Foot_State->Camera_X, (float)Engine->On_Foot_State->Camera_Y, (float)Engine->On_Foot_State->Camera_Z, 1.0 };
            glLightfv(GL_LIGHT1, GL_POSITION, light_position);
            GLfloat spot_direction[] = { (float)Engine->On_Foot_State->Camera_Front_X, (float)Engine->On_Foot_State->Camera_Front_Y, (float)Engine->On_Foot_State->Camera_Front_Z};
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
        }


        else if(Engine->On_Foot_State->On_Foot_Player->Current_Location == LOCATION_SUBMARINE_VEHICLE)
        {
                Vec3 cam;
                cam.x = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_x - Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Forward_Vector.x * 2000 + Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.x * 30.0f;
                cam.y = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_y - Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Forward_Vector.y * 2000 + Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.y * 30.0f;
                cam.z = Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_z - Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Forward_Vector.z * 2000 + Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.z * 30.0f;

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(cam.x, cam.y, cam.z, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_x, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_y, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->plane_z, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.x, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.y, Engine->On_Foot_State->On_Foot_Region.Submarine_Vehicles[0]->Up_Vector.z);

        }
}

void Process_On_Foot_State(struct _Engine* Engine, struct _Audio_Chip* Audio_Chip)
{

    for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Seats; j++)
    {
        if(Engine->On_Foot_State->On_Foot_Region.Seats[j]->New_Computer->Power_State)
        {
            Process_New_Computer_Cycles(Engine, Engine->On_Foot_State->On_Foot_Region.Seats[j], 10, Audio_Chip);
        }
    }


    if(Engine->On_Foot_State->Render)
    {
    if(Engine->On_Foot_State->Current_State == STATE_UNPAUSED)
    {
        Handle_Unpaused_Processing(Engine);
    }



    if(Engine->On_Foot_State->Connection_Status == 1)
    {
        network_update(Engine->On_Foot_State->SENC);
        Engine->On_Foot_State->On_Foot_Player->x = Engine->On_Foot_State->SENC->local_players[Engine->On_Foot_State->SENC->my_id].x;
        Engine->On_Foot_State->On_Foot_Player->y = Engine->On_Foot_State->SENC->local_players[Engine->On_Foot_State->SENC->my_id].y;
       // Engine->On_Foot_State->On_Foot_Player->z = Engine->On_Foot_State->SENC->local_players[Engine->On_Foot_State->SENC->my_id].z;

        for(int j = 0; j < MAX_PLAYERS; j++)
        {
            if(j != Engine->On_Foot_State->SENC->my_id)
            {
                Engine->On_Foot_State->On_Foot_Region.Characters[j]->x = Engine->On_Foot_State->SENC->local_players[j].x;
                Engine->On_Foot_State->On_Foot_Region.Characters[j]->y = Engine->On_Foot_State->SENC->local_players[j].y;
               //Engine->On_Foot_State->On_Foot_Region.Characters[j]->z = Engine->On_Foot_State->SENC->local_players[j].z;
                Engine->On_Foot_State->On_Foot_Region.Characters[j]->Pitch_Angle = Engine->On_Foot_State->SENC->local_players[j].pitch;
                Engine->On_Foot_State->On_Foot_Region.Characters[j]->Yaw_Angle = Engine->On_Foot_State->SENC->local_players[j].yaw;
            }
        }
    }
    }

    double z = Get_Ticks(Engine->On_Foot_State->Go_Back_Timer);
    if(z > 2 && Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
    {
        if(Engine->This_Is_The_Current_Loaded_Level_Triple >= Engine->Number_Of_Levels-1)
        {
            //Then go to Arc Cleared!
            glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_NORMALIZE);

            Engine->The_End_State = Create_The_End_State();
            Initialize_The_End_State(Engine);
            Engine->Current_State = STATE_THE_END;
        }

        else if(Engine->This_Is_The_Current_Loaded_Level_Triple < Engine->Number_Of_Levels-1)
        {
            glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_NORMALIZE);
        Reinitialize_Overworld_State(Engine);
        Engine->Current_State = STATE_OVERWORLD;
        }
    }
}

void updateCameraVectors(struct _Engine* Engine)
{
    float radYaw = Engine->On_Foot_State->Camera_Yaw * (M_PI / 180.0f);
    float radPitch = Engine->On_Foot_State->Camera_Pitch * (M_PI / 180.0f);
    Engine->On_Foot_State->Camera_Front_X = cos(radYaw) * cos(radPitch);
    Engine->On_Foot_State->Camera_Front_Y = sin(radYaw) * cos(radPitch);
    Engine->On_Foot_State->Camera_Front_Z = sin(radPitch);
    float length = sqrt(Engine->On_Foot_State->Camera_Front_X * Engine->On_Foot_State->Camera_Front_X + Engine->On_Foot_State->Camera_Front_Y * Engine->On_Foot_State->Camera_Front_Y + Engine->On_Foot_State->Camera_Front_Z * Engine->On_Foot_State->Camera_Front_Z);
    Engine->On_Foot_State->Camera_Front_X /= length;
    Engine->On_Foot_State->Camera_Front_Y /= length;
    Engine->On_Foot_State->Camera_Front_Z /= length;

    Engine->On_Foot_State->Light_Front_X = cos(radYaw-3.14159) * cos(radPitch);
    Engine->On_Foot_State->Light_Front_Y = sin(radYaw-3.14159) * cos(radPitch);
    Engine->On_Foot_State->Light_Front_Z = sin(radPitch);
    float llength = sqrt(Engine->On_Foot_State->Light_Front_X * Engine->On_Foot_State->Light_Front_X + Engine->On_Foot_State->Light_Front_Y * Engine->On_Foot_State->Light_Front_Y + Engine->On_Foot_State->Light_Front_Z * Engine->On_Foot_State->Light_Front_Z);
    Engine->On_Foot_State->Light_Front_X /= llength;
    Engine->On_Foot_State->Light_Front_Y /= llength;
    Engine->On_Foot_State->Light_Front_Z /= llength;
}

void mouseMotion(struct _Engine* Engine, double x, double y,struct _Keypad Keypad)
{
    int centerX = Keypad.Screen_Width / 2;
    int centerY = Keypad.Screen_Height / 2;

    float xoffset = (x - centerX) * Engine->On_Foot_State->Camera_Sensitivity;
    float yoffset = (centerY - y) * Engine->On_Foot_State->Camera_Sensitivity;

    if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
    {
        xoffset = Keypad.Controller.Controller_Camera_Thumbpad[0];
        yoffset = Keypad.Controller.Controller_Camera_Thumbpad[1]*-1;
    }

    Engine->On_Foot_State->Camera_Pitch += yoffset;

    Engine->On_Foot_State->Camera_Yaw -= xoffset;

    // Constrain Pitch
    if (Engine->On_Foot_State->Camera_Pitch > 89.0f) Engine->On_Foot_State->Camera_Pitch = 89.0f;
    if (Engine->On_Foot_State->Camera_Pitch < -89.0f) Engine->On_Foot_State->Camera_Pitch = -89.0f;

    if(Engine->On_Foot_State->Camera_Yaw > 359)
    {
        Engine->On_Foot_State->Camera_Yaw -= 360;
    }

    if(Engine->On_Foot_State->Camera_Yaw < 0)
    {
        Engine->On_Foot_State->Camera_Yaw += 360;
    }

    updateCameraVectors(Engine);

    if (x != centerX || y != centerY)
    {
        glfwSetCursorPos(Engine->W, centerX, centerY);
    }

    Engine->On_Foot_State->On_Foot_Player->Yaw_Angle = Engine->On_Foot_State->Camera_Yaw;
}

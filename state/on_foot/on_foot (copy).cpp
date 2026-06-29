/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "on_foot.h"
#include <stdio.h>
#include <string.h>
void Enter_Subspace(struct _Engine* Engine)
{
    //Super Mario Bros. 2 callback
    if(!Engine->On_Foot_State->Is_In_Subspace)
    {
        Engine->On_Foot_State->Is_In_Subspace = true;
        glClearColor(0.34375,0.46875,0.65625, 1.f);
        GLfloat gen_ambient[] = { 0.25,0.25,0.25,1.0 }; // Base light so shadows aren't pitch black
        GLfloat gen_diffuse[] = { 0.25,0.25,0.25,1.0 }; // Dim diffuse light
        GLfloat gen_specular[] = { 0.25,0.25,0.25,1.0 }; // Very little shine

        glLightfv(GL_LIGHT1, GL_AMBIENT, gen_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, gen_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, gen_specular);

        glLightfv(GL_LIGHT2, GL_AMBIENT, gen_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, gen_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, gen_specular);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if(Engine->On_Foot_State->Is_In_Subspace)
        {
            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 25);
        }

        else if(!Engine->On_Foot_State->Is_In_Subspace)
        {
            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
        }
        glCullFace(GL_BACK); // Temporarily flip culling
    }
}

void Exit_Subspace(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Is_In_Subspace)
    {
        Engine->On_Foot_State->Is_In_Subspace = false;
        glClearColor(0,0,0,1.f);
        GLfloat gen_ambient[] = { 5.0,5.0,5.0,1.0 }; // Base light so shadows aren't pitch black
        GLfloat gen_diffuse[] = { 5.0,5.0,5.0,1.0 }; // Dim diffuse light
        GLfloat gen_specular[] = { 5.0,5.0,5.0,1.0 }; // Very little shine

        glLightfv(GL_LIGHT1, GL_AMBIENT, gen_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, gen_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, gen_specular);

        GLfloat gn_ambient[] = { 0.45, 0.8, 0.45, 1.0 }; // Base light so shadows aren't pitch black
        GLfloat gn_diffuse[] = { 0.45, 0.8, 0.45, 1.0 }; // Dim diffuse light
        GLfloat gn_specular[] = { 0.45, 0.8, 0.45, 1.0 }; // Very little shine

        glLightfv(GL_LIGHT2, GL_AMBIENT, gn_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, gn_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, gn_specular);

        // Position the general light
        // IMPORTANT: The 0.0 at the end makes this a DIRECTIONAL light (like the sun)
        // coming from the top-right-front, rather than a positional point light.
        GLfloat gn_position[] = { 0.8, 0.8, 0.8, 0.0 };
        glLightfv(GL_LIGHT2, GL_POSITION, gn_position);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glCullFace(GL_BACK); // Temporarily flip culling
    }
}

struct _On_Foot_State* Create_On_Foot_State()
{
    struct _On_Foot_State* M = (struct _On_Foot_State*) calloc(1,sizeof(struct _On_Foot_State));
    return M;
}
void Destroy_On_Foot_State(struct _Engine* Engine)
{

}

void Load_Ship(struct _Engine* Engine, int Ship_Index, const char* Filename)
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

    char Shell_Model_Name[256];
    char Shell_Texture_Filepath[256];
    for(int j = 0; j < 256; j++)
    {
        Shell_Model_Name[j] = Buffer[Buffer_Location+j];
        Shell_Texture_Filepath[j] = Buffer[Buffer_Location+j+256];
    }

    Engine->On_Foot_State->Ship_Objects[Ship_Index]->Ship_Shell_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Ship_Objects[Ship_Index]->Ship_Shell_Model, Shell_Model_Name);
    Load_SE3_Texture(Engine->On_Foot_State->Ship_Objects[Ship_Index]->Ship_Shell_Model, Shell_Texture_Filepath);

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index] = (struct _Ship_Internals_Pointer*) calloc(1,sizeof(struct _Ship_Internals_Pointer));

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position = Engine->On_Foot_State->Safe_X_Position;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position = Engine->On_Foot_State->Safe_Y_Position;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position = Engine->On_Foot_State->Safe_Z_Position;

    //printf("I will place the ship at z=%f\n", Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position);

    Engine->On_Foot_State->Safe_Z_Position += SHIP_INTERNALS_Z_MARGIN;

    Buffer_Location += 512;


    for(int j = 0; j < 256; j++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Region_Name[j] = Buffer[Buffer_Location+j];
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Region_Description[j] = Buffer[Buffer_Location+j+256];
    }
    Buffer_Location += 512;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Default_Spawnpoint[0] = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Default_Spawnpoint[1] = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Default_Spawnpoint[2] = *(double*)&(Buffer[Buffer_Location+16]);
    Buffer_Location += 24;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Skyboxes = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Ships = *(int*)&(Buffer[Buffer_Location+4]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Wall_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Ceiling_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+12]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Goalposts = *(int*)&(Buffer[Buffer_Location+16]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Characters = *(int*)&(Buffer[Buffer_Location+20]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Cars = *(int*)&(Buffer[Buffer_Location+24]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Submarines = *(int*)&(Buffer[Buffer_Location+28]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Fires = *(int*)&(Buffer[Buffer_Location+32]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Terrains = *(int*)&(Buffer[Buffer_Location+36]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Docking_Ports = *(int*)&(Buffer[Buffer_Location+40]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Switches = *(int*)&(Buffer[Buffer_Location+44]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_On_Board_Cameras = *(int*)&(Buffer[Buffer_Location+48]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_RT_Screens = *(int*)&(Buffer[Buffer_Location+52]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Slope_Collision_Meshes= *(int*)&(Buffer[Buffer_Location+56]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Seats= *(int*)&(Buffer[Buffer_Location+60]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Vortexes= *(int*)&(Buffer[Buffer_Location+64]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_TVs= *(int*)&(Buffer[Buffer_Location+68]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Combo_Seat_Screens= *(int*)&(Buffer[Buffer_Location+72]);

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Skyboxes = *(int*)&(Buffer[Buffer_Location+76]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Ships = *(int*)&(Buffer[Buffer_Location+80]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Wall_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+84]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Ceiling_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+88]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Goalposts = *(int*)&(Buffer[Buffer_Location+92]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Characters = *(int*)&(Buffer[Buffer_Location+96]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Cars = *(int*)&(Buffer[Buffer_Location+100]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Submarines = *(int*)&(Buffer[Buffer_Location+104]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Fires = *(int*)&(Buffer[Buffer_Location+108]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Terrains = *(int*)&(Buffer[Buffer_Location+112]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Docking_Ports = *(int*)&(Buffer[Buffer_Location+116]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Switches = *(int*)&(Buffer[Buffer_Location+120]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_On_Board_Cameras = *(int*)&(Buffer[Buffer_Location+124]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_RT_Screens = *(int*)&(Buffer[Buffer_Location+128]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Slope_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+132]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Seats= *(int*)&(Buffer[Buffer_Location+136]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Vortexes= *(int*)&(Buffer[Buffer_Location+140]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_TVs= *(int*)&(Buffer[Buffer_Location+144]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Combo_Seat_Screens= *(int*)&(Buffer[Buffer_Location+148]);

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Combo_Seat_Screens; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Combo_Seat_Screen_Object[m] = (struct _Combo_Seat_Screen_Object*)calloc(1,sizeof(struct _Combo_Seat_Screen_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Wall_Collision_Meshes; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[m] = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Ceiling_Collision_Meshes; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[m] = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Terrains; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[m] = (struct _Terrain_Object*)calloc(1,sizeof(struct _Terrain_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Docking_Ports; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[m] = (struct _Docking_Port*)calloc(1,sizeof(struct _Docking_Port));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Switches; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[m] = (struct _Switch_Object*)calloc(1,sizeof(struct _Switch_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_On_Board_Cameras; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[m] = (struct _Camera_Object*)calloc(1,sizeof(struct _Camera_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_RT_Screens; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[m] = (struct _RT_Screen_Object*)calloc(1,sizeof(struct _RT_Screen_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Slope_Collision_Meshes; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Slope_Collision_Meshes[m] = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));
    }

    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Seats; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[m] = (struct _Seat_Object*)calloc(1,sizeof(struct _Seat_Object));
    }



    for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_TVs; m++)
    {
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[m] = (struct _TV_Object*)calloc(1,sizeof(struct _TV_Object));
    }


    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Seats; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Seats + (j * sizeof(struct _Seat));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        Buffer_Location += 512;

        double x = *(double*)&(Buffer[Buffer_Location]);
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

        int Drivers_Seat = *(int*)&(Buffer[Buffer_Location+96]);


        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Model, Texture_Filename);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->x = x;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->y = y;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->z = z;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.min.x = x1 + x;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.min.y = y1 + y;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.min.z = z1 + z;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.max.x = x2 + x;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.max.y = y2 + y;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.max.z = z2 + z;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Eye_Offset_x = Eye_Offset_x + x;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Eye_Offset_y = Eye_Offset_y + y;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Eye_Offset_z = Eye_Offset_z + z;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.min.z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Collision_Box.max.z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Eye_Offset_z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Seat_Objects[j]->Is_A_Drivers_Seat = Drivers_Seat;
    }

    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_RT_Screens; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_RT_Screens + (j * sizeof(struct _RT_Screen));

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->Status = *(int*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->Channel = *(int*)&(Buffer[Buffer_Location+4]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->x = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->y = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->z = *(double*)&(Buffer[Buffer_Location+24]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

        Buffer_Location += 32;

        char Model_Filepath[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filepath[l] = Buffer[Buffer_Location+l];
        }

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->RT_Screen_Objects[j]->Model, Model_Filepath);
    }

    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_On_Board_Cameras; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_On_Board_Cameras + (j * sizeof(struct _Camera));

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->Look_x = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->Look_y = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->Look_z = *(double*)&(Buffer[Buffer_Location+40]);

        glGenFramebuffers(1, &Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->fbo);

        glGenTextures(1, &Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256,256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->textureColorBuffer, 0);

        glGenRenderbuffers(1, &Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 256,256);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }




     for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_TVs; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_TVs + (j * sizeof(struct _On_Foot_TV));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        Buffer_Location += 512;


        double x = *(double*)&(Buffer[Buffer_Location]);
    double y = *(double*)&(Buffer[Buffer_Location+8]);
    double z = *(double*)&(Buffer[Buffer_Location+16]);

    double Screen_x1 = *(double*)&(Buffer[Buffer_Location+24]);
    double Screen_y1 = *(double*)&(Buffer[Buffer_Location+32]);
    double Screen_z1 = *(double*)&(Buffer[Buffer_Location+40]);

    double Screen_x2 = *(double*)&(Buffer[Buffer_Location+48]);
    double Screen_y2 = *(double*)&(Buffer[Buffer_Location+56]);
    double Screen_z2 = *(double*)&(Buffer[Buffer_Location+64]);

    double Screen_x3 = *(double*)&(Buffer[Buffer_Location+72]);
    double Screen_y3 = *(double*)&(Buffer[Buffer_Location+80]);
    double Screen_z3 = *(double*)&(Buffer[Buffer_Location+88]);

    double Screen_x4 = *(double*)&(Buffer[Buffer_Location+96]);
    double Screen_y4 = *(double*)&(Buffer[Buffer_Location+104]);
    double Screen_z4 = *(double*)&(Buffer[Buffer_Location+112]);

    double Receiver_Frequency = *(double*)&(Buffer[Buffer_Location+120]);
    double Receiver_Audio_Frequency = *(double*)&(Buffer[Buffer_Location+128]);
    int State = *(int*)&(Buffer[Buffer_Location+136]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Model,Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Model,Texture_Filename);

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->x = x;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->y = y;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->z = z+Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_x1 = Screen_x1;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_y1 = Screen_y1;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_z1 = Screen_z1;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_x2 = Screen_x2;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_y2 = Screen_y2;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_z2 = Screen_z2;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_x3 = Screen_x3;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_y3 = Screen_y3;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_z3 = Screen_z3;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_x4 = Screen_x4;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_y4 = Screen_y4;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Screen_z4 = Screen_z4;


    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Receiver_Frequency = Receiver_Frequency;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Receiver_Audio_Frequency = Receiver_Audio_Frequency;
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->State = State;

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Broadcaster = Create_Broadcaster();
    Encode_Image(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Broadcaster,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Camera_Objects[j]->textureColorBuffer,5500,120);
    Receive_Image(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->TV_Objects[j]->Broadcaster, 5500, 120,1,0);

    }





    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Terrains; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Terrains + (j * sizeof(struct _Terrain));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        ////printf(Model_Filename);
        ////printf(Texture_Filename);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->Model, Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Terrain_Objects[j]->z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;
    }


    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Switches; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Switches + (j * sizeof(struct _Switch));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Model, Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Frame = 0;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.min.x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.min.y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.min.z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.max.x = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.max.y = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.max.z = *(double*)&(Buffer[Buffer_Location+40]);


        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->State = *(int*)&(Buffer[Buffer_Location+48]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->z += Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.min.z += Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box.max.z += Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;


    }


    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Wall_Collision_Meshes + (j * 256);

        char Model_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
        }
        ////printf(Model_Filename);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j] = Create_Preloaded_Collision_Mesh(Model_Filename);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j]->z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Ceiling_Collision_Meshes + (j * 256);

        char Model_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
        }
        ////printf(Model_Filename);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j] = Create_Preloaded_Collision_Mesh(Model_Filename);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j]->z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;
    }

    //printf("Loading Docking Ports\n");
    for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Docking_Ports; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Docking_Ports + (j * sizeof(struct _Docking_Port));

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Radius = *(double*)&(Buffer[Buffer_Location+24]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_X = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Y = *(double*)&(Buffer[Buffer_Location+40]);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Z = *(double*)&(Buffer[Buffer_Location+48]);

        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;

        //printf("Docking port Dest: %f %f %f\n", Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_X,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Y,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Z);
    }
}

void Initialize_On_Foot_State(struct _Engine* Engine)
{
    srand(time(NULL));
    Engine->On_Foot_State->Time_Refreshed_Screens = 0;
    Engine->On_Foot_State->Shaky_Camera = false;

    Engine->On_Foot_State->FPS_Reticle = Create_Image();
    BMPtoSEI("resources/artwork/gui/fps_reticle.bmp","resources/artwork/gui/fps_reticle.sei");
    Load_Image(Engine->On_Foot_State->FPS_Reticle, "resources/artwork/gui/fps_reticle.sei");

    Engine->On_Foot_State->Subspace_Hole_x = 100;
    Engine->On_Foot_State->Subspace_Hole_y = 100;
    Engine->On_Foot_State->Subspace_Hole_z = 30;

    Engine->On_Foot_State->Universal_Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Universal_Timer);

    Engine->On_Foot_State->Cooldown_Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Cooldown_Timer);
    Pause_Timer(Engine->On_Foot_State->Cooldown_Timer);
    Engine->On_Foot_State->Cooldown_Complete = true;

    Engine->USND_Thud = Mix_LoadWAV( "resources/tracks_and_sounds/general/thud.wav" );
    Engine->USND_Walking = Mix_LoadWAV( "resources/tracks_and_sounds/general/carpet.wav" );
    Engine->USND_Fire = Mix_LoadWAV( "resources/tracks_and_sounds/general/thud.wav" );
    Engine->USND_Flashlight = Mix_LoadWAV( "resources/tracks_and_sounds/general/select.wav" );
    Engine->USND_Explosion = Mix_LoadWAV( "resources/tracks_and_sounds/general/anime_explosion.wav" );
    Engine->USND_Glass_Thud = Mix_LoadWAV( "resources/tracks_and_sounds/general/glass_thud.wav" );
    Engine->USND_Glass_Roll = Mix_LoadWAV( "resources/tracks_and_sounds/general/glass_roll.wav" );

    Engine->USND_Vortex = Mix_LoadWAV( "resources/tracks_and_sounds/general/vortex.wav" );

    Engine->USND_Computer_Beep = Mix_LoadWAV( "resources/tracks_and_sounds/general/computer_keyboard_value_adjustment.wav");

    Engine->On_Foot_State->Debug = false;

    Engine->On_Foot_State->Interact_Not_Pressed = true;

    Engine->On_Foot_State->Flashlight = true;

    Engine->On_Foot_State->LightMag = 1;

    Engine->On_Foot_State->Reticle_Three = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Reticle_Three, "resources/models/shooting_hand.se3");
    Load_SE3_Texture(Engine->On_Foot_State->Reticle_Three, "notexture");

    Engine->On_Foot_State->Debug_Text_Player_X = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Y = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Z = Create_Image();

    Engine->On_Foot_State->Debug_Text_Player_Yaw = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Pitch = Create_Image();

    Engine->On_Foot_State->Debug_Text_Player_Look_X = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Look_Y = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Look_Z = Create_Image();

    Engine->On_Foot_State->Debug_Text_Player_Flashlight_On = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Flashlight_Off = Create_Image();

    Engine->On_Foot_State->Ammo_Count = Create_Image();

    double number = 3;
    char buffer[256]; // Allocate a buffer large enough for the string representation

    // Use sprintf to format the double into the buffer
    // "%f" is the format specifier for a double
    sprintf(buffer, "%f", number);



    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_X, buffer, 16, 16, 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Y, buffer, 16, 16, 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Z, buffer, 16, 16, 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Yaw, buffer, 16, 16, 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Pitch, buffer, 16, 16, 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Look_X, buffer, 16, 16, 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Look_Y, buffer, 16, 16, 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Look_Z, buffer, 16, 16, 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Flashlight_On, buffer, 16, 16, 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Flashlight_Off, buffer, 16, 16, 64);


    Write_Ship("resources/data/ships/test_ship_details.det");

    Engine->On_Foot_State->Safe_X_Position = 0;
    Engine->On_Foot_State->Safe_Y_Position = 0;
    Engine->On_Foot_State->Safe_Z_Position = SHIP_INTERNALS_Z_MARGIN;

    Engine->On_Foot_State->FPS_Mode = true;

    if(Engine->On_Foot_State->FPS_Mode)
    {
        glfwSetInputMode(Engine->W, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

        Engine->On_Foot_State->width = 1920;
    Engine->On_Foot_State->height = 1080-32-32;

    // Camera configuration
    Engine->On_Foot_State->yaw = 0;   // Vertical angle (degrees)
    Engine->On_Foot_State->pitch = 0.0f;   // Vertical angle (degrees)
    Engine->On_Foot_State->sensitivity = 0.01f; // Mouse sensitivity

    // Camera Vectors
    Engine->On_Foot_State->camX = 0.0f;
    Engine->On_Foot_State->camY = 0.0f;
    Engine->On_Foot_State->camZ = 50.0f; // Camera position
    Engine->On_Foot_State->frontX = 0.0f;
    Engine->On_Foot_State->frontY = 1.0f;
    Engine->On_Foot_State->frontZ = 0.0f; // Where we are looking
    Engine->On_Foot_State->upX = 0.0f;
    Engine->On_Foot_State->upY = 0.0f;
    Engine->On_Foot_State->upZ = 1.0f; // The Up direction

    Engine->On_Foot_State->Current_State = STATE_ABOVE_WATER;
    Engine->On_Foot_State->r = 0;

    Engine->On_Foot_State->Player_Is_In_Car = false;
    Engine->On_Foot_State->In_This_Car = -1;

    Engine->On_Foot_State->Player_Is_In_Submarine = false;
    Engine->On_Foot_State->In_This_Submarine = -1;

    Write_Region(Engine, "region");
    Load_Region(Engine, "region");

    Engine->On_Foot_State->Go_Back_Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Go_Back_Timer);
    Pause_Timer(Engine->On_Foot_State->Go_Back_Timer);


    Engine->On_Foot_State->Death_Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Death_Timer);
    Pause_Timer(Engine->On_Foot_State->Death_Timer);



    Engine->On_Foot_State->Enter_Not_Pressed = true;
    Engine->On_Foot_State->Mini_State = MINI_STATE_UNPAUSED;
    Engine->On_Foot_State->Current_Choice = 0;

    glColor3f(1,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH); // Enable smooth shading (Gouraud shading)
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(Engine->On_Foot_State->Is_In_Subspace)
{
 gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 25);
}

else if(!Engine->On_Foot_State->Is_In_Subspace)
{
 gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
}
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40,40,40,0,0,20,0,0,1);





    // Somewhere in the initialization part of your program…
    // Enable depth testing and lighting
    // --- Enable Depth Test and Smooth Shading ---

glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // 1. Enable our two lights
    glEnable(GL_LIGHT1); // Our new general scene light

    // --- MATERIAL SETUP ---
    GLfloat mat_specular[] = { 0.7, 1.0, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


    // --- LIGHT 1: GENERAL LIGHT (Dim & everywhere) ---
    GLfloat gen_ambient[] = { 0.5,1.0,0.5,1.0 }; // Base light so shadows aren't pitch black
    GLfloat gen_diffuse[] = { 0.5,1.0,0.5,1.0 }; // Dim diffuse light
    GLfloat gen_specular[] = { 0.5,1.0,0.5,1.0 }; // Very little shine

    glLightfv(GL_LIGHT1, GL_AMBIENT, gen_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, gen_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, gen_specular);

    // Position the general light
    // IMPORTANT: The 0.0 at the end makes this a DIRECTIONAL light (like the sun)
    // coming from the top-right-front, rather than a positional point light.
//    GLfloat gen_position[] = { 1.0, -1.0, 1.0, 0.0 };
 //   glLightfv(GL_LIGHT1, GL_POSITION, gen_position);


    glEnable(GL_LIGHT2);
    // --- LIGHT 1: GENERAL LIGHT (Dim & everywhere) ---
    GLfloat gn_ambient[] = { 0.73, 2, 0.73, 1.0 }; // Base light so shadows aren't pitch black
    GLfloat gn_diffuse[] = { 0.73,2, 0.73, 1.0 }; // Dim diffuse light
    GLfloat gn_specular[] = {0.73, 2, 0.73, 1.0 }; // Very little shine

    glLightfv(GL_LIGHT2, GL_AMBIENT, gn_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, gn_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, gn_specular);

    // Position the general light
    // IMPORTANT: The 0.0 at the end makes this a DIRECTIONAL light (like the sun)
    // coming from the top-right-front, rather than a positional point light.
    GLfloat gn_position[] = { 0.8, 0.8, 40, 0.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, gn_position);







    // 1. Enable our two lights
    glEnable(GL_LIGHT3); // Explosion

    // --- MATERIAL SETUP ---
    GLfloat matr_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat matr_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matr_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matr_shininess);


    // --- LIGHT 1: GENERAL LIGHT (Dim & everywhere) ---
    GLfloat genr_ambient[] = { 10.0,0.0,0.0,1.0 }; // Base light so shadows aren't pitch black
    GLfloat genr_diffuse[] = { 10.0,0.0,0.0,1.0 }; // Dim diffuse light
    GLfloat genr_specular[] = { 10.0,0.0,0.0,1.0 }; // Very little shine

    glLightfv(GL_LIGHT3, GL_AMBIENT, genr_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, genr_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, genr_specular);
    GLfloat gnr_position[] = { -200,-200,-200, 0.0 };
    glLightfv(GL_LIGHT3, GL_POSITION, gnr_position);
    glDisable(GL_LIGHT3);




    Engine->On_Foot_State->Camera_Y = 180;
    Engine->On_Foot_State->Camera_Z = 40;

    Engine->On_Foot_State->Reticle = Create_Image();
    Load_Image(Engine->On_Foot_State->Reticle, "resources/artwork/gui/submarine_hud_iff_reticle.sei");

    Engine->On_Foot_State->Resume = Create_Image();
    Engine->On_Foot_State->Exit = Create_Image();
    Engine->On_Foot_State->Inventory = Create_Image();

    Load_Image(Engine->On_Foot_State->Resume, "resources/artwork/gui/english/resume.sei");
    Load_Image(Engine->On_Foot_State->Exit, "resources/artwork/gui/english/exit_to_level_select.sei");
    Load_Image(Engine->On_Foot_State->Inventory, "resources/artwork/gui/english/exit_to_title.sei");





    Engine->On_Foot_State->Resume_Highlighted = Create_Image();
    Engine->On_Foot_State->Exit_Highlighted = Create_Image();
    Engine->On_Foot_State->Inventory_Highlighted = Create_Image();

    Load_Image(Engine->On_Foot_State->Resume_Highlighted, "resources/artwork/gui/english/resume_highlighted.sei");
    Load_Image(Engine->On_Foot_State->Exit_Highlighted, "resources/artwork/gui/english/exit_to_level_select_highlighted.sei");
    Load_Image(Engine->On_Foot_State->Inventory_Highlighted, "resources/artwork/gui/english/exit_to_title_highlighted.sei");

    Engine->On_Foot_State->Current_Choice = 0;

    Engine->On_Foot_State->Up_Not_Pressed = false;
    Engine->On_Foot_State->Down_Not_Pressed = false;

    Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = false;

    Engine->On_Foot_State->Selector_Y = 52;

    Engine->On_Foot_State->On_Foot_Player = Create_Player();
    Initialize_On_Foot_Player(Engine, "none", 0,0,0);
    Engine->On_Foot_State->On_Foot_Player->Player_Type = PLAYER_TYPE_ON_FOOT;
    Engine->On_Foot_State->On_Foot_Player->Throttle = 0;

    Engine->On_Foot_State->Torpedo_Billboard = Create_Image();
    Load_Image(Engine->On_Foot_State->Torpedo_Billboard, "resources/artwork/gui/torpedo.sei");

    Engine->On_Foot_State->Torpedo_Explosion_Billboard = Create_Image();
    Load_Image(Engine->On_Foot_State->Torpedo_Explosion_Billboard, "resources/artwork/gui/explosion.sei");



    Engine->On_Foot_State->Torpedo_Explosion_Billboard_Two = Create_Image();
    Load_Image(Engine->On_Foot_State->Torpedo_Explosion_Billboard_Two, "resources/artwork/gui/explosion2.sei");


    Engine->On_Foot_State->Torpedo_Explosion_Billboard_Three = Create_Image();
    Load_Image(Engine->On_Foot_State->Torpedo_Explosion_Billboard_Three, "resources/artwork/gui/explosion3.sei");


    Engine->On_Foot_State->Torpedo_Explosion_Billboard_Four = Create_Image();
    Load_Image(Engine->On_Foot_State->Torpedo_Explosion_Billboard_Four, "resources/artwork/gui/explosion4.sei");

    Exit_Subspace(Engine);

    Engine->On_Foot_State->On_Foot_Player->Number_Of_Weapons = 0;
    Engine->On_Foot_State->On_Foot_Player->Current_Weapon = 0;

    Load_Weapon(Engine, 0, "test.wpn");
    Give_Weapon(Engine,0,0);

    Load_Weapon(Engine, 1, "test2.wpn");
    Give_Weapon(Engine,1,1);

    Load_Weapon(Engine, 2, "test3.wpn");
    Give_Weapon(Engine,2,2);

    Load_Weapon(Engine, 2, "test3.wpn");
    Give_Weapon(Engine,2,3);

    Engine->On_Foot_State->Current_Weapon_Selector = Create_Image();
   // BMPtoSEI("resources/data/items/selection.bmp","resources/data/items/selection.sei");
    Load_Image(Engine->On_Foot_State->Current_Weapon_Selector, "resources/data/items/selection.sei");


    int Use_This_Weapon = Engine->On_Foot_State->On_Foot_Player->Weapons_Available[Engine->On_Foot_State->On_Foot_Player->Current_Weapon];
    double Ammonumber = (double)Engine->On_Foot_State->Weapons[Use_This_Weapon]->Weapon_Entry.Current_Ammo;
    char bufferTwo[256]; // Allocate a buffer large enough for the string representation

    // Use sprintf to format the double into the buffer
    // "%f" is the format specifier for a double
    sprintf(bufferTwo, "%f", Ammonumber);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Ammo_Count, bufferTwo, 16, 16, 64);


    Engine->On_Foot_State->Atmosphere_Skybox = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Atmosphere_Skybox, "resources/models/model_skybox_ocean.se3");
    Load_SE3_Texture(Engine->On_Foot_State->Atmosphere_Skybox, "resources/models/texture_skybox_ocean.sei");

    Engine->On_Foot_State->Ocean_Surface = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Ocean_Surface, "resources/models/model_ocean.se3");
    Load_SE3_Texture(Engine->On_Foot_State->Ocean_Surface, "resources/models/texture_ocean.sei");

    Engine->On_Foot_State->Ocean_Seabox = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Ocean_Seabox, "resources/models/model_skybox.se3");
    Load_SE3_Texture(Engine->On_Foot_State->Ocean_Seabox, "resources/models/texture_skybox.sei");
}


void Render_Vortexes(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Region.Number_Of_Vortexes > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Vortexes; j++)
        {
        glPushMatrix();
        Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Billboard,Engine->On_Foot_State->Vortexes[j]->x,Engine->On_Foot_State->Vortexes[j]->y,Engine->On_Foot_State->Vortexes[j]->z,1,Engine->On_Foot_State->Vortexes[j]->Suction_Force);
        glPopMatrix();
        }
    }
}


void Render_Seats(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Region.Number_Of_Seats > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Seats; j++)
        {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Seat_Objects[j]->x,Engine->On_Foot_State->Seat_Objects[j]->y,Engine->On_Foot_State->Seat_Objects[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Seat_Objects[j]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);



        Draw_TV_Vertices(Engine->On_Foot_State->Seat_Objects[j]->New_Computer->Texture_ID,Engine->On_Foot_State->Seat_Objects[j]->Screen_x1,Engine->On_Foot_State->Seat_Objects[j]->Screen_y1,Engine->On_Foot_State->Seat_Objects[j]->Screen_z1,Engine->On_Foot_State->Seat_Objects[j]->Screen_x2,Engine->On_Foot_State->Seat_Objects[j]->Screen_y2,Engine->On_Foot_State->Seat_Objects[j]->Screen_z2,Engine->On_Foot_State->Seat_Objects[j]->Screen_x3,Engine->On_Foot_State->Seat_Objects[j]->Screen_y3,Engine->On_Foot_State->Seat_Objects[j]->Screen_z3,Engine->On_Foot_State->Seat_Objects[j]->Screen_x4,Engine->On_Foot_State->Seat_Objects[j]->Screen_y4,Engine->On_Foot_State->Seat_Objects[j]->Screen_z4);

        glPopMatrix();
        }
    }
}


void Render_Goalposts(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Region.Number_Of_Goalposts > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Goalposts; j++)
        {
        glPushMatrix();
        glTranslatef(Engine->On_Foot_State->Goalposts[j]->x,Engine->On_Foot_State->Goalposts[j]->y,Engine->On_Foot_State->Goalposts[j]->z);
        Render_SE3_Model(Engine->On_Foot_State->Goalposts[j]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
        glPopMatrix();
        }
    }
}

void Redraw_Text(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Debug)
    {
    free(Engine->On_Foot_State->Debug_Text_Player_X);
    free(Engine->On_Foot_State->Debug_Text_Player_Y);
    free(Engine->On_Foot_State->Debug_Text_Player_Z);

    free(Engine->On_Foot_State->Debug_Text_Player_Yaw);
    free(Engine->On_Foot_State->Debug_Text_Player_Pitch);

    free(Engine->On_Foot_State->Debug_Text_Player_Look_X);
    free(Engine->On_Foot_State->Debug_Text_Player_Look_Y);
    free(Engine->On_Foot_State->Debug_Text_Player_Look_Z);

    free(Engine->On_Foot_State->Debug_Text_Player_Flashlight_Off);
    free(Engine->On_Foot_State->Debug_Text_Player_Flashlight_On);

    free(Engine->On_Foot_State->Ammo_Count);

    Engine->On_Foot_State->Debug_Text_Player_X = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Y = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Z = Create_Image();

    Engine->On_Foot_State->Debug_Text_Player_Yaw = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Pitch = Create_Image();

    Engine->On_Foot_State->Debug_Text_Player_Look_X = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Look_Y = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Look_Z = Create_Image();

    Engine->On_Foot_State->Debug_Text_Player_Flashlight_On = Create_Image();
    Engine->On_Foot_State->Debug_Text_Player_Flashlight_Off = Create_Image();

    Engine->On_Foot_State->Ammo_Count = Create_Image();


    float Xnumber = Engine->On_Foot_State->On_Foot_Player->x;
    char Xbuffer[256];
    int Xlength_int = snprintf(Xbuffer, sizeof(Xbuffer), "%f", Xnumber);


    float Ynumber = Engine->On_Foot_State->On_Foot_Player->y;
    char Ybuffer[256];
    int Ylength_int = snprintf(Ybuffer, sizeof(Ybuffer), "%f", Ynumber);

    float Znumber = Engine->On_Foot_State->On_Foot_Player->z/METER_CONVERSION;
    char Zbuffer[256];
    int Zlength_int = snprintf(Zbuffer, sizeof(Zbuffer), "%f", Znumber);



    float Yanumber = Engine->On_Foot_State->yaw;
    char Yabuffer[256];
    int Yalength_int = snprintf(Yabuffer, sizeof(Yabuffer), "%f", Yanumber);

    float Pinumber = Engine->On_Foot_State->pitch;
    char Pibuffer[256];
    int Pilength_int = snprintf(Pibuffer, sizeof(Pibuffer), "%f", Pinumber);



    float LVDXnumber = 100*(Engine->On_Foot_State->frontX);
    char LVDXbuffer[256];
    int LVDXlength_int = snprintf(LVDXbuffer, sizeof(LVDXbuffer), "%f", LVDXnumber);

    float LVDYnumber = 100*(Engine->On_Foot_State->frontY);
    char LVDYbuffer[256];
    int LVDYlength_int = snprintf(LVDYbuffer, sizeof(LVDYbuffer), "%f", LVDYnumber);

    float LVDZnumber = 100*(Engine->On_Foot_State->frontZ);
    char LVDZbuffer[256];
    int LVDZlength_int = snprintf(LVDZbuffer, sizeof(LVDZbuffer), "%f", LVDZnumber);

    //int length_int = snprintf(buffer, sizeof(buffer), "%f", value);

    char Flonbuffer[256];
    char Floffbuffer[256];
    sprintf(Flonbuffer, "Flashlight ON");
    sprintf(Floffbuffer, "Flashlight OFF");

int Use_This_Weapon = Engine->On_Foot_State->On_Foot_Player->Weapons_Available[Engine->On_Foot_State->On_Foot_Player->Current_Weapon];
    int Ammonumber = Engine->On_Foot_State->Weapons[Use_This_Weapon]->Weapon_Entry.Current_Ammo;
    char Ammobuffer[256];
    int Ammolength_int = snprintf(Ammobuffer, sizeof(Ammobuffer), "%d", Ammonumber);


    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_X, Xbuffer, Xlength_int+1, 16+(17*Xlength_int), 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Y, Ybuffer, Ylength_int+1, 16+(17*Ylength_int), 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Z, Zbuffer, Zlength_int+1, 16+(17*Zlength_int), 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Yaw, Yabuffer, Yalength_int+1, 16+(17*Yalength_int), 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Pitch, Pibuffer, Pilength_int+1, 16+(17*Pilength_int), 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Look_X, LVDXbuffer, LVDXlength_int+1, 16+(17*LVDXlength_int), 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Look_Y, LVDYbuffer, LVDYlength_int+1, 16+(17*LVDYlength_int), 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Look_Z, LVDZbuffer, LVDZlength_int+1, 16+(17*LVDZlength_int), 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Flashlight_On, Flonbuffer, 14, 16+(17*14), 64);
    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Debug_Text_Player_Flashlight_Off, Floffbuffer, 15, 16+(17*15), 64);

    Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Ammo_Count, Floffbuffer, 15, 16+(17*15), 64);

    }
}

void Render_Skyboxes(struct _Engine* Engine)
{
    if(!Engine->On_Foot_State->Is_In_Subspace)
    {
    if(Engine->On_Foot_State->Current_State == STATE_ABOVE_WATER)
    {
            glPushMatrix();
            glTranslatef(0,0,0);
            glDisable(GL_LIGHTING);
           // Render_SE3_Model(Engine->On_Foot_State->Atmosphere_Skybox,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
            glPopMatrix();
            glEnable(GL_LIGHTING);

            glPushMatrix();
            glTranslatef(0,0,0);
            glDisable(GL_LIGHTING);
            //Render_SE3_Model(Engine->On_Foot_State->Ocean_Surface,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
            glPopMatrix();
            glEnable(GL_LIGHTING);
    }

    else if(Engine->On_Foot_State->Current_State == STATE_UNDER_WATER)
    {
        glPushMatrix();
            glTranslatef(0,0,0);
            glDisable(GL_LIGHTING);
            //Render_SE3_Model(Engine->On_Foot_State->Ocean_Seabox,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
            glPopMatrix();
            glEnable(GL_LIGHTING);
    }
    }
}

void Render_Ships(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Region.Number_Of_Ships > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ships; j++)
        {
            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->Ship_Objects[j]->Shell_x,Engine->On_Foot_State->Ship_Objects[j]->Shell_y,Engine->On_Foot_State->Ship_Objects[j]->Shell_z);
            //glRotatef(Engine->On_Foot_State->r,0,0,1);
            Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[j]->Ship_Shell_Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
            glPopMatrix();
            for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_Terrains; m++)
            {
                glPushMatrix();
                glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->z);
                Render_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
                glPopMatrix();

            }

            for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_RT_Screens; m++)
            {
                glPushMatrix();
                glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->RT_Screen_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->RT_Screen_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->RT_Screen_Objects[m]->z);
                Render_SE3_Model_Dynamic_Texture(Engine->On_Foot_State->Ship_Internals_Pointer[j]->RT_Screen_Objects[m]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Camera_Objects[Engine->On_Foot_State->Ship_Internals_Pointer[j]->RT_Screen_Objects[m]->Channel]->textureColorBuffer);
                glPopMatrix();
            }


            for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_Switches; m++)
            {
                glPushMatrix();
                glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->z);
                Render_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->Model,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->Frame,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
                glPopMatrix();
            }


	for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_Seats; m++)
        {
        	glPushMatrix();
        	glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->z);
        	Render_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Seat_Objects[m]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
        	glPopMatrix();
        }


        for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_TVs; m++)
        {
        	glPushMatrix();
        	glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->z);
        	Render_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);


        double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
        if(Engine->On_Foot_State->On_Foot_Player->Is_On_Board)
        {
        if(x > Engine->On_Foot_State->Time_Refreshed_Screens+0.2)
        {

            if (!Mix_Playing(3))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(3, Engine->USND_Computer_Beep, 0); // Start the sound
                    }

            double y = Get_Distance(Engine->On_Foot_State->Submarines[Engine->On_Foot_State->Ship_Objects[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Tied_To_This_Submarine]->plane_x,Engine->On_Foot_State->Submarines[Engine->On_Foot_State->Ship_Objects[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Tied_To_This_Submarine]->plane_y,Engine->On_Foot_State->Submarines[Engine->On_Foot_State->Ship_Objects[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Tied_To_This_Submarine]->plane_z,0,0,0);

            double Strength = 40000000/y;

            Encode_Image(Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Broadcaster,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Camera_Objects[0]->textureColorBuffer,5500,120);
            Receive_Image(Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Broadcaster, 5500,0,4000000000,0);
            Engine->On_Foot_State->Time_Refreshed_Screens = x;
        }
        }


glCullFace(GL_BACK);
Draw_TV_Vertices(Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Broadcaster->textureId,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_x1,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_y1,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_z1,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_x2,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_y2,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_z2,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_x3,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_y3,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_z3,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_x4,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_y4,Engine->On_Foot_State->Ship_Internals_Pointer[j]->TV_Objects[m]->Screen_z4);
glCullFace(GL_FRONT);


        	glPopMatrix();
        }



        }
    }
}

void Render_Cars(struct _Engine* Engine)
{

if(Engine->On_Foot_State->Region.Number_Of_Cars > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Cars; j++)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Cars[j]->x,Engine->On_Foot_State->Cars[j]->y,Engine->On_Foot_State->Cars[j]->z);
    Render_SE3_Model(Engine->On_Foot_State->Cars[j]->Models[0],0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
    Process_Player_Car(Engine, j);
    glPopMatrix();
    }
    }
}

void Render_Characters(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Region.Number_Of_Characters > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Characters; j++)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->NPC_Character_Objects[j]->x,Engine->On_Foot_State->NPC_Character_Objects[j]->y,Engine->On_Foot_State->NPC_Character_Objects[j]->z);
    if(!Engine->On_Foot_State->NPC_Character_Objects[j]->Dead)
    {
    Render_SE3_Model(Engine->On_Foot_State->NPC_Character_Objects[j]->Character_Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
    }

    else if(Engine->On_Foot_State->NPC_Character_Objects[j]->Dead)
    {
    Render_SE3_Model(Engine->On_Foot_State->NPC_Character_Objects[j]->Character_Model,1,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
    }
    glPopMatrix();
    }
    }
}

void Render_Submarines(struct _Engine* Engine)
{
     if(Engine->On_Foot_State->Region.Number_Of_Submarines > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Submarines; j++)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Submarines[j]->plane_x, Engine->On_Foot_State->Submarines[j]->plane_y, Engine->On_Foot_State->Submarines[j]->plane_z);
        //Process_Submarine_Object(Engine, j);
    // Apply the orientation matrix
    float mat[16] = {
        (float)Engine->On_Foot_State->Submarines[j]->Right_Vector.x, (float)Engine->On_Foot_State->Submarines[j]->Right_Vector.y, (float)Engine->On_Foot_State->Submarines[j]->Right_Vector.z, 0,
        (float)Engine->On_Foot_State->Submarines[j]->Up_Vector.x,    (float)Engine->On_Foot_State->Submarines[j]->Up_Vector.y,    (float)Engine->On_Foot_State->Submarines[j]->Up_Vector.z,    0,
        -(float)Engine->On_Foot_State->Submarines[j]->Forward_Vector.x,  -(float)Engine->On_Foot_State->Submarines[j]->Forward_Vector.y,  -(float)Engine->On_Foot_State->Submarines[j]->Forward_Vector.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);
    if(j != Engine->On_Foot_State->In_This_Submarine)
    {
    //Process_Submarine_Object(Engine, j);
    }
    Render_Submarine_Object(Engine, j);
    glPopMatrix();
    }
    }
}

void Render_Fires(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Region.Number_Of_Fires > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Fires; j++)
    {
    glPushMatrix();
            glTranslatef(0,0,0);
            Render_Image_As_Billboard(Engine->On_Foot_State->Fires[j]->Fire_Image,Engine->On_Foot_State->Fires[j]->x,Engine->On_Foot_State->Fires[j]->y,Engine->On_Foot_State->Fires[j]->z,1,Engine->On_Foot_State->Fires[j]->Fire_Size);
            glPopMatrix();
    }

    }
}

void Render_Terrain(struct _Engine* Engine)
{
     if(Engine->On_Foot_State->Region.Number_Of_Terrains > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Terrains; j++)
    {
    glPushMatrix();
           glTranslatef(Engine->On_Foot_State->Terrain_Objects[j]->x,Engine->On_Foot_State->Terrain_Objects[j]->y,Engine->On_Foot_State->Terrain_Objects[j]->z);
    Render_SE3_Model(Engine->On_Foot_State->Terrain_Objects[j]->Model,0,0,0,0,0,0,0,false,1,1,1,Engine->On_Foot_State->Is_In_Subspace);
            glPopMatrix();
    }

    }
}

void Render_Torpedoes(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Number_Of_Torpedoes > 0)
    {
    for(int j = 0; j < Engine->On_Foot_State->Number_Of_Torpedoes; j++)
    {
        glPushMatrix();
        //glTranslatef(0,0,0);
        Process_Torpedo(Engine,j);

        if(Engine->On_Foot_State->Torpedoes[j]->Visible)
        {
            if(Engine->On_Foot_State->Torpedoes[j]->Mode == TORPEDO_MODE_GRENADE || Engine->On_Foot_State->Torpedoes[j]->Mode == TORPEDO_MODE_HOMING || Engine->On_Foot_State->Torpedoes[j]->Mode == TORPEDO_MODE_PROJECTILE)
            {
            int Use_This_Weapon = Engine->On_Foot_State->On_Foot_Player->Weapons_Available[Engine->On_Foot_State->On_Foot_Player->Current_Weapon];
        Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Billboard,Engine->On_Foot_State->Torpedoes[j]->x,Engine->On_Foot_State->Torpedoes[j]->y,Engine->On_Foot_State->Torpedoes[j]->z,1,4);
        }
            else if(Engine->On_Foot_State->Torpedoes[j]->Mode == TORPEDO_MODE_EXPLODED)
            {
            switch(Engine->On_Foot_State->Torpedoes[j]->Current_Frame)
            {
                glDisable(GL_LIGHTING);
                case 1:
                Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Explosion_Billboard,Engine->On_Foot_State->Torpedoes[j]->x,Engine->On_Foot_State->Torpedoes[j]->y,Engine->On_Foot_State->Torpedoes[j]->z+4,1,0.4);
            break;

                case 2:
                Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Explosion_Billboard_Two,Engine->On_Foot_State->Torpedoes[j]->x,Engine->On_Foot_State->Torpedoes[j]->y,Engine->On_Foot_State->Torpedoes[j]->z+4,1,0.4);
            break;

            case 3:
                Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Explosion_Billboard_Three,Engine->On_Foot_State->Torpedoes[j]->x,Engine->On_Foot_State->Torpedoes[j]->y,Engine->On_Foot_State->Torpedoes[j]->z+4,1,0.4);
                Engine->On_Foot_State->Shaky_Camera = false;
            break;

            case 4:
                Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Explosion_Billboard_Four,Engine->On_Foot_State->Torpedoes[j]->x,Engine->On_Foot_State->Torpedoes[j]->y,Engine->On_Foot_State->Torpedoes[j]->z+4,1,0.4);
            break;
            glEnable(GL_LIGHTING);
            }
        }

        }
        glPopMatrix();
    }
    }
}

void Render_Debug(struct _Engine* Engine)
{
    if(Engine->On_Foot_State->Debug)
            {
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_X,16,0,1);
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Y,16,32,1);
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Z,16,64,1);

            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Yaw,16,64+32+32,1);
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Pitch,16,64+64+32,1);

            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Look_X,16,128+32+64,1);
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Look_Y,16,128+64+64,1);
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Look_Z,16,128+96+64,1);

            if(!Engine->On_Foot_State->Flashlight)
            {
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Flashlight_Off,256+64,0,1);
            }

            else if(Engine->On_Foot_State->Flashlight)
            {
            Render_Image(Engine->On_Foot_State->Debug_Text_Player_Flashlight_On,256+64,0,1);
            }
            }
}

void Render_On_Foot_State(struct _Engine* Engine)
{
    
    if(!Engine->On_Foot_State->On_Foot_Player->Dead)
    {
        if(!Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
        {
            if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
            {
                
                
                
                
                Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Billboard,Engine->On_Foot_State->Subspace_Hole_x,Engine->On_Foot_State->Subspace_Hole_y,Engine->On_Foot_State->Subspace_Hole_z,1,0.4);
                
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0.0, 1920,1080, 0.0, 1.0, -1.0);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glViewport(0, 0, 1920,1080);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                if(Engine->On_Foot_State->Is_In_Subspace)
                {
                    gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 25);
                }
                else if(!Engine->On_Foot_State->Is_In_Subspace)
                {
                    gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
                }
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                for(int a = 0; a < Engine->On_Foot_State->Region.Number_Of_Ships; a++)
                {
                    for(int b = 0; b < Engine->On_Foot_State->Ship_Internals_Pointer[a]->Region.Number_Of_On_Board_Cameras; b++)
                    {
                        // 1. RENDER TO TEXTURE (The "CCTV" perspective)
                        glBindFramebuffer(GL_FRAMEBUFFER, Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->fbo);
                        glViewport(0, 0, 256,256);

                        // Encode_Image(Engine->On_Foot_State->Ship_Internals_Pointer[0]->TV_Objects[0]->Broadcaster,GLuint textureID,5500,1280000);
                        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                        glClearColor(0.34375,0.46875,0.65625, 1.f);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity();
                        if(Engine->On_Foot_State->Is_In_Subspace)
                        {
                            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 25);
                        }

                        else if(!Engine->On_Foot_State->Is_In_Subspace)
                        {
                            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
                        }
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        // Engine->On_Foot_State->camX = Engine->On_Foot_State->On_Foot_Player->x;
                        // Engine->On_Foot_State->camY = Engine->On_Foot_State->On_Foot_Player->y;
                        // Engine->On_Foot_State->camZ = Engine->On_Foot_State->On_Foot_Player->z+40;//gluLookAt(Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->x,Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->y,Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->z,Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->Look_x,Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->Look_y,Engine->On_Foot_State->Ship_Internals_Pointer[a]->Camera_Objects[b]->Look_z,0,0,1);
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        if(Engine->On_Foot_State->Is_In_Subspace)
                        {
                            glScalef(-1.0f, 1.0f, 1.0f); // Flips the x-coordinates
                        }
                        // Camera offset: Follow behind and slightly above
                        Engine->On_Foot_State->In_This_Submarine = 0;
                        Vec3 cam;
                        cam.x = Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_x - Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Forward_Vector.x * 1200.0f + Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.x * 3.0f;
                        cam.y = Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_y - Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Forward_Vector.y * 1200.0f + Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.y * 3.0f;
                        cam.z = Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_z - Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Forward_Vector.z * 1200.0f + Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.z * 3.0f;
                        if(Engine->On_Foot_State->On_Foot_Player->Is_On_Board)
                        {
                            if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                            {
                                    if(Engine->On_Foot_State->Ship_Internals_Pointer[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Seat_Objects[Engine->On_Foot_State->On_Foot_Player->Chair_ID]->Is_A_Drivers_Seat)
                                    {
                                        Process_Submarine_Object(Engine, Engine->On_Foot_State->Ship_Objects[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Tied_To_This_Submarine);
                                    }
                            }
                        }
                        gluLookAt(cam.x, cam.y, cam.z, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_x, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_y, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_z, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.x, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.y, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.z);
                        Render_Skyboxes(Engine);
                        Render_Goalposts(Engine);
                        Render_Ships(Engine);
                        Render_Cars(Engine);
                        Render_Characters(Engine);
                        Render_Submarines(Engine);
                        Render_Fires(Engine);
                        Render_Terrain(Engine);
                        Render_Torpedoes(Engine);
                        Render_Seats(Engine);
                        Render_Vortexes(Engine);

                        glBindFramebuffer(GL_FRAMEBUFFER, 0);
                        glViewport(0, 0, 1920,1080);
                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity();
                        if(Engine->On_Foot_State->Is_In_Subspace)
                        {
                            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 25);
                        }

                        else if(!Engine->On_Foot_State->Is_In_Subspace)
                        {
                            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
                        }
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        glEnable(GL_LIGHTING);

                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity();
                        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();

                        glDisable(GL_LIGHTING);
                        glColor3f(1,1,1);

                        if(Engine->On_Foot_State->Show_Weapons_Menu)
                        {
                            for(int j = 0; j < Engine->On_Foot_State->On_Foot_Player->Number_Of_Weapons; j++)
                            {
                                Render_Image(Engine->On_Foot_State->Weapons[Engine->On_Foot_State->On_Foot_Player->Weapons_Available[j]]->Weapon_Icon,104+(72*j),360-64-8,0);
                            }
                            Render_Image(Engine->On_Foot_State->Current_Weapon_Selector,104+(72*Engine->On_Foot_State->On_Foot_Player->Current_Weapon),360-64-8-24,0);
                        }
                        Render_Image(Engine->On_Foot_State->FPS_Reticle,320-16,180-16,1);
                        Render_Image(Engine->On_Foot_State->Ammo_Count,128,0,1);
           // Render_Image(Engine->On_Foot_State->Ship_Internals_Pointer[0]->TV_Objects[0]->Broadcaster->No_Signal,0,0,1);

                        Render_Debug(Engine);

                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity();
                        if(Engine->On_Foot_State->Is_In_Subspace)
                        {
                            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 25);
                        }
                        else if(!Engine->On_Foot_State->Is_In_Subspace)
                        {
                            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
                        }
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();

                        glEnable(GL_LIGHTING);
        }
    }







    }

    else if(Engine->On_Foot_State->Mini_State == MINI_STATE_PAUSE)
    {
        if(Engine->On_Foot_State->Current_Choice == 0)
        {
            Render_Image(Engine->On_Foot_State->Inventory_Highlighted,176,32+16,1);
        }
        if(Engine->On_Foot_State->Current_Choice == 1 || Engine->On_Foot_State->Current_Choice == 2)
        {
            Render_Image(Engine->On_Foot_State->Inventory,176,32+16,1);
        }


        if(Engine->On_Foot_State->Current_Choice == 1)
        {
            Render_Image(Engine->On_Foot_State->Resume_Highlighted,176,128+16,1);
        }
        if(Engine->On_Foot_State->Current_Choice == 0 || Engine->On_Foot_State->Current_Choice == 2)
        {
            Render_Image(Engine->On_Foot_State->Resume,176,128+16,1);
        }



        if(Engine->On_Foot_State->Current_Choice == 2)
        {
            Render_Image(Engine->On_Foot_State->Exit_Highlighted,176,224+16,1);
        }
        if(Engine->On_Foot_State->Current_Choice == 0 || Engine->On_Foot_State->Current_Choice == 1)
        {
            Render_Image(Engine->On_Foot_State->Exit,176,224+16,1);
        }


    }
    }
    }
}
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad, struct GB_AUDIO_PROCESSOR apu)
{
    if(!Engine->On_Foot_State->On_Foot_Player->Dead)
    {
    if(!Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
    {
    if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
    {
        if(!Engine->On_Foot_State->Player_Is_In_Car && !Engine->On_Foot_State->Player_Is_In_Submarine)
        {

            Check_Car_Input(Engine, Keypad);
            Check_Submarine_Input(Engine, Keypad);
            if(Engine->On_Foot_State->On_Foot_Player->Is_On_Board)
            {
                if(Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
                {
                    if(Engine->On_Foot_State->Ship_Internals_Pointer[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Seat_Objects[Engine->On_Foot_State->On_Foot_Player->Chair_ID]->Is_A_Drivers_Seat)
                    {
                        Handle_Submarine_Object_Inputs(Engine, Keypad, Engine->On_Foot_State->Ship_Objects[Engine->On_Foot_State->On_Foot_Player->Ship_ID]->Tied_To_This_Submarine);
                    }
                }
            }

        if(Engine->On_Foot_State->Cooldown_Timer != NULL)
        {
            double x = Get_Ticks(Engine->On_Foot_State->Cooldown_Timer);
            if(x >= FIRE_COOLDOWN && Engine->On_Foot_State->Cooldown_Complete == false)
            {
                Engine->On_Foot_State->Cooldown_Complete = true;
                free(Engine->On_Foot_State->Cooldown_Timer);
                Engine->On_Foot_State->Cooldown_Timer = Create_Timer();
                Set_Timer(Engine->On_Foot_State->Cooldown_Timer);
                Pause_Timer(Engine->On_Foot_State->Cooldown_Timer);
            }
        }

        if(!Engine->On_Foot_State->On_Foot_Player->Is_Locked_Into_Chair)
        {
        if(Keypad.Mouse_Is_Clicked && Engine->On_Foot_State->Cooldown_Complete && !Engine->On_Foot_State->Show_Weapons_Menu)
        {
            int Use_This_Weapon = Engine->On_Foot_State->On_Foot_Player->Weapons_Available[Engine->On_Foot_State->On_Foot_Player->Current_Weapon];

            if(Engine->On_Foot_State->Weapons[Use_This_Weapon]->Weapon_Entry.Current_Ammo > 0)
            {
            Initialize_Torpedo(Engine, Engine->On_Foot_State->On_Foot_Player->x+Engine->On_Foot_State->On_Foot_Player->displacement.x,Engine->On_Foot_State->On_Foot_Player->y+Engine->On_Foot_State->On_Foot_Player->displacement.y,Engine->On_Foot_State->On_Foot_Player->z+40+Engine->On_Foot_State->On_Foot_Player->displacement.z,0,0,0,4,Engine->On_Foot_State->Weapons[Use_This_Weapon]->Weapon_Entry.Ammo_Mode,Engine->On_Foot_State->frontX,Engine->On_Foot_State->frontY,Engine->On_Foot_State->frontZ);

            Engine->On_Foot_State->Weapons[Use_This_Weapon]->Weapon_Entry.Current_Ammo -= 1;
            }

            if (!Mix_Playing(3))
                    { // If channel 1 is not playing anything
                        Mix_PlayChannel(3, Engine->USND_Fire, 0); // Start the sound
                    }
                Resume_Timer(Engine->On_Foot_State->Cooldown_Timer);
                Engine->On_Foot_State->Cooldown_Complete = false;
        }
        }

        if(Keypad.Keyboard_C)
        {
          //  Exit_Subspace(Engine);
            // glEnable(GL_LIGHT1);
             Engine->On_Foot_State->Flashlight = true;
             if (!Mix_Playing(3))
                { // If channel 1 is not playing anything
                    Mix_PlayChannel(3, Engine->USND_Flashlight, 0); // Start the sound
                }
            Turn_Computer_Off(Engine, Engine->On_Foot_State->Seat_Objects[0]);
        }


        if(Engine->On_Foot_State->Region.Number_Of_Seats > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Seats; j++)
        {
            Process_New_Computer_Cycles(Engine, Engine->On_Foot_State->Seat_Objects[j], 100);
            Render_New_Computer(Engine, Engine->On_Foot_State->Seat_Objects[j]);
            Handle_New_Computer_Inputs(Engine, Engine->On_Foot_State->Seat_Objects[j], Keypad);
        }
    }

        if(Keypad.Keyboard_D)
        {
            //Enter_Subspace(Engine);
            //glDisable(GL_LIGHT1);
            Engine->On_Foot_State->Flashlight = false;
            Turn_Computer_On(Engine, Engine->On_Foot_State->Seat_Objects[0]);
            apu.ch1 = (PulseChannel){ .enabled = true, .frequency = 1600, .duty_idx = 1, .env = {15, 15, 0, 2, 2} };
        }




        if(Engine->On_Foot_State->Region.Number_Of_Ships > 0)
    {
         for(int Ship_Index = 0; Ship_Index < Engine->On_Foot_State->Region.Number_Of_Ships; Ship_Index++)
    {

//        for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Computers; m++)
        {
       //     Process_Computer(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Computers[m], 10, Keypad);
        }

        if(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Switches > 0)
        {
        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Switches; j++)
        {
            if(Check_If_Ray_Sensor_Detects_AABB(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box,Engine->On_Foot_State->camX, Engine->On_Foot_State->camY, Engine->On_Foot_State->camZ,Engine->On_Foot_State->camX+(Engine->On_Foot_State->frontX*100),Engine->On_Foot_State->camY+(Engine->On_Foot_State->frontY*100),Engine->On_Foot_State->camZ+(Engine->On_Foot_State->frontZ*100)))
            {

                Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Frame = 1;
            }

            else if(!Check_If_Ray_Sensor_Detects_AABB(Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Collision_Box,Engine->On_Foot_State->camX, Engine->On_Foot_State->camY, Engine->On_Foot_State->camZ,Engine->On_Foot_State->camX+(Engine->On_Foot_State->frontX*100),Engine->On_Foot_State->camY+(Engine->On_Foot_State->frontY*100),Engine->On_Foot_State->camZ+(Engine->On_Foot_State->frontZ*100)))
            {

                Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Switch_Objects[j]->Frame = 0;
            }
        }
        }
    }
    }

    if(!Keypad.Mouse_Is_Clicked)
        {
             Engine->On_Foot_State->Interact_Not_Pressed = true;
        }



        if(Engine->On_Foot_State->FPS_Mode)
        {
            
        }
        }

        else if(Engine->On_Foot_State->Player_Is_In_Car &&!Engine->On_Foot_State->Player_Is_In_Submarine)
        {
            Handle_Player_Car_Inputs(Engine, Keypad, Engine->On_Foot_State->In_This_Car);
        }

        else if(Engine->On_Foot_State->Player_Is_In_Submarine && !Engine->On_Foot_State->Player_Is_In_Car)
        {
            Handle_Submarine_Object_Inputs(Engine, Keypad, Engine->On_Foot_State->In_This_Submarine);
        }
    }


    if(Engine->On_Foot_State->Mini_State == MINI_STATE_PAUSE)
    {
    ////printf("Current Choice: %d\n", Engine->On_Foot_State->Current_Choice);
    if(Keypad.Keyboard_Down && Engine->On_Foot_State->Current_Choice < 2 && Engine->On_Foot_State->Down_Not_Pressed)
    {
         Engine->On_Foot_State->Selector_Y += 96;
         Engine->On_Foot_State->Current_Choice += 1;
         Engine->On_Foot_State->Down_Not_Pressed = false;
    }

    if(Keypad.Keyboard_Up && Engine->On_Foot_State->Current_Choice > 0 && Engine->On_Foot_State->Up_Not_Pressed)
    {
         Engine->On_Foot_State->Selector_Y -= 96;
         Engine->On_Foot_State->Current_Choice -= 1;
         Engine->On_Foot_State->Up_Not_Pressed = false;
    }

    if(Keypad.Keyboard_Start)
    {

        if(Engine->On_Foot_State->Current_Choice == 2)
        {
            Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
            Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
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



    }
    }
}
void Process_On_Foot_State(struct _Engine* Engine)
{
    if(!Engine->On_Foot_State->On_Foot_Player->Dead)
    {
     double z = Get_Ticks(Engine->On_Foot_State->Go_Back_Timer);
    if(z > 2)
    {
        glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            //glDisable(GL_LIGHTING);
            glColor3f(1,1,1);
             glDisable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
//    glDepthMask(GL_FALSE);
    glDisable(GL_NORMALIZE);
           // glTranslatef(0,0,0);
        Reinitialize_Overworld_State(Engine);
        Engine->Current_State = STATE_OVERWORLD;
    }

    if(!Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
    {
    if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
    {


    if(Engine->On_Foot_State->Region.Number_Of_Characters > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Characters; j++)
        {
            Process_NPC_Character(Engine, j);
        }
    }









        if(!Engine->On_Foot_State->Player_Is_In_Car && !Engine->On_Foot_State->Player_Is_In_Submarine)
        {
    if(Engine->On_Foot_State->FPS_Mode)
            {
            

            GLfloat gen_position[] = { Engine->On_Foot_State->camX+(Engine->On_Foot_State->frontX),Engine->On_Foot_State->camY+(Engine->On_Foot_State->frontY),Engine->On_Foot_State->camZ+(Engine->On_Foot_State->frontZ), 1.0 };
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gen_position);


            GLfloat gen_origin[] = { (float)Engine->On_Foot_State->camX,(float)Engine->On_Foot_State->camY,(float)Engine->On_Foot_State->camZ, -1.0 };
            glLightfv(GL_LIGHT1, GL_POSITION, gen_origin);
    // 3. Set the cone angle (cutoff)
    // A cutoff of 15 means a 30-degree total cone of light
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);

    // 4. Focus the beam (exponent)
    // Higher numbers make the center of the beam brighter than the edges
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);

    double ShakyCamX, ShakyCamY, ShakyCamZ;

    if(Engine->On_Foot_State->Shaky_Camera)
    {
            ShakyCamX = rand() % 2;
    ShakyCamY = rand() % 2;
    ShakyCamZ = rand() % 2;

    ShakyCamX /= 100;
    ShakyCamY /= 100;
    ShakyCamZ /= 100;
    }

    else if(!Engine->On_Foot_State->Shaky_Camera)
    {
    ShakyCamX = 0;
    ShakyCamY = 0;
    ShakyCamZ = 0;
    }

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            if(Engine->On_Foot_State->Is_In_Subspace)
    {
        glScalef(-1.0f, 1.0f, 1.0f); // Flips the x-coordinates
    }
            gluLookAt(ShakyCamX+Engine->On_Foot_State->camX, ShakyCamY+Engine->On_Foot_State->camY, ShakyCamZ+Engine->On_Foot_State->camZ,
              Engine->On_Foot_State->camX + Engine->On_Foot_State->frontX, Engine->On_Foot_State->camY + Engine->On_Foot_State->frontY, Engine->On_Foot_State->camZ + Engine->On_Foot_State->frontZ,
              0,0,1);
            }

            else if(!Engine->On_Foot_State->FPS_Mode)
            {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(Engine->On_Foot_State->Is_In_Subspace)
    {
        glScalef(-1.0f, 1.0f, 1.0f); // Flips the x-coordinates
    }
    gluLookAt(90*cos((180+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->On_Foot_Player->x, Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+20, 0,0,1);
    }
        }

        else if(Engine->On_Foot_State->Player_Is_In_Car && !Engine->On_Foot_State->Player_Is_In_Submarine)
        {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(Engine->On_Foot_State->Is_In_Subspace)
    {
        glScalef(-1.0f, 1.0f, 1.0f); // Flips the x-coordinates
    }
    gluLookAt(90*cos((180+Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->y,Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->x, Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->y,Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->z+20, 0,0,1);
        }

        else if(!Engine->On_Foot_State->Player_Is_In_Car && Engine->On_Foot_State->Player_Is_In_Submarine)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            if(Engine->On_Foot_State->Is_In_Subspace)
    {
        glScalef(-1.0f, 1.0f, 1.0f); // Flips the x-coordinates
    }
            // Camera offset: Follow behind and slightly above
            Vec3 cam;
            cam.x = Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_x - Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Forward_Vector.x * 1200.0f + Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.x * 3.0f;
            cam.y = Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_y - Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Forward_Vector.y * 1200.0f + Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.y * 3.0f;
            cam.z = Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_z - Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Forward_Vector.z * 1200.0f + Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.z * 3.0f;

            Process_Submarine_Object(Engine, Engine->On_Foot_State->In_This_Submarine);

            gluLookAt(cam.x, cam.y, cam.z, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_x, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_y, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->plane_z, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.x, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.y, Engine->On_Foot_State->Submarines[Engine->On_Foot_State->In_This_Submarine]->Up_Vector.z);
        }
    }
    }
    }
}


void Write_Ship(const char* Filename)
{
    struct _Ship_Details Ship;

    memset(Ship.Shell_Model_Name,0,256);
    memset(Ship.Shell_Texture_Filepath,0,256);
    sprintf(Ship.Shell_Model_Name, "resources/data/ships/sonichu.se3");
    sprintf(Ship.Shell_Texture_Filepath, "resources/data/ships/sonichu.sei");
    Ship.Shell_Model_Name[32] ='\0';
    Ship.Shell_Texture_Filepath[32] = '\0';

    memset(Ship.Internal_Layout.Region_Name,0,256);
    memset(Ship.Internal_Layout.Region_Description,0,256);

    Ship.Internal_Layout.Number_Of_Skyboxes = 0;
    Ship.Internal_Layout.Number_Of_Ships = 0;
    Ship.Internal_Layout.Number_Of_Wall_Collision_Meshes = 1;
    Ship.Internal_Layout.Number_Of_Ceiling_Collision_Meshes = 1;
    Ship.Internal_Layout.Number_Of_Goalposts = 0;
    Ship.Internal_Layout.Number_Of_Characters = 0;
    Ship.Internal_Layout.Number_Of_Cars = 0;
    Ship.Internal_Layout.Number_Of_Submarines = 0;
    Ship.Internal_Layout.Number_Of_Fires = 0;
    Ship.Internal_Layout.Number_Of_Terrains = 1;
    Ship.Internal_Layout.Number_Of_Docking_Ports = 1;
    Ship.Internal_Layout.Number_Of_Switches = 1;
    Ship.Internal_Layout.Number_Of_On_Board_Cameras = 1;
    Ship.Internal_Layout.Number_Of_RT_Screens = 1;
    Ship.Internal_Layout.Number_Of_Slope_Collision_Meshes = 0;
    Ship.Internal_Layout.Number_Of_Seats = 1;
    Ship.Internal_Layout.Number_Of_TVs = 1;
    Ship.Internal_Layout.Number_Of_Combo_Seat_Screens = 1;

    int Size_Of_Skyboxes = Ship.Internal_Layout.Number_Of_Skyboxes * sizeof(struct _Skybox);
    int Size_Of_Ships = Ship.Internal_Layout.Number_Of_Ships  * sizeof(struct _Ship);
    int Size_Of_Wall_Collision_Meshes = Ship.Internal_Layout.Number_Of_Wall_Collision_Meshes * 256;
    int Size_Of_Ceiling_Collision_Meshes = Ship.Internal_Layout.Number_Of_Ceiling_Collision_Meshes * 256;
    int Size_Of_Goalposts = Ship.Internal_Layout.Number_Of_Goalposts  * sizeof(struct _Goalpost);
    int Size_Of_Characters = Ship.Internal_Layout.Number_Of_Characters * 256;
    int Size_Of_Cars = Ship.Internal_Layout.Number_Of_Cars  * sizeof(struct _Car);
    int Size_Of_Submarines = Ship.Internal_Layout.Number_Of_Submarines  * sizeof(struct _Submarine);
    int Size_Of_Fires = Ship.Internal_Layout.Number_Of_Fires  * sizeof(struct _Fire);
    int Size_Of_Terrains = Ship.Internal_Layout.Number_Of_Terrains  * sizeof(struct _Terrain);
    int Size_Of_Docking_Ports = Ship.Internal_Layout.Number_Of_Docking_Ports  * sizeof(struct _Docking_Port);
    int Size_Of_Switches = Ship.Internal_Layout.Number_Of_Switches  * sizeof(struct _Switch);
    int Size_Of_On_Board_Cameras = Ship.Internal_Layout.Number_Of_On_Board_Cameras  * sizeof(struct _Camera);
    int Size_Of_RT_Screens = Ship.Internal_Layout.Number_Of_RT_Screens  * sizeof(struct _RT_Screen);
    int Size_Of_Slope_Collision_Meshes = Ship.Internal_Layout.Number_Of_Slope_Collision_Meshes * 256;
    int Size_Of_Seats = Ship.Internal_Layout.Number_Of_Seats  * sizeof(struct _Seat);
    int Size_Of_Vortexes = Ship.Internal_Layout.Number_Of_Vortexes  * sizeof(struct _Vortex);
    int Size_Of_TVs = Ship.Internal_Layout.Number_Of_TVs  *  sizeof(struct _On_Foot_TV);
    int Size_Of_Combo_Seat_Screens = Ship.Internal_Layout.Number_Of_Combo_Seat_Screens  * sizeof(struct _Combo_Seat_Screen);

    Ship.Internal_Layout.Offset_To_Skyboxes = sizeof(struct _Region);
    Ship.Internal_Layout.Offset_To_Ships = Ship.Internal_Layout.Offset_To_Skyboxes + Size_Of_Skyboxes;
    Ship.Internal_Layout.Offset_To_Wall_Collision_Meshes = Ship.Internal_Layout.Offset_To_Ships + Size_Of_Ships;
    Ship.Internal_Layout.Offset_To_Ceiling_Collision_Meshes = Ship.Internal_Layout.Offset_To_Wall_Collision_Meshes + Size_Of_Wall_Collision_Meshes;
    Ship.Internal_Layout.Offset_To_Goalposts = Ship.Internal_Layout.Offset_To_Ceiling_Collision_Meshes + Size_Of_Ceiling_Collision_Meshes;
    Ship.Internal_Layout.Offset_To_Characters = Ship.Internal_Layout.Offset_To_Goalposts + Size_Of_Goalposts;
    Ship.Internal_Layout.Offset_To_Cars = Ship.Internal_Layout.Offset_To_Characters + Size_Of_Characters;
    Ship.Internal_Layout.Offset_To_Submarines = Ship.Internal_Layout.Offset_To_Cars+Size_Of_Cars;
    Ship.Internal_Layout.Offset_To_Fires = Ship.Internal_Layout.Offset_To_Submarines + Size_Of_Submarines;
    Ship.Internal_Layout.Offset_To_Terrains = Ship.Internal_Layout.Offset_To_Fires+Size_Of_Fires;
    Ship.Internal_Layout.Offset_To_Docking_Ports = Ship.Internal_Layout.Offset_To_Terrains + Size_Of_Terrains;
    Ship.Internal_Layout.Offset_To_Switches = Ship.Internal_Layout.Offset_To_Docking_Ports + Size_Of_Docking_Ports;
    Ship.Internal_Layout.Offset_To_On_Board_Cameras = Ship.Internal_Layout.Offset_To_Switches + Size_Of_Switches;
    Ship.Internal_Layout.Offset_To_RT_Screens = Ship.Internal_Layout.Offset_To_On_Board_Cameras + Size_Of_On_Board_Cameras;
    Ship.Internal_Layout.Offset_To_Slope_Collision_Meshes = Ship.Internal_Layout.Offset_To_RT_Screens + Size_Of_RT_Screens;
    Ship.Internal_Layout.Offset_To_Seats = Ship.Internal_Layout.Offset_To_Slope_Collision_Meshes + Size_Of_Slope_Collision_Meshes;
    Ship.Internal_Layout.Offset_To_Vortexes = Ship.Internal_Layout.Offset_To_Seats + Size_Of_Seats;
    Ship.Internal_Layout.Offset_To_TVs = Ship.Internal_Layout.Offset_To_Vortexes + Size_Of_Vortexes;
    Ship.Internal_Layout.Offset_To_Combo_Seat_Screens = Ship.Internal_Layout.Offset_To_TVs + Size_Of_TVs;

    struct _Combo_Seat_Screen Combo;

    Combo.Model_Filepath[256];
    Combo.Texture_Filepath[256];
    Combo.x = 0;
    Combo.y = 0;
    Combo.z = 0;

    Combo.x1 = 0;
    Combo.y1 = 0;
    Combo.z1 = 0;

    Combo.x2 = 0;
    Combo.y2 = 0;
    Combo.z2 = 0;

    Combo.Eye_Offset_x = 0;
    Combo.Eye_Offset_y = 0;
    Combo.Eye_Offset_z = 0;

    Combo.Is_A_Drivers_Seat = 0;
    Combo.Screen_Type = 0;
    Combo.Screen_State = 0;


    Combo.Screen_y1 = 7;
    Combo.Screen_x1 = -13;
    Combo.Screen_z1 = 26;

    Combo.Screen_y2 = -7;
    Combo.Screen_x2 = -13;
    Combo.Screen_z2 = 26;

    Combo.Screen_y3 = -7;
    Combo.Screen_x3 = -15;
    Combo.Screen_z3 = 39;

    Combo.Screen_y4 = 7;
    Combo.Screen_x4 = -15;
    Combo.Screen_z4 = 39;

    Combo.Receiver_Frequency = 0;
    Combo.Receiver_Audio_Frequency = 0;

    Combo.Red_Level_Dial = 0;
    Combo.Blue_Level_Dial = 0;
    Combo.Green_Level_Dial = 0;
    Combo.Contrast_Level = 0;
    Combo.Brightness_Level = 0;

    Combo.HDD_Slot_Status = 0;
    Combo.Removable_Slot_Status = 0;

    Combo.Sensor_Link_Port_Status = 0;
    Combo.Peripheral_Link_Port_Status = 0;
    Combo.Network_Link_Port_Status = 0;

    Combo.Monitor_Link_Port_Status = 0;

    Combo.Keyboard_Link_Port_Status = 0;
    Combo.Mouse_Link_Port_Status = 0;
    Combo.Controller_Link_Port_Status = 0;

    Combo.Power_Supply_Status = 0;
    Combo.Fan_Status = 0;

    Combo.CPU_Is_Running = 0;

    Combo.Color_Mode = 0;
    Combo.Background_Color = 0;
    Combo.Current_RAM_Bank = 0;



    struct _On_Foot_TV TV;
    memset(TV.Model_Filepath,0,256);
    memset(TV.Texture_Filepath,0,256);
    sprintf(TV.Model_Filepath,"resources/models/TV.se3");
    sprintf(TV.Texture_Filepath,"resources/models/TV.sei");
    TV.Texture_Filepath[23]='\0';
    TV.Model_Filepath[23]='\0';

    TV.x = 0;
    TV.y = 0;
    TV.z = 0;

    TV.Screen_y1 = 7;
    TV.Screen_x1 = -13;
    TV.Screen_z1 = 26;

    TV.Screen_y2 = -7;
    TV.Screen_x2 = -13;
    TV.Screen_z2 = 26;

    TV.Screen_y3 = -7;
    TV.Screen_x3 = -15;
    TV.Screen_z3 = 39;

    TV.Screen_y4 = 7;
    TV.Screen_x4 = -15;
    TV.Screen_z4 = 39;

    TV.Receiver_Frequency = 0;
    TV.Receiver_Audio_Frequency = 0;
    TV.State = 0;


    struct _Seat Seat;

    memset(Seat.Model_Filepath,0,256);
    memset(Seat.Texture_Filepath,0,256);
    sprintf(Seat.Model_Filepath, "resources/data/seats/seat.se3");
    sprintf(Seat.Texture_Filepath, "resources/data/seats/seat.sei");

    Seat.Model_Filepath[29] = '\0';
    Seat.Texture_Filepath[29] = '\0';

    Seat.x=0;
    Seat.y=0;
    Seat.z=0;

    Seat.x1=-30;
    Seat.y1=-30;
    Seat.z1= 0;

    Seat.x2=30;
    Seat.y2=30;
    Seat.z2=60;

    Seat.Eye_Offset_x = 0;
    Seat.Eye_Offset_y = 0;
    Seat.Eye_Offset_z = 20;

    Seat.Is_A_Drivers_Seat = 1;





    struct _Camera C;
    C.x = -150;
    C.y = 0;
    C.z = 40;

    C.Look_x = -170;
    C.Look_y = 0;
    C.Look_z = 40;

    struct _RT_Screen RTS;

    RTS.Status = 0;
    RTS.Channel = 0;

    RTS.x = -150;
    RTS.y = 0;
    RTS.z = 30;

    memset(RTS.Model_Filepath,0,256);
    sprintf(RTS.Model_Filepath, "resources/data/ships/sonichu_screen.se3");
    RTS.Model_Filepath[39] = '\0';



    struct _Switch Switch;

    memset(Switch.Model_Filepath,0,256);
    memset(Switch.Texture_Filepath,0,256);
    sprintf(Switch.Model_Filepath, "resources/models/switch.se3");
    sprintf(Switch.Texture_Filepath, "resources/models/switch.sei");
    Switch.Model_Filepath[27] = '\0';
    Switch.Texture_Filepath[27] = '\0';

    Switch.x = 0;
    Switch.y = -30;
    Switch.z = 30;

    Switch.x2 = 0+0;
    Switch.y2 = -30+20;
    Switch.z2 = 30+40;
    Switch.State = 0;

    struct _Terrain Terrain;
    memset(Terrain.Name,0,256);
    memset(Terrain.Texture_Filepath,0,256);
    sprintf(Terrain.Name, "resources/data/ships/test_ship_internal.se3");
    sprintf(Terrain.Texture_Filepath, "resources/data/ships/test_ship_internal.sei");
    Terrain.Name[43] ='\0';
    Terrain.Texture_Filepath[43] = '\0';
    Terrain.x = 0;
    Terrain.y = 0;
    Terrain.z = 0;


    char Wall_Collision_Mesh[256];
    char Ceiling_Collision_Mesh[256];

    memset(Wall_Collision_Mesh,0,256);
    memset(Ceiling_Collision_Mesh,0,256);

    sprintf(Wall_Collision_Mesh, "resources/data/ships/test_ship_internal_wall_cg.se3");
    sprintf(Ceiling_Collision_Mesh, "resources/data/ships/test_ship_internal_ceil_cg.se3");

    Wall_Collision_Mesh[51] = '\0';
    Ceiling_Collision_Mesh[51] = '\0';


    struct _Docking_Port DP;
    DP.x = 0;
    DP.y = -30;
    DP.z = 0;

    DP.Destination_X = 0;
    DP.Destination_Y = -30;
    DP.Destination_Z = 10;


    FILE* f = fopen(Filename, "wb");
    fwrite(&Ship, 1, sizeof(struct _Ship_Details),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Terrains, SEEK_SET);
    fwrite(&Terrain, 1, sizeof(struct _Terrain),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Wall_Collision_Meshes, SEEK_SET);
    fwrite(&Wall_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Ceiling_Collision_Meshes, SEEK_SET);
    fwrite(&Ceiling_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Docking_Ports, SEEK_SET);
    fwrite(&DP, 1, sizeof(struct _Docking_Port),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Switches, SEEK_SET);
    fwrite(&Switch, 1, sizeof(struct _Switch),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_On_Board_Cameras, SEEK_SET);
    fwrite(&C, 1, sizeof(struct _Camera),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_RT_Screens, SEEK_SET);
    fwrite(&RTS, 1, sizeof(struct _RT_Screen),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Seats, SEEK_SET);
    fwrite(&Seat, 1, sizeof(struct _Seat),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_TVs, SEEK_SET);
    fwrite(&TV, 1, sizeof(struct _On_Foot_TV),f);
    rewind(f);

    fseek(f, Ship.Internal_Layout.Offset_To_Combo_Seat_Screens, SEEK_SET);
    fwrite(&Combo, 1, sizeof(struct _Combo_Seat_Screen),f);
    fclose(f);
}

void Write_Region(struct _Engine* Engine, const char* Filename)
{
    struct _Region Region;

    memset(Region.Region_Name,0,256);
    memset(Region.Region_Description,0,256);
    sprintf(Region.Region_Name, "Test Region");
    sprintf(Region.Region_Description, "Test Description");
    Region.Default_Spawnpoint[0] = 0;
    Region.Default_Spawnpoint[1] = 0;
    Region.Default_Spawnpoint[2] = 0;

    Region.Number_Of_Skyboxes = 0;
    Region.Number_Of_Ships = 0;
    Region.Number_Of_Wall_Collision_Meshes = 1;
    Region.Number_Of_Ceiling_Collision_Meshes = 1;
    Region.Number_Of_Goalposts = 0;
    Region.Number_Of_Characters = 1;
    Region.Number_Of_Cars = 0;
    Region.Number_Of_Submarines = 2;
    Region.Number_Of_Fires = 0;
    Region.Number_Of_Terrains = 20;
    Region.Number_Of_Docking_Ports = 0;
    Region.Number_Of_Switches = 0;
    Region.Number_Of_On_Board_Cameras = 0;
    Region.Number_Of_RT_Screens = 0;
    Region.Number_Of_Slope_Collision_Meshes = 1;
    Region.Number_Of_Seats = 0;
    Region.Number_Of_Vortexes = 1;
    Region.Number_Of_TVs = 0;
    Region.Number_Of_Combo_Seat_Screens = 0;

    int Size_Of_Skyboxes = Region.Number_Of_Skyboxes * sizeof(struct _Skybox);
    int Size_Of_Ships = Region.Number_Of_Ships  * sizeof(struct _Ship);
    int Size_Of_Wall_Collision_Meshes = Region.Number_Of_Wall_Collision_Meshes * 256;
    int Size_Of_Ceiling_Collision_Meshes = Region.Number_Of_Ceiling_Collision_Meshes * 256;
    int Size_Of_Goalposts = Region.Number_Of_Goalposts  * sizeof(struct _Goalpost);
    int Size_Of_Characters = Region.Number_Of_Characters * 256;
    int Size_Of_Cars = Region.Number_Of_Cars  * sizeof(struct _Car);
    int Size_Of_Submarines = Region.Number_Of_Submarines  * sizeof(struct _Submarine);
    int Size_Of_Fires = Region.Number_Of_Fires  * sizeof(struct _Fire);
    int Size_Of_Terrains = Region.Number_Of_Terrains  * sizeof(struct _Terrain);
    int Size_Of_Docking_Ports = Region.Number_Of_Docking_Ports  * sizeof(struct _Docking_Port);
    int Size_Of_Switches = Region.Number_Of_Switches  * sizeof(struct _Switch);
    int Size_Of_On_Board_Cameras = Region.Number_Of_On_Board_Cameras  * sizeof(struct _Camera);
    int Size_Of_RT_Screens = Region.Number_Of_RT_Screens  * sizeof(struct _RT_Screen);
    int Size_Of_Slope_Collision_Meshes = Region.Number_Of_Slope_Collision_Meshes * 256;
    int Size_Of_Seats = Region.Number_Of_Seats  * sizeof(struct _Seat);
    int Size_Of_Vortexes = Region.Number_Of_Vortexes  * sizeof(struct _Vortex);
    int Size_Of_TVs = Region.Number_Of_TVs  *  sizeof(struct _On_Foot_TV);
    int Size_Of_Combo_Seat_Screens = Region.Number_Of_Combo_Seat_Screens  * sizeof(struct _Combo_Seat_Screen);

    Region.Offset_To_Skyboxes = sizeof(struct _Region);
    Region.Offset_To_Ships = Region.Offset_To_Skyboxes + Size_Of_Skyboxes;
    Region.Offset_To_Wall_Collision_Meshes = Region.Offset_To_Ships + Size_Of_Ships;
    Region.Offset_To_Ceiling_Collision_Meshes = Region.Offset_To_Wall_Collision_Meshes + Size_Of_Wall_Collision_Meshes;
    Region.Offset_To_Goalposts = Region.Offset_To_Ceiling_Collision_Meshes + Size_Of_Ceiling_Collision_Meshes;
    Region.Offset_To_Characters = Region.Offset_To_Goalposts + Size_Of_Goalposts;
    Region.Offset_To_Cars = Region.Offset_To_Characters + Size_Of_Characters;
    Region.Offset_To_Submarines = Region.Offset_To_Cars+Size_Of_Cars;
    Region.Offset_To_Fires = Region.Offset_To_Submarines + Size_Of_Submarines;
    Region.Offset_To_Terrains = Region.Offset_To_Fires+Size_Of_Fires;
    Region.Offset_To_Docking_Ports = Region.Offset_To_Terrains + Size_Of_Terrains;
    Region.Offset_To_Switches = Region.Offset_To_Docking_Ports + Size_Of_Docking_Ports;
    Region.Offset_To_On_Board_Cameras = Region.Offset_To_Switches + Size_Of_Switches;
    Region.Offset_To_RT_Screens = Region.Offset_To_On_Board_Cameras + Size_Of_On_Board_Cameras;
    Region.Offset_To_Slope_Collision_Meshes = Region.Offset_To_RT_Screens + Size_Of_RT_Screens;
    Region.Offset_To_Seats = Region.Offset_To_Slope_Collision_Meshes + Size_Of_Slope_Collision_Meshes;
    Region.Offset_To_Vortexes = Region.Offset_To_Seats + Size_Of_Seats;
    Region.Offset_To_TVs = Region.Offset_To_Vortexes + Size_Of_Vortexes;
    Region.Offset_To_Combo_Seat_Screens = Region.Offset_To_TVs + Size_Of_TVs;

    struct _Seat Seat;

    memset(Seat.Model_Filepath,0,256);
    memset(Seat.Texture_Filepath,0,256);
    sprintf(Seat.Model_Filepath, "resources/data/seats/seat.se3");
    sprintf(Seat.Texture_Filepath, "resources/data/seats/seat.sei");

    Seat.Model_Filepath[29] = '\0';
    Seat.Texture_Filepath[29] = '\0';

    Seat.x=0;
    Seat.y=0;
    Seat.z=0;

    Seat.x1=-30;
    Seat.y1=-30;
    Seat.z1= 0;

    Seat.x2=30;
    Seat.y2=30;
    Seat.z2=60;

    Seat.Eye_Offset_x = 0;
    Seat.Eye_Offset_y = 0;
    Seat.Eye_Offset_z = 20;

    Seat.Is_A_Drivers_Seat = 1;
    Seat.Seat_Contains_Console = 1;

    Seat.Screen_y3=6.24936;
    Seat.Screen_x3=-20.31796;
    Seat.Screen_z3=27.15369;

    Seat.Screen_y2=6.24936;
    Seat.Screen_x2=-22.14693;
    Seat.Screen_z2=39.65241;

    Seat.Screen_y1=-6.24936;
    Seat.Screen_x1=-22.14693;
    Seat.Screen_z1=39.65241;

    Seat.Screen_y4=-6.24936;
    Seat.Screen_x4=-20.31796;
    Seat.Screen_z4=27.15369;


    struct _Vortex V;
    V.x = 200;
    V.y = 200;
    V.z = 200;
    V.Suction_Force = 5;
    V.Gravity_Radius = METER_CONVERSION*10;


    BMPtoSEI("/home/drew/Desktop/M263/SonichuTrek-MAR2026/resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_clothes_pile.bmp","/home/drew/Desktop/M263/SonichuTrek-MAR2026/resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_clothes_pile.sei");


    char Character_Filepath[256];
    memset(Character_Filepath,0,256);
    sprintf(Character_Filepath,"resources/data/characters/npc/rosechu/rosechu.npc");
    Character_Filepath[49] = '\0';

    struct _Terrain Terrain;
    memset(Terrain.Name,0,256);
    memset(Terrain.Texture_Filepath,0,256);
    sprintf(Terrain.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_bed.se3");
    sprintf(Terrain.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_bed.sei");
    Terrain.Name[75] ='\0';
    Terrain.Texture_Filepath[75] = '\0';
    Terrain.x = 0;
    Terrain.y = 0;
    Terrain.z = 0;


    struct _Terrain TerrainA;
    memset(TerrainA.Name,0,256);
    memset(TerrainA.Texture_Filepath,0,256);
    sprintf(TerrainA.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_headboard.se3");
    sprintf(TerrainA.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_headboard.sei");
    TerrainA.Name[81] ='\0';
    TerrainA.Texture_Filepath[81] = '\0';
    TerrainA.x = 0;
    TerrainA.y = 0;
    TerrainA.z = 0;


    struct _Terrain TerrainB;
    memset(TerrainB.Name,0,256);
    memset(TerrainB.Texture_Filepath,0,256);
    sprintf(TerrainB.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_tv_shelf.se3");
    sprintf(TerrainB.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_tv_shelf.sei");
    TerrainB.Name[80] ='\0';
    TerrainB.Texture_Filepath[80] = '\0';
    TerrainB.x = 0;
    TerrainB.y = 0;
    TerrainB.z = 0;



    struct _Terrain TerrainC;
    memset(TerrainC.Name,0,256);
    memset(TerrainC.Texture_Filepath,0,256);
    sprintf(TerrainC.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_master_shelf.se3");
    sprintf(TerrainC.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_master_shelf.sei");
    TerrainC.Name[84] ='\0';
    TerrainC.Texture_Filepath[84] = '\0';
    TerrainC.x = 0;
    TerrainC.y = 0;
    TerrainC.z = 0;



    struct _Terrain TerrainD;
    memset(TerrainD.Name,0,256);
    memset(TerrainD.Texture_Filepath,0,256);
    sprintf(TerrainD.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_ceiling_and_carpet.se3");
    sprintf(TerrainD.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_ceiling_and_carpet.sei");
    TerrainD.x = 0;
    TerrainD.y = 0;
    TerrainD.z = 0;



    struct _Terrain TerrainE;
    memset(TerrainE.Name,0,256);
    memset(TerrainE.Texture_Filepath,0,256);
    sprintf(TerrainE.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_behind_tv_wall.se3");
    sprintf(TerrainE.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_behind_tv_wall.sei");
    TerrainE.x = 0;
    TerrainE.y = 0;
    TerrainE.z = 0;



    struct _Terrain TerrainF;
    memset(TerrainF.Name,0,256);
    memset(TerrainF.Texture_Filepath,0,256);
    sprintf(TerrainF.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_right_wall.se3");
    sprintf(TerrainF.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_right_wall.sei");
    TerrainF.x = 0;
    TerrainF.y = 0;
    TerrainF.z = 0;



    struct _Terrain TerrainG;
    memset(TerrainG.Name,0,256);
    memset(TerrainG.Texture_Filepath,0,256);
    sprintf(TerrainG.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_window_wall.se3");
    sprintf(TerrainG.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_window_wall.sei");
    TerrainG.x = 0;
    TerrainG.y = 0;
    TerrainG.z = 0;


    struct _Terrain TerrainH;
    memset(TerrainH.Name,0,256);
    memset(TerrainH.Texture_Filepath,0,256);
    sprintf(TerrainH.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_wall_near_door.se3");
    sprintf(TerrainH.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_wall_near_door.sei");
    TerrainH.x = 0;
    TerrainH.y = 0;
    TerrainH.z = 0;



    struct _Terrain TerrainI;
    memset(TerrainI.Name,0,256);
    memset(TerrainI.Texture_Filepath,0,256);
    sprintf(TerrainI.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_wall_near_megan_shelf.se3");
    sprintf(TerrainI.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_wall_near_megan_shelf.sei");
    TerrainI.x = 0;
    TerrainI.y = 0;
    TerrainI.z = 0;







    struct _Terrain TerrainJ;
    memset(TerrainJ.Name,0,256);
    memset(TerrainJ.Texture_Filepath,0,256);
    sprintf(TerrainJ.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_tall_and_small_shelf.se3");
    sprintf(TerrainJ.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_tall_and_small_shelf.sei");
    TerrainJ.x = 0;
    TerrainJ.y = 0;
    TerrainJ.z = 0;



    struct _Terrain TerrainK;
    memset(TerrainK.Name,0,256);
    memset(TerrainK.Texture_Filepath,0,256);
    sprintf(TerrainK.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_door_shelf.se3");
    sprintf(TerrainK.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_door_shelf.sei");
    TerrainK.x = 0;
    TerrainK.y = 0;
    TerrainK.z = 0;




     struct _Terrain TerrainL;
    memset(TerrainL.Name,0,256);
    memset(TerrainL.Texture_Filepath,0,256);
    sprintf(TerrainL.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_lego_table.se3");
    sprintf(TerrainL.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_lego_table.sei");
    TerrainL.x = 0;
    TerrainL.y = 0;
    TerrainL.z = 0;




         struct _Terrain TerrainM;
    memset(TerrainM.Name,0,256);
    memset(TerrainM.Texture_Filepath,0,256);
    sprintf(TerrainM.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_megan_shelf.se3");
    sprintf(TerrainM.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_megan_shelf.sei");
    TerrainM.x = 0;
    TerrainM.y = 0;
    TerrainM.z = 0;


        struct _Terrain TerrainN;
    memset(TerrainN.Name,0,256);
    memset(TerrainN.Texture_Filepath,0,256);
    sprintf(TerrainN.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_sofa.se3");
    sprintf(TerrainN.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_sofa.sei");
    TerrainN.x = 0;
    TerrainN.y = 0;
    TerrainN.z = 0;


         struct _Terrain TerrainO;
    memset(TerrainO.Name,0,256);
    memset(TerrainO.Texture_Filepath,0,256);
    sprintf(TerrainO.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_clothes_pile.se3");
    sprintf(TerrainO.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_clothes_pile.sei");
    TerrainO.x = 0;
    TerrainO.y = 0;
    TerrainO.z = 0;


         struct _Terrain TerrainP;
    memset(TerrainP.Name,0,256);
    memset(TerrainP.Texture_Filepath,0,256);
    sprintf(TerrainP.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_door.se3");
    sprintf(TerrainP.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_door.sei");
    TerrainP.x = 0;
    TerrainP.y = 0;
    TerrainP.z = 0;


          struct _Terrain TerrainQ;
    memset(TerrainQ.Name,0,256);
    memset(TerrainQ.Texture_Filepath,0,256);
    sprintf(TerrainQ.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_lamp.se3");
    sprintf(TerrainQ.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_lamp.sei");
    TerrainQ.x = 0;
    TerrainQ.y = 0;
    TerrainQ.z = 0;


           struct _Terrain TerrainR;
    memset(TerrainR.Name,0,256);
    memset(TerrainR.Texture_Filepath,0,256);
    sprintf(TerrainR.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_chair.se3");
    sprintf(TerrainR.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_chair.sei");
    TerrainR.x = 0;
    TerrainR.y = 0;
    TerrainR.z = 0;

             struct _Terrain TerrainS;
    memset(TerrainS.Name,0,256);
    memset(TerrainS.Texture_Filepath,0,256);
    sprintf(TerrainS.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_hallway.se3");
    sprintf(TerrainS.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/chris_room_hallway.sei");
    TerrainS.x = 0;
    TerrainS.y = 0;
    TerrainS.z = 0;


    struct _Ship Ship;

    memset(Ship.Ship_Filepath,0,256);
    sprintf(Ship.Ship_Filepath, "resources/data/ships/test_ship_details.det");
    Ship.Ship_Filepath[42] ='\0';

    Ship.Shell_x = 0;
    Ship.Shell_y = 0;
    Ship.Shell_z = 0;

    Ship.Shell_Roll = 0;
    Ship.Shell_Yaw = 0;
    Ship.Shell_Pitch = 0;

    Ship.Tied_To_This_Submarine = 0;





    char Wall_Collision_Mesh[256];
    char Ceiling_Collision_Mesh[256];
    char Slope_Collision_Mesh[256];

    memset(Wall_Collision_Mesh,0,256);
    memset(Ceiling_Collision_Mesh,0,256);
    memset(Slope_Collision_Mesh,0,256);

    sprintf(Wall_Collision_Mesh, "resources/data/arcs/the_days_of_nova/regions/test_region/wall_cg.se3");
    sprintf(Ceiling_Collision_Mesh, "resources/data/arcs/the_days_of_nova/regions/test_region/ceil_cg.se3");
    sprintf(Slope_Collision_Mesh, "resources/data/arcs/the_days_of_nova/regions/test_region/slope_cg.se3");

    Wall_Collision_Mesh[68] = '\0';
    Ceiling_Collision_Mesh[68] = '\0';
    Ceiling_Collision_Mesh[69] = '\0';

    struct _Goalpost Goalpost;
    memset(Goalpost.Name,0,256);
    memset(Goalpost.Texture_Filepath,0,256);

    sprintf(Goalpost.Name, "resources/models/model_goalpost.se3");
    sprintf(Goalpost.Texture_Filepath, "resources/models/texture_goalpost.sei");

    Goalpost.Name[35] = '\0';
    Goalpost.Texture_Filepath[37] = '\0';

    Goalpost.x = 60;
    Goalpost.y = 60;
    Goalpost.z = 0;

    Goalpost.Enable_This_Exit = LEVEL_CLEARED_YELLOW_FORWARDS;




    struct _Goalpost Easy_Goalpost;
    memset(Easy_Goalpost.Name,0,256);
    memset(Easy_Goalpost.Texture_Filepath,0,256);

    sprintf(Easy_Goalpost.Name, "resources/models/model_goalpost.se3");
    sprintf(Easy_Goalpost.Texture_Filepath, "resources/models/texture_goalpost.sei");

    Easy_Goalpost.Name[35] = '\0';
    Easy_Goalpost.Texture_Filepath[37] = '\0';

    Easy_Goalpost.x = -60;
    Easy_Goalpost.y = -60;
    Easy_Goalpost.z = 0;

    Easy_Goalpost.Enable_This_Exit = LEVEL_CLEARED_YELLOW_UPWARDS;

    struct _Car Car;
    memset(Car.Name,0,256);
    memset(Car.Texture_Filepath,0,256);

    sprintf(Car.Name, "resources/data/cars/car.se3");
    sprintf(Car.Texture_Filepath, "resources/data/cars/car.sei");
    Car.Name[27] = '\0';
    Car.Texture_Filepath[27] = '\0';
    Car.x = 100;
    Car.y = 0;
    Car.z = 10;
    Car.Yaw = 0;



    struct _Submarine Submarine;
    memset(Submarine.Name,0,256);
    memset(Submarine.Texture_Filepath,0,256);

    sprintf(Submarine.Name, "resources/data/submarines/car.se3");
    sprintf(Submarine.Texture_Filepath, "resources/data/submarines/car.sei");
    Submarine.Name[33] = '\0';
    Submarine.Texture_Filepath[33] = '\0';
    Submarine.x = 1000;
    Submarine.y = 100;
    Submarine.z = 50;
    Submarine.Yaw = 270;
    Submarine.Pitch = 0;
    Submarine.Roll = 0;
    Submarine.Single_Pilot = 1;


    struct _Submarine SubmarineT;
    memset(SubmarineT.Name,0,256);
    memset(SubmarineT.Texture_Filepath,0,256);

    sprintf(SubmarineT.Name, "resources/data/submarines/car.se3");
    sprintf(SubmarineT.Texture_Filepath, "resources/data/submarines/car.sei");
    SubmarineT.Name[33] = '\0';
    SubmarineT.Texture_Filepath[33] = '\0';
    SubmarineT.x = 0;
    SubmarineT.y = 1000;
    SubmarineT.z = 50;
    SubmarineT.Yaw = 270;
    SubmarineT.Pitch = 0;
    SubmarineT.Roll = 0;
    SubmarineT.Single_Pilot = 1;


    struct _Fire Fire;
    Fire.Fire_Damage_Radius = 30;
    Fire.Fire_Size = 0.75;
    Fire.x = 0;
    Fire.y = 0;
    Fire.z = 5;


    FILE* f = fopen(Filename, "wb");
    fwrite(&Region, 1, sizeof(struct _Region),f);
    rewind(f);

    fseek(f, Region.Offset_To_Ships, SEEK_SET);
    //fwrite(&Ship, 1, sizeof(struct _Ship),f);
    rewind(f);

    fseek(f, Region.Offset_To_Wall_Collision_Meshes, SEEK_SET);
    fwrite(&Wall_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Region.Offset_To_Ceiling_Collision_Meshes, SEEK_SET);
    fwrite(&Ceiling_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Region.Offset_To_Goalposts, SEEK_SET);
    //fwrite(&Goalpost, 1, sizeof(struct _Goalpost),f);
    //fwrite(&Easy_Goalpost, 1, sizeof(struct _Goalpost),f);
    rewind(f);

    fseek(f, Region.Offset_To_Cars, SEEK_SET);
    //fwrite(&Car, 1, sizeof(struct _Car),f);
    rewind(f);


    fseek(f, Region.Offset_To_Submarines, SEEK_SET);
    fwrite(&Submarine, 1, sizeof(struct _Submarine),f);
    fwrite(&SubmarineT, 1, sizeof(struct _Submarine),f);
    rewind(f);

    fseek(f, Region.Offset_To_Fires, SEEK_SET);
    //fwrite(&Fire, 1, sizeof(struct _Fire),f);
    rewind(f);

    fseek(f, Region.Offset_To_Characters, SEEK_SET);
    fwrite(&Character_Filepath, 1, 256,f);
    rewind(f);

    fseek(f, Region.Offset_To_Terrains, SEEK_SET);
    fwrite(&Terrain, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainA, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainB, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainC, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainD, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainE, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainF, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainG, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainH, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainI, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainJ, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainK, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainL, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainM, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainN, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainO, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainP, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainQ, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainR, 1, sizeof(struct _Terrain),f);
    fwrite(&TerrainS, 1, sizeof(struct _Terrain),f);
    rewind(f);

    fseek(f, Region.Offset_To_Slope_Collision_Meshes, SEEK_SET);
    fwrite(&Slope_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Region.Offset_To_Vortexes, SEEK_SET);
    fwrite(&V, 1, sizeof(struct _Vortex),f);
    rewind(f);

    fseek(f, Region.Offset_To_Seats, SEEK_SET);
    //fwrite(&Seat, 1, sizeof(struct _Seat),f);
    fclose(f);
}

void Load_Region(struct _Engine* Engine, const char* Filename)
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
    Engine->On_Foot_State->Region.Region_Name[j] = Buffer[Buffer_Location+j];
    Engine->On_Foot_State->Region.Region_Description[j] = Buffer[Buffer_Location+j+256];
    }
    Buffer_Location += 512;

    Engine->On_Foot_State->Region.Default_Spawnpoint[0] = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Region.Default_Spawnpoint[1] = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->Region.Default_Spawnpoint[2] = *(double*)&(Buffer[Buffer_Location+16]);
    Buffer_Location += 24;

    Engine->On_Foot_State->Region.Number_Of_Skyboxes = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Region.Number_Of_Ships = *(int*)&(Buffer[Buffer_Location+4]);
    Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+12]);
    Engine->On_Foot_State->Region.Number_Of_Goalposts = *(int*)&(Buffer[Buffer_Location+16]);
    Engine->On_Foot_State->Region.Number_Of_Characters = *(int*)&(Buffer[Buffer_Location+20]);
    Engine->On_Foot_State->Region.Number_Of_Cars = *(int*)&(Buffer[Buffer_Location+24]);
    Engine->On_Foot_State->Region.Number_Of_Submarines = *(int*)&(Buffer[Buffer_Location+28]);
    Engine->On_Foot_State->Region.Number_Of_Fires = *(int*)&(Buffer[Buffer_Location+32]);
    Engine->On_Foot_State->Region.Number_Of_Terrains = *(int*)&(Buffer[Buffer_Location+36]);
    Engine->On_Foot_State->Region.Number_Of_Docking_Ports = *(int*)&(Buffer[Buffer_Location+40]);
    Engine->On_Foot_State->Region.Number_Of_Switches = *(int*)&(Buffer[Buffer_Location+44]);
    Engine->On_Foot_State->Region.Number_Of_On_Board_Cameras = *(int*)&(Buffer[Buffer_Location+48]);
    Engine->On_Foot_State->Region.Number_Of_RT_Screens = *(int*)&(Buffer[Buffer_Location+52]);
    Engine->On_Foot_State->Region.Number_Of_Slope_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+56]);
    Engine->On_Foot_State->Region.Number_Of_Seats = *(int*)&(Buffer[Buffer_Location+60]);
    Engine->On_Foot_State->Region.Number_Of_Vortexes = *(int*)&(Buffer[Buffer_Location+64]);
    Engine->On_Foot_State->Region.Number_Of_TVs = *(int*)&(Buffer[Buffer_Location+68]);
    Engine->On_Foot_State->Region.Number_Of_Combo_Seat_Screens = *(int*)&(Buffer[Buffer_Location+72]);

    Engine->On_Foot_State->Region.Offset_To_Skyboxes = *(int*)&(Buffer[Buffer_Location+76]);
    Engine->On_Foot_State->Region.Offset_To_Ships = *(int*)&(Buffer[Buffer_Location+80]);
    Engine->On_Foot_State->Region.Offset_To_Wall_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+84]);
    Engine->On_Foot_State->Region.Offset_To_Ceiling_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+88]);
    Engine->On_Foot_State->Region.Offset_To_Goalposts = *(int*)&(Buffer[Buffer_Location+92]);
    Engine->On_Foot_State->Region.Offset_To_Characters = *(int*)&(Buffer[Buffer_Location+96]);
    Engine->On_Foot_State->Region.Offset_To_Cars = *(int*)&(Buffer[Buffer_Location+100]);
    Engine->On_Foot_State->Region.Offset_To_Submarines = *(int*)&(Buffer[Buffer_Location+104]);
    Engine->On_Foot_State->Region.Offset_To_Fires = *(int*)&(Buffer[Buffer_Location+108]);
    Engine->On_Foot_State->Region.Offset_To_Terrains = *(int*)&(Buffer[Buffer_Location+112]);
    Engine->On_Foot_State->Region.Offset_To_Docking_Ports = *(int*)&(Buffer[Buffer_Location+116]);
    Engine->On_Foot_State->Region.Offset_To_Switches = *(int*)&(Buffer[Buffer_Location+120]);
    Engine->On_Foot_State->Region.Offset_To_On_Board_Cameras = *(int*)&(Buffer[Buffer_Location+124]);
    Engine->On_Foot_State->Region.Offset_To_RT_Screens = *(int*)&(Buffer[Buffer_Location+128]);
    Engine->On_Foot_State->Region.Offset_To_Slope_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+132]);
    Engine->On_Foot_State->Region.Offset_To_Seats = *(int*)&(Buffer[Buffer_Location+136]);
    Engine->On_Foot_State->Region.Offset_To_Vortexes = *(int*)&(Buffer[Buffer_Location+140]);
    Engine->On_Foot_State->Region.Offset_To_TVs = *(int*)&(Buffer[Buffer_Location+144]);
    Engine->On_Foot_State->Region.Offset_To_Combo_Seat_Screens = *(int*)&(Buffer[Buffer_Location+148]);

    ////printf("Number Of Skyboxes: %d\n", Engine->On_Foot_State->Region.Number_Of_Skyboxes);
    ////printf("Offset To Skyboxes: %d\n", Engine->On_Foot_State->Region.Offset_To_Skyboxes);

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Seats; m++)
    {
        Engine->On_Foot_State->Seat_Objects[m] = (struct _Seat_Object*)calloc(Engine->On_Foot_State->Region.Number_Of_Seats,sizeof(struct _Seat_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Vortexes; m++)
    {
        Engine->On_Foot_State->Vortexes[m] = (struct _Vortex*)calloc(Engine->On_Foot_State->Region.Number_Of_Vortexes,sizeof(struct _Vortex));
    }


    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Skyboxes; m++)
    {
        Engine->On_Foot_State->Skybox_Objects[m] = (struct _Skybox_Object*)calloc(Engine->On_Foot_State->Region.Number_Of_Skyboxes,sizeof(struct _Skybox_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Ships; m++)
    {
        Engine->On_Foot_State->Ship_Objects[m] = (struct _Ship_Object*)calloc(Engine->On_Foot_State->Region.Number_Of_Ships,sizeof(struct _Ship_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; m++)
    {
        Engine->On_Foot_State->Wall_Collision_Meshes[m] = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; m++)
    {
        Engine->On_Foot_State->Ceiling_Collision_Meshes[m] = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Goalposts; m++)
    {
        Engine->On_Foot_State->Goalposts[m] = (struct _Goalpost_Object*)calloc(1,sizeof(struct _Goalpost_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Characters; m++)
    {
        Engine->On_Foot_State->NPC_Character_Objects[m] = (struct _Character_Object*)calloc(1,sizeof(struct _Character_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Cars; m++)
    {
        Engine->On_Foot_State->Cars[m] = (struct _New_Player_Car*)calloc(1,sizeof(struct _New_Player_Car));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Submarines; m++)
    {
        Engine->On_Foot_State->Submarines[m] = (struct _Submarine_Object*)calloc(1,sizeof(struct _Submarine_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Fires; m++)
    {
        Engine->On_Foot_State->Fires[m] = (struct _Fire_Object*)calloc(1,sizeof(struct _Fire_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Terrains; m++)
    {
        Engine->On_Foot_State->Terrain_Objects[m] = (struct _Terrain_Object*)calloc(1,sizeof(struct _Terrain_Object));
    }

    for(int m = 0; m < Engine->On_Foot_State->Region.Number_Of_Slope_Collision_Meshes; m++)
    {
        Engine->On_Foot_State->Slope_Collision_Meshes[m] = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Vortexes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Vortexes + (j * sizeof(struct _Vortex));

        double x = *(double*)&(Buffer[Buffer_Location]);
        double y = *(double*)&(Buffer[Buffer_Location+8]);
        double z = *(double*)&(Buffer[Buffer_Location+16]);
        double Gravity_Radius = *(double*)&(Buffer[Buffer_Location+24]);
        double Suction_Force = *(double*)&(Buffer[Buffer_Location+32]);

        Engine->On_Foot_State->Vortexes[j]->x = x;
        Engine->On_Foot_State->Vortexes[j]->y = y;
        Engine->On_Foot_State->Vortexes[j]->z = z;

        Engine->On_Foot_State->Vortexes[j]->Gravity_Radius = Gravity_Radius;
        Engine->On_Foot_State->Vortexes[j]->Suction_Force = Suction_Force;
    }


    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Seats; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Seats + (j * sizeof(struct _Seat));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        Buffer_Location += 512;

        double x = *(double*)&(Buffer[Buffer_Location]);
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

        int Is_A_Drivers_Seat = *(int*)&(Buffer[Buffer_Location+96]);
        int Seat_Contains_Console = *(int*)&(Buffer[Buffer_Location+100]);


        double Screen_x1 = *(double*)&(Buffer[Buffer_Location+104]);
        double Screen_y1 = *(double*)&(Buffer[Buffer_Location+112]);
        double Screen_z1 = *(double*)&(Buffer[Buffer_Location+120]);

        double Screen_x2 = *(double*)&(Buffer[Buffer_Location+128]);
        double Screen_y2 = *(double*)&(Buffer[Buffer_Location+136]);
        double Screen_z2 = *(double*)&(Buffer[Buffer_Location+144]);

        double Screen_x3 = *(double*)&(Buffer[Buffer_Location+152]);
        double Screen_y3 = *(double*)&(Buffer[Buffer_Location+160]);
        double Screen_z3 = *(double*)&(Buffer[Buffer_Location+168]);

        double Screen_x4 = *(double*)&(Buffer[Buffer_Location+176]);
        double Screen_y4 = *(double*)&(Buffer[Buffer_Location+184]);
        double Screen_z4 = *(double*)&(Buffer[Buffer_Location+192]);


        Engine->On_Foot_State->Seat_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Seat_Objects[j]->Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Seat_Objects[j]->Model, Texture_Filename);

        Engine->On_Foot_State->Seat_Objects[j]->x = x;
        Engine->On_Foot_State->Seat_Objects[j]->y = y;
        Engine->On_Foot_State->Seat_Objects[j]->z = z;
        Engine->On_Foot_State->Seat_Objects[j]->Collision_Box.min.x = x1 + x;
        Engine->On_Foot_State->Seat_Objects[j]->Collision_Box.min.y = y1 + y;
        Engine->On_Foot_State->Seat_Objects[j]->Collision_Box.min.z = z1 + z;

        Engine->On_Foot_State->Seat_Objects[j]->Collision_Box.max.x = x2 + x;
        Engine->On_Foot_State->Seat_Objects[j]->Collision_Box.max.y = y2 + y;
        Engine->On_Foot_State->Seat_Objects[j]->Collision_Box.max.z = z2 + z;

        Engine->On_Foot_State->Seat_Objects[j]->Eye_Offset_x = Eye_Offset_x + x;
        Engine->On_Foot_State->Seat_Objects[j]->Eye_Offset_y = Eye_Offset_y + y;
        Engine->On_Foot_State->Seat_Objects[j]->Eye_Offset_z = Eye_Offset_z + z;

        Engine->On_Foot_State->Seat_Objects[j]->Seat_Contains_Console = Seat_Contains_Console;
        Engine->On_Foot_State->Seat_Objects[j]->Is_A_Drivers_Seat = Is_A_Drivers_Seat;


        Engine->On_Foot_State->Seat_Objects[j]->Screen_x1 = Screen_x1;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_y1 = Screen_y1;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_z1 = Screen_z1;

    Engine->On_Foot_State->Seat_Objects[j]->Screen_x2 = Screen_x2;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_y2 = Screen_y2;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_z2 = Screen_z2;

    Engine->On_Foot_State->Seat_Objects[j]->Screen_x3 = Screen_x3;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_y3 = Screen_y3;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_z3 = Screen_z3;

    Engine->On_Foot_State->Seat_Objects[j]->Screen_x4 = Screen_x4;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_y4 = Screen_y4;
    Engine->On_Foot_State->Seat_Objects[j]->Screen_z4 = Screen_z4;

    Engine->On_Foot_State->Seat_Objects[j]->New_Computer = Create_New_Computer();
    Initialize_New_Computer(Engine, Engine->On_Foot_State->Seat_Objects[j]);

    Load_Program(Engine->On_Foot_State->Seat_Objects[j], "test.rom");

    }






    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Terrains; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Terrains + (j * sizeof(struct _Terrain));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        ////printf(Model_Filename);
        ////printf(Texture_Filename);

        Engine->On_Foot_State->Terrain_Objects[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Terrain_Objects[j]->Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Terrain_Objects[j]->Model, Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Terrain_Objects[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Terrain_Objects[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Terrain_Objects[j]->z = *(double*)&(Buffer[Buffer_Location+16]);
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Skyboxes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Skyboxes + (j * sizeof(struct _Skybox));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        ////printf(Model_Filename);
        ////printf(Texture_Filename);

        Engine->On_Foot_State->Skybox_Objects[j]->Ship_Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Skybox_Objects[j]->Ship_Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Skybox_Objects[j]->Ship_Model, Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Skybox_Objects[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Skybox_Objects[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Skybox_Objects[j]->z = *(double*)&(Buffer[Buffer_Location+16]);
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ships; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Ships + (j * sizeof(struct _Ship));

        char Model_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
        }

        ////printf(Model_Filename);
        Buffer_Location += 256;
        Engine->On_Foot_State->Ship_Objects[j]->Shell_x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Objects[j]->Shell_Roll = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_Yaw = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_Pitch = *(double*)&(Buffer[Buffer_Location+40]);

        Engine->On_Foot_State->Ship_Objects[j]->Tied_To_This_Submarine = *(int*)&(Buffer[Buffer_Location+48]);

        Load_Ship(Engine, j, Model_Filename);
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Wall_Collision_Meshes + (j * 256);

        char Model_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
        }
        ////printf(Model_Filename);

        Engine->On_Foot_State->Wall_Collision_Meshes[j] = Create_Preloaded_Collision_Mesh(Model_Filename);
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Ceiling_Collision_Meshes + (j * 256);

        char Model_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
        }
        ////printf(Model_Filename);

        Engine->On_Foot_State->Ceiling_Collision_Meshes[j] = Create_Preloaded_Collision_Mesh(Model_Filename);
    }








    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Goalposts; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Goalposts + (j * sizeof(struct _Goalpost));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        ////printf(Model_Filename);
        ////printf(Texture_Filename);

        Engine->On_Foot_State->Goalposts[j]->Model = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Goalposts[j]->Model, Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Goalposts[j]->Model, Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Goalposts[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Goalposts[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Goalposts[j]->z = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->On_Foot_State->Goalposts[j]->Enable_This_Exit = *(int*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Goalposts[j]->Collision_Mesh = Create_Preloaded_Collision_Mesh(Model_Filename);
    }





    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Cars; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Cars + (j * sizeof(struct _Car));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        ////printf(Model_Filename);
        ////printf(Texture_Filename);

        Engine->On_Foot_State->Cars[j]->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Cars[j]->Models[0], Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Cars[j]->Models[0], Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Cars[j]->x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Cars[j]->y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Cars[j]->z = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->On_Foot_State->Cars[j]->Yaw_Angle = *(double*)&(Buffer[Buffer_Location+24]);


        Engine->On_Foot_State->Cars[j]->On_The_Ground = false;


    Engine->On_Foot_State->Cars[j]->Start_Frame = 0;
    Engine->On_Foot_State->Cars[j]->Current_Frame = 0;
    Engine->On_Foot_State->Cars[j]->End_Frame = 0;
    Engine->On_Foot_State->Cars[j]->Current_Model = 0;

    Engine->On_Foot_State->Cars[j]->Current_State = 0;

    Engine->On_Foot_State->Cars[j]->Forward_Vector.x = 0;
    Engine->On_Foot_State->Cars[j]->Forward_Vector.y = 0;
    Engine->On_Foot_State->Cars[j]->Forward_Vector.z = 0;


    Engine->On_Foot_State->Cars[j]->Right_Vector.x = 0;
    Engine->On_Foot_State->Cars[j]->Right_Vector.y = 0;
    Engine->On_Foot_State->Cars[j]->Right_Vector.z = 0;


    Engine->On_Foot_State->Cars[j]->Up_Vector.x = 0;
    Engine->On_Foot_State->Cars[j]->Up_Vector.y = 0;
    Engine->On_Foot_State->Cars[j]->Up_Vector.z = 0;



    Engine->On_Foot_State->Cars[j]->Ray_Sensor_Vector.x = 0;
    Engine->On_Foot_State->Cars[j]->Ray_Sensor_Vector.y = 0;
    Engine->On_Foot_State->Cars[j]->Ray_Sensor_Vector.z = 0;

    Engine->On_Foot_State->Cars[j]->Pitch_Angle = 0;

        Engine->On_Foot_State->Cars[j]->Throttle = 0;

    Engine->On_Foot_State->Cars[j]->Floor_Collision = false;
    Engine->On_Foot_State->Cars[j]->Wall_Collision = false;
    Engine->On_Foot_State->Cars[j]->Ceiling_Collision = false;
    Engine->On_Foot_State->Cars[j]->Object_Collision = false;

    Engine->On_Foot_State->Cars[j]->On_Top_Of_An_AABB_Collision = false;
    Engine->On_Foot_State->Cars[j]->Underneath_An_AABB_Collision = false;
    Engine->On_Foot_State->Cars[j]->Colliding_With_An_AABB_Collision = false;

    Engine->On_Foot_State->Cars[j]->On_Top_Of_Mesh_Collision = false;
    Engine->On_Foot_State->Cars[j]->Underneath_Mesh_Collision = false;
    Engine->On_Foot_State->Cars[j]->Colliding_With_Mesh_Collision = false;

    Engine->On_Foot_State->Cars[j]->Able_To_Move_Away = true;

    Engine->On_Foot_State->Cars[j]->Able_To_Move_Off_Platform = true;


    Engine->On_Foot_State->Cars[j]->Feet_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->Cars[j]->Feet_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->Cars[j]->Feet_Collision_Geometry.min.z = 0;

    Engine->On_Foot_State->Cars[j]->Feet_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->Cars[j]->Feet_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->Cars[j]->Feet_Collision_Geometry.max.z = 20.5;


    Engine->On_Foot_State->Cars[j]->Head_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->Cars[j]->Head_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->Cars[j]->Head_Collision_Geometry.min.z = 20.5;

    Engine->On_Foot_State->Cars[j]->Head_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->Cars[j]->Head_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->Cars[j]->Head_Collision_Geometry.max.z = 40.5;


    Engine->On_Foot_State->Cars[j]->Do_Not_Alter_Velocity = false;

    Engine->On_Foot_State->Cars[j]->displacement.x = 0;
    Engine->On_Foot_State->Cars[j]->displacement.y = 0;
    Engine->On_Foot_State->Cars[j]->displacement.z = 0;
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Characters; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Characters + (j * sizeof(struct _Character));
        char Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Filename[l] = Buffer[Buffer_Location+l];
        }
        Load_NPC_Character(Engine, j, Filename);
    }



    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Submarines; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Submarines + (j * sizeof(struct _Submarine));

        char Model_Filename[256];
        char Texture_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
            Texture_Filename[l] = Buffer[Buffer_Location+l+256];
        }
        ////printf(Model_Filename);
        ////printf(Texture_Filename);

        Engine->On_Foot_State->Submarines[j]->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Submarines[j]->Models[0], Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Submarines[j]->Models[0], Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Submarines[j]->plane_x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Submarines[j]->plane_y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Submarines[j]->plane_z = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->On_Foot_State->Submarines[j]->plane_yaw = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Submarines[j]->plane_pitch = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->On_Foot_State->Submarines[j]->plane_roll = *(double*)&(Buffer[Buffer_Location+40]);
        Engine->On_Foot_State->Submarines[j]->Single_Pilot = *(int*)&(Buffer[Buffer_Location+48]);

        Engine->On_Foot_State->Submarines[j]->On_The_Ground = false;


    Engine->On_Foot_State->Submarines[j]->Start_Frame = 0;
    Engine->On_Foot_State->Submarines[j]->Current_Frame = 0;
    Engine->On_Foot_State->Submarines[j]->End_Frame = 0;
    Engine->On_Foot_State->Submarines[j]->Current_Model = 0;

    Engine->On_Foot_State->Submarines[j]->Current_State = 0;

    Engine->On_Foot_State->Submarines[j]->Forward_Vector.x = 0;
    Engine->On_Foot_State->Submarines[j]->Forward_Vector.y = -1;
    Engine->On_Foot_State->Submarines[j]->Forward_Vector.z = 0;


    Engine->On_Foot_State->Submarines[j]->Right_Vector.x = 1;
    Engine->On_Foot_State->Submarines[j]->Right_Vector.y = 0;
    Engine->On_Foot_State->Submarines[j]->Right_Vector.z = 0;


    Engine->On_Foot_State->Submarines[j]->Up_Vector.x = 0;
    Engine->On_Foot_State->Submarines[j]->Up_Vector.y = 0;
    Engine->On_Foot_State->Submarines[j]->Up_Vector.z = 1;



    Engine->On_Foot_State->Submarines[j]->Ray_Sensor_Vector.x = 0;
    Engine->On_Foot_State->Submarines[j]->Ray_Sensor_Vector.y = 0;
    Engine->On_Foot_State->Submarines[j]->Ray_Sensor_Vector.z = 0;


        Engine->On_Foot_State->Submarines[j]->Throttle = 0;

    Engine->On_Foot_State->Submarines[j]->Floor_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Wall_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Ceiling_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Object_Collision = false;

    Engine->On_Foot_State->Submarines[j]->On_Top_Of_An_AABB_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Underneath_An_AABB_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Colliding_With_An_AABB_Collision = false;

    Engine->On_Foot_State->Submarines[j]->On_Top_Of_Mesh_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Underneath_Mesh_Collision = false;
    Engine->On_Foot_State->Submarines[j]->Colliding_With_Mesh_Collision = false;

    Engine->On_Foot_State->Submarines[j]->Able_To_Move_Away = true;

    Engine->On_Foot_State->Submarines[j]->Able_To_Move_Off_Platform = true;


    Engine->On_Foot_State->Submarines[j]->Feet_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->Submarines[j]->Feet_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->Submarines[j]->Feet_Collision_Geometry.min.z = 0;

    Engine->On_Foot_State->Submarines[j]->Feet_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->Submarines[j]->Feet_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->Submarines[j]->Feet_Collision_Geometry.max.z = 20.5;


    Engine->On_Foot_State->Submarines[j]->Head_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->Submarines[j]->Head_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->Submarines[j]->Head_Collision_Geometry.min.z = 20.5;

    Engine->On_Foot_State->Submarines[j]->Head_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->Submarines[j]->Head_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->Submarines[j]->Head_Collision_Geometry.max.z = 40.5;


    Engine->On_Foot_State->Submarines[j]->Do_Not_Alter_Velocity = false;

    Engine->On_Foot_State->Submarines[j]->displacement.x = 0;
    Engine->On_Foot_State->Submarines[j]->displacement.y = 0;
    Engine->On_Foot_State->Submarines[j]->displacement.z = 0;




      Engine->On_Foot_State->Submarines[j]->SPEED_STEP = 0.005f;
    Engine->On_Foot_State->Submarines[j]->ANGLE_STEP = 2.0f;
    Engine->On_Foot_State->Submarines[j]->TIMER_MS = 16; // ~60 FPS update rate

// --- Global State Variables ---
// Position (World Coordinates)

    Engine->On_Foot_State->Submarines[j]->plane_roll = 0.0f;  // Rotation around Y-axis (Forward/Backward)

// Speed (Magnitude of movement)
    Engine->On_Foot_State->Submarines[j]->plane_speed = 0.5;

    }


    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Fires; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Fires + (j * sizeof(struct _Fire));
        Engine->On_Foot_State->Fires[j]->Fire_Size = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Fires[j]->Fire_Damage_Radius = *(double*)&(Buffer[Buffer_Location+8]);

        Engine->On_Foot_State->Fires[j]->x = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->On_Foot_State->Fires[j]->y = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Fires[j]->z = *(double*)&(Buffer[Buffer_Location+32]);

        Engine->On_Foot_State->Fires[j]->Fire_Image = Create_Image();
        Load_Image(Engine->On_Foot_State->Fires[j]->Fire_Image, "resources/artwork/placeholders/fire.sei");
    }

    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Slope_Collision_Meshes; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Region.Offset_To_Slope_Collision_Meshes + (j * 256);

        char Model_Filename[256];
        for(int l = 0; l < 256; l++)
        {
            Model_Filename[l] = Buffer[Buffer_Location+l];
        }
        ////printf(Model_Filename);

        Engine->On_Foot_State->Slope_Collision_Meshes[j] = Create_Preloaded_Collision_Mesh(Model_Filename);
    }
}


void Write_Character_Object(struct _Engine* Engine, const char* Filename)
{

}

void Write_Conversation_Object(struct _Engine* Engine, const char* Filename)
{

}

void Write_Neural_Network_Object(struct _Engine* Engine, const char* Filename)
{

}

void Write_Navigation_Mesh_Object(struct _Engine* Engine, const char* Filename)
{

}


void Load_Character_Object(struct _Engine* Engine, const char* Filename)
{

}

void Load_Conversation_Object(struct _Engine* Engine, const char* Filename)
{

}

void Load_Neural_Network_Object(struct _Engine* Engine, const char* Filename)
{

}

void Load_Navigation_Mesh_Object(struct _Engine* Engine, const char* Filename)
{

}

void Check_Car_Input(struct _Engine* Engine, struct _Keypad Keypad)
{
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Cars; j++)
        {
            double x = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,Engine->On_Foot_State->Cars[j]->x,Engine->On_Foot_State->Cars[j]->y,Engine->On_Foot_State->Cars[j]->z);
            ////printf("Distance from car: %f\n", x);
            if(x <= 20)
            {
                if(Keypad.Controller.Controller_Y || Keypad.Keyboard_E)
                {
                ////printf("Entering Car\n\n\n\n");
                Engine->On_Foot_State->Player_Is_In_Car = true;
                Engine->On_Foot_State->In_This_Car = j;
                }
            }
        }

}



void Check_Submarine_Input(struct _Engine* Engine, struct _Keypad Keypad)
{
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Submarines; j++)
        {
            double x = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,Engine->On_Foot_State->Submarines[j]->plane_x,Engine->On_Foot_State->Submarines[j]->plane_y,Engine->On_Foot_State->Submarines[j]->plane_z);
            //printf("Distance from Submarine: %f\n", x);
            if(x <= 120)
            {
                if(Keypad.Controller.Controller_Y || Keypad.Keyboard_E)
                {
                    if(!Engine->On_Foot_State->Player_Is_In_Submarine)
                    {
                    if(Engine->On_Foot_State->Submarines[j]->Single_Pilot)
                    {
                    Engine->On_Foot_State->Player_Is_In_Submarine = true;
                    Engine->On_Foot_State->In_This_Submarine = j;
                    }
                    }
                }
                ////printf("Entering Submarine\n\n\n\n");
            }
        }

}



void updateCameraVectors(struct _Engine* Engine) {
    float radYaw = Engine->On_Foot_State->yaw * (M_PI / 180.0f);
    float radPitch = Engine->On_Foot_State->pitch * (M_PI / 180.0f);

    // Adjusted for Z-up, Y-forward/backward system:
    // Pitch pushes the vector up/down the Z axis
    // Yaw rotates the vector around the XY plane
    Engine->On_Foot_State->frontX = cos(radYaw) * cos(radPitch);
    Engine->On_Foot_State->frontY = sin(radYaw) * cos(radPitch);
    Engine->On_Foot_State->frontZ = sin(radPitch);

    // Normalize
    float length = sqrt(Engine->On_Foot_State->frontX * Engine->On_Foot_State->frontX + Engine->On_Foot_State->frontY * Engine->On_Foot_State->frontY + Engine->On_Foot_State->frontZ * Engine->On_Foot_State->frontZ);
    Engine->On_Foot_State->frontX /= length;
    Engine->On_Foot_State->frontY /= length;
    Engine->On_Foot_State->frontZ /= length;
}

void mouseMotion(struct _Engine* Engine, double x, double y,struct _Keypad Keypad) {
    int centerX = Keypad.Screen_Width / 2;
    int centerY = Keypad.Screen_Height / 2;

    float xoffset = (x - centerX) * Engine->On_Foot_State->sensitivity;
    float yoffset = (centerY - y) * Engine->On_Foot_State->sensitivity;

    if(Keypad.Controller_Mode == CONTROL_MODE_CONTROLLER)
    {
        xoffset = Keypad.Controller.Controller_Camera_Thumbpad[0];
        yoffset = Keypad.Controller.Controller_Camera_Thumbpad[1]*-1;
    }

    if(Engine->On_Foot_State->Is_In_Subspace)
    {
        Engine->On_Foot_State->yaw += xoffset;
    }

    else if(!Engine->On_Foot_State->Is_In_Subspace)
    {
        Engine->On_Foot_State->yaw -= xoffset;
    }
    Engine->On_Foot_State->pitch += yoffset;

    Engine->On_Foot_State->On_Foot_Player->Yaw_Angle = Engine->On_Foot_State->yaw;

    // Constrain Pitch
    if (Engine->On_Foot_State->pitch > 89.0f) Engine->On_Foot_State->pitch = 89.0f;
    if (Engine->On_Foot_State->pitch < -89.0f) Engine->On_Foot_State->pitch = -89.0f;

    if(Engine->On_Foot_State->yaw > 359)
    {
        Engine->On_Foot_State->yaw = 0;
    }

    if(Engine->On_Foot_State->yaw < 0)
    {
        Engine->On_Foot_State->yaw = 359;
    }


    updateCameraVectors(Engine);

    if (x != centerX || y != centerY) {
        glfwSetCursorPos(Engine->W, centerX, centerY);
    }
}





GLuint fbo, textureColorBuffer, rbo;

void setupFBO() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    // Initialize empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256,256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //Widescreen viewscreen.

    // Filters are important for the texture to appear correctly
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 256,256);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Write_NPC_Character(struct _Engine* Engine, const char* Filename)
{
    struct _Character C;

    memset(C.Character_Model_Filepath, 0, 256);
    memset(C.Character_Texture_Filepath, 0, 256);
    sprintf(C.Character_Model_Filepath, "resources/data/characters/npc/rosechu/rosechu.se3");
    sprintf(C.Character_Texture_Filepath, "resources/data/characters/npc/rosechu/rosechu.sei");
    C.Character_Model_Filepath[49] = '\0';
    C.Character_Texture_Filepath[49] = '\0';

    C.x = -60;
    C.y = -60;
    C.z = 100;

    FILE* f = fopen(Filename, "wb");
    fwrite(&C,1,sizeof(struct _Character),f);
    fclose(f);
}

void Load_NPC_Character(struct _Engine* Engine, int Index, const char* Filename)
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

    char Model_Filepath[256];
    char Texture_Filepath[256];

    for(int j = 0; j < 256; j++)
    {
        Model_Filepath[j] = Buffer[Buffer_Location+j];
        Texture_Filepath[j] = Buffer[Buffer_Location+j+256];
    }
    Buffer_Location += 512;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Character_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->NPC_Character_Objects[Index]->Character_Model, Model_Filepath);
    Load_SE3_Texture(Engine->On_Foot_State->NPC_Character_Objects[Index]->Character_Model, Texture_Filepath);

    Engine->On_Foot_State->NPC_Character_Objects[Index]->x = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->y = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->z = *(double*)&(Buffer[Buffer_Location+16]);

    Buffer_Location += 24;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.x = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.y = *(double*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.z = *(double*)&(Buffer[Buffer_Location+16]);

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.x = *(double*)&(Buffer[Buffer_Location+24]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.y = *(double*)&(Buffer[Buffer_Location+32]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.z = *(double*)&(Buffer[Buffer_Location+40]);



    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.x = *(double*)&(Buffer[Buffer_Location+48]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.y = *(double*)&(Buffer[Buffer_Location+56]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.z = *(double*)&(Buffer[Buffer_Location+64]);

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.x = *(double*)&(Buffer[Buffer_Location+72]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.y = *(double*)&(Buffer[Buffer_Location+80]);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.z = *(double*)&(Buffer[Buffer_Location+88]);


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Jump_Not_Pressed = true;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->On_The_Ground = false;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Start_Frame = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_Frame = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->End_Frame = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_Model = 0;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_State = 0;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector.x = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector.y = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector.z = 0;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector.x = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector.y = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector.z = 0;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Up_Vector.x = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Up_Vector.y = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Up_Vector.z = 0;



    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ray_Sensor_Vector.x = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ray_Sensor_Vector.y = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ray_Sensor_Vector.z = 0;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Yaw_Angle = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Pitch_Angle = 0;

    if(Engine->On_Foot_State->NPC_Character_Objects[Index]->Player_Type == PLAYER_TYPE_CAR || Engine->On_Foot_State->NPC_Character_Objects[Index]->Player_Type == PLAYER_TYPE_ON_FOOT)
    {
        Engine->On_Foot_State->NPC_Character_Objects[Index]->Throttle = 0;
    }


    else if(Engine->On_Foot_State->NPC_Character_Objects[Index]->Player_Type == PLAYER_TYPE_SUBMARINE)
    {
        Engine->On_Foot_State->NPC_Character_Objects[Index]->Throttle = 3;
    }


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Floor_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Wall_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ceiling_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Object_Collision = false;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->On_Top_Of_An_AABB_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Underneath_An_AABB_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Colliding_With_An_AABB_Collision = false;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->On_Top_Of_Mesh_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Underneath_Mesh_Collision = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Colliding_With_Mesh_Collision = false;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Able_To_Move_Away = true;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Able_To_Move_Off_Platform = true;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.z = 0;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.z = 20.5;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.x = -4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.y = -4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.z = 20.5;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.x = 4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.y = 4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.z = 40.5;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Do_Not_Alter_Velocity = false;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y = 0;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = 0;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead = false;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Playing_Dead_Animation = false;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_Health = 100;
}

void Process_NPC_Character(struct _Engine* Engine, int Index)
{
    if(!Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead)
    {
    if(Check_If_Ray_Sensor_Detects_AABB(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->camX, Engine->On_Foot_State->camY, Engine->On_Foot_State->camZ,Engine->On_Foot_State->camX+(Engine->On_Foot_State->frontX*100),Engine->On_Foot_State->camY+(Engine->On_Foot_State->frontY*100),Engine->On_Foot_State->camZ+(Engine->On_Foot_State->frontZ*100)))
            {
                glPushMatrix();
                Render_Image_As_Billboard(Engine->On_Foot_State->Torpedo_Billboard,Engine->On_Foot_State->NPC_Character_Objects[Index]->x,Engine->On_Foot_State->NPC_Character_Objects[Index]->y,Engine->On_Foot_State->NPC_Character_Objects[Index]->z+50,1,20);
                glPopMatrix();
            }
    }


    if(Engine->On_Foot_State->NPC_Character_Objects[Index]->z <= -100)
    {
        //Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead = true;
    }

    if(!Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead)
    {
    if(Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_Health <= 0)
    {
        Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead = true;
        //printf("YOU DIED!\n");
    }
    }


    float pitch_rad = Engine->On_Foot_State->NPC_Character_Objects[Index]->Yaw_Angle * M_PI / 180.0f;
    float yaw_rad = Engine->On_Foot_State->NPC_Character_Objects[Index]->Pitch_Angle * M_PI / 180.0f;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector.x = cos(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector.y = sin(pitch_rad);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector.z = sin(yaw_rad) * cos(pitch_rad);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector = vec3_normalize(Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector);
    Vec3 global_up = {0.0f, 0.0f, 1.0f};
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector = vec3_cross(Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector, global_up);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector = vec3_normalize(Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Up_Vector = vec3_cross(Engine->On_Foot_State->NPC_Character_Objects[Index]->Right_Vector, Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Up_Vector = vec3_normalize(Engine->On_Foot_State->NPC_Character_Objects[Index]->Up_Vector);


    Vec3 displacement = vec3_scale(Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector, Engine->On_Foot_State->NPC_Character_Objects[Index]->Throttle * 0.16);

    Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x = displacement.x;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y = displacement.y;
    //Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = displacement.z;




    Vec3 ray_sensor = vec3_scale(Engine->On_Foot_State->NPC_Character_Objects[Index]->Forward_Vector, 10 * 0.16);

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ray_Sensor_Vector.x = (displacement.x*16);
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ray_Sensor_Vector.y = (displacement.y*16);

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Ray_Sensor_Vector.z = Engine->On_Foot_State->NPC_Character_Objects[Index]->z+25;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.x = Engine->On_Foot_State->NPC_Character_Objects[Index]->x-4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.y = Engine->On_Foot_State->NPC_Character_Objects[Index]->y-4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.min.z = Engine->On_Foot_State->NPC_Character_Objects[Index]->z-2;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.x = Engine->On_Foot_State->NPC_Character_Objects[Index]->x+4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.y = Engine->On_Foot_State->NPC_Character_Objects[Index]->y+4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry.max.z = Engine->On_Foot_State->NPC_Character_Objects[Index]->z+20.5;


    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.x = Engine->On_Foot_State->NPC_Character_Objects[Index]->x+-4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.y = Engine->On_Foot_State->NPC_Character_Objects[Index]->y+-4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.min.z = Engine->On_Foot_State->NPC_Character_Objects[Index]->z+20.5;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.x = Engine->On_Foot_State->NPC_Character_Objects[Index]->x+4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.y = Engine->On_Foot_State->NPC_Character_Objects[Index]->y+4;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry.max.z = Engine->On_Foot_State->NPC_Character_Objects[Index]->z+40.5;




    bool Mesh_Already_Detected = false;


    //FOOT COLLISION DETECTION
    Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z -= 0.05;




    if(!Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead)
    {
    for(int j = 0; j < Engine->On_Foot_State->Number_Of_Torpedoes; j++)
    {
        if(checkAABBCollision(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Torpedoes[j]->Collision_Geometry,0,0,0))
        {
            if(!Engine->On_Foot_State->Torpedoes[j]->Exploded)
            {
            Detonate_Torpedo(Engine, j);
            Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_Health -= 50;
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = 2;
            }
        }

        if(checkAABBCollision(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Torpedoes[j]->Collision_Geometry,0,0,0))
        {
            if(!Engine->On_Foot_State->Torpedoes[j]->Exploded)
            {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_Health -= 50;
            Detonate_Torpedo(Engine, j);
            }
        }
    }
    }








    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = 0;
            ////printf("Colliding with floor!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            ////printf("No Collision with floor!\n");
        }


        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = 0;
            ////printf("Colliding with Ceiling!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            ////printf("No Collision with Ceiling!\n");
        }
    }


    //FOOT COLLISION DETECTION
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x = 0;
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            ////printf("No Collision with wall!\n");
        }


        if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x = 0;
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y = 0;
            ////printf("Colliding with Wall!\n");
        }

        else if(!Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z))
        {
            ////printf("No Collision with Wall!\n");
        }
    }



     for(int Ship_Index = 0; Ship_Index < Engine->On_Foot_State->Region.Number_Of_Ships; Ship_Index++)
    {
        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Ceiling_Collision_Meshes; j++)
        {
        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = 0;
            ////printf("Colliding with floor!\n");
        }
        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with floor!\n");
        }
        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z = 0;
            ////printf("Colliding with Ceiling!\n");
        }
        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with Ceiling!\n");
        }
        }


        for(int j = 0; j < Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Number_Of_Wall_Collision_Meshes; j++)
        {
            if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x = 0;
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y = 0;
            ////printf("Colliding with wall!\n");
        }

        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Feet_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with wall!\n");
        }


        if(Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x = 0;
            Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y = 0;
            ////printf("Colliding with Wall!\n");
        }

        else if(!Check_If_AABB_Detects_Off_Center_Mesh(Engine->On_Foot_State->NPC_Character_Objects[Index]->Head_Collision_Geometry,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Wall_Collision_Meshes[j],Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y,Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z, Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_X_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Y_Position,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position))
        {
            ////printf("No Collision with Wall!\n");
        }
        }
    }




    if(Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z > 0)
    {
        Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_State = JUMPING;
       // ////printf("Jumping \n");
    }

    if(Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z < 0)
    {
        Engine->On_Foot_State->NPC_Character_Objects[Index]->Current_State = FALLING;
      //  ////printf("Falling \n");
    }



    if(!Engine->On_Foot_State->NPC_Character_Objects[Index]->Dead)
    {
    Engine->On_Foot_State->NPC_Character_Objects[Index]->z += Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->x += Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->y += Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y;

    Engine->On_Foot_State->NPC_Character_Objects[Index]->OldDisX = Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.x;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->OldDisY = Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.y;
    Engine->On_Foot_State->NPC_Character_Objects[Index]->OldDisZ = Engine->On_Foot_State->NPC_Character_Objects[Index]->displacement.z;

    //printf("x %f y %f z %f\n",Engine->On_Foot_State->NPC_Character_Objects[Index]->x,Engine->On_Foot_State->NPC_Character_Objects[Index]->y,Engine->On_Foot_State->NPC_Character_Objects[Index]->z);

    //Engine->On_Foot_State->NPC_Character_Objects[Index]->x += 0.1;
    }
};




#define EPSILON 1e-8


bool New_Check_If_Ray_Sensor_Detects_Mesh(float Ray_x1, float Ray_y1, float Ray_z1, float Ray_x2, float Ray_y2, float Ray_z2, struct _Preloaded_Collision_Mesh* Mesh)
{
    Vec3 Start_Vector;
    Vec3 End_Vector;

    Start_Vector.x = Ray_x1;
    Start_Vector.y = Ray_y1;
    Start_Vector.z = Ray_z1;

    End_Vector.x = Ray_x2;
    End_Vector.y = Ray_y2;
    End_Vector.z = Ray_z2;

    for(int j = 0; j < Mesh->Number_Of_Triangles; j++)
    {
        Vec3 vec1;
        Vec3 vec2;
        Vec3 vec3;

        vec1.x = Mesh->Triangles[j].v[0].x;
        vec1.y = Mesh->Triangles[j].v[0].y;
        vec1.z = Mesh->Triangles[j].v[0].z;

        vec2.x = Mesh->Triangles[j].v[1].x;
        vec2.y = Mesh->Triangles[j].v[1].y;
        vec2.z = Mesh->Triangles[j].v[1].z;

        vec3.x = Mesh->Triangles[j].v[2].x;
        vec3.y = Mesh->Triangles[j].v[2].y;
        vec3.z = Mesh->Triangles[j].v[2].z;

        if(lineIntersectsTriangle(Start_Vector,End_Vector,vec1,vec2,vec3))
        {
            return true;
        }
    }
    return false;
}


// Returns 1 if intersects, 0 otherwise
bool lineIntersectsTriangle(Vec3 Start_Vector, Vec3 End_Vector, Vec3 v0, Vec3 v1, Vec3 v2)
{
    Vec3 dir = vec3_sub(End_Vector, Start_Vector);
    Vec3 edge1 = vec3_sub(v1, v0);
    Vec3 edge2 = vec3_sub(v2, v0);

    Vec3 h = vec3_cross(dir, edge2);
    double a = vec3_dot(edge1, h);

    if (fabs(a) < EPSILON)
        return 0; // Parallel

    double f = 1.0 / a;
    Vec3 s = vec3_sub(Start_Vector, v0);
    double u = f * vec3_dot(s, h);

    if (u < 0.0 || u > 1.0)
        return 0;

    Vec3 q = vec3_cross(s, edge1);
    double v = f * vec3_dot(dir, q);

    if (v < 0.0 || u + v > 1.0)
        return 0;

    double t = f * vec3_dot(edge2, q);

    // Check if intersection is within the segment
    if (t >= 0.0 && t <= 1.0)
    {
        return true;
    }
    return 0;
}


/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "on_foot.h"
#include <stdio.h>
#include <string.h>
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

    printf("I will place the ship at z=%f\n", Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position);

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

    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Skyboxes = *(int*)&(Buffer[Buffer_Location+48]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Ships = *(int*)&(Buffer[Buffer_Location+52]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Wall_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+56]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Ceiling_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+60]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Goalposts = *(int*)&(Buffer[Buffer_Location+64]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Characters = *(int*)&(Buffer[Buffer_Location+68]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Cars = *(int*)&(Buffer[Buffer_Location+72]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Submarines = *(int*)&(Buffer[Buffer_Location+76]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Fires = *(int*)&(Buffer[Buffer_Location+80]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Terrains = *(int*)&(Buffer[Buffer_Location+84]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Docking_Ports = *(int*)&(Buffer[Buffer_Location+88]);
    Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Region.Offset_To_Switches = *(int*)&(Buffer[Buffer_Location+92]);

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
        //printf(Model_Filename);
        //printf(Texture_Filename);

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
        //printf(Model_Filename);
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
        //printf(Model_Filename);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j] = Create_Preloaded_Collision_Mesh(Model_Filename);
        Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Ceiling_Collision_Meshes[j]->z+=Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Safe_Z_Position;
    }

    printf("Loading Docking Ports\n");
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

        printf("Docking port Dest: %f %f %f\n", Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_X,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Y,Engine->On_Foot_State->Ship_Internals_Pointer[Ship_Index]->Docking_Ports[j]->Destination_Z);
    }
}

void Initialize_On_Foot_State(struct _Engine* Engine)
{
    Write_Ship("resources/data/ships/test_ship_details.det");

    Engine->On_Foot_State->Safe_X_Position = 0;
    Engine->On_Foot_State->Safe_Y_Position = 0;
    Engine->On_Foot_State->Safe_Z_Position = SHIP_INTERNALS_Z_MARGIN;

    Engine->On_Foot_State->FPS_Mode = true;

        Engine->On_Foot_State->width = 1920;
    Engine->On_Foot_State->height = 1080-32-32;

    // Camera configuration
    Engine->On_Foot_State->yaw = 0;   // Vertical angle (degrees)
    Engine->On_Foot_State->pitch = 0.0f;   // Vertical angle (degrees)
    Engine->On_Foot_State->sensitivity = 0.05f; // Mouse sensitivity

    // Camera Vectors
    Engine->On_Foot_State->camX = 0.0f;
    Engine->On_Foot_State->camY = 0.0f;
    Engine->On_Foot_State->camZ = 50.0f; // Camera position
    Engine->On_Foot_State->frontX = 0.0f;
    Engine->On_Foot_State->frontY = -1.0f;
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
    glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH); // Enable smooth shading (Gouraud shading)
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,1.0f, METER_CONVERSION * 75000);
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
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


    // --- LIGHT 1: GENERAL LIGHT (Dim & everywhere) ---
    GLfloat gen_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; // Base light so shadows aren't pitch black
    GLfloat gen_diffuse[] = { 1.0,1.0,1.0,1.0 }; // Dim diffuse light
    GLfloat gen_specular[] = { 1.0,1.0,1.0,1.0 }; // Very little shine

    glLightfv(GL_LIGHT1, GL_AMBIENT, gen_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, gen_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, gen_specular);

    // Position the general light
    // IMPORTANT: The 0.0 at the end makes this a DIRECTIONAL light (like the sun)
    // coming from the top-right-front, rather than a positional point light.
    GLfloat gen_position[] = { 1.0, -1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, gen_position);


    Engine->On_Foot_State->Camera_Y = 180;
    Engine->On_Foot_State->Camera_Z = 40;

    Engine->On_Foot_State->Reticle = Create_Image();
    Load_Image(Engine->On_Foot_State->Reticle, "resources/artwork/gui/submarine_hud_iff_reticle.bmp");

    Engine->On_Foot_State->Resume = Create_Image();
    Engine->On_Foot_State->Exit = Create_Image();
    Engine->On_Foot_State->Inventory = Create_Image();

    Load_Image(Engine->On_Foot_State->Resume, "resources/artwork/gui/english/resume.bmp");
    Load_Image(Engine->On_Foot_State->Exit, "resources/artwork/gui/english/exit_to_level_select.bmp");
    Load_Image(Engine->On_Foot_State->Inventory, "resources/artwork/gui/english/exit_to_title.bmp");





    Engine->On_Foot_State->Resume_Highlighted = Create_Image();
    Engine->On_Foot_State->Exit_Highlighted = Create_Image();
    Engine->On_Foot_State->Inventory_Highlighted = Create_Image();

    Load_Image(Engine->On_Foot_State->Resume_Highlighted, "resources/artwork/gui/english/resume_highlighted.bmp");
    Load_Image(Engine->On_Foot_State->Exit_Highlighted, "resources/artwork/gui/english/exit_to_level_select_highlighted.bmp");
    Load_Image(Engine->On_Foot_State->Inventory_Highlighted, "resources/artwork/gui/english/exit_to_title_highlighted.bmp");

    Engine->On_Foot_State->Current_Choice = 0;

    Engine->On_Foot_State->Up_Not_Pressed = false;
    Engine->On_Foot_State->Down_Not_Pressed = false;

    Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = false;

    Engine->On_Foot_State->Selector_Y = 52;

    Engine->On_Foot_State->On_Foot_Player = Create_Player();
    Initialize_On_Foot_Player(Engine, "none", 0,0,0);
    Engine->On_Foot_State->On_Foot_Player->Player_Type = PLAYER_TYPE_ON_FOOT;
    Engine->On_Foot_State->On_Foot_Player->Throttle = 0;
}

void Render_On_Foot_State(struct _Engine* Engine)
{
    double x = Get_Ticks(Engine->On_Foot_State->Death_Timer);
    {
        if(x > 5 && Engine->On_Foot_State->On_Foot_Player->Dead)
        {
            Reload_Region(Engine);
        }
    }

    if(!Engine->On_Foot_State->On_Foot_Player->Dead)
    {

    Engine->On_Foot_State->r += 0.01;

    if(Engine->On_Foot_State->On_Foot_Player->z < 0)
    {
        Engine->On_Foot_State->Current_State = STATE_UNDER_WATER;
    }

    else if(Engine->On_Foot_State->On_Foot_Player->z >= 0)
    {
        Engine->On_Foot_State->Current_State = STATE_ABOVE_WATER;
    }


   // //printf("Player's Coords: %f %f %f\n", Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
  //  //printf("Player's Corrected Coords: %f %f %f\n", New_Coords.x, New_Coords.y, New_Coords.z);

    if(!Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
    {
    if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
    glRotatef(Engine->On_Foot_State->On_Foot_Player->Yaw_Angle,0,0,1);
    GLfloat light_pos[] = { Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+500, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);



    Render_Player(Engine);
    glPopMatrix();

    if(Engine->On_Foot_State->Current_State == STATE_ABOVE_WATER)
    {
        if(Engine->On_Foot_State->Region.Number_Of_Skyboxes > 0)
        {
            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->Skybox_Objects[0]->x,Engine->On_Foot_State->Skybox_Objects[0]->y,Engine->On_Foot_State->Skybox_Objects[0]->z);
            Render_SE3_Model(Engine->On_Foot_State->Skybox_Objects[0]->Ship_Model,0,0,0,0,0,0,0,false,1,1,1);
            glPopMatrix();
        }

        if(Engine->On_Foot_State->Region.Number_Of_Skyboxes > 1)
        {
            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->Skybox_Objects[1]->x,Engine->On_Foot_State->Skybox_Objects[1]->y,Engine->On_Foot_State->Skybox_Objects[1]->z);
            Render_SE3_Model(Engine->On_Foot_State->Skybox_Objects[1]->Ship_Model,0,0,0,0,0,0,0,false,1,1,1);
            glPopMatrix();
        }
    }

    if(Engine->On_Foot_State->Current_State == STATE_UNDER_WATER)
    {
        if(Engine->On_Foot_State->Region.Number_Of_Skyboxes > 2)
        {
            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->Skybox_Objects[2]->x,Engine->On_Foot_State->Skybox_Objects[2]->y,Engine->On_Foot_State->Skybox_Objects[2]->z);
            Render_SE3_Model(Engine->On_Foot_State->Skybox_Objects[2]->Ship_Model,0,0,0,0,0,0,0,false,1,1,1);
            glPopMatrix();
        }
    }

    if(Engine->On_Foot_State->Region.Number_Of_Ships > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ships; j++)
        {
            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->Ship_Objects[j]->Shell_x,Engine->On_Foot_State->Ship_Objects[j]->Shell_y,Engine->On_Foot_State->Ship_Objects[j]->Shell_z);
            //glRotatef(Engine->On_Foot_State->r,0,0,1);
            Render_SE3_Model(Engine->On_Foot_State->Ship_Objects[j]->Ship_Shell_Model,0,0,0,0,0,0,0,false,1,1,1);
            glPopMatrix();
            for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_Terrains; m++)
            {
                glPushMatrix();
                glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->z);
                Render_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Terrain_Objects[m]->Model,0,0,0,0,0,0,0,false,1,1,1);
                glPopMatrix();

            }




            for(int m = 0; m < Engine->On_Foot_State->Ship_Internals_Pointer[j]->Region.Number_Of_Switches; m++)
            {
                glPushMatrix();
                glTranslatef(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->x,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->y,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->z);
                Render_SE3_Model(Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->Model,Engine->On_Foot_State->Ship_Internals_Pointer[j]->Switch_Objects[m]->Frame,0,0,0,0,0,0,false,1,1,1);
                glPopMatrix();
            }

        }
    }

    if(Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Wall_Collision_Meshes; j++)
    {
    glPushMatrix();
    glTranslatef(0,0,0);
//    Render_SE3_Model(Engine->On_Foot_State->Wall_Collision_Meshes[j],0,0,0,0,0,0,0,false,1,1,1);
    glPopMatrix();
    }
    }

    if(Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Ceiling_Collision_Meshes; j++)
    {
    glPushMatrix();
    glTranslatef(0,0,0);
   // Render_SE3_Model(Engine->On_Foot_State->Ceiling_Collision_Meshes[j],0,0,0,0,0,0,0,false,1,1,1);
    glPopMatrix();
    }
    }


    if(Engine->On_Foot_State->Region.Number_Of_Goalposts > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Goalposts; j++)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Goalposts[j]->x,Engine->On_Foot_State->Goalposts[j]->y,Engine->On_Foot_State->Goalposts[j]->z);
    Render_SE3_Model(Engine->On_Foot_State->Goalposts[j]->Model,0,0,0,0,0,0,0,false,1,1,1);
    glPopMatrix();
    }
    }


if(Engine->On_Foot_State->Region.Number_Of_Cars > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Cars; j++)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Cars[j]->x,Engine->On_Foot_State->Cars[j]->y,Engine->On_Foot_State->Cars[j]->z);
    Render_SE3_Model(Engine->On_Foot_State->Cars[j]->Models[0],0,0,0,0,0,0,0,false,1,1,1);
    Process_Player_Car(Engine, j);
    glPopMatrix();
    }
    }


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
   // Render_Submarine_Object(Engine, j);
    glPopMatrix();
    }
    }


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




     if(Engine->On_Foot_State->Region.Number_Of_Terrains > 0)
        {
    for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Terrains; j++)
    {
    glPushMatrix();
           glTranslatef(Engine->On_Foot_State->Terrain_Objects[j]->x,Engine->On_Foot_State->Terrain_Objects[j]->y,Engine->On_Foot_State->Terrain_Objects[j]->z);
    Render_SE3_Model(Engine->On_Foot_State->Terrain_Objects[j]->Model,0,0,0,0,0,0,0,false,1,1,1);
            glPopMatrix();
    }

    }


    if(Engine->On_Foot_State->Number_Of_Torpedoes > 0)
    {
    for(int j = 0; j < Engine->On_Foot_State->Number_Of_Torpedoes; j++)
    {
        glPushMatrix();
        //glTranslatef(0,0,0);
        if(!Engine->On_Foot_State->Torpedoes[j]->Exploded)
        {
        Process_Torpedo(Engine,j);
        Render_Image_As_Billboard(Engine->On_Foot_State->Resume,Engine->On_Foot_State->Torpedoes[j]->x,Engine->On_Foot_State->Torpedoes[j]->y,Engine->On_Foot_State->Torpedoes[j]->z,1,0.1);
        }
        glPopMatrix();
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
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(!Engine->On_Foot_State->On_Foot_Player->Dead)
    {
    if(!Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
    {
    if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
    {
        if(!Engine->On_Foot_State->Player_Is_In_Car && !Engine->On_Foot_State->Player_Is_In_Submarine)
        {
        Handle_Player_Inputs(Engine, Keypad);
        Check_Car_Input(Engine, Keypad);
        Check_Submarine_Input(Engine, Keypad);
        if(Engine->On_Foot_State->FPS_Mode)
        {
            mouseMotion(Engine, Keypad.Mouse_X, Keypad.Mouse_Y, Keypad);
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
    //printf("Current Choice: %d\n", Engine->On_Foot_State->Current_Choice);
    if(Keypad.Down && Engine->On_Foot_State->Current_Choice < 2 && Engine->On_Foot_State->Down_Not_Pressed)
    {
         Engine->On_Foot_State->Selector_Y += 96;
         Engine->On_Foot_State->Current_Choice += 1;
         Engine->On_Foot_State->Down_Not_Pressed = false;
    }

    if(Keypad.Up && Engine->On_Foot_State->Current_Choice > 0 && Engine->On_Foot_State->Up_Not_Pressed)
    {
         Engine->On_Foot_State->Selector_Y -= 96;
         Engine->On_Foot_State->Current_Choice -= 1;
         Engine->On_Foot_State->Up_Not_Pressed = false;
    }

    if(Keypad.Start)
    {

        if(Engine->On_Foot_State->Current_Choice == 2)
        {
            Resume_Timer(Engine->On_Foot_State->Go_Back_Timer);
            Engine->On_Foot_State->Go_Back_Timer_Was_Triggered = true;
        }
    }

    if(!Keypad.Up)
    {
        Engine->On_Foot_State->Up_Not_Pressed = true;
    }

    if(!Keypad.Down)
    {
        Engine->On_Foot_State->Down_Not_Pressed = true;
    }
    }




    if(Keypad.Start && Engine->On_Foot_State->Enter_Not_Pressed)
    {
         Engine->On_Foot_State->Enter_Not_Pressed = false;
         if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
         {
            Engine->On_Foot_State->Mini_State = MINI_STATE_PAUSE;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_LIGHTING);
            glColor3f(1,1,1);
        }

        else if(Engine->On_Foot_State->Mini_State == MINI_STATE_PAUSE)
         {
            Engine->On_Foot_State->Mini_State = MINI_STATE_UNPAUSED;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(35, 1.7777,1.0f, METER_CONVERSION * 75000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(40,40,40,0,0,20,0,0,1);
            glEnable(GL_LIGHTING);
        }
    }
    if(!Keypad.Start)
    {
        Engine->On_Foot_State->Enter_Not_Pressed = true;
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
            glDisable(GL_LIGHTING);
            glColor3f(1,1,1);
             glDisable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glDisable(GL_NORMALIZE);
           // glTranslatef(0,0,0);
        Reinitialize_Overworld_State(Engine);
        Engine->Current_State = STATE_OVERWORLD;
    }

    if(!Engine->On_Foot_State->Go_Back_Timer_Was_Triggered)
    {
    if(Engine->On_Foot_State->Mini_State == MINI_STATE_UNPAUSED)
    {
        if(!Engine->On_Foot_State->Player_Is_In_Car && !Engine->On_Foot_State->Player_Is_In_Submarine)
        {
    if(Engine->On_Foot_State->FPS_Mode)
            {
            Engine->On_Foot_State->camX = Engine->On_Foot_State->On_Foot_Player->x;
            Engine->On_Foot_State->camY = Engine->On_Foot_State->On_Foot_Player->y;
            Engine->On_Foot_State->camZ = Engine->On_Foot_State->On_Foot_Player->z+40;

            glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
            gluLookAt(Engine->On_Foot_State->camX, Engine->On_Foot_State->camY, Engine->On_Foot_State->camZ,
              Engine->On_Foot_State->camX + Engine->On_Foot_State->frontX, Engine->On_Foot_State->camY + Engine->On_Foot_State->frontY, Engine->On_Foot_State->camZ + Engine->On_Foot_State->frontZ,
              0,0,1);

            Engine->On_Foot_State->On_Foot_Player->Look_Vector_Origin_X = Engine->On_Foot_State->camX;
            Engine->On_Foot_State->On_Foot_Player->Look_Vector_Origin_Y = Engine->On_Foot_State->camY;
            Engine->On_Foot_State->On_Foot_Player->Look_Vector_Origin_Z = Engine->On_Foot_State->camZ;

            Engine->On_Foot_State->On_Foot_Player->Look_Vector_Destination_X = (Engine->On_Foot_State->camX + Engine->On_Foot_State->frontX);
            Engine->On_Foot_State->On_Foot_Player->Look_Vector_Destination_Y = (Engine->On_Foot_State->camY + Engine->On_Foot_State->frontY);
            Engine->On_Foot_State->On_Foot_Player->Look_Vector_Destination_Z = (Engine->On_Foot_State->camZ + Engine->On_Foot_State->frontZ);


            }

            else if(!Engine->On_Foot_State->FPS_Mode)
            {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(90*cos((180+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->On_Foot_Player->x, Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+20, 0,0,1);
    }
        }

        else if(Engine->On_Foot_State->Player_Is_In_Car && !Engine->On_Foot_State->Player_Is_In_Submarine)
        {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(90*cos((180+Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->y,Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->x, Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->y,Engine->On_Foot_State->Cars[Engine->On_Foot_State->In_This_Car]->z+20, 0,0,1);
        }

        else if(!Engine->On_Foot_State->Player_Is_In_Car && Engine->On_Foot_State->Player_Is_In_Submarine)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
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
    sprintf(Ship.Shell_Texture_Filepath, "resources/data/ships/sonichu.bmp");
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

    int Size_Of_Skyboxes = Ship.Internal_Layout.Number_Of_Skyboxes * sizeof(struct _Skybox);
    int Size_Of_Ships = Ship.Internal_Layout.Number_Of_Ships * sizeof(struct _Ship);
    int Size_Of_Wall_Meshes = Ship.Internal_Layout.Number_Of_Wall_Collision_Meshes * 256;
    int Size_Of_Ceiling_Meshes = Ship.Internal_Layout.Number_Of_Ceiling_Collision_Meshes * 256;
    int Size_Of_Goalposts = Ship.Internal_Layout.Number_Of_Goalposts * sizeof(struct _Goalpost);
    int Size_Of_Characters = Ship.Internal_Layout.Number_Of_Characters * 256;
    int Size_Of_Cars = Ship.Internal_Layout.Number_Of_Cars * sizeof(struct _Car);
    int Size_Of_Submarines = Ship.Internal_Layout.Number_Of_Submarines * sizeof(struct _Submarine);
    int Size_Of_Fires = Ship.Internal_Layout.Number_Of_Fires * sizeof(struct _Fire);
    int Size_Of_Terrains = Ship.Internal_Layout.Number_Of_Terrains * sizeof(struct _Terrain);
    int Size_Of_Docking_Ports = Ship.Internal_Layout.Number_Of_Docking_Ports * sizeof(struct _Docking_Port);
    int Size_Of_Switches = Ship.Internal_Layout.Number_Of_Switches * sizeof(struct _Switch);

    Ship.Internal_Layout.Offset_To_Skyboxes = sizeof(struct _Ship_Details);
    Ship.Internal_Layout.Offset_To_Ships = Ship.Internal_Layout.Offset_To_Skyboxes + Size_Of_Skyboxes;
    Ship.Internal_Layout.Offset_To_Wall_Collision_Meshes = Ship.Internal_Layout.Offset_To_Ships + Size_Of_Ships;
    Ship.Internal_Layout.Offset_To_Ceiling_Collision_Meshes = Ship.Internal_Layout.Offset_To_Wall_Collision_Meshes + Size_Of_Wall_Meshes;
    Ship.Internal_Layout.Offset_To_Goalposts = Ship.Internal_Layout.Offset_To_Ceiling_Collision_Meshes + Size_Of_Ceiling_Meshes;
    Ship.Internal_Layout.Offset_To_Characters = Ship.Internal_Layout.Offset_To_Goalposts + Size_Of_Goalposts;
    Ship.Internal_Layout.Offset_To_Cars = Ship.Internal_Layout.Offset_To_Characters + Size_Of_Characters;
    Ship.Internal_Layout.Offset_To_Submarines=Ship.Internal_Layout.Offset_To_Cars + Size_Of_Cars;
    Ship.Internal_Layout.Offset_To_Fires=Ship.Internal_Layout.Offset_To_Submarines+Size_Of_Submarines;
    Ship.Internal_Layout.Offset_To_Terrains=Ship.Internal_Layout.Offset_To_Fires+Size_Of_Fires;
    Ship.Internal_Layout.Offset_To_Docking_Ports=Ship.Internal_Layout.Offset_To_Terrains+Size_Of_Terrains;
    Ship.Internal_Layout.Offset_To_Switches=Ship.Internal_Layout.Offset_To_Docking_Ports+Size_Of_Docking_Ports;

    struct _Switch Switch;

    memset(Switch.Model_Filepath,0,256);
    memset(Switch.Texture_Filepath,0,256);
    sprintf(Switch.Model_Filepath, "resources/models/switch.se3");
    sprintf(Switch.Texture_Filepath, "resources/models/switch.bmp");
    Switch.Model_Filepath[27] = '\0';
    Switch.Texture_Filepath[27] = '\0';

    Switch.x = 0;
    Switch.y = -30;
    Switch.z = 30;

    Switch.x2 = 0+10;
    Switch.y2 = -30+10;
    Switch.z2 = 30+10;
    Switch.State = 0;

    struct _Terrain Terrain;
    memset(Terrain.Name,0,256);
    memset(Terrain.Texture_Filepath,0,256);
    sprintf(Terrain.Name, "resources/data/ships/test_ship_internal.se3");
    sprintf(Terrain.Texture_Filepath, "resources/data/ships/test_ship_internal.bmp");
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

    Region.Number_Of_Skyboxes = 3;
    Region.Number_Of_Ships = 1;
    Region.Number_Of_Wall_Collision_Meshes = 1;
    Region.Number_Of_Ceiling_Collision_Meshes = 1;
    Region.Number_Of_Goalposts = 2;
    Region.Number_Of_Characters = 0;
    Region.Number_Of_Cars = 0;
    Region.Number_Of_Submarines = 2;
    Region.Number_Of_Fires = 0;
    Region.Number_Of_Terrains = 1;
    Region.Number_Of_Docking_Ports = 0;

    int Size_Of_Skyboxes = Region.Number_Of_Skyboxes * sizeof(struct _Skybox);
    int Size_Of_Ships = Region.Number_Of_Ships * sizeof(struct _Ship);
    int Size_Of_Wall_Meshes = Region.Number_Of_Wall_Collision_Meshes * 256;
    int Size_Of_Ceiling_Meshes = Region.Number_Of_Ceiling_Collision_Meshes * 256;
    int Size_Of_Goalposts = Region.Number_Of_Goalposts * sizeof(struct _Goalpost);
    int Size_Of_Characters = Region.Number_Of_Characters * 256;
    int Size_Of_Cars = Region.Number_Of_Cars * sizeof(struct _Car);
    int Size_Of_Submarines = Region.Number_Of_Submarines * sizeof(struct _Submarine);
    int Size_Of_Fires = Region.Number_Of_Fires * sizeof(struct _Fire);
    int Size_Of_Terrains = Region.Number_Of_Terrains * sizeof(struct _Terrain);
    int Size_Of_Docking_Ports = Region.Number_Of_Docking_Ports * sizeof(struct _Docking_Port);

    Region.Offset_To_Skyboxes = sizeof(struct _Region);
    Region.Offset_To_Ships = Region.Offset_To_Skyboxes + Size_Of_Skyboxes;
    Region.Offset_To_Wall_Collision_Meshes = Region.Offset_To_Ships + Size_Of_Ships;
    Region.Offset_To_Ceiling_Collision_Meshes = Region.Offset_To_Wall_Collision_Meshes + Size_Of_Wall_Meshes;
    Region.Offset_To_Goalposts = Region.Offset_To_Ceiling_Collision_Meshes + Size_Of_Ceiling_Meshes;
    Region.Offset_To_Characters = Region.Offset_To_Goalposts + Size_Of_Goalposts;
    Region.Offset_To_Cars = Region.Offset_To_Characters + Size_Of_Characters;
    Region.Offset_To_Submarines=Region.Offset_To_Cars + Size_Of_Cars;
    Region.Offset_To_Fires=Region.Offset_To_Submarines+Size_Of_Submarines;
    Region.Offset_To_Terrains=Region.Offset_To_Fires+Size_Of_Fires;
    Region.Offset_To_Docking_Ports=Region.Offset_To_Terrains+Size_Of_Terrains;

    struct _Terrain Terrain;
    memset(Terrain.Name,0,256);
    memset(Terrain.Texture_Filepath,0,256);
    sprintf(Terrain.Name, "resources/data/arcs/the_days_of_nova/regions/test_region/terrain.se3");
    sprintf(Terrain.Texture_Filepath, "resources/data/arcs/the_days_of_nova/regions/test_region/terrain.bmp");
    Terrain.Name[68] ='\0';
    Terrain.Texture_Filepath[68] = '\0';
    Terrain.x = 0;
    Terrain.y = 0;
    Terrain.z = 0;

    struct _Skybox Skybox;
    struct _Skybox Seabox;
    struct _Skybox UWbox;
    struct _Ship Ship;

    memset(Ship.Ship_Filepath,0,256);
    sprintf(Ship.Ship_Filepath, "resources/data/ships/test_ship_details.det");
    Ship.Ship_Filepath[42] ='\0';

    Ship.Shell_x = 0;
    Ship.Shell_y = 0;
    Ship.Shell_z = 50;

    Ship.Shell_Roll = 0;
    Ship.Shell_Yaw = 0;
    Ship.Shell_Pitch = 0;



    memset(Skybox.Name,0,256);
    memset(Skybox.Texture_Filepath,0,256);
    sprintf(Skybox.Name, "resources/models/model_skybox.se3");
    sprintf(Skybox.Texture_Filepath, "resources/models/texture_skybox.bmp");
    Skybox.Name[33] ='\0';
    Skybox.Texture_Filepath[35] = '\0';
    Skybox.x = 0;
    Skybox.y = 0;
    Skybox.z = 0;
    Skybox.Rotational_Velocity = 0;

    memset(Seabox.Name,0,256);
    memset(Seabox.Texture_Filepath,0,256);
    sprintf(Seabox.Name, "resources/models/model_ocean.se3");
    sprintf(Seabox.Texture_Filepath, "resources/models/texture_ocean.bmp");
    Seabox.Name[32] ='\0';
    Seabox.Texture_Filepath[34] = '\0';
    Seabox.x = 0;
    Seabox.y = 0;
    Seabox.z = 0;
    Seabox.Rotational_Velocity = 0;

    memset(UWbox.Name,0,256);
    memset(UWbox.Texture_Filepath,0,256);
    sprintf(UWbox.Name, "resources/models/model_skybox_ocean.se3");
    sprintf(UWbox.Texture_Filepath, "resources/models/texture_skybox_ocean.bmp");
    UWbox.Name[39] ='\0';
    UWbox.Texture_Filepath[41] = '\0';
    UWbox.x = 0;
    UWbox.y = 0;
    UWbox.z = 0;
    UWbox.Rotational_Velocity = 0;

    char Wall_Collision_Mesh[256];
    char Ceiling_Collision_Mesh[256];

    memset(Wall_Collision_Mesh,0,256);
    memset(Ceiling_Collision_Mesh,0,256);

    sprintf(Wall_Collision_Mesh, "resources/data/arcs/the_days_of_nova/regions/test_region/wall_cg.se3");
    sprintf(Ceiling_Collision_Mesh, "resources/data/arcs/the_days_of_nova/regions/test_region/ceil_cg.se3");

    Wall_Collision_Mesh[68] = '\0';
    Ceiling_Collision_Mesh[68] = '\0';

    struct _Goalpost Goalpost;
    memset(Goalpost.Name,0,256);
    memset(Goalpost.Texture_Filepath,0,256);

    sprintf(Goalpost.Name, "resources/models/model_goalpost.se3");
    sprintf(Goalpost.Texture_Filepath, "resources/models/texture_goalpost.bmp");

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
    sprintf(Easy_Goalpost.Texture_Filepath, "resources/models/texture_goalpost.bmp");

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
    sprintf(Car.Texture_Filepath, "resources/data/cars/car.bmp");
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
    sprintf(Submarine.Texture_Filepath, "resources/data/submarines/car.bmp");
    Submarine.Name[33] = '\0';
    Submarine.Texture_Filepath[33] = '\0';
    Submarine.x = 100;
    Submarine.y = 100;
    Submarine.z = 50;
    Submarine.Yaw = 270;
    Submarine.Pitch = 0;
    Submarine.Roll = 0;


    struct _Submarine SubmarineT;
    memset(SubmarineT.Name,0,256);
    memset(SubmarineT.Texture_Filepath,0,256);

    sprintf(SubmarineT.Name, "resources/data/submarines/car.se3");
    sprintf(SubmarineT.Texture_Filepath, "resources/data/submarines/car.bmp");
    SubmarineT.Name[33] = '\0';
    SubmarineT.Texture_Filepath[33] = '\0';
    SubmarineT.x = 0;
    SubmarineT.y = 100;
    SubmarineT.z = 50;
    SubmarineT.Yaw = 270;
    SubmarineT.Pitch = 0;
    SubmarineT.Roll = 0;


    struct _Fire Fire;
    Fire.Fire_Damage_Radius = 30;
    Fire.Fire_Size = 0.75;
    Fire.x = 0;
    Fire.y = 0;
    Fire.z = 5;


    FILE* f = fopen(Filename, "wb");
    fwrite(&Region, 1, sizeof(struct _Region),f);
    rewind(f);

    fseek(f, Region.Offset_To_Skyboxes, SEEK_SET);
    fwrite(&Skybox, 1, sizeof(struct _Skybox),f);
    fwrite(&Seabox, 1, sizeof(struct _Skybox),f);
    fwrite(&UWbox, 1, sizeof(struct _Skybox),f);
    rewind(f);

    fseek(f, Region.Offset_To_Ships, SEEK_SET);
    fwrite(&Ship, 1, sizeof(struct _Ship),f);
    rewind(f);

    fseek(f, Region.Offset_To_Wall_Collision_Meshes, SEEK_SET);
    fwrite(&Wall_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Region.Offset_To_Ceiling_Collision_Meshes, SEEK_SET);
    fwrite(&Ceiling_Collision_Mesh, 1, 256,f);
    rewind(f);

    fseek(f, Region.Offset_To_Goalposts, SEEK_SET);
    fwrite(&Goalpost, 1, sizeof(struct _Goalpost),f);
    fwrite(&Easy_Goalpost, 1, sizeof(struct _Goalpost),f);
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

    fseek(f, Region.Offset_To_Terrains, SEEK_SET);
    fwrite(&Terrain, 1, sizeof(struct _Terrain),f);
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

    Engine->On_Foot_State->Region.Offset_To_Skyboxes = *(int*)&(Buffer[Buffer_Location+48]);
    Engine->On_Foot_State->Region.Offset_To_Ships = *(int*)&(Buffer[Buffer_Location+52]);
    Engine->On_Foot_State->Region.Offset_To_Wall_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+56]);
    Engine->On_Foot_State->Region.Offset_To_Ceiling_Collision_Meshes = *(int*)&(Buffer[Buffer_Location+60]);
    Engine->On_Foot_State->Region.Offset_To_Goalposts = *(int*)&(Buffer[Buffer_Location+64]);
    Engine->On_Foot_State->Region.Offset_To_Characters = *(int*)&(Buffer[Buffer_Location+68]);
    Engine->On_Foot_State->Region.Offset_To_Cars = *(int*)&(Buffer[Buffer_Location+72]);
    Engine->On_Foot_State->Region.Offset_To_Submarines = *(int*)&(Buffer[Buffer_Location+76]);
    Engine->On_Foot_State->Region.Offset_To_Fires = *(int*)&(Buffer[Buffer_Location+80]);
    Engine->On_Foot_State->Region.Offset_To_Terrains = *(int*)&(Buffer[Buffer_Location+84]);
    Engine->On_Foot_State->Region.Offset_To_Docking_Ports = *(int*)&(Buffer[Buffer_Location+88]);
    Engine->On_Foot_State->Region.Offset_To_Switches = *(int*)&(Buffer[Buffer_Location+92]);

    //printf("Number Of Skyboxes: %d\n", Engine->On_Foot_State->Region.Number_Of_Skyboxes);
    //printf("Offset To Skyboxes: %d\n", Engine->On_Foot_State->Region.Offset_To_Skyboxes);

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
        Engine->On_Foot_State->NPC_Characters[m] = (struct _Player_Submarine*)calloc(1,sizeof(struct _Player_Submarine));
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
        //printf(Model_Filename);
        //printf(Texture_Filename);

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
        //printf(Model_Filename);
        //printf(Texture_Filename);

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

        //printf(Model_Filename);
        Buffer_Location += 256;
        Engine->On_Foot_State->Ship_Objects[j]->Shell_x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_z = *(double*)&(Buffer[Buffer_Location+16]);

        Engine->On_Foot_State->Ship_Objects[j]->Shell_Roll = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_Yaw = *(double*)&(Buffer[Buffer_Location+32]);
        Engine->On_Foot_State->Ship_Objects[j]->Shell_Pitch = *(double*)&(Buffer[Buffer_Location+40]);

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
        //printf(Model_Filename);

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
        //printf(Model_Filename);

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
        //printf(Model_Filename);
        //printf(Texture_Filename);

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
        //printf(Model_Filename);
        //printf(Texture_Filename);

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
        //printf(Model_Filename);
        //printf(Texture_Filename);

        Engine->On_Foot_State->Submarines[j]->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Engine->On_Foot_State->Submarines[j]->Models[0], Model_Filename);
        Load_SE3_Texture(Engine->On_Foot_State->Submarines[j]->Models[0], Texture_Filename);
        Buffer_Location += 512;

        Engine->On_Foot_State->Submarines[j]->plane_x = *(double*)&(Buffer[Buffer_Location]);
        Engine->On_Foot_State->Submarines[j]->plane_y = *(double*)&(Buffer[Buffer_Location+8]);
        Engine->On_Foot_State->Submarines[j]->plane_z = *(double*)&(Buffer[Buffer_Location+16]);
        Engine->On_Foot_State->Submarines[j]->plane_yaw = *(double*)&(Buffer[Buffer_Location+24]);
        Engine->On_Foot_State->Submarines[j]->plane_pitch = *(double*)&(Buffer[Buffer_Location+32]);


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
        Load_Image(Engine->On_Foot_State->Fires[j]->Fire_Image, "resources/artwork/placeholders/fire.bmp");
    }
}


void Write_Character_Object(struct _Engine* Engine, const char* Filename)
{
    struct _Character Character;

    sprintf(Character.Character_Filepath, "None");

    Character.Number_Of_Neural_Networks = 0;
    sprintf(Character.Character_Neural_Network_List_Filepath, "None");

    Character.Number_Of_Conversations = 0;
    sprintf(Character.Character_Conversation_List_Filepath, "None");

    Character.Number_Of_Navigation_Meshes = 0;
    sprintf(Character.Character_Navigation_Mesh_Filepath, "None");

    Character.Player_Type = 0;
    Character.dThrottle = 0;
    Character.Z_velocity = 0;

    Character.x = 0;
    Character.y = 0;
    Character.z = 0;

    Character.dx = 0;
    Character.dy = 0;
    Character.dz = 0;

    Character.Start_Frame = 0;
    Character.Current_Frame = 0;
    Character.End_Frame = 0;
    Character.Current_Model = 0;

    Character.Current_State = 0;

    Character.Yaw_Angle = 0;
    Character.Pitch_Angle = 0;

    Character.Throttle = 0;
    Character.Minimum_Feet_Collision_Geometry[0] = 0;
    Character.Minimum_Feet_Collision_Geometry[1] = 0;
    Character.Minimum_Feet_Collision_Geometry[2] = 0;
    Character.Maximum_Feet_Collision_Geometry[0] = 0;
    Character.Maximum_Feet_Collision_Geometry[1] = 0;
    Character.Maximum_Feet_Collision_Geometry[2] = 0;

    Character.Minimum_Head_Collision_Geometry[0] = 0;
    Character.Minimum_Head_Collision_Geometry[1] = 0;
    Character.Minimum_Head_Collision_Geometry[2] = 0;
    Character.Maximum_Head_Collision_Geometry[0] = 0;
    Character.Maximum_Head_Collision_Geometry[1] = 0;
    Character.Maximum_Head_Collision_Geometry[2] = 0;

    Character.Current_Attack = 10;
    Character.Current_Defense = 10;
    Character.Current_Speed = 10;
    Character.Current_Health =10;

    Character.Dead = false;
    Character.Playing_Dead_Animation = false;

    FILE* f = fopen(Filename, "wb");
    fwrite(&Character, 1, sizeof(struct _Character), f);
    fclose(f);
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
            //printf("Distance from car: %f\n", x);
            if(x <= 20 && Keypad.B)
            {
                //printf("Entering Car\n\n\n\n");
                Engine->On_Foot_State->Player_Is_In_Car = true;
                Engine->On_Foot_State->In_This_Car = j;
            }
        }

}



void Check_Submarine_Input(struct _Engine* Engine, struct _Keypad Keypad)
{
        for(int j = 0; j < Engine->On_Foot_State->Region.Number_Of_Submarines; j++)
        {
            double x = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,Engine->On_Foot_State->Submarines[j]->plane_x,Engine->On_Foot_State->Submarines[j]->plane_y,Engine->On_Foot_State->Submarines[j]->plane_z);
            printf("Distance from Submarine: %f\n", x);
            if(x <= 60 && Keypad.B)
            {
                //printf("Entering Submarine\n\n\n\n");
                Engine->On_Foot_State->Player_Is_In_Submarine = true;
                Engine->On_Foot_State->In_This_Submarine = j;
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
    int centerX = Keypad.Width / 2;
    int centerY = Keypad.Height / 2;

    float xoffset = (x - centerX) * Engine->On_Foot_State->sensitivity;
    float yoffset = (centerY - y) * Engine->On_Foot_State->sensitivity;

    Engine->On_Foot_State->yaw -= xoffset;
    Engine->On_Foot_State->pitch += yoffset;

    Engine->On_Foot_State->On_Foot_Player->Yaw_Angle = Engine->On_Foot_State->yaw;

    // Constrain Pitch
    if (Engine->On_Foot_State->pitch > 89.0f) Engine->On_Foot_State->pitch = 89.0f;
    if (Engine->On_Foot_State->pitch < -89.0f) Engine->On_Foot_State->pitch = -89.0f;

    updateCameraVectors(Engine);

    if (x != centerX || y != centerY) {
        glfwSetCursorPos(Engine->W, centerX, centerY);
    }
}

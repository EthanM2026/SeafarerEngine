/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "submarine_scene.h"
#include "math.h"
struct _Submarine_State* Create_Submarine_State()
{
    struct _Submarine_State* I = (struct _Submarine_State*) malloc(sizeof (struct _Submarine_State));
    return I;
}
void Destroy_Submarine_State(struct _Engine* Engine)
{

}

void Initialize_Submarine_State(struct _Engine* Engine, const char* Level_Filename)
{
    Engine->Submarine_State->Torpedo_Registry = Create_Torpedo_Registry();
Engine->Submarine_State->Torpedo_Registry->Current_Torpedo = -1;
Engine->Submarine_State->Torpedo_Registry->Number_Of_Torpedoes = 0;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,0);


    Engine->Submarine_State->Submarine = Create_MD2_Model();
    Load_MD2_Model(Engine->Submarine_State->Submarine, "resources/models/sonichu.dat");
    Load_MD2_Texture(Engine->Submarine_State->Submarine, "resources/models/sonichu.bmp");


    Engine->Submarine_State->Skybox_Manager = (struct _Skybox_Manager*)malloc(sizeof(struct _Skybox_Manager));
    Engine->Submarine_State->Skybox_Manager->Skybox = Create_MD2_Model();
    Load_MD2_Model(Engine->Submarine_State->Skybox_Manager->Skybox, "resources/models/skybox.dat");
    Load_MD2_Texture(Engine->Submarine_State->Skybox_Manager->Skybox, "resources/models/skybox.bmp");

   // Engine->Submarine_State->Submarine_State_HUD_Manager = (struct _Submarine_State_HUD_Manager*)malloc(sizeof(struct _Submarine_State_HUD_Manager));
   // Engine->Submarine_State->Submarine_State_HUD_Manager->HUD = Create_Image();
   // Load_Image(Engine->Submarine_State->Submarine_State_HUD_Manager->HUD, "dd");



    //Engine->Submarine_State->Submarine_State_In_Flight_Dialogue_Manager = (struct _Submarine_State_In_Flight_Dialogue_Manager*)malloc(sizeof(struct _Submarine_State_In_Flight_Dialogue_Manager));
    //Engine->Submarine_State->Submarine_State_In_Flight_Dialogue_Manager->Dialogue = Create_Image();
    //Load_Image(Engine->Submarine_State->Submarine_State_In_Flight_Dialogue_Manager->Dialogue, "dd");


Engine->Submarine_State->Submarine_Image = Create_Image();
    Load_Image(Engine->Submarine_State->Submarine_Image, "resources/artwork/gui/chris.bmp");

    Engine->Submarine_State->Dialogue = Create_Image();
    Load_Image(Engine->Submarine_State->Dialogue, "resources/artwork/gui/dbox.bmp");


    Engine->Submarine_State->Sonar = Create_Image();
    Load_Image(Engine->Submarine_State->Sonar, "resources/artwork/gui/sonar.bmp");


    Engine->Submarine_State->Camera = (struct _New_Camera*)malloc(sizeof(struct _New_Camera));

    Engine->Submarine_State->Camera->x = 0.00001;
    Engine->Submarine_State->Camera->y = 0.00001;
    Engine->Submarine_State->Camera->z = 0.00001;

Engine->Submarine_State->Camera->Subject_X = 0.00001;
    Engine->Submarine_State->Camera->Subject_Y = 0.00001;
    Engine->Submarine_State->Camera->Subject_Z = 0.00001;

    Engine->Submarine_State->Player_Submarine = Create_Player();
 // Initialize_Player_Submarine(Engine);

//printf("Test");


glDisable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	GLfloat lightpos[] = {1,1,1,1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightpos);


GLfloat light_ambient[] = { 1,1,1, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);


//	FILE* f;
  //  f = fopen(Level_Filename, "rb");
 //   int File_Size;
 //   fseek(f, 0, SEEK_END);
 //   File_Size = ftell(f);
    //printf("File Size: %d\n", File_Size);
 //   rewind(f);
 //   unsigned char Buffer[File_Size];
 //   fread(Buffer, 1, File_Size, f);
 //   fclose(f);
 //   int Buffer_Location = 0;

  //  Engine->Submarine_State->Level_File_Header = (struct _Level_File_Header*)malloc(sizeof(struct _Level_File_Header));

  //  for(int j = 0; j < 256; j++)
  //  {
 //       Engine->Submarine_State->Level_File_Header->Name[j] = Buffer[j];
 //   }

   // Engine->Submarine_State->Level_File_Header->Arc = *(int*)&(Buffer[256]);
  //  Engine->Submarine_State->Level_File_Header->Level_ID = *(int*)&(Buffer[260]);
   // Engine->Submarine_State->Level_File_Header->Objective= *(int*)&(Buffer[264]);

//    Engine->Submarine_State->Level_File_Header->Number_Of_Capital_Submarines = *(int*)&(Buffer[265]);
//    Engine->Submarine_State->Level_File_Header->Offset_To_Capital_Submarines = *(int*)&(Buffer[269]);

//    Engine->Submarine_State->Capital_Submarines = (struct _Capital_Submarine*) malloc(Engine->Submarine_State->Level_File_Header->Number_Of_Capital_Submarines * sizeof(struct _Capital_Submarine));

//    printf("Ships: %d", Engine->Submarine_State->Level_File_Header->Number_Of_Capital_Submarines);
   // printf("Level's name: ");
   // for(int j = 0; j < 256; j++)
    {
   //     printf("%c", Engine->Submarine_State->Level_File_Header->Name[j]);
    }

//    for(int j = 0; j < Engine->Submarine_State->Level_File_Header->Number_Of_Capital_Submarines; j++)
    {
        //Engine->Submarine_State->Capital_Submarines[j].Model = Create_MD2_Model();
//        Load_MD2_Model(Engine->Submarine_State->Capital_Submarines[j].Model, "resources/models/clydeship.dat");
//        Load_MD2_Texture(Engine->Submarine_State->Capital_Submarines[j].Model, "resources/models/sonichu.bmp");
    }



}

void Render_Submarine_State(struct _Engine* Engine)
{
    //You need GlTranslate before you Render!

    glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glTranslatef(0,0,0);
    Render_MD2_Model(Engine->Submarine_State->Skybox_Manager->Skybox, 0,0,0,0,0,0,0,true,1,1,1);
    Render_Player(Engine);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glTranslatef(0,0,0);

    //Render_MD2_Model(Engine->Submarine_State->Submarine, 0,0,0,0,0,0,0,true,1,1,1);
    glPopMatrix();

    for(int j = 0; j < Engine->Submarine_State->Torpedo_Registry->Number_Of_Torpedoes; j++)
    {
    glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glTranslatef(0,0,0);
if(Engine->Submarine_State->Torpedo_Registry->Number_Of_Torpedoes >0)
    {
    Render_Torpedo(Engine, 0);
    }
    glPopMatrix();
   }

   //2D Section
   glTranslatef(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Render_Image(Engine->Submarine_State->Dialogue,128,256,1);
    Render_Image(Engine->Submarine_State->Submarine_Image,48,256,1);
    Render_Image(Engine->Submarine_State->Sonar,480,212,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,1, 12500000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,0);


}
void Input_Submarine_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    Handle_Player_Inputs(Engine, Keypad);

    if(Keypad.A)
    {
    Engine->Current_LVL_Step = 2;
    for(int j = 0; j < Engine->Number_Of_Operations; j++)
    {
        if(Engine->LVL_Operations[j].Step == Engine->Current_LVL_Step && Engine->LVL_Operations[j].Track == Engine->Current_LVL_Track)
        {
            if(!Engine->LVL_Operations[j].End_Level)
            {
                if(!Engine->LVL_Operations[j].Cutscene_Or_Level)
                {
                printf("Loading Cutscene\n");
                Engine->Cutscene_State = Create_Cutscene_State();
                Initialize_Cutscene_State(Engine);
                Load_Cutscene(Engine, Engine->Cutscene_Filenames[0].Filepath);
                Engine->Current_State = STATE_CUTSCENE;
                }

                else if(Engine->LVL_Operations[j].Cutscene_Or_Level)
                {
                //IF STEP 0 is a LEVEL
                printf("Loading Level\n");
                Engine->Submarine_State = Create_Submarine_State();
                Initialize_Submarine_State(Engine, "None");
                Engine->Current_State = STATE_PLATFORMING;
                }
            }
            else if(Engine->LVL_Operations[j].End_Level)
            {
                printf("Level Ended\n");
                glTranslatef(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        //        Engine->Overworld_State= Create_Overworld_State();
       // Initialize_Overworld_State(Engine, Arc_Image_Filepath);
        Engine->Current_State = STATE_OVERWORLD;
            }
        }
    }
    }

}
void Process_Submarine_State(struct _Engine* Engine)
{
    if(Engine->Submarine_State->Torpedo_Registry->Number_Of_Torpedoes >0)
    {
    Process_Torpedo(Engine);
    }
     Process_Player(Engine);

     float Look_Z = 750*cos(Engine->Submarine_State->Player_Submarine->Yaw_Angle);

     glMatrixMode(GL_MODELVIEW);
     //glPopMatrix();
     glLoadIdentity();
    //gluLookAt(Engine->Submarine_State->Player_Submarine->Forward_Vector->i-1000,Engine->Submarine_State->Player_Submarine->Forward_Vector->j-1000,Engine->Submarine_State->Player_Submarine->Forward_Vector->k-1000,Engine->Submarine_State->Player_Submarine->x, Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z, Engine->Submarine_State->Player_Submarine->Up_Vector->i,Engine->Submarine_State->Player_Submarine->Up_Vector->j,Engine->Submarine_State->Player_Submarine->Up_Vector->k);

    //800 and -800 for looking up and down
//if(Engine->Submarine_State->Player_Submarine->Look_Direction == LOOK_FORWARD)
{
    //gluLookAt(0,0,0,Engine->Submarine_State->Player_Submarine->x, Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+80, 0,0,1);

if(Engine->Submarine_State->Player_Submarine->FP_Mode == true)
{
    gluLookAt(-750*sin(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->x+800,-750*cos(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+80,Engine->Submarine_State->Player_Submarine->x, Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z, 0,0,1);
}

else if(Engine->Submarine_State->Player_Submarine->FP_Mode == false)
{
       gluLookAt(-750*sin(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->x,-750*cos(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+80,Engine->Submarine_State->Player_Submarine->x, Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+80, 0,0,1);
    //glPushMatrix();
}
}


//else if(Engine->Submarine_State->Player_Submarine->Look_Direction == LOOK_DOWN)
{
   //    gluLookAt(-750*sin(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->x,-750*cos(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+Engine->Submarine_State->Player_Submarine->Camera_Z_Rotation,Engine->Submarine_State->Player_Submarine->x, Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+80, 0,0,1);
   // glPushMatrix();
}

//else if(Engine->Submarine_State->Player_Submarine->Look_Direction == LOOK_UP)
{
     //gluLookAt(-750*sin(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->x,-750*cos(Engine->Submarine_State->Player_Submarine->Yaw_Angle*0.01745329251) + Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+Engine->Submarine_State->Player_Submarine->Camera_Z_Rotation,Engine->Submarine_State->Player_Submarine->x, Engine->Submarine_State->Player_Submarine->y,Engine->Submarine_State->Player_Submarine->z+80, 0,0,1);
   // glPushMatrix();
}


}

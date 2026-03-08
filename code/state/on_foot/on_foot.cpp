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

// --- Configuration Constants ---

void Initialize_On_Foot_State(struct _Engine* Engine)
{



    Engine->On_Foot_State->On_Foot_Computer_Object = (struct _On_Foot_Computer_Object*)calloc(1,sizeof(struct _On_Foot_Computer_Object));

    Engine->On_Foot_State->On_Foot_Computer_Object->Computer = Create_Computer();
    Initialize_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer);
    Turn_On_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer);

    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_One_x = 0;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_One_y = 0;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_One_z = 0;

    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Two_x = -1200;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Two_y = -1480;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Two_z = 750;

    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Three_x = -1200;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Three_y = -1480;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Three_z = 3000;

    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Four_x = -180;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Four_y = -1480;
    Engine->On_Foot_State->On_Foot_Computer_Object->Screen_Vertex_Four_z = 3000;

    Engine->On_Foot_State->Underwater_Explosion_Billboard = (struct _TwoD_Sprite_Object*)calloc(1,sizeof(struct _TwoD_Sprite_Object));
    Load_TwoD_Sprite_Animation(Engine->On_Foot_State->Underwater_Explosion_Billboard, "explosion.and", 0);

    Engine->On_Foot_State->Number_Of_Underwater_Explosions = 0;
    Engine->On_Foot_State->Current_Underwater_Explosion = -1;
    setupFBO(Engine);
    Engine->On_Foot_State->Current_Location = PLAYER_TYPE_ON_FOOT;
    const float FOG_DENSITY = 0.01f; // Controls how thick the fog is (0.01 is light, 0.1 is thick)
// Dark Blue Fog Color (R, G, B, A)
const float fogColor[4] = {0.0f, 0.0f, 0.15f, 1.0f};

    Engine->On_Foot_State->Universal_Circular_Shadow=Create_Circular_Shadow();
    Initialize_Circular_Shadow(Engine, 0,0,0,10);


    Engine->On_Foot_State->Death_Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Death_Timer);
   // Pause_Timer(Engine->On_Foot_State->Death_Timer);

Engine->On_Foot_State->Is_In_Submarine = false;
Engine->On_Foot_State->Number_Of_Torpedoes = 0;
Engine->On_Foot_State->Current_Torpedo = -1;
    Engine->On_Foot_State->On_Foot_Submarine_Player = Create_New_Player_Submarine();
    Initialize_New_Player_Submarine(Engine);

    Engine->On_Foot_State->On_Foot_Car_Player = Create_New_Player_Car();
    Initialize_New_Player_Car(Engine);

Engine->On_Foot_State->Camera_Y = 180;
 Engine->On_Foot_State->Camera_Z = 40;

    Engine->On_Foot_State->Rigid_Body_Test_Block = Create_MD2_Model();
    Load_MD2_Model(Engine->On_Foot_State->Rigid_Body_Test_Block, "resources/artwork/placeholders/no_model.dat");
    Load_MD2_Texture(Engine->On_Foot_State->Rigid_Body_Test_Block, "resources/artwork/placeholders/no_texture.bmp");

    glColor3f(1,1,1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glShadeModel(GL_FLAT); // Enable smooth shading (Gouraud shading)
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,1.0f, 100000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(40,40,40,0,0,20,0,0,1);
// Somewhere in the initialization part of your program…
    // Enable depth testing and lighting
    // --- Enable Depth Test and Smooth Shading ---


//     --- Enable Lighting ---
   // glEnable(GL_LIGHTING);  //General lighting switch
//     Enable all three light sources
   // glEnable(GL_LIGHT0);   // Green Directional Light
  //  glEnable(GL_LIGHT1);   // White Point Light
  //  glEnable(GL_LIGHT2);   // Blue Spotlight

  //   --- Define Material Properties (for the Teapot) ---
   //  The material properties dictate how the object reacts to light.
  //  GLfloat mat_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};   //Dark ambient reflection
 //   GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};   //Bright white diffuse reflection
  //  GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  //White specular highlight
  // GLfloat mat_shininess[] = {50.0f};                 // High shininess (smaller, brighter highlight)

   // glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   // glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   //  --- Define Light Source 0 Properties (GL_LIGHT0) - GREEN DIRECTIONAL ---
  //   Position/Direction: -X, +Y(Forward), +Z(Up)
  //  GLfloat light0_ambient[] = {0.10f, 0.8f, 0.8f, 1.0f};
    GLfloat light0_diffuse[] = {0.4f, 0.8f, 0.8f, 1.0f};
    //GLfloat light0_specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light0_position[] = {-1.0f, 1.0f, 1.0f, 0.1f};  //Directional (w=0.0)

  //  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   //  --- Define Light Source 1 Properties (GL_LIGHT1) - WHITE POINT LIGHT (Fill Light) ---
    // Position: +X, +Y(Forward), +Z(Up)
    //GLfloat light1_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    //GLfloat light1_diffuse[] = {0.4f, 0.4f, 0.4f, 1.0f};
    //GLfloat light1_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    //GLfloat light1_position[] = {2.0f, 2.0f, 2.0f, 1.0f};  //Point Light (w=1.0)

   // glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
   // glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
   // glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
   // glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  //  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.05f);

   //  --- Define Light Source 2 Properties (GL_LIGHT2) - BLUE SPOTLIGHT (Flashlight properties) ---
  //  GLfloat light2_ambient[] = {0.0f, 0.0f, 0.1f, 1.0f};
   // GLfloat light2_diffuse[] = {0.2f, 0.2f, 0.8f, 1.0f};
  //GLfloat light2_specular[] = {0.5f, 0.5f, 1.0f, 1.0f};

  //  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
   // glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
   // glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

   // glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);  //Tighter 30 degree cone
   // glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f);  //Tighter beam falloff




    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Engine->On_Foot_State->Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Timer);

    Engine->On_Foot_State->Red = 1;
    Engine->On_Foot_State->Green = 1;
    Engine->On_Foot_State->Blue = 1;

    Engine->On_Foot_State->Talk_Arrow = Create_Image();
    Load_Image(Engine->On_Foot_State->Talk_Arrow, "resources/artwork/gui/on_foot_talk.bmp");

    Engine->On_Foot_State->Open_Arrow = Create_Image();
    Load_Image(Engine->On_Foot_State->Open_Arrow, "resources/artwork/gui/on_foot_open.bmp");

    Engine->On_Foot_State->Conversation_Mini_State = (struct _Conversation_Mini_State*)calloc(1,sizeof(struct _Conversation_Mini_State));
    Engine->On_Foot_State->Conversation_Mini_State->In_Conversation = false;

    Engine->On_Foot_State->Paused_Background = Create_Image();
    Load_Image(Engine->On_Foot_State->Paused_Background, "resources/artwork/gui/on_foot_paused.bmp");


    Engine->On_Foot_State->Resume = Create_Image();
    Engine->On_Foot_State->Exit_To_Level_Select = Create_Image();
    Engine->On_Foot_State->Exit_To_Title = Create_Image();

    Load_Image(Engine->On_Foot_State->Resume, "resources/artwork/gui/resume.bmp");
    Load_Image(Engine->On_Foot_State->Exit_To_Level_Select, "resources/artwork/gui/exit_to_level_select.bmp");
    Load_Image(Engine->On_Foot_State->Exit_To_Title, "resources/artwork/gui/exit_to_title.bmp");

    Engine->On_Foot_State->Laps = 0;

    Engine->On_Foot_State->Broadcaster = Create_Broadcaster();
    initFBO(Engine->On_Foot_State->Broadcaster);
    Encode_Image(Engine->On_Foot_State->Broadcaster, 5512, 1);

// --- Fog Implementation ---
    //glEnable(GL_FOG);

    // 1. Set the Fog Color (The same color as the clear color for seamless transition)
    glFogfv(GL_FOG_COLOR, fogColor);

    // 2. Set the Fog Mode (How the fog density changes with distance)
    // GL_EXP2 is often used for a realistic, atmospheric effect
    glFogi(GL_FOG_MODE, GL_EXP2);

    // 3. Set the Fog Density
    glFogf(GL_FOG_DENSITY, FOG_DENSITY);

    // 4. (Optional) Make sure the rendering hint is set for quality
    glHint(GL_FOG_HINT, GL_NICEST);

    Engine->On_Foot_State->Explosion_Placeholder=Create_Image();
    Load_Image(Engine->On_Foot_State->Explosion_Placeholder, "resources/artwork/placeholders/explosion.bmp");
}

void Render_On_Foot_State(struct _Engine* Engine)
{
    // Unbind FBO - return to default window system framebuffer

    //glLightfv(GL_LIGHT0, GL_POSITION, Engine->On_Foot_State->light_direction);

    if(!Engine->On_Foot_State->Paused)
    {
        GLfloat light0_position[] = {Engine->On_Foot_State->On_Foot_Player->x, Engine->On_Foot_State->On_Foot_Player->y, Engine->On_Foot_State->On_Foot_Player->z+50, 0.1f};  //Directional (w=0.0)
        //glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

        //glMatrixMode(GL_MODELVIEW);
       // glLoadIdentity(); // Reset to camera/view

        glPushMatrix();
        glTranslatef(0,0,0);
        //Render_MD2_Model_GLDraw(Engine->On_Foot_State->Rigid_Body_Test_Block,0,0,0,0,0,0,0,false,1,1,1);
        glPopMatrix();
    //glPointSize(4);



    if(Engine->On_Foot_State->Number_Of_Underwater_Explosions > 0)
    {
    //printf("Rendering Backgrounds\n");
    for(int j =0; j < Engine->On_Foot_State->Number_Of_Underwater_Explosions; j++)
    {

        glPushMatrix();
        glTranslatef(0,0,0);
        if(Engine->On_Foot_State->On_Foot_Underwater_Explosions[j]->Visible)
        {
            Render_Image_As_Billboard(Engine->On_Foot_State->Explosion_Placeholder,Engine->On_Foot_State->On_Foot_Underwater_Explosions[j]->x,Engine->On_Foot_State->On_Foot_Underwater_Explosions[j]->y,Engine->On_Foot_State->On_Foot_Underwater_Explosions[j]->z,1,1);
        }
        glPopMatrix();
    }
    }

    //printf("Rendering Backgrounds\n");


        glPushMatrix();
        glTranslatef(0,0,0);
//            Process_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer,5);
            Render_Computer_Vertices(Engine->On_Foot_State->On_Foot_Computer_Object->Computer,0,0,75,0,100,75,0,100,175,0,0,175);
        glPopMatrix();





    if(Engine->Loaded_Level_Regions->Number_Of_Backgrounds > 0)
    {
    //printf("Rendering Backgrounds\n");
    for(int j =0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {

        glPushMatrix();
        glTranslatef(0,0,0);
        //if(Engine->On_Foot_State->On_Foot_Background_Object[j].Visible == 1)
        {
        Render_MD2_Model(Engine->On_Foot_State->On_Foot_Background_Object[j].Model,0,0,0,0,0,0,0,true,Engine->On_Foot_State->Red,Engine->On_Foot_State->Green,Engine->On_Foot_State->Blue);
        }
        //////Render_MD2_Model_GLDraw(Engine->On_Foot_State->Model,0,0,0,0,0,0,0,false,1,1,1);
        glPopMatrix();
    }
    }

    if(Engine->On_Foot_State->Number_Of_Torpedoes > 0)
    {
        for(int j = 0; j < Engine->On_Foot_State->Current_Torpedo; j++)
        {
            if(!Engine->On_Foot_State->On_Foot_Torpedoes[Engine->On_Foot_State->Current_Torpedo]->Exploded)
            {
                Process_Torpedo(Engine,j);
                glPushMatrix();
                glTranslatef(Engine->On_Foot_State->On_Foot_Torpedoes[j]->x,Engine->On_Foot_State->On_Foot_Torpedoes[j]->y,Engine->On_Foot_State->On_Foot_Torpedoes[j]->z);
                Render_Torpedo(Engine,j);
                glPopMatrix();
            }
        }
    }

    if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_ON_FOOT)
    {
    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);
    glRotatef(Engine->On_Foot_State->On_Foot_Player->Yaw_Angle,0,0,1);

    GLfloat light2_position[] = {0.0f, 1.5f, 0.0f, 1.0f}; // Position: slightly above, 1.5 units in front of the Teapot (Point Light)
    GLfloat spot_direction[] = {0.0f, 1.0f, 0.0f};       // Direction: Pointing straight forward (Negative Z in Teapot's local space)

    // Redefine light position and direction for the current frame
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

    Render_Player(Engine);
    //Recalculate_Circular_Shadow_XY_Coordinates(Engine, Engine->On_Foot_State->On_Foot_Player->x, Engine->On_Foot_State->On_Foot_Player->y, 10);
    //Recalculate_Circular_Shadow_Z_Coordinate(Engine,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,32);

   // printf("Player x y z %f %f %f\n", Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z);


    double z = Get_Ticks(Engine->On_Foot_State->Timer);
   // printf("Ticks: %f\n", z);
    int x = z * 100;
    if(x % 6 == 0)
    {
        float Distance = Get_Distance(Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z,0,0,75);
    if(Distance <= 1000*METER_CONVERSION)
    {
    Engine->On_Foot_State->Broadcaster->Signal_Received = true;

    if(Distance <= 3*METER_CONVERSION)
    {
    //Receive_Image(Engine->On_Foot_State->Broadcaster, 5512, 1, (Calculate_Inverse_Square_Law(10000000000000, Distance)), 0);
    }

    else if(Distance > 3*METER_CONVERSION)
    {
    //Receive_Image(Engine->On_Foot_State->Broadcaster, 5512, 1, (Calculate_Inverse_Square_Law(10000000000000, Distance)), Distance * METER_CONVERSION);
    }

    }

    else if(Distance > 25*METER_CONVERSION)
    {
        Engine->On_Foot_State->Broadcaster->Signal_Received = false;
    }
    }



    glPopMatrix();

    glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Universal_Circular_Shadow->x,Engine->On_Foot_State->Universal_Circular_Shadow->y,Engine->On_Foot_State->Universal_Circular_Shadow->z);
    Render_Shadow(Engine);
    glPopMatrix();

    }

    else if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_SUBMARINE)
    {

            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x,Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y,Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z);
            glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_yaw,0,0,1);
            glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_pitch,1,0,0);
            Render_Player_Submarine(Engine);
            glPopMatrix();
    }

    else if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_CAR)
    {
            glPushMatrix();
            glTranslatef(Engine->On_Foot_State->On_Foot_Car_Player->x,Engine->On_Foot_State->On_Foot_Car_Player->y,Engine->On_Foot_State->On_Foot_Car_Player->z);
            glRotatef(Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle,0,0,1);
            glRotatef(Engine->On_Foot_State->On_Foot_Car_Player->Pitch_Angle,1,0,0);
            Render_Player_Car(Engine);
            glPopMatrix();
    }

    //if(Engine->On_Foot_State->Conversation_Mini_State->In_Conversation)
    //{
    //    Render_Conversation(Engine);
    //}

        //Draw_Camera_Scene(Engine);

        // 3. BLIT THE CAMERA FEED
        // This copies the FBO pixels into a small rectangle on the screen
       // glBindFramebuffer(GL_READ_FRAMEBUFFER, Engine->On_Foot_State->fbo);
       // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
       // glBlitFramebuffer(0, 0, 256,256,
        //          0, 0, 256, 256, // Destination: X from 20 to 300, Y from 380 to 580
         //         GL_COLOR_BUFFER_BIT, GL_LINEAR);



    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if(Engine->On_Foot_State->Broadcaster->Signal_Received)
        {
        //drawSceneToTexture(Engine->On_Foot_State->Broadcaster, Engine);
       // Draw_Camera_Scene(Engine);


        }

        else if(!Engine->On_Foot_State->Broadcaster->Signal_Received)
        {
           //Render_Image(Engine->On_Foot_State->Broadcaster->No_Signal,0,0,1);
        }


   // glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
  //  glDepthFunc(GL_LESS);

   // glBindFramebuffer(GL_FRAMEBUFFER, 0);
   // glViewport(0, 0, 640,360);
   // glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
   // gluPerspective(35, 1.7777,1.0f, 100000000);
   // glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();



    // 1. RENDER TO TEXTURE (The "CCTV" perspective)
   // glBindFramebuffer(GL_FRAMEBUFFER, Engine->On_Foot_State->fbo);
   // glViewport(0, 0, 256,256);
   // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // glMatrixMode(GL_PROJECTION);
   // glLoadIdentity();
   // gluPerspective(35, 1.0f,1.0f, 100000000);
   // glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();

    gluLookAt(90*cos((180+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->On_Foot_Player->x, Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+20, 0,0,1);


    if(Engine->Loaded_Level_Regions->Number_Of_Backgrounds > 0)
    {
    //printf("Rendering Backgrounds\n");
    for(int j =0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
    {

        glPushMatrix();
        glTranslatef(0,0,0);
        //if(Engine->On_Foot_State->On_Foot_Background_Object[j].Visible == 1)
        {
        Render_MD2_Model(Engine->On_Foot_State->On_Foot_Background_Object[j].Model,0,0,0,0,0,0,0,true,Engine->On_Foot_State->Red,Engine->On_Foot_State->Green,Engine->On_Foot_State->Blue);
        }
        //////Render_MD2_Model_GLDraw(Engine->On_Foot_State->Model,0,0,0,0,0,0,0,false,1,1,1);
        glPopMatrix();
    }
    }



    // 3. BLIT TO TOP-LEFT
     //   glBindFramebuffer(GL_READ_FRAMEBUFFER, Engine->On_Foot_State->fbo);
     //   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      //  glBlitFramebuffer(0, 0, 256,256, // Source (the whole FBO)
        //                  0, 0, 256, 256,         // Destination (Top-left quad)
         //                 GL_COLOR_BUFFER_BIT, GL_NEAREST);



    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1920,1080);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,1.0f, 100000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


   }
    if(Engine->On_Foot_State->Paused == true)
    {
        Render_Image(Engine->On_Foot_State->Paused_Background,0,0,1);
        Render_Image(Engine->On_Foot_State->Resume,176,32,1);
        Render_Image(Engine->On_Foot_State->Exit_To_Level_Select,176,128,1);
        Render_Image(Engine->On_Foot_State->Exit_To_Title,176,224,1);
    }



}
void Input_On_Foot_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Keypad.Start && !Engine->On_Foot_State->Enter_Pressed)
    {
        if(!Engine->On_Foot_State->Paused)
        {
        Engine->On_Foot_State->Paused = true;
        Engine->On_Foot_State->Enter_Pressed = true;
        Engine->On_Foot_State->Red = 1;
        Engine->On_Foot_State->Green = 1;
        Engine->On_Foot_State->Blue = 1;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        }

        else if(Engine->On_Foot_State->Paused)
        {
        Engine->On_Foot_State->Paused = false;
        Engine->On_Foot_State->Enter_Pressed = true;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.7777,1.0f, 100000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40,40,40,0,0,20,0,0,1);
    Engine->On_Foot_State->Red = 1;
    Engine->On_Foot_State->Green = 1;
    Engine->On_Foot_State->Blue = 1;
        }
    }

    if(!Keypad.Start)
    {
        Engine->On_Foot_State->Enter_Pressed = false;
    }



    if(Engine->On_Foot_State->Conversation_Mini_State->In_Conversation)
    {
        Handle_Conversation_Inputs(Engine, Keypad);
    }
    else if(!Engine->On_Foot_State->Conversation_Mini_State->In_Conversation)
    {

    if(Engine->On_Foot_State->Current_Location==PLAYER_TYPE_ON_FOOT)
    {
    Handle_Player_Inputs(Engine, Keypad);
    }

    else if(Engine->On_Foot_State->Current_Location==PLAYER_TYPE_SUBMARINE)
    {
    Handle_Player_Submarine_Inputs(Engine, Keypad);
    }

    else if(Engine->On_Foot_State->Current_Location==PLAYER_TYPE_CAR)
    {
    Handle_Player_Car_Inputs(Engine, Keypad);
    }

    if(Keypad.Left)
    {
        Turn_Off_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer);
    }

    if(Keypad.Right)
    {
        Turn_On_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer);
    }
//    Process_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer,5);

    if(Keypad.A)
    {
        Add_Underwater_Explosion(Engine);
        Set_Underwater_Explosion_Parameters(Engine,0,true,true,10,10,10,10,0,100,100,100);

//        for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Actors; j++)
        {
           // if(checkAABBCollision(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry, Engine->On_Foot_State->On_Foot_Actor_Object[j].Collision_Geometry, 0,0,0))
            {
              //  if(Engine->On_Foot_State->On_Foot_Actor_Object[j].Number_Of_Conversations > 0)
                {
                    //if(Engine->On_Foot_State->On_Foot_Actor_Object[j].Current_Conversation < Engine->On_Foot_State->On_Foot_Actor_Object[j].Number_Of_Conversations)
                    {
                       // Engine->On_Foot_State->On_Foot_Actor_Object[j].Current_Conversation += 1;
                       // Engine->On_Foot_State->On_Foot_Actor_Object[j].Show_Talk_Arrow = 0;
                       // Load_Conversation_File(Engine, Engine->On_Foot_State->On_Foot_Actor_Object[j].Conversation_Filepath);
                      //  Start_Conversation(Engine);
                    }
                }
            }
        }


        //for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Stage_Changing_Doors; j++)
        {
            //if(checkAABBCollision(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry, Engine->On_Foot_State->On_Foot_Stage_Changing_Door_Object[j].Collision_Geometry, 0,0,0))
            {

            }
        }
    }

    if(Keypad.Start)
    {
       // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Stage_Changing_Doors; j++)
        {
           // if(checkAABBCollision(Engine->On_Foot_State->On_Foot_Player->Head_Collision_Geometry, Engine->On_Foot_State->On_Foot_Stage_Changing_Door_Object[j].Collision_Geometry, 0,0,0))
            {
                       // Engine->Interim_State->Current_Instruction = INSTRUCTION_GO_TO_REGION;
                       // Engine->Interim_State->Current_Instruction_Parameter = Engine->On_Foot_State->On_Foot_Stage_Changing_Door_Object[0].Go_To_This_Region;
                       // Engine->Current_State = STATE_INTERIM;
            }
        }
    }





    if(Keypad.Up)
    {
        for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
        {
            if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,0))
        {
            if(!Mix_Playing(1) && Engine->On_Foot_State->On_Foot_Player->Current_State == CURRENT_STATE_WALKING)
            {
            Mix_PlayChannel( 1, Engine->On_Foot_State->On_Foot_SFX_Data_Object[0].SFX_Data, 0 );
            }
        }
        }
    }

    if(!Keypad.Up)
    {
        for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++)
        {
            if(Check_If_AABB_Detects_Mesh(Engine->On_Foot_State->On_Foot_Player->Feet_Collision_Geometry,Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh,0,0,0))
        {
            if(Mix_Playing(1))
            {
            Mix_HaltChannel(1);
            }
        }
        }
    }
    if(Keypad.C)
    {
        Resume_Timer(Engine->On_Foot_State->Death_Timer);
        Engine->On_Foot_State->Camera_Z += 1;
    }
    if(Keypad.D)
    {
        Engine->On_Foot_State->Camera_Z -= 1;
    }


    if(Keypad.E)
    {
        Engine->On_Foot_State->Camera_Y += 1;
    }
    if(Keypad.F)
    {
        Engine->On_Foot_State->Camera_Y -= 1;
    }

    }

}


void Process_On_Foot_State(struct _Engine* Engine)
{
   // double Death_Current_Time = Get_Ticks(Engine->On_Foot_State->Death_Timer);
    //printf("Death Timer: %f\n", Death_Current_Time);
   // if(Death_Current_Time >= 5)
   // {
    //    printf("Death Clock!\n");
       //Assuming the Level file has been read
    //    Engine->Overworld_State->Enter_Pressed = false;
    //    free(Engine->Overworld_State->Timer);
    //    Engine->Overworld_State->Timer = Create_Timer();
    //    Set_Timer(Engine->Overworld_State->Timer);
    //    Pause_Timer(Engine->Overworld_State->Timer);
        //
     //   Engine->Overworld_State->Current_Choice = 100;
     //   Engine->Overworld_State->Selector_x = 48;
     //   Engine->Overworld_State->Selector_y = 180-64;
     //   glColor3f(1,1,1);
      //  Engine->Overworld_State->Times_Right_Pressed = 0;

     //   glMatrixMode(GL_PROJECTION);
     //   glLoadIdentity();
     //   glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
     //   glMatrixMode(GL_MODELVIEW);
     //   glLoadIdentity();

      //  Engine->Current_State = STATE_OVERWORLD;

    //}

    double Current_Time = Get_Ticks(Engine->On_Foot_State->Timer)/1000;



//    Process_Computer(Engine->On_Foot_State->On_Foot_Computer_Object->Computer,5);

    if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_ON_FOOT)
    {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(90*cos((180+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->On_Foot_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->On_Foot_Player->x, Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+20, 0,0,1);
    }

    if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_CAR)
    {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(90*cos((180+Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Car_Player->x,90*sin((Engine->On_Foot_State->Camera_Y+Engine->On_Foot_State->On_Foot_Car_Player->Yaw_Angle)*0.01745329251) + Engine->On_Foot_State->On_Foot_Car_Player->y,Engine->On_Foot_State->On_Foot_Car_Player->z+Engine->On_Foot_State->Camera_Z,Engine->On_Foot_State->On_Foot_Car_Player->x, Engine->On_Foot_State->On_Foot_Car_Player->y,Engine->On_Foot_State->On_Foot_Car_Player->z+20, 0,0,1);
    }

    else if(Engine->On_Foot_State->Current_Location == PLAYER_TYPE_SUBMARINE)
    {
        Process_Player_Submarine(Engine);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

    // --- 1. Calculate Camera Position and Orientation based on plane state ---

    // Push the current identity matrix onto the stack
        glPushMatrix();
        glLoadIdentity();

    // Rotation Order: Yaw (Z) -> Pitch (X) -> Roll (Y)
        glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_yaw, 0.0f, 0.0f, 1.0f);     // Z-axis (Yaw)
        glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_pitch, 1.0f, 0.0f, 0.0f);   // X-axis (Pitch)
        glRotatef(Engine->On_Foot_State->On_Foot_Submarine_Player->plane_roll, 0.0f, 1.0f, 0.0f);    // Y-axis (Roll)

    // Retrieve the resulting rotation matrix (m)
        GLfloat m[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        glPopMatrix(); // Restore the original ModelView matrix (Identity)

    // Extract Forward (Y) and Up (Z) vectors from the rotation matrix:
    // Forward Vector (Y-axis components are m[4], m[5], m[6])
        float fwdX = m[4], fwdY = m[5], fwdZ = m[6];
    // Local Up Vector (Z-axis components are m[8], m[9], m[10])
        float upX_local = m[8], upY_local = m[9], upZ_local = m[10];

    // Define camera offset in local space (e.g., 12 units back, 4 units up)
        float offset_distance = -160.0f;
        float height_offset = 4.0f;

    // Calculate World Camera Position (Eye)
    // Eye = PlanePos + (Forward * BackDistance) + (Up * Height)
        float eyeX = Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x + (fwdX * offset_distance) + (upX_local * height_offset);
        float eyeY = Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y + (fwdY * offset_distance) + (upY_local * height_offset);
        float eyeZ = Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z + (fwdZ * offset_distance) + (upZ_local * height_offset);

    // --- 2. Apply Camera (gluLookAt) ---
    // The camera's Up Vector now correctly follows the plane's Roll, making the horizon tilt.
    gluLookAt(
        eyeX, eyeY, eyeZ,                       // Eye Position: Calculated behind and above the plane
        Engine->On_Foot_State->On_Foot_Submarine_Player->plane_x, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_y, Engine->On_Foot_State->On_Foot_Submarine_Player->plane_z,              // Center Position: Look at the plane's position
        upX_local, upY_local, upZ_local         // Up Vector: Use the plane's local Z-axis (accounts for roll)
    );

    }
}

//THIS IS THE CURRENT REGION LOADER!!!!
void Load_CSS_On_Foot_Level_Header(struct _Engine* Engine, const char* Filename)
{
    //WHAT We're Loading
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

//    Engine->Loaded_Level_Header = (struct _On_Foot_Level_Header*)calloc(1,sizeof(struct _On_Foot_Level_Header));

    //INCOMPLETE

    //////printf(Engine->Level_Region_Filenames[Engine->Current_Level].Filepath);
    Load_CSS_On_Foot_File_Header(Engine,Engine->Level_Region_Filenames[Engine->Current_Level].Filepath);

}

//THIS IS THE CURRENT REGION LOADER!!!!
void Load_CSS_On_Foot_File_Header(struct _Engine* Engine, const char* Filename)
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

    Engine->Loaded_Level_Regions = (struct _On_Foot_File_Header*)calloc(1,sizeof(struct _On_Foot_File_Header));
    //INCOMPLETE
    int Current_Buffer_Location =0;

    //REMADE and COMPLETE
    Engine->Loaded_Level_Regions->Number_Of_Backgrounds = *(int*)&(Buffer[Current_Buffer_Location]); //268
    Engine->Loaded_Level_Regions->Offset_To_Backgrounds = *(int*)&(Buffer[Current_Buffer_Location+4]); //268
    printf("Backgrounds = %d\n", Engine->Loaded_Level_Regions->Number_Of_Backgrounds);
    Current_Buffer_Location += 4;

//    Engine->Loaded_Level_Regions->Number_Of_Actors = *(int*)&(Buffer[Current_Buffer_Location]); //272
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Tracks = *(int*)&(Buffer[Current_Buffer_Location]); //276
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_SFX_Data = *(int*)&(Buffer[Current_Buffer_Location]); //280
   // Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Number_Of_Spawnpoints = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Props = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

    //Engine->Loaded_Level_Regions->Number_Of_Endpoints = *(int*)&(Buffer[Current_Buffer_Location]); //Enter these to end the level!
    //Current_Buffer_Location += 4;

    //

    //Room Controls
   // Engine->Loaded_Level_Regions->Number_Of_Doors = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Conveyor_Belts = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Ladders = *(int*)&(Buffer[Current_Buffer_Location]);
    //Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Switches = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Dials = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Pickup_Items = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

    //Engine->Loaded_Level_Regions->Number_Of_Fixed_Camera_Boxes = *(int*)&(Buffer[Current_Buffer_Location]); //Switch from third-person view to a security camera view in certain rooms.
  //  Current_Buffer_Location += 4;

    //

    //Dialogue controls
   // Engine->Loaded_Level_Regions->Number_Of_Conversations = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Conversation_Nodes = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Conversation_Connections = *(int*)&(Buffer[Current_Buffer_Location]);
  // Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Shops = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Number_Of_Stage_Changing_Doors = *(int*)&(Buffer[Current_Buffer_Location]); //These do not end the level! Only endpoints do so!
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Number_Of_Vehicle_Entry_Points = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Elevators = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Number_Of_Lights = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;



  //  Engine->Loaded_Level_Regions->Offset_To_Backgrounds = *(int*)&(Buffer[Current_Buffer_Location]); //268
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Actors = *(int*)&(Buffer[Current_Buffer_Location]); //272
  //  Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Tracks = *(int*)&(Buffer[Current_Buffer_Location]); //276
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_SFX_Data = *(int*)&(Buffer[Current_Buffer_Location]); //280
  //  Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Offset_To_Spawnpoints = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Offset_To_Props = *(int*)&(Buffer[Current_Buffer_Location]);
   // Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Endpoints = *(int*)&(Buffer[Current_Buffer_Location]); //Enter these to end the level!
   // Current_Buffer_Location += 4;

    //

    //Room Controls
  //  Engine->Loaded_Level_Regions->Offset_To_Doors = *(int*)&(Buffer[Current_Buffer_Location]);
 //   Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Conveyor_Belts = *(int*)&(Buffer[Current_Buffer_Location]);
 //   Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Ladders = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Switches = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Dials = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Pickup_Items = *(int*)&(Buffer[Current_Buffer_Location]);
 //   Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Fixed_Camera_Boxes = *(int*)&(Buffer[Current_Buffer_Location]); //Switch from third-person view to a security camera view in certain rooms.
    //Current_Buffer_Location += 4;

    //

    //Dialogue controls
  //  Engine->Loaded_Level_Regions->Offset_To_Conversations = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Conversation_Nodes = *(int*)&(Buffer[Current_Buffer_Location]);
 //   Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Conversation_Connections = *(int*)&(Buffer[Current_Buffer_Location]);
 //   Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Shops = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

   // Engine->Loaded_Level_Regions->Offset_To_Stage_Changing_Doors = *(int*)&(Buffer[Current_Buffer_Location]); //These do not end the level! Only endpoints do so!
 //   Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Vehicle_Entry_Points = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Elevators = *(int*)&(Buffer[Current_Buffer_Location]);
 //   Current_Buffer_Location += 4;

 //   Engine->Loaded_Level_Regions->Offset_To_Lights = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;

  //  Engine->Loaded_Level_Regions->Offset_To_Computers = *(int*)&(Buffer[Current_Buffer_Location]);
  //  Current_Buffer_Location += 4;



    Engine->On_Foot_State->On_Foot_Background_Object = (struct _On_Foot_Background_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Backgrounds, sizeof(struct _On_Foot_Background_Object));

//    Engine->On_Foot_State->On_Foot_Actor_Object = (struct _On_Foot_Actor_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Actors, sizeof(struct _On_Foot_Actor_Object));

   // Engine->On_Foot_State->On_Foot_Track_Object = (struct _On_Foot_Track_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Tracks, sizeof(struct _On_Foot_Track_Object));

  //  Engine->On_Foot_State->On_Foot_SFX_Data_Object = (struct _On_Foot_SFX_Data_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_SFX_Data, sizeof(struct _On_Foot_SFX_Data_Object));

  //  Engine->On_Foot_State->On_Foot_Spawnpoint_Object = (struct _On_Foot_Spawnpoint_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Spawnpoints, sizeof(struct _On_Foot_Spawnpoint_Object));

  //  Engine->On_Foot_State->On_Foot_Prop_Object = (struct _On_Foot_Prop_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Props, sizeof(struct _On_Foot_Prop_Object));

   // Engine->On_Foot_State->On_Foot_Endpoint_Object = (struct _On_Foot_Endpoint_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Endpoints, sizeof(struct _On_Foot_Endpoint_Object));
    //


    //Room Controls
  //  Engine->On_Foot_State->On_Foot_Door_Object= (struct _On_Foot_Door_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Doors, sizeof(struct _On_Foot_Door_Object));

   // Engine->On_Foot_State->On_Foot_Conveyor_Belt_Object= (struct _On_Foot_Conveyor_Belt_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Conveyor_Belts, sizeof(struct _On_Foot_Conveyor_Belt_Object));

  //  Engine->On_Foot_State->On_Foot_Ladder_Object= (struct _On_Foot_Ladder_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Ladders, sizeof(struct _On_Foot_Ladder_Object));

   // Engine->On_Foot_State->On_Foot_Switch_Object= (struct _On_Foot_Switch_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Switches, sizeof(struct _On_Foot_Switch_Object));

   // Engine->On_Foot_State->On_Foot_Dial_Object= (struct _On_Foot_Dial_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Dials, sizeof(struct _On_Foot_Dial_Object));

    //Engine->On_Foot_State->On_Foot_Pickup_Item_Object= (struct _On_Foot_Pickup_Item_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Pickup_Items, sizeof(struct _On_Foot_Pickup_Item_Object));

 //   Engine->On_Foot_State->On_Foot_Fixed_Camera_Box_Object= (struct _On_Foot_Fixed_Camera_Box_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Fixed_Camera_Boxes, sizeof(struct _On_Foot_Fixed_Camera_Box_Object));

    //

    //Dialogue controls
  //  Engine->On_Foot_State->On_Foot_Conversation_Object= (struct _On_Foot_Conversation_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Conversations, sizeof(struct _On_Foot_Conversation_Object));

  // Engine->On_Foot_State->On_Foot_Conversation_Node_Object= (struct _On_Foot_Conversation_Node_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Conversation_Nodes, sizeof(struct _On_Foot_Conversation_Node_Object));

   // Engine->On_Foot_State->On_Foot_Conversation_Connection_Object= (struct _On_Foot_Conversation_Connection_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Conversation_Connections, sizeof(struct _On_Foot_Conversation_Connection_Object));

   // Engine->On_Foot_State->On_Foot_Shop_Object= (struct _On_Foot_Shop_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Shops, sizeof(struct _On_Foot_Shop_Object));

  //  Engine->On_Foot_State->On_Foot_Stage_Changing_Door_Object= (struct _On_Foot_Stage_Changing_Door_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Stage_Changing_Doors, sizeof(struct _On_Foot_Stage_Changing_Door_Object));
     //These do not end the level! Only endpoints do so!
   // Engine->On_Foot_State->On_Foot_Vehicle_Entry_Point_Object= (struct _On_Foot_Vehicle_Entry_Point_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Vehicle_Entry_Points, sizeof(struct _On_Foot_Vehicle_Entry_Point_Object));

  //  Engine->On_Foot_State->On_Foot_Elevator_Object= (struct _On_Foot_Elevator_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Elevators, sizeof(struct _On_Foot_Elevator_Object));

   // Engine->On_Foot_State->On_Foot_Light_Object= (struct _On_Foot_Light_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Lights, sizeof(struct _On_Foot_Light_Object));

    //Engine->On_Foot_State->On_Foot_Computer_Object= (struct _On_Foot_Computer_Object*)calloc(Engine->Loaded_Level_Regions->Number_Of_Computers, sizeof(struct _On_Foot_Computer_Object));


    printf("The Offset to Backgrounds is %d\n", Engine->Loaded_Level_Regions->Offset_To_Backgrounds);

    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Backgrounds; j++) //268
    {
        Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Backgrounds + (j*sizeof(struct _On_Foot_Background));

        char Filepath[256];
        char Texture_Filepath[256];

        printf("Loading backgrounds: \n");
        for(int j = 0; j < 256; j++)
        {
            Filepath[j] = Buffer[Current_Buffer_Location+j];
            Texture_Filepath[j] = Buffer[Current_Buffer_Location+j+256];
            printf("%c", Filepath[j]);
        }
        printf("\n");


        Current_Buffer_Location += 512;

        Engine->On_Foot_State->On_Foot_Background_Object[j].Model = Create_MD2_Model();
        Load_MD2_Model(Engine->On_Foot_State->On_Foot_Background_Object[j].Model, Filepath);
        Load_MD2_Texture(Engine->On_Foot_State->On_Foot_Background_Object[j].Model, Texture_Filepath);
        Engine->On_Foot_State->On_Foot_Background_Object[j].Mesh = Create_Preloaded_Collision_Mesh(Filepath);

        Engine->On_Foot_State->On_Foot_Background_Object[j].Visible = *(int*)&(Buffer[Current_Buffer_Location]);
        Engine->On_Foot_State->On_Foot_Background_Object[j].DIP_Switch_ID = *(int*)&(Buffer[Current_Buffer_Location+4]);
        Engine->On_Foot_State->On_Foot_Background_Object[j].Rotational_Velocity = *(float*)&(Buffer[Current_Buffer_Location+8]);
        Engine->On_Foot_State->On_Foot_Background_Object[j].Phase_Through = *(int*)&(Buffer[Current_Buffer_Location+12]);
        Engine->On_Foot_State->On_Foot_Background_Object[j].Is_Floor_Ceiling_Or_Wall = *(int*)&(Buffer[Current_Buffer_Location+16]);
    }

//    for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Actors; j++) //272
    {
     //   Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Actors + (j*sizeof(struct _On_Foot_Actor));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Tracks; j++) //276
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Tracks + (j*sizeof(struct _On_Foot_Track));

    }

    //for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_SFX_Data; j++) //280
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_SFX_Data + (j*sizeof(struct _On_Foot_SFX_Data));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Spawnpoints; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Spawnpoints + (j*sizeof(struct _On_Foot_Spawnpoint));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Props; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Props + (j*sizeof(struct _On_Foot_Prop));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Endpoints; j++) //Enter these to end the level!
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Endpoints + (j*sizeof(struct _On_Foot_Endpoint));

    }

    //

    //Room Controls
   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Doors; j++)
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Doors + (j*sizeof(struct _On_Foot_Door));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Conveyor_Belts; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Conveyor_Belts + (j*sizeof(struct _On_Foot_Conveyor_Belt));

    }

    //for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Ladders; j++)
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Ladders + (j*sizeof(struct _On_Foot_Ladder));

    }

    //for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Switches; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Switches + (j*sizeof(struct _On_Foot_Switch));

    }

  //  for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Dials; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Dials + (j*sizeof(struct _On_Foot_Dial));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Pickup_Items; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Pickup_Items + (j*sizeof(struct _On_Foot_Pickup_Item));

    }

    //for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Fixed_Camera_Boxes; j++) //Switch from third-person view to a security camera view in certain rooms.
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Fixed_Camera_Boxes + (j*sizeof(struct _On_Foot_Fixed_Camera_Box));

    }

    //

    //Dialogue controls
   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Conversations; j++)
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Conversations + (j*sizeof(struct _On_Foot_Conversation));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Conversation_Nodes; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Conversation_Nodes + (j*sizeof(struct _On_Foot_Conversation_Node));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Conversation_Connections; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Conversation_Connections + (j*sizeof(struct _On_Foot_Conversation_Connection));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Shops; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Shops + (j*sizeof(struct _On_Foot_Shop));

    }

  //  for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Stage_Changing_Doors; j++) //These do not end the level! Only endpoints do so!
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Stage_Changing_Doors + (j*sizeof(struct _On_Foot_Stage_Changing_Door));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Vehicle_Entry_Points; j++)
    {
    //    Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Vehicle_Entry_Points + (j*sizeof(struct _On_Foot_Vehicle_Entry_Point));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Elevators; j++)
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Elevators + (j*sizeof(struct _On_Foot_Elevator));

    }

   // for(int j = 0; j < Engine->Loaded_Level_Regions->Number_Of_Lights; j++)
    {
   //     Current_Buffer_Location = Engine->Loaded_Level_Regions->Offset_To_Lights + (j*sizeof(struct _On_Foot_Light));

    }

    Engine->On_Foot_State->On_Foot_Player = Create_Player();
    Initialize_On_Foot_Player(Engine, "None",0,0,100); //PUT THIS IN LOAD CSS ON FOOT FILE HEADER
}


void Write_On_Foot_Backgrounds(FILE* f)
{
    struct _On_Foot_Background B;
    memset(B.Filepath,0,256);
    memset(B.Texture_Filepath,0,256);

    sprintf(B.Filepath, "resources/models/model_skybox.dat");
    sprintf(B.Texture_Filepath, "resources/models/texture_skybox.bmp");

    B.Filepath[33] = '\0';
    B.Texture_Filepath[35] = '\0';

    B.DIP_Switch_ID = -1;
    B.Rotational_Velocity = 0;
    B.Visible = 1;
    B.Phase_Through = 1;
    //B.SFX_ID = -1;




    struct _On_Foot_Background BTwo;
    memset(BTwo.Filepath,0,256);
    memset(BTwo.Texture_Filepath,0,256);

    sprintf(BTwo.Filepath, "resources/models/model_skybox.dat");
    sprintf(BTwo.Texture_Filepath, "resources/models/texture_skybox.bmp");

    BTwo.Filepath[33] = '\0';
    BTwo.Texture_Filepath[35] = '\0';

    BTwo.DIP_Switch_ID = -1;
    BTwo.Rotational_Velocity = 0;
    BTwo.Visible = 1;
    BTwo.Phase_Through = 1;
    //B.SFX_ID = -1;

    fwrite(&BTwo,1,sizeof(struct _On_Foot_Background),f);
}

void Write_On_Foot_Actors(FILE* f)
{

}
void Write_On_Foot_Tracks(FILE* f)
{

}
void Write_On_Foot_SFX_Data(FILE* f)
{

}
void Write_On_Foot_Spawnpoints(FILE* f)
{
    struct _On_Foot_Spawnpoint S;
    S.DIP_Switch_ID = -1;
    S.x = 0;
    S.y = 0;
    S.z = 75;
    fwrite(&S,1,sizeof(struct _On_Foot_Spawnpoint),f);
}
void Write_On_Foot_Props(FILE* f)
{

}
void Write_On_Foot_Endpoints(FILE* f)
{
    struct _On_Foot_Endpoint E;
}
void Write_On_Foot_Doors(FILE* f)
{
    struct _On_Foot_Door D;
}
void Write_On_Foot_Conveyor_Belts(FILE* f)
{
    struct _On_Foot_Conveyor_Belt C;
}
void Write_On_Foot_Ladders(FILE* f)
{
    struct _On_Foot_Ladder L;
}
void Write_On_Foot_Switches(FILE* f)
{
    struct _On_Foot_Switch S;
}
void Write_On_Foot_Dials(FILE* f)
{
    struct _On_Foot_Dial D;
}
void Write_On_Foot_Pickup_Items(FILE* f)
{
    struct _On_Foot_Pickup_Item P;
}
void Write_On_Foot_Fixed_Camera_Boxes(FILE* f)
{
    struct _On_Foot_Fixed_Camera_Box F;
}
void Write_On_Foot_Conversations(FILE* f)
{
    struct _On_Foot_Conversation C;
}
void Write_On_Foot_Conversation_Nodes(FILE* f)
{
    struct _On_Foot_Conversation_Node C;
}
void Write_On_Foot_Conversation_Connections(FILE* f)
{
    struct _On_Foot_Conversation_Connection C;
}
void Write_On_Foot_Shops(FILE* f)
{
    struct _On_Foot_Shop S;
}
void Write_On_Foot_Shop_Inventory(FILE* f)
{
    struct _On_Foot_Shop_Inventory S;
}
void Write_On_Foot_Stage_Changing_Doors(FILE* f)
{

}
void Write_On_Foot_Vehicle_Entry_Points(FILE* f)
{
    struct _On_Foot_Vehicle_Entry_Point V;
}
void Write_On_Foot_Elevators(FILE* f)
{
    struct _On_Foot_Elevator E;
}
void Write_On_Foot_Lights(FILE* f)
{

}

void Write_On_Foot_Computers(FILE* f)
{

}

void Write_On_Foot_Vehicles(FILE* f)
{

}

//THIS IS CURRENT!!!
void Write_CSS_On_Foot_File_Header(const char* Filename)
{
    struct _On_Foot_File_Header Header;

    Header.Number_Of_Backgrounds = 2; //268 //First background is a collision box!
//    Header.Number_Of_Actors = 0;//272
 //   Header.Number_Of_Tracks = 0; //276
  //  Header.Number_Of_SFX_Data = 0; //280
  //  Header.Number_Of_Spawnpoints = 1;
  //  Header.Number_Of_Props = 0;
  //  Header.Number_Of_Endpoints = 0; //Enter these to end the level!
    //

    //Room Controls
   // Header.Number_Of_Doors = 0;
   // Header.Number_Of_Conveyor_Belts = 0;
  //  Header.Number_Of_Ladders = 0;
   // Header.Number_Of_Switches = 0;
  //  Header.Number_Of_Dials = 0;
  //  Header.Number_Of_Pickup_Items = 0;
  //  Header.Number_Of_Fixed_Camera_Boxes = 0; //Switch from third-person view to a security camera view in certain rooms.
    //

    //Dialogue controls
  //  Header.Number_Of_Conversations = 0;
  //  Header.Number_Of_Conversation_Nodes = 0;
  //  Header.Number_Of_Conversation_Connections = 0;
  //  Header.Number_Of_Shops = 0;
  //  Header.Number_Of_Stage_Changing_Doors = 0; //These do not end the level! Only endpoints do so!
  //  Header.Number_Of_Vehicle_Entry_Points = 0;
  //  Header.Number_Of_Elevators = 0;
  //  Header.Number_Of_Lights = 0;

   // Header.Number_Of_Computers = 1;
//    Header.Number_Of_Vehicles = 0;
    //Header.Number_Of_Computer_Storage_Devices = 0;

   // Header.Number_Of_Broadcast_Towers = 0;
    //Header.Number_Of_Volcanoes = 0;
    //Header.Number_Of_Sonic_Black_Holes = 0;
   // Header.Number_Of_Rainstorms = 0;
   // Header.Number_Of_Duststorms = 0;
    //Header.Number_Of_Snowstorms = 0;
   // Header.Number_Of_Hailstorms = 0;
   // Header.Number_Of_Earthquakes = 0;
    //Header.Number_Of_Tornadoes = 0;
    //Header.Number_Of_Hurricanes = 0;
   // Header.Number_Of_Auroras = 0;

   // Header.Number_Of_Docking_Stations = 0;

   // Header.Number_Of_Ships = 0;

   // Header.Number_Of_NPC_Plants = 0;
   // Header.Number_Of_NPC_Swimming_Creatures = 0;
    //Header.Number_Of_NPC_Terrestrial_Creatures = 0;
   // Header.Number_Of_NPC_Flying_Creatures = 0;
   // Header.Number_Of_NPC_Fungi = 0;
   // Header.Number_Of_NPC_Microbes = 0;

   // Header.Number_Of_Pools_Of_Liquid = 0;
  //  Header.Number_Of_Mirrors = 0;





    int Size_Of_Backgrounds = Header.Number_Of_Backgrounds * sizeof(struct _On_Foot_Background); //268
    printf("Size of Backgrounds: %d\n", Size_Of_Backgrounds);
//    int Size_Of_Actors = Header.Number_Of_Actors * sizeof(struct _On_Foot_Actor); //272
 //   int Size_Of_Tracks = Header.Number_Of_Tracks * sizeof(struct _On_Foot_Track); //276
 //   int Size_Of_SFX_Data = Header.Number_Of_SFX_Data * sizeof(struct _On_Foot_SFX_Data); //280
 //   int Size_Of_Spawnpoints = Header.Number_Of_Spawnpoints * sizeof(struct _On_Foot_Spawnpoint);
 //   int Size_Of_Props = Header.Number_Of_Props * sizeof(struct _On_Foot_Prop);
 //   int Size_Of_Endpoints = Header.Number_Of_Endpoints * sizeof(struct _On_Foot_Endpoint); //Enter these to end the level!
    //

    //Room Controls
  //  int Size_Of_Doors = Header.Number_Of_Doors * sizeof(struct _On_Foot_Door);
  //  int Size_Of_Conveyor_Belts = Header.Number_Of_Conveyor_Belts * sizeof(struct _On_Foot_Conveyor_Belt);
 //   int Size_Of_Ladders = Header.Number_Of_Ladders * sizeof(struct _On_Foot_Ladder);
  //  int Size_Of_Switches = Header.Number_Of_Switches * sizeof(struct _On_Foot_Switch);
  //  int Size_Of_Dials = Header.Number_Of_Dials * sizeof(struct _On_Foot_Dial);
  //  int Size_Of_Pickup_Items = Header.Number_Of_Pickup_Items * sizeof(struct _On_Foot_Pickup_Item);
  //  int Size_Of_Fixed_Camera_Boxes = Header.Number_Of_Fixed_Camera_Boxes * sizeof(struct _On_Foot_Fixed_Camera_Box); //Switch from third-person view to a security camera view in certain rooms.
    //

    //Dialogue controls
  //  int Size_Of_Conversations = Header.Number_Of_Conversations * sizeof(struct _On_Foot_Conversation);
  //  int Size_Of_Conversation_Nodes = Header.Number_Of_Conversation_Nodes * sizeof(struct _On_Foot_Conversation_Node);
  //  int Size_Of_Conversation_Connections = Header.Number_Of_Conversation_Connections * sizeof(struct _On_Foot_Conversation_Connection);
  //  int Size_Of_Shops = Header.Number_Of_Shops * sizeof(struct _On_Foot_Shop);
  //  int Size_Of_Stage_Changing_Doors = Header.Number_Of_Stage_Changing_Doors * sizeof(struct _On_Foot_Stage_Changing_Door); //These do not end the level! Only endpoints do so!
   // int Size_Of_Vehicle_Entry_Points = Header.Number_Of_Vehicle_Entry_Points * sizeof(struct _On_Foot_Vehicle_Entry_Point);
  //  int Size_Of_Elevators = Header.Number_Of_Elevators * sizeof(struct _On_Foot_Elevator);
  //  int Size_Of_Lights = Header.Number_Of_Lights * sizeof(struct _On_Foot_Light);

  //  int Size_Of_Computers = Header.Number_Of_Computers * sizeof(struct _On_Foot_Computer);
   // int Size_Of_Vehicles = Header.Number_Of_Vehicles * sizeof(struct _On_Foot_Vehicle);
    //int Size_Of_Computer_Storage_Devices = Header.Number_Of_Computer_Storage_Devices * sizeof(struct _On_Foot_Computer_Storage_Device);

   // int Size_Of_Broadcast_Towers = Header.Number_Of_Broadcast_Towers * sizeof(struct _On_Foot_Broadcast_Tower);
  //  int Size_Of_Volcanoes = Header.Number_Of_Volcanoes * sizeof(struct _On_Foot_Volcano);
  //  int Size_Of_Sonic_Black_Holes = Header.Number_Of_Sonic_Black_Holes * sizeof(struct _On_Foot_Sonic_Black_Hole);
  //  int Size_Of_Rainstorms = Header.Number_Of_Rainstorms * sizeof(struct _On_Foot_Rainstorm);
  //  int Size_Of_Duststorms = Header.Number_Of_Duststorms * sizeof(struct _On_Foot_Duststorm);
  //  int Size_Of_Snowstorms = Header.Number_Of_Snowstorms * sizeof(struct _On_Foot_Snowstorm);
  //  int Size_Of_Hailstorms = Header.Number_Of_Hailstorms * sizeof(struct _On_Foot_Hailstorm);
  //  int Size_Of_Earthquakes = Header.Number_Of_Earthquakes * sizeof(struct _On_Foot_Earthquake);
   // int Size_Of_Tornadoes = Header.Number_Of_Tornadoes * sizeof(struct _On_Foot_Tornado);
    //int Size_Of_Hurricanes = Header.Number_Of_Hurricanes * sizeof(struct _On_Foot_Hurricane);
   // int Size_Of_Auroras = Header.Number_Of_Auroras * sizeof(struct _On_Foot_Aurora);

   // int Size_Of_Docking_Stations = Header.Number_Of_Docking_Stations * sizeof(struct _On_Foot_Docking_Station);

  //  int Size_Of_Ships = Header.Number_Of_Ships * sizeof(struct _On_Foot_Ship);

  //  int Size_Of_NPC_Plants = Header.Number_Of_NPC_Plants * sizeof(struct _On_Foot_NPC_Plant);
  //  int Size_Of_NPC_Swimming_Creatures = Header.Number_Of_NPC_Swimming_Creatures * sizeof(struct _On_Foot_NPC_Swimming_Creature);
   // int Size_Of_NPC_Terrestrial_Creatures = Header.Number_Of_NPC_Terrestrial_Creatures * sizeof(struct _On_Foot_NPC_Terrestrial_Creature);
   // int Size_Of_NPC_Flying_Creatures = Header.Number_Of_NPC_Flying_Creatures  * sizeof(struct _On_Foot_NPC_Flying_Creature);
   // int Size_Of_NPC_Fungi = Header.Number_Of_NPC_Fungi * sizeof(struct _On_Foot_NPC_Fungi);
   // int Size_Of_NPC_Microbes = Header.Number_Of_NPC_Microbes * sizeof(struct _On_Foot_NPC_Microbes);

  //  int Size_Of_Pools_Of_Liquid = Header.Number_Of_Pools_Of_Liquid * sizeof(struct _On_Foot_Pool_Of_Liquid);

  //  int Size_Of_Mirrors = Header.Number_Of_Mirrors * sizeof(struct _On_Foot_Mirror);



    int Current_Buffer_Location = sizeof(struct _On_Foot_File_Header);

    Header.Offset_To_Backgrounds = Current_Buffer_Location;
    printf("Offset to Backgrounds: %d\n", Header.Offset_To_Backgrounds);
    Current_Buffer_Location += Size_Of_Backgrounds;

//    Header.Offset_To_Actors = Current_Buffer_Location;
 //   Current_Buffer_Location += Size_Of_Actors;

  //  Header.Offset_To_Tracks = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Tracks;

   // Header.Offset_To_SFX_Data = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_SFX_Data;

   // Header.Offset_To_Spawnpoints = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Spawnpoints;

  //  Header.Offset_To_Props = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Props;

  //  Header.Offset_To_Endpoints = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Endpoints;

    //

    //Room Controls
   // Header.Offset_To_Doors = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Doors;

  //  Header.Offset_To_Conveyor_Belts = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Conveyor_Belts;

  //  Header.Offset_To_Ladders = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Ladders;

   // Header.Offset_To_Switches = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_Switches;

   // Header.Offset_To_Dials = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_Dials;

   // Header.Offset_To_Pickup_Items = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Pickup_Items;

  //  Header.Offset_To_Fixed_Camera_Boxes = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Fixed_Camera_Boxes;

    //

    //Dialogue controls
  //  Header.Offset_To_Conversations = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Conversations;

  //  Header.Offset_To_Conversation_Nodes = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Conversation_Nodes;

  //  Header.Offset_To_Conversation_Connections = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Conversation_Connections;

   // Header.Offset_To_Shops = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_Shops;

   // Header.Offset_To_Stage_Changing_Doors = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_Stage_Changing_Doors;

  //  Header.Offset_To_Vehicle_Entry_Points = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Vehicle_Entry_Points;

  //  Header.Offset_To_Elevators = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_Elevators;

   // Header.Offset_To_Lights = Current_Buffer_Location;
  //  Current_Buffer_Location += Size_Of_Lights;

   // Header.Offset_To_Computers = Current_Buffer_Location;
   // Current_Buffer_Location += Size_Of_Computers;

//    Header.Offset_To_Vehicles = Current_Buffer_Location;
 //   Current_Buffer_Location += Size_Of_Vehicles;


    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _On_Foot_File_Header),f);
    rewind(f);


    fseek(f, Header.Offset_To_Backgrounds, SEEK_SET);
    Write_On_Foot_Backgrounds(f); //268
    //rewind(f);

  //  fseek(f, Header.Offset_To_Actors, SEEK_SET);
 //   Write_On_Foot_Actors(f); //272
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Tracks, SEEK_SET);
  //  Write_On_Foot_Tracks(f); //276
  //  rewind(f);

  //  fseek(f, Header.Offset_To_SFX_Data, SEEK_SET);
  //  Write_On_Foot_SFX_Data(f); //280
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Spawnpoints, SEEK_SET);
  //  Write_On_Foot_Spawnpoints(f);
  //  rewind(f);

 //   fseek(f, Header.Offset_To_Props, SEEK_SET);
  //  Write_On_Foot_Props(f);
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Endpoints, SEEK_SET);
 //   Write_On_Foot_Endpoints(f); //Enter these to end the level!
  //  rewind(f);
    //



    //Room Controls
  //  fseek(f, Header.Offset_To_Doors, SEEK_SET);
  //  Write_On_Foot_Doors(f);
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Conveyor_Belts, SEEK_SET);
  //  Write_On_Foot_Conveyor_Belts(f);
  //  rewind(f);

   // fseek(f, Header.Offset_To_Ladders, SEEK_SET);
   // Write_On_Foot_Ladders(f);
   // rewind(f);

   // fseek(f, Header.Offset_To_Switches, SEEK_SET);
   // Write_On_Foot_Switches(f);
   // rewind(f);

  //  fseek(f, Header.Offset_To_Dials, SEEK_SET);
  //  Write_On_Foot_Dials(f);
  //  rewind(f);

   // fseek(f, Header.Offset_To_Pickup_Items, SEEK_SET);
   // Write_On_Foot_Pickup_Items(f);
   // rewind(f);

   // fseek(f, Header.Offset_To_Fixed_Camera_Boxes, SEEK_SET);
  //  Write_On_Foot_Fixed_Camera_Boxes(f); //Switch from third-person view to a security camera view in certain rooms.
   // rewind(f);
    //





    //Dialogue controls
 //   fseek(f, Header.Offset_To_Conversations, SEEK_SET);
 //   Write_On_Foot_Conversations(f);
  //  rewind(f);

 //   fseek(f, Header.Offset_To_Conversation_Nodes, SEEK_SET);
 //   Write_On_Foot_Conversation_Nodes(f);
  //  rewind(f);

 //   fseek(f, Header.Offset_To_Conversation_Connections, SEEK_SET);
 //   Write_On_Foot_Conversation_Connections(f);
  //  rewind(f);

 //   fseek(f, Header.Offset_To_Shops, SEEK_SET);
 //   Write_On_Foot_Shops(f);
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Stage_Changing_Doors, SEEK_SET);
  //  Write_On_Foot_Stage_Changing_Doors(f); //These do not end the level! Only endpoints do so!
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Vehicle_Entry_Points, SEEK_SET);
  //  Write_On_Foot_Vehicle_Entry_Points(f);
  //  rewind(f);

  //  fseek(f, Header.Offset_To_Elevators, SEEK_SET);
  //  Write_On_Foot_Elevators(f);
  //  rewind(f);

 //   fseek(f, Header.Offset_To_Lights, SEEK_SET);
  //  Write_On_Foot_Lights(f);
   // rewind(f);

   // fseek(f, Header.Offset_To_Computers, SEEK_SET);
   // Write_On_Foot_Computers(f);
    //rewind(f);

  //  fseek(f, Header.Offset_To_Vehicles, SEEK_SET);
  //  Write_On_Foot_Vehicles(f);
  //  rewind(f);


    fclose(f);

}


void Write_Conversation_File(const char* Filename)
{

}
void Load_Conversation_File(struct _Engine* Engine, const char* Filename)
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

    Engine->On_Foot_State->Conversation_Mini_State->Current_Step = 0;
    Engine->On_Foot_State->Conversation_Mini_State->Timer = Create_Timer();

    Set_Timer(Engine->On_Foot_State->Conversation_Mini_State->Timer);
    Pause_Timer(Engine->On_Foot_State->Conversation_Mini_State->Timer);

    for(int l = 0; l < 256; l++)
    {
        Engine->On_Foot_State->Conversation_Mini_State->Header.Conversation_Name[l] = Buffer[Buffer_Location+l];
    }
    Buffer_Location+=256;

    Engine->On_Foot_State->Conversation_Mini_State->Header.Conversation_Type= *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Conversation_Mini_State->Header.Conversation_Complete= *(int*)&(Buffer[Buffer_Location+4]);

    Engine->On_Foot_State->Conversation_Mini_State->Header.Maximum_Conversation_Duration= *(float*)&(Buffer[Buffer_Location+8]);

    Engine->On_Foot_State->Conversation_Mini_State->Header.Number_Of_Steps= *(int*)&(Buffer[Buffer_Location+12]);
    Engine->On_Foot_State->Conversation_Mini_State->Header.Offset_To_Steps= *(int*)&(Buffer[Buffer_Location+16]);

    Engine->On_Foot_State->Conversation_Mini_State->Steps = (struct _Conversation_Step_Object*)calloc(1,Engine->On_Foot_State->Conversation_Mini_State->Header.Number_Of_Steps*sizeof(struct _Conversation_Step_Object));

    for(int j = 0; j < Engine->On_Foot_State->Conversation_Mini_State->Header.Number_Of_Steps; j++)
    {
        Buffer_Location = Engine->On_Foot_State->Conversation_Mini_State->Header.Offset_To_Steps + (j*sizeof(struct _Conversation_Step));

        char Voice_Line_Filepath[256];
        char Voice_Line[256];

        for(int l = 0; l < 256; l++)
        {
            Voice_Line_Filepath[l] = Buffer[Buffer_Location+l];
            Voice_Line[l] = Buffer[256+Buffer_Location+l];
            //printf("%c", Voice_Line_Filepath[l]);
        }

        Buffer_Location += 512;

        int Voice_Line_Length = *(int*)&(Buffer[Buffer_Location]);
        int Step_Type = *(int*)&(Buffer[Buffer_Location+4]);

        int Step_ID = *(int*)&(Buffer[Buffer_Location+8]);
        int Next_Step = *(int*)&(Buffer[Buffer_Location+12]);

        float Start_Time = *(float*)&(Buffer[Buffer_Location+16]);
        float End_Time = *(float*)&(Buffer[Buffer_Location+20]);
        int Complete = *(int*)&(Buffer[Buffer_Location+24]);

        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Voice_Line_Caption = Create_Image();
        Initialize_Text_Image(Engine->Text_Engine, Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Voice_Line_Caption, Voice_Line,Voice_Line_Length,Voice_Line_Length*17,24);

        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Voice_Line = Mix_LoadWAV(Voice_Line_Filepath);

        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Complete = Complete;
        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].End_Time = End_Time;
        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Next_Step = Next_Step;
        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Start_Time = Start_Time;
        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Step_ID = Step_ID;
        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Step_Type = Step_Type;
        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Voice_Line_Length = Voice_Line_Length;

        Engine->On_Foot_State->Conversation_Mini_State->Steps[j].Voice_Line_Played = 0;
    }
}

void Start_Conversation(struct _Engine* Engine)
{
    Resume_Timer(Engine->On_Foot_State->Conversation_Mini_State->Timer);
    Engine->On_Foot_State->Conversation_Mini_State->In_Conversation = true;
    Engine->On_Foot_State->Conversation_Mini_State->Current_Step = 0;
}
void End_Conversation(struct _Engine* Engine)
{
    free(Engine->On_Foot_State->Conversation_Mini_State->Timer);
    Engine->On_Foot_State->Conversation_Mini_State->Timer = Create_Timer();
    Set_Timer(Engine->On_Foot_State->Conversation_Mini_State->Timer);
    Pause_Timer(Engine->On_Foot_State->Conversation_Mini_State->Timer);
    Engine->On_Foot_State->Conversation_Mini_State->In_Conversation = false;
}

void Process_Conversation(struct _Engine* Engine)
{
    double Ticks = Get_Ticks(Engine->On_Foot_State->Conversation_Mini_State->Timer);
    //printf("Ticks: %f\n", Ticks);
    //printf("Current Step: %d\n", Engine->On_Foot_State->Conversation_Mini_State->Current_Step);
    //printf("Next Step: %d\n", Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Next_Step);

    if(Ticks > Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Start_Time)
    {
        if(Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Voice_Line_Played == 0)
        {
            Mix_PlayChannel( -1, Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Voice_Line, 0 );
            Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Voice_Line_Played = 1;
        }
        if(Ticks > Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].End_Time)
        {
            if(Engine->On_Foot_State->Conversation_Mini_State->Current_Step+1 < Engine->On_Foot_State->Conversation_Mini_State->Header.Number_Of_Steps)
            {
                Engine->On_Foot_State->Conversation_Mini_State->Current_Step+=1;
            }
        }
    }
    if(Ticks > Engine->On_Foot_State->Conversation_Mini_State->Header.Maximum_Conversation_Duration)
    {
        End_Conversation(Engine);

    }
}
void Render_Conversation(struct _Engine* Engine)
{
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640,360, 0.0, 1.0, -1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Render_Image(Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Voice_Line_Caption,Center_Coordinates(Engine->On_Foot_State->Conversation_Mini_State->Steps[Engine->On_Foot_State->Conversation_Mini_State->Current_Step].Voice_Line_Caption->Width,640),256+32,1);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(35, 1.7777,1.0f, 100000000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}
void Handle_Conversation_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{

}

float Face_Object(float x1, float x2, float y1, float y2)
{
    float result;

    float x;
    float y;

    x = abs(x2 - x1);
    y = abs(y2 - y1);

    if(x != 0)
    {
        result = (atan2f(y,x))*57.2958279088;
    }


    if(result < 0)
    {
        result += 360;
    }

    if(result > 359)
    {
        result -= 360;
    }

    //printf("Result: %f\n", result);

    return result;
}

struct _Interim_State* Create_Interim_State()
{
    struct _Interim_State* M = (struct _Interim_State*) calloc(1,sizeof(struct _Interim_State));
    return M;
}
void Destroy_Interim_State(struct _Engine* Engine)
{

}
void Initialize_Interim_State(struct _Engine* Engine)
{

}
void Process_Interim_State(struct _Engine* Engine)
{
    switch(Engine->Interim_State->Current_Instruction)
    {
        case INSTRUCTION_GO_TO_REGION:
            Mix_HaltChannel(0);
            Mix_HaltMusic();
            free(Engine->On_Foot_State);
            Engine->On_Foot_State = Create_On_Foot_State();
            Initialize_On_Foot_State(Engine);
            Engine->Current_Step = Engine->Interim_State->Current_Instruction_Parameter;
            Load_CSS_On_Foot_File_Header(Engine, Engine->Loaded_Steps[Engine->Current_Step].Filepath);
            Engine->Current_State =  STATE_ON_FOOT;
        break;
    }
}

void calculateLookAtRotation(float x1, float y1, float z1, float x2, float y2, float z2,
                             float *angle_out, float axis_out[3]) {

    // 1. Calculate the direction vector D from source to target
    float dir[3] = {
        x2 - x1,
        y2 - y1,
        z2 - z1
    };
    // Calculate length of the direction vector
    float len = sqrtf(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);

    // Safety check: if blocks are too close, prevent division by zero or jitter
    if (len < 0.001f) {
        *angle_out = 0.0f;
        // Rotation axis defaults to Z-up, but since the angle is 0, it won't matter.
        axis_out[0] = 0.0f; axis_out[1] = 0.0f; axis_out[2] = 1.0f;
        return;
    }

    // Normalized desired direction vector (D_hat)
    float D_hat[3] = {dir[0]/len, dir[1]/len, dir[2]/len};

    // The initial forward vector of the cube is now Y- (0, -1, 0) <--- **UPDATED**
    const float FORWARD_hat[3] = {0.0f, 1.0f, 0.0f};

    // 2. Calculate Angle (Dot Product: FORWARD_hat . D_hat)
    float dot_product = FORWARD_hat[0]*D_hat[0] + FORWARD_hat[1]*D_hat[1] + FORWARD_hat[2]*D_hat[2];

    // Clamp dot product to [-1, 1] due to potential floating point inaccuracies
    if (dot_product > 1.0f) dot_product = 1.0f;
    if (dot_product < -1.0f) dot_product = -1.0f;

    float angle_radians = acosf(dot_product);
    *angle_out = angle_radians * 180.0f / (float)M_PI; // Convert to degrees

    // 3. Calculate Axis of Rotation (Cross Product: FORWARD_hat x D_hat)
    axis_out[0] = FORWARD_hat[1]*D_hat[2] - FORWARD_hat[2]*D_hat[1];
    axis_out[1] = FORWARD_hat[2]*D_hat[0] - FORWARD_hat[0]*D_hat[2];
    axis_out[2] = FORWARD_hat[0]*D_hat[1] - FORWARD_hat[1]*D_hat[0];
}

void Add_Underwater_Explosion(struct _Engine* Engine)
{
    Engine->On_Foot_State->Number_Of_Underwater_Explosions += 1;
    Engine->On_Foot_State->Current_Underwater_Explosion += 1;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[Engine->On_Foot_State->Current_Underwater_Explosion] = (struct _Underwater_Explosion*)calloc(1,sizeof(struct _Underwater_Explosion));
}

void Process_Underwater_Explosions(struct _Engine* Engine)
{

}

void Set_Underwater_Explosion_Parameters(struct _Engine* Engine, unsigned char ID,bool Visible,
    bool Active,float Explosion_Pressure_Wave_Radius,float Explosion_Fireball_Radius,float Maximum_Explosion_Pressure_Wave_Radius,float Maximum_Explosion_Fireball_Radius,int Explosion_Frame,float x,float y,float z)
{
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Active=Active;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Explosion_Fireball_Radius=Explosion_Fireball_Radius;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Explosion_Frame=Explosion_Frame;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Explosion_Pressure_Wave_Radius=Explosion_Pressure_Wave_Radius;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Maximum_Explosion_Fireball_Radius=Maximum_Explosion_Fireball_Radius;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Maximum_Explosion_Pressure_Wave_Radius=Maximum_Explosion_Pressure_Wave_Radius;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->Visible=Visible;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->x=x;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->y=y;
    Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->z=z;
}

void Render_Underwater_Explosion(struct _Engine* Engine, unsigned char ID)
{
    Render_TwoD_Sprite_Animation(Engine->On_Foot_State->Underwater_Explosion_Billboard,Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->x,Engine->On_Foot_State->On_Foot_Underwater_Explosions[ID]->y);
}

#ifndef SUBMARINE_SCENE_H_INCLUDED
#define SUBMARINE_SCENE_H_INCLUDED
#include "../../engine/engine.h"
#include "../../engine/graphics/new_model.h"
//This is the NEW PLATFORMING STATE.
#include "../../engine/graphics/image.h"
#include "../../engine/camera/camera.h"
#include "../../engine/physics/vector.h"
#include "submarine.h"
#include "submarine_weapon.h"
#include "npc_submarine.h"
#include "../../engine/level.h"

struct _Skybox_Manager
{
    struct _MD2_Model* Skybox;
};

struct _Submarine_State_HUD_Manager
{
    struct _Image* HUD;
};

struct _Submarine_State_In_Flight_Dialogue_Manager
{
    struct _Image* Dialogue;
};

struct _Player_Submarine_Manager
{
    struct _MD2_Model* Player_Submarine;
    float Throttle_In_KM_H;
    float x;
    float y;
    float z;

    Vector Up;
    Vector Forward;
    Vector Right;

    float dx_Vector; //"Up" is perpendcular to the movement vector.
    float dy_Vector; //Plus y = Going forwards
    float dz_Vector;

    float Roll;
    float Yaw;
    float Pitch;
};

struct _Capital_Submarine
{
    struct _MD2_Model* Model;
    int ID;

    double x;
    double y;
    double z;

    double dx;
    double dy;
    double dz;
};


struct _Submarine_State
{
    struct _Level_File_Header* Level_File_Header;
    struct _Image* Submarine_Image;

    struct _Image* Dialogue;

    struct _Image* Sonar;

    struct _MD2_Model* Submarine;

    struct _Capital_Submarine* Capital_Submarines;

    struct _Submarine_Laser_Bolt_Registry* Registry;
    struct _Submarine_Torpedo_Registry* Torpedo_Registry;

 //   unsigned int Number_Of_NPC_Submarines;
  //  struct _NPC_Submarine* NPC_Submarine_Registry[2];

    struct _New_Camera* Camera;
    struct _Player_Submarine* Player_Submarine;
    struct _Skybox_Manager* Skybox_Manager;
};

struct _Submarine_State* Create_Submarine_State();
void Destroy_Submarine_State(struct _Engine* Engine);

void Initialize_Submarine_State(struct _Engine* Engine, const char* Level_Filename);

void Render_Submarine_State(struct _Engine* Engine);
void Input_Submarine_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Submarine_State(struct _Engine* Engine);
#endif // SUBMARINE_SCENE_H_INCLUDED

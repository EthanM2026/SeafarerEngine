#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
#include <stdio.h>
#include "../engine.h"

#pragma pack(push, 1)

#define GRASP_MODE_TWO_HANDED 1
#define GRASP_MODE_ONE_HANDED 2
#define GRASP_MODE_HOLD_WITH_ONE_HAND 3
#define GRASP_MODE_HOLD_WITH_TWO_HAND 4
#define GRASP_MODE_LIFT 5

struct _Weapon_Entry
{
    char Name[256];
    int Grasp_Mode; //Two-handed, one-handed, hold with one hand, hold with two hands, Lift.
    char Weapon_Model_Filepath[256];
    char Weapon_Texture_Filepath[256];
    char Weapon_Icon_Filepath[256];
    int Weapon_Unholster_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Weapon_Holster_Sound_ID; //Use one of the Universal Sounds.
    int Weapon_Fire_Sound_ID; //Use one of the Universal Sounds.
    int Weapon_Reload_Sound_ID; //Use one of the Universal Sounds.

    int Current_Ammo;
    bool Allowed_To_Reload;
    double Reload_Speed;
    int Ammo_Mode; //Homing, projectile, grenade
    double Ammo_Lifetime_In_Seconds;
    double Ammo_Maximum_Distance_In_Meters;
    int Ammo_Moving_Through_Space_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Ammo_Rolling_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Ammo_Striking_Object_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Ammo_Exploding_Object_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.

    double Ammo_Collision_Radius;
    double Ammo_Mass;
    double Cooldown_Speed;

    int Ammo_Per_Clip;
    int Current_Stored_Ammo;

    int Damage;
    int Status_Effect;
};

#pragma pack(pop)

struct _Weapon_Object
{
    struct _Weapon_Entry Weapon_Entry;
    struct _SE3_Model* Weapon_Model;
    int Using_This_Projectile_Model;
    struct _Image* Weapon_Icon;
};

struct _Weapon_Round_Object
{
    double x;
    double y;
    double z;

    Vec3 localRight;
    Vec3 localUp;
    Vec3 localForward;

    double Velocity;

    double Time_Created;

    bool Dead;

    int Using_This_Weapon_Round_Model;

};

void Write_Weapon(const char* Filename);
void Load_Weapon(struct _Engine* Engine, int ID, const char* Filename);

void Give_Weapon(struct _Engine* Engine, int ID, int Destination_ID);

void Handle_Weapon_Inputs(struct _Engine* Engine, struct _Keypad Keypad, int ID);
void Process_Weapon(struct _Engine* Engine, int ID);
void Render_Weapon(struct _Engine* Engine, int ID);

void Create_Weapon_Round(struct _Engine* Engine, Vec3 localRight, Vec3 localUp, Vec3 localForward);
void Process_Weapon_Round(struct _Engine* Engine, int ID);
void Render_Weapon_Round(struct _Engine* Engine, int ID);
void Handle_Weapon_Round_Inputs(struct _Engine* Engine, struct _Keypad Keypad);
#endif // ITEM_H_INCLUDED

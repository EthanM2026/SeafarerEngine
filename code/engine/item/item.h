#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
#include <stdio.h>
#include "../engine.h"

struct _Weapon_Entry
{
    char Name[256];
    int Category;
    int ID;
    int Variant;
    int Grasp_Mode; //Two-handed, one-handed, hold with one hand, hold with two hands, Lift.
    char Weapon_Model_Filepath[256];
    char Weapon_Texture_Filepath[256];
    char Projectile_Model_Filepath[256];
    char Projectile_Texture_Filepath[256];
    char Weapon_Icon_Filepath[256];
    int Weapon_Unholster_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Weapon_Holster_Sound_ID; //Use one of the Universal Sounds.
    int Weapon_Fire_Sound_ID; //Use one of the Universal Sounds.
    int Weapon_Reload_Sound_ID; //Use one of the Universal Sounds.

    int Maximum_Ammo;
    int Current_Ammo;
    int Allowed_To_Reload;
    int Ammo_Mode; //Homing, projectile, grenade
    double Ammo_Lifetime_In_Seconds;
    double Ammo_Maximum_Distance_In_Meters;
    int Ammo_Moving_Through_Space_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Ammo_Rolling_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Ammo_Striking_Object_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.
    int Ammo_Exploding_Object_Sound_ID; //Use one of the Universal Sounds. -1 means No Sound.

    double Projectile_Size;
};

struct _Weapon
{
    struct _Weapon_Entry Weapon_Entry;
    struct _SE3_Model* Weapon_Model;
    struct _SE3_Model* Projectile_Model;
    struct _Image* Weapon_Icon;
};

void Write_Weapon(const char* Filename);
void Load_Weapon(struct _Engine* Engine, int ID, const char* Filename);

void Give_Weapon(struct _Engine* Engine, int ID, int Destination_ID);

void Handle_Weapon_Inputs(struct _Engine* Engine, struct Keypad Keypad, int ID);
void Process_Weapon(struct _Engine* Engine, int ID);
void Render_Weapon(struct _Engine* Engine, int ID);
#endif // ITEM_H_INCLUDED

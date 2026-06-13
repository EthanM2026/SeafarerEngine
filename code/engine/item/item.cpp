#include "item.h"

void Write_Weapon(const char* Filename)
{
    struct _Weapon_Entry Weapon;

    memset(Weapon.Name,0,256);
    sprintf(Weapon.Name, "TEST WEAPON");
    Weapon.Name[11] = '\0';

    Weapon.Category = 0;
    Weapon.ID = 0;
    Weapon.Variant = 0;
    Weapon.Grasp_Mode = 0; //Two-handed, one-handed, hold with one hand, hold with two hands, Lift.

    memset(Weapon.Weapon_Model_Filepath,0,256);
    memset(Weapon.Weapon_Texture_Filepath,0,256);
    memset(Weapon.Projectile_Model_Filepath,0,256);
    memset(Weapon.Projectile_Texture_Filepath,0,256);
    memset(Weapon.Weapon_Icon_Filepath,0,256);

    sprintf(Weapon.Weapon_Model_Filepath, "resources/data/items/test_weapon/weapon.se3");
    sprintf(Weapon.Weapon_Texture_Filepath, "resources/data/items/test_weapon/weapon.sei");
    sprintf(Weapon.Projectile_Model_Filepath, "resources/data/items/test_weapon/projectile.se3");
    sprintf(Weapon.Projectile_Texture_Filepath, "resources/data/items/test_weapon/projectile.sei");
    sprintf(Weapon.Weapon_Icon_Filepath, "resources/data/items/test_weapon3/icon.sei");

    Weapon.Weapon_Model_Filepath[43] = '\0';
    Weapon.Weapon_Texture_Filepath[43] = '\0';
    Weapon.Projectile_Model_Filepath[47] = '\0';
    Weapon.Projectile_Texture_Filepath[47] = '\0';
    Weapon.Weapon_Icon_Filepath[42] = '\0';

    Weapon.Weapon_Unholster_Sound_ID = 0; //Use one of the Universal Sounds. -1 means No Sound.
    Weapon.Weapon_Holster_Sound_ID = 0; //Use one of the Universal Sounds.
    Weapon.Weapon_Fire_Sound_ID = 0; //Use one of the Universal Sounds.
    Weapon.Weapon_Reload_Sound_ID = 0; //Use one of the Universal Sounds.

    Weapon.Maximum_Ammo = 10;
    Weapon.Current_Ammo = 10;
    Weapon.Allowed_To_Reload = 0;
    Weapon.Ammo_Mode = TORPEDO_MODE_GRENADE; //Homing, projectile, grenade
    Weapon.Ammo_Lifetime_In_Seconds = 0;
    Weapon.Ammo_Maximum_Distance_In_Meters = 0;
    Weapon.Ammo_Moving_Through_Space_Sound_ID = 0; //Use one of the Universal Sounds. -1 means No Sound.
    Weapon.Ammo_Rolling_Sound_ID = 0; //Use one of the Universal Sounds. -1 means No Sound.
    Weapon.Ammo_Striking_Object_Sound_ID = 0; //Use one of the Universal Sounds. -1 means No Sound.
    Weapon.Ammo_Exploding_Object_Sound_ID = 0; //Use one of the Universal Sounds. -1 means No Sound.

    Weapon.Projectile_Size = 32; //Use one of the Universal Sounds. -1 means No Sound.

    FILE* f = fopen(Filename, "wb");
    fwrite(&Weapon, 1, sizeof(struct _Weapon_Entry),f);
    fclose(f);

    //BMPtoSEI("resources/data/items/test_weapon3/icon.bmp", "resources/data/items/test_weapon3/icon.sei");
}

void Load_Weapon(struct _Engine* Engine, int ID, const char* Filename)
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

    Engine->On_Foot_State->Number_Of_Weapons += 1;
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1] = (struct _Weapon*)calloc(1,sizeof(struct _Weapon));

    for(int j = 0; j < 256; j++)
    {
        Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Name[j] = Buffer[Buffer_Location+j];
    }
    Buffer_Location += 256;

    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Category = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.ID = *(int*)&(Buffer[Buffer_Location+4]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Variant = *(int*)&(Buffer[Buffer_Location+8]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Grasp_Mode = *(int*)&(Buffer[Buffer_Location+12]); //Two-handed, one-handed, hold with one hand, hold with two hands, Lift.
    Buffer_Location += 16;

    for(int j = 0; j < 256; j++)
    {
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Model_Filepath[j] = Buffer[Buffer_Location+j];
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Texture_Filepath[j] = Buffer[Buffer_Location+j+256];
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Projectile_Model_Filepath[j] = Buffer[Buffer_Location+j+512];
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Projectile_Texture_Filepath[j] = Buffer[Buffer_Location+j+768];
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Icon_Filepath[j] = Buffer[Buffer_Location+j+1024];
    }

    Buffer_Location += 1024 + 256;



    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Unholster_Sound_ID = *(int*)&(Buffer[Buffer_Location]); //Use one of the Universal Sounds. -1 means No Sound.
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Holster_Sound_ID = *(int*)&(Buffer[Buffer_Location+4]); //Use one of the Universal Sounds.
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Fire_Sound_ID = *(int*)&(Buffer[Buffer_Location+8]); //Use one of the Universal Sounds.
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Reload_Sound_ID = *(int*)&(Buffer[Buffer_Location+12]); //Use one of the Universal Sounds.

    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Maximum_Ammo = *(int*)&(Buffer[Buffer_Location+16]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Current_Ammo = *(int*)&(Buffer[Buffer_Location+20]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Allowed_To_Reload = *(int*)&(Buffer[Buffer_Location+24]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Mode = *(int*)&(Buffer[Buffer_Location+28]); //Homing, projectile, grenade
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Lifetime_In_Seconds = *(double*)&(Buffer[Buffer_Location+32]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Maximum_Distance_In_Meters = *(double*)&(Buffer[Buffer_Location+40]);
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Moving_Through_Space_Sound_ID = *(int*)&(Buffer[Buffer_Location+48]); //Use one of the Universal Sounds. -1 means No Sound.
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Rolling_Sound_ID = *(int*)&(Buffer[Buffer_Location+56]); //Use one of the Universal Sounds. -1 means No Sound.
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Striking_Object_Sound_ID = *(int*)&(Buffer[Buffer_Location+60]); //Use one of the Universal Sounds. -1 means No Sound.
    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Ammo_Exploding_Object_Sound_ID = *(int*)&(Buffer[Buffer_Location+64]); //Use one of the Universal Sounds. -1 means No Sound.

    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Projectile_Size = *(double*)&(Buffer[Buffer_Location+68]);

    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Model, Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Model_Filepath);
    Load_SE3_Texture(Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Model, Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Texture_Filepath);

    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Projectile_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Projectile_Model, Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Projectile_Model_Filepath);
    Load_SE3_Texture(Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Projectile_Model, Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Projectile_Texture_Filepath);

    Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Icon = Create_Image();
    Load_Image(Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Icon, Engine->On_Foot_State->Weapons[Engine->On_Foot_State->Number_Of_Weapons-1]->Weapon_Entry.Weapon_Icon_Filepath);
}

void Handle_Weapon_Inputs(struct _Engine* Engine, struct _Keypad Keypad, int ID)
{

}

void Process_Weapon(struct _Engine* Engine, int ID)
{

}

void Render_Weapon(struct _Engine* Engine, int ID)
{

}

void Give_Weapon(struct _Engine* Engine, int ID, int Destination_ID)
{
    Engine->On_Foot_State->On_Foot_Player->Number_Of_Weapons += 1;
    Engine->On_Foot_State->On_Foot_Player->Weapons_Available[Destination_ID] = ID;
}

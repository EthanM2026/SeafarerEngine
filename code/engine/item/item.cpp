#include "item.h"

void Write_Weapon(const char* Filename)
{
    struct _Weapon_Entry Weapon;

    memset(Weapon.Name,0,256);
    sprintf(Weapon.Name, "TEST WEAPON");

    Weapon.Grasp_Mode = GRASP_MODE_ONE_HANDED;
    sprintf(Weapon.Weapon_Model_Filepath, "weapon2.se3");
    sprintf(Weapon.Weapon_Texture_Filepath, "rosemary.sei");
    sprintf(Weapon.Weapon_Icon_Filepath, "rosemary.sei");
    Weapon.Weapon_Unholster_Sound_ID = -1;
    Weapon.Weapon_Holster_Sound_ID = -1;
    Weapon.Weapon_Fire_Sound_ID = -1;
    Weapon.Weapon_Reload_Sound_ID = -1;

    Weapon.Current_Ammo = 10;
    Weapon.Allowed_To_Reload = 1;
    Weapon.Reload_Speed = 1.106;
    Weapon.Ammo_Mode = TORPEDO_MODE_KNIFE;
    Weapon.Ammo_Lifetime_In_Seconds = 1;
    Weapon.Ammo_Maximum_Distance_In_Meters = 100;
    Weapon.Ammo_Moving_Through_Space_Sound_ID = -1;
    Weapon.Ammo_Rolling_Sound_ID = -1;
    Weapon.Ammo_Striking_Object_Sound_ID = -1;
    Weapon.Ammo_Exploding_Object_Sound_ID = -1;

    Weapon.Ammo_Collision_Radius = 10;
    Weapon.Ammo_Mass = 0;
    Weapon.Cooldown_Speed = 0.5;

    Weapon.Ammo_Per_Clip = 10;
    Weapon.Current_Stored_Ammo = 17;

    Weapon.Damage = 10;
    Weapon.Status_Effect = -1;

    FILE* f = fopen(Filename, "wb");
    fwrite(&Weapon, 1, sizeof(struct _Weapon_Entry),f);
    fclose(f);
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

    Engine->On_Foot_State->Weapon_Objects[ID] = (struct _Weapon_Object*)calloc(1,sizeof(struct _Weapon_Object));

    for(int j = 0; j < 256; j++)
    {
        Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Name[j] = Buffer[Buffer_Location+j];
    }
    Buffer_Location += 256;

    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Grasp_Mode = *(int*)&(Buffer[Buffer_Location]); //Two-handed, one-handed, hold with one hand, hold with two hands, Lift.
    Buffer_Location += 4;

    for(int j = 0; j < 256; j++)
    {
        Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Model_Filepath[j] = Buffer[Buffer_Location+j];
        Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Texture_Filepath[j] = Buffer[Buffer_Location+j+256];
        Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Icon_Filepath[j] = Buffer[Buffer_Location+j+512];
    }

    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Model, Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Model_Filepath);
    Load_SE3_Texture(Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Model, Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Texture_Filepath);

    Buffer_Location += 256+256+256;

    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Unholster_Sound_ID = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Holster_Sound_ID = *(int*)&(Buffer[Buffer_Location+4]); //Use one of the Universal Sounds.
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Fire_Sound_ID = *(int*)&(Buffer[Buffer_Location+8]); //Use one of the Universal Sounds.
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Weapon_Reload_Sound_ID = *(int*)&(Buffer[Buffer_Location+12]); //Use one of the Universal Sounds.
    Buffer_Location += 16;

    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Current_Ammo = *(int*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Allowed_To_Reload = *(bool*)&(Buffer[Buffer_Location+4]);
    Buffer_Location += 5;


    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Reload_Speed = *(double*)&(Buffer[Buffer_Location]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Mode = *(int*)&(Buffer[Buffer_Location+8]); //Homing, projectile, grenade
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Lifetime_In_Seconds = *(double*)&(Buffer[Buffer_Location+12]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Maximum_Distance_In_Meters = *(double*)&(Buffer[Buffer_Location+20]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Moving_Through_Space_Sound_ID = *(int*)&(Buffer[Buffer_Location+28]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Rolling_Sound_ID = *(int*)&(Buffer[Buffer_Location+32]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Striking_Object_Sound_ID = *(int*)&(Buffer[Buffer_Location+36]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Exploding_Object_Sound_ID = *(int*)&(Buffer[Buffer_Location+40]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Collision_Radius = *(double*)&(Buffer[Buffer_Location+44]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Mass = *(double*)&(Buffer[Buffer_Location+52]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Cooldown_Speed = *(double*)&(Buffer[Buffer_Location+60]);

    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Ammo_Per_Clip = *(int*)&(Buffer[Buffer_Location+68]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Current_Stored_Ammo = *(int*)&(Buffer[Buffer_Location+72]);

    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Damage = *(int*)&(Buffer[Buffer_Location+76]);
    Engine->On_Foot_State->Weapon_Objects[ID]->Weapon_Entry.Status_Effect = *(int*)&(Buffer[Buffer_Location+80]);
}

void Give_Weapon(struct _Engine* Engine, int ID, int Destination_ID)
{

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


void Create_Weapon_Round(struct _Engine* Engine, Vec3 localRight, Vec3 localUp, Vec3 localForward)
{
    Engine->On_Foot_State->Current_Weapon_Round += 1;
    Engine->On_Foot_State->Number_Of_Weapon_Rounds += 1;
    printf("Weapon Rounds: %d\n", Engine->On_Foot_State->Number_Of_Weapon_Rounds);

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round] = (struct _Weapon_Round_Object*)calloc(1,sizeof(struct _Weapon_Round_Object));

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->x = Engine->On_Foot_State->On_Foot_Player->x;
    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->y = Engine->On_Foot_State->On_Foot_Player->y;
    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->z = Engine->On_Foot_State->On_Foot_Player->z+40;

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->localRight = localRight;
    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->localUp = localUp;

    Vec3 localCam;
    localCam.x = Engine->On_Foot_State->Camera_Front_X;
    localCam.y = Engine->On_Foot_State->Camera_Front_Y;
    localCam.z = Engine->On_Foot_State->Camera_Front_Z;

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->localForward = localCam;

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->Velocity = 10;

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->Time_Created = Get_Ticks(Engine->On_Foot_State->Universal_Timer);

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->Dead = false;

    Engine->On_Foot_State->Weapon_Round_Objects[Engine->On_Foot_State->Current_Weapon_Round]->Using_This_Weapon_Round_Model = 1;
}

void Process_Weapon_Round(struct _Engine* Engine, int ID)
{
    if(!Engine->On_Foot_State->Weapon_Round_Objects[ID]->Dead)
    {
        Engine->On_Foot_State->Weapon_Round_Objects[ID]->x += Engine->On_Foot_State->Weapon_Round_Objects[ID]->Velocity * Engine->On_Foot_State->Weapon_Round_Objects[ID]->localForward.x;
        Engine->On_Foot_State->Weapon_Round_Objects[ID]->y += Engine->On_Foot_State->Weapon_Round_Objects[ID]->Velocity * Engine->On_Foot_State->Weapon_Round_Objects[ID]->localForward.y;
        Engine->On_Foot_State->Weapon_Round_Objects[ID]->z += Engine->On_Foot_State->Weapon_Round_Objects[ID]->Velocity * Engine->On_Foot_State->Weapon_Round_Objects[ID]->localForward.z;

        double u = Get_Distance(Engine->On_Foot_State->Weapon_Round_Objects[ID]->x,Engine->On_Foot_State->Weapon_Round_Objects[ID]->y,Engine->On_Foot_State->Weapon_Round_Objects[ID]->z,Engine->On_Foot_State->On_Foot_Player->x,Engine->On_Foot_State->On_Foot_Player->y,Engine->On_Foot_State->On_Foot_Player->z+20);
       // printf("Distance: %f\n", u);
        if(u <= METER_CONVERSION * 1 && !Engine->On_Foot_State->On_Foot_Player->Dead)
        {
            //Engine->On_Foot_State->On_Foot_Player->Current_Health -= 10;
            //printf("You took damage!\n");
            //printf("Your health: %f\n", Engine->On_Foot_State->On_Foot_Player->Current_Health);
            //Engine->On_Foot_State->Weapon_Round_Objects[ID]->Dead = true;

        }

        for(int j = 0; j < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Characters; j++)
        {
            u = Get_Distance(Engine->On_Foot_State->Weapon_Round_Objects[ID]->x,Engine->On_Foot_State->Weapon_Round_Objects[ID]->y,Engine->On_Foot_State->Weapon_Round_Objects[ID]->z,Engine->On_Foot_State->On_Foot_Region.Characters[j]->x,Engine->On_Foot_State->On_Foot_Region.Characters[j]->y,Engine->On_Foot_State->On_Foot_Region.Characters[j]->z+20);
            //printf("Distance: %f\n", u);
            if(u <= METER_CONVERSION * 1 && !Engine->On_Foot_State->On_Foot_Region.Characters[j]->Dead)
            {
                Engine->On_Foot_State->On_Foot_Region.Characters[j]->Current_Health -= 10;
                printf("did damage\n");
                printf("health: %f\n", Engine->On_Foot_State->On_Foot_Region.Characters[j]->Current_Health);
                Engine->On_Foot_State->Weapon_Round_Objects[ID]->Dead = true;

            }
        }

        for(int m = 0; m < Engine->On_Foot_State->On_Foot_Region_File.Number_Of_Ships; m++)
        {
                for(int j = 0; j < Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior_File.Number_Of_Characters; j++)
                {
                    u = Get_Distance(Engine->On_Foot_State->Weapon_Round_Objects[ID]->x,Engine->On_Foot_State->Weapon_Round_Objects[ID]->y,Engine->On_Foot_State->Weapon_Round_Objects[ID]->z,Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior.Characters[j]->x,Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior.Characters[j]->y,Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior.Characters[j]->z);
                 //   printf("Distance: %f\n", u);
                    if(u <= METER_CONVERSION * 1 && !Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior.Characters[j]->Dead)
                    {
                        Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior.Characters[j]->Current_Health -= 10;
                        printf("did damage\n");
                        printf("health: %f\n", Engine->On_Foot_State->Ship_Objects[m]->Ship_Interior.Characters[j]->Current_Health);
                        Engine->On_Foot_State->Weapon_Round_Objects[ID]->Dead = true;
                    }
                }
        }


    double x = Get_Ticks(Engine->On_Foot_State->Universal_Timer);
    if(x > Engine->On_Foot_State->Weapon_Round_Objects[ID]->Time_Created + 5)
    {
        Engine->On_Foot_State->Weapon_Round_Objects[ID]->Dead = true;
    }
    }
}

void Render_Weapon_Round(struct _Engine* Engine, int ID)
{
    if(!Engine->On_Foot_State->Weapon_Round_Objects[ID]->Dead)
    {
           glPushMatrix();
    glTranslatef(Engine->On_Foot_State->Weapon_Round_Objects[ID]->x,Engine->On_Foot_State->Weapon_Round_Objects[ID]->y,Engine->On_Foot_State->Weapon_Round_Objects[ID]->z);
    // Apply the orientation matrix
    float mat[16] = {
        (float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localRight.x, (float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localRight.y, (float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localRight.z, 0,
        (float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localUp.x,    (float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localUp.y,    (float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localUp.z,    0,
        -(float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localForward.x,  -(float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localForward.y,  -(float)Engine->On_Foot_State->Weapon_Round_Objects[ID]->localForward.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);
    Render_SE3_Model(Engine->On_Foot_State->Rosechu,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
    }
}

void Handle_Weapon_Round_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{

}


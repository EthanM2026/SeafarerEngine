#include "new_player.h"


void Create_Character_Card()
{
    struct _Character_Card Character;

//    sprintf(Character.Internal_Name, "Chris");
    Character.Internal_Name[5] = '\0';
    Character.Character_ID = 1;

    Character.Animation_Speed = 1;
    Character.Collision_Type = COLLISION_TYPE_SPHERE;
   // sprintf(Character.Filepath_To_Character_Collision_Geometry, "None");

    Character.Collision_Radius = 10;
    Character.Collision_Height = 0; //for Capsule.

    Character.Starting_Health = 255;
    Character.Starting_Energy = 255;

    Character.Jump_Height = 10;
    Character.Friction = 1;
    Character.Gravity = 1;

    Character.Number_Of_Character_Animation_Filepaths = 1;
    Character.Offset_To_Character_Animation_Filepaths = sizeof(struct _Character_Card);
};
void Load_Character_Card()
{

};

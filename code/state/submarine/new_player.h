#ifndef NEW_PLAYER_H_INCLUDED
#define NEW_PLAYER_H_INCLUDED

#define COLLISION_TYPE_NONE 1
#define COLLISION_TYPE_POINT 2
#define COLLISION_TYPE_SPHERE 3
#define COLLISION_TYPE_CAPSULE 4
#define COLLISION_TYPE_AABB 5
#define COLLISION_TYPE_TRIANGLE_MESH 6

struct _Character_Card
{
    char Internal_Name[256];
    int Character_ID;

    float Animation_Speed;
    int Collision_Type;
    char Filepath_To_Character_Collision_Geometry[256];

    float Collision_Radius;
    float Collision_Height; //for Capsule.

    int Starting_Health;
    int Starting_Energy;

    float Jump_Height;
    float Friction;
    float Gravity;

    int Number_Of_Character_Animation_Filepaths;
    int Offset_To_Character_Animation_Filepaths;
};

void Create_Character_Card();
void Load_Character_Card();

#endif // NEW_PLAYER_H_INCLUDED

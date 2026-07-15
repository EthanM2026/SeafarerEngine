/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define ENVIRONMENT_ON_SHIP 1
#define ENVIRONMENT_UNDERWATER 2
#define ENVIRONMENT_ABOVE_WATER 3
#define ENVIRONMENT_ON_LAND 4
#define ENVIRONMENT_IN_SONICSPACE 5


struct _World
{
    char Name[256];
    unsigned int Compatible_CSS_Version;
    unsigned char Current_Environment;

    int Number_Of_Ships;
    int Number_Of_Underwater_Terrain_Formations;
    int Number_Of_Surface_Wave_Formations;
    int Number_Of_Islands;

    struct _World_Environment_Object* Ships[8192];
    struct _World_Environment_Object* Underwaters[8192];
    struct _World_Environment_Object* Surfaces[8192];
    struct _World_Environment_Object* Islands[8192];
};

struct _World_Environment_Object
{
    struct _Background_Object* ;
    struct _Actor_Object* Actors[32256];
    struct _Track_Object* ;
    struct _SFX_Data_Objects;
    struct _Spawnpoint_Object* ;
    struct _Prop_Object* ;
    struct _Endpoint_Object* ;

    struct _Door_Object* ;
    struct _Conveyor_Belt_Object* ;
    struct _Ladder_Object* ;
    struct _Switche_Object* ;
    struct _Dial_Object* ;
    struct _Pickup_Item_Object* ;
    struct _Fixed_Camera_Boxe_Object* ;

    struct _Conversation_Object* ;
    struct _Conversation_Node_Object* ;
    struct _Conversation_Connection_Object* ;
    struct _Shop_Object* ;
    struct _Stage_Changing_Door_Object* ;
    struct _Vehicle_Entry_Point_Object* ;
    struct _Elevator_Object* ;
    struct _Light_Object* ;

    struct _Computer_Object* ;
    struct _Vehicle_Object* ;
    struct _Computer_Storage_Device_Object* ;

    struct _Broadcast_Tower_Object* ;
    struct _Volcanoe_Object* ;
    struct _Sonic_Black_Hole_Object* ;
    struct _Rainstorm_Object* ;
    struct _Duststorm_Object* ;
    struct _Snowstorm_Object* ;
    struct _Hailstorm_Object* ;
    struct _Earthquake_Object* ;
    struct _Tornado_Object* ;
    struct _Hurricane_Object* ;
    struct _Aurora_Object* ;

    struct _Docking_Station_Object* ;

    struct _Ship_Object* ;

    struct _NPC_Plant_Object* ;
    struct _NPC_Swimming_Creature_Object* ;
    struct _NPC_Terrestrial_Creature_Object* ;
    struct _NPC_Flying_Creature_Object* ;
    struct _NPC_Fungi_Objects;
    struct _NPC_Microbe_Object* ;

    struct _Pools_Of_Liquid_Objects;

    struct _Mirror_Object* ;
    struct _Fire_Object* ;

    struct _Electrical_Arc_Object* ;
    struct _Electrical_Spark_Object* ;

    struct _Steam_Plume_Object* ;
    struct _Smoke_Cloud_Object* ;
    struct _Fog_Cloud_Object* ;
};


struct _World_File_Header_Dependent_Objects
{

}

#endif // WORLD_H_INCLUDED

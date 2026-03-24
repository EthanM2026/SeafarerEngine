/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

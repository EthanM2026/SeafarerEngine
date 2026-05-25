/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

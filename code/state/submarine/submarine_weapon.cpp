/*
Copyright (c) 2024 Ethan D. Carbonate.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "submarine.h"
#include "math.h"
#include "submarine_weapon.h"

struct _Submarine_Torpedo_Registry* Create_Torpedo_Registry()
{
    struct _Submarine_Torpedo_Registry* I = (struct _Submarine_Torpedo_Registry*) malloc(sizeof (struct _Submarine_Torpedo_Registry));
    I->Torpedo = Create_MD2_Model();
    Load_MD2_Model(I->Torpedo, "resources/models/sonichu.dat");
    Load_MD2_Texture(I->Torpedo, "torpedo.bmp");
    return I;
};

void Add_Torpedo(struct _Engine* Engine)
{
    Engine->Submarine_State->Torpedo_Registry->Current_Torpedo += 1;
    Engine->Submarine_State->Torpedo_Registry->Number_Of_Torpedoes += 1;
    if(Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo] == NULL)
    {
        Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo] = (struct _Submarine_Torpedo*)malloc(sizeof(struct _Submarine_Torpedo));
        Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo]->x = Engine->Submarine_State->Player_Submarine->x;

        Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo]->y = Engine->Submarine_State->Player_Submarine->y;

        Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo]->z = Engine->Submarine_State->Player_Submarine->z;



        Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo]->dx = Engine->Submarine_State->Player_Submarine->Throttle * Engine->Submarine_State->Player_Submarine->Forward_Vector->i;

        Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo]->dy = Engine->Submarine_State->Player_Submarine->Throttle * Engine->Submarine_State->Player_Submarine->Forward_Vector->j;

        //Engine->Submarine_State->Torpedo_Registry->Torpedoes[Engine->Submarine_State->Torpedo_Registry->Current_Torpedo]->dz = Engine->Submarine_State->Player_Submarine->dz;


    }
};
void Set_Torpedo_Target(struct _Engine* Engine)
{

};
void Process_Torpedo(struct _Engine* Engine)
{
    printf("Torpedo X: %f\n",Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->x);
    printf("Torpedo Y: %f\n",Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->y);
    printf("Torpedo Z: %f\n",Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->z);

    Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->x+=Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->dx;

    Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->y+= 10;

    Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->z+=Engine->Submarine_State->Torpedo_Registry->Torpedoes[0]->dz;
};
void Render_Torpedo(struct _Engine* Engine, unsigned char This_Torpedo)
{
    Render_MD2_Model(Engine->Submarine_State->Torpedo_Registry->Torpedo,0,Engine->Submarine_State->Torpedo_Registry->Torpedoes[This_Torpedo]->x,Engine->Submarine_State->Torpedo_Registry->Torpedoes[This_Torpedo]->y,Engine->Submarine_State->Torpedo_Registry->Torpedoes[This_Torpedo]->z,0,0,0,false,1,1,1);
};

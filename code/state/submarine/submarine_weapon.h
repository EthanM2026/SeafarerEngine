/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SUBMARINE_WEAPON_H_INCLUDED
#define SUBMARINE_WEAPON_H_INCLUDED

struct _Submarine_Torpedo
{
    struct _MD2_Model* Torpedo;

    bool Dead = false;
    bool Spawned = false;

    float x2;
    float y2;
    float z2;
    float Throttle;

    float dx;
    float dy;
    float dz;

    float x;
    float y;
    float z;

};

struct _Submarine_Torpedo_Registry
{
    struct _MD2_Model* Torpedo;
    struct _Submarine_Torpedo* Torpedoes[256];
    unsigned char Number_Of_Torpedoes;
    char Current_Torpedo;
};


struct _Submarine_Torpedo_Registry* Create_Torpedo_Registry();

void Add_Torpedo(struct _Engine* Engine);
void Set_Torpedo_Target(struct _Engine* Engine);
void Process_Torpedo(struct _Engine* Engine);
void Render_Torpedo(struct _Engine* Engine, unsigned char This_Torpedo);
#endif // SUBMARINE_WEAPON_H_INCLUDED

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

#ifndef NPC_SUBMARINE_H_INCLUDED
#define NPC_SUBMARINE_H_INCLUDED
#include "../../engine/engine.h"
#include "../../engine/graphics/new_model.h"
//This is the NEW PLATFORMING STATE.
#include "../../engine/graphics/image.h"
#include "../../engine/camera/camera.h"
#include "../../engine/physics/vector.h"

#define LOOK_FORWARD 1
#define LOOK_UP 2
#define LOOK_DOWN 3

struct _NPC_Submarine
{
    struct _MD2_Model* Submarine;

    struct _S_Vector* Right_Vector;
    struct _S_Vector* Forward_Vector;
    struct _S_Vector* Up_Vector;
    float Throttle;

    unsigned char Look_Direction;

    float Camera_Elevator_Z = 0;

    float x;
    float y;
    float z;

    double Always_Pitch_Angle;

    double Pitch_Angle;
    double Roll_Angle;
    double Yaw_Angle;

    double Camera_Z_Rotation;
};

struct _NPC_Submarine* Create_NPC();
void Initialize_NPC_Submarine(struct _Engine* Engine, unsigned int ID);

void Render_NPC(struct _Engine* Engine, unsigned int ID);
void Process_NPC(struct _Engine* Engine, unsigned int ID);

void Calculate_NPC_Pitch(struct _Engine* Engine, double Angle, unsigned int ID);
void Calculate_NPC_Roll(struct _Engine* Engine, double Angle, unsigned int ID);
void Calculate_NPC_Yaw(struct _Engine* Engine, double Angle, unsigned int ID);


#endif // NPC_SUBMARINE_H_INCLUDED

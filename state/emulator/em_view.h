#ifndef EM_VIEW_H_INCLUDED
#define EM_VIEW_H_INCLUDED

#include "../../engine/emulator/computer.h"

//BIG-ENDIAN!

struct _Emulator_Viewer_State
{
    struct _Seat_Object* Seat;
};

struct _Emulator_Viewer_State* Create_Emulator_Viewer_State();
void Destroy_Emulator_Viewer_State(struct _Engine* Engine);
void Initialize_Emulator_Viewer_State(struct _Engine* Engine);

void Render_Emulator_Viewer_State(struct _Engine* Engine);
void Input_Emulator_Viewer_State(struct _Engine* Engine, struct _Keypad Keypad);
void Process_Emulator_Viewer_State(struct _Engine* Engine, struct _Audio_Chip* Audio_Chip);

#endif // EM_VIEW_H_INCLUDED

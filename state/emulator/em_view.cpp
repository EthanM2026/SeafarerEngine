#include "em_view.h"


struct _Emulator_Viewer_State* Create_Emulator_Viewer_State()
{
    struct _Emulator_Viewer_State* I = (struct _Emulator_Viewer_State*) calloc(1,sizeof (struct _Emulator_Viewer_State));
    return I;
};

void Destroy_Emulator_Viewer_State(struct _Engine* Engine)
{

};

void Initialize_Emulator_Viewer_State(struct _Engine* Engine)
{
    Engine->Emulator_Viewer_State->Seat = (struct _Seat_Object*)calloc(1,sizeof(struct _Seat_Object));
    Engine->Emulator_Viewer_State->Seat->New_Computer = Create_New_Computer();
    Initialize_New_Computer(Engine, Engine->Emulator_Viewer_State->Seat);
    //Load_Program(Engine->Emulator_Viewer_State->Seat, "test.rom");
    Turn_Computer_On(Engine, Engine->Emulator_Viewer_State->Seat);
};


void Render_Emulator_Viewer_State(struct _Engine* Engine)
{
    float _x,_y;
    _x = 0;
    _y = 0;
    Engine->Emulator_Viewer_State->Seat->New_Computer->Width = 256;
    Engine->Emulator_Viewer_State->Seat->New_Computer->Height = 256;


    Render_New_Computer(Engine, Engine->Emulator_Viewer_State->Seat);

    glBindTexture(GL_TEXTURE_2D, Engine->Emulator_Viewer_State->Seat->New_Computer->Texture_ID);

    float Vertices[] = {_x, _y, _x + Engine->Emulator_Viewer_State->Seat->New_Computer->Width, _y, _x + Engine->Emulator_Viewer_State->Seat->New_Computer->Width, _y + Engine->Emulator_Viewer_State->Seat->New_Computer->Height, _x, _y + Engine->Emulator_Viewer_State->Seat->New_Computer->Height};
    float Texture_Vertices[] = {0, 0, 1, 0, 1, -1, 0, -1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(2, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
};

void Input_Emulator_Viewer_State(struct _Engine* Engine, struct _Keypad Keypad)
{
    Handle_New_Computer_Inputs(Engine, Engine->Emulator_Viewer_State->Seat, Keypad);

    if(Keypad.Keyboard_A)
    {
        Turn_Computer_On(Engine, Engine->Emulator_Viewer_State->Seat);
    }
};

void Process_Emulator_Viewer_State(struct _Engine* Engine, struct _Audio_Chip* Audio_Chip)
{
    Process_New_Computer_Cycles(Engine, Engine->Emulator_Viewer_State->Seat, 50, Audio_Chip);
};

#ifndef COMPUTER_H_INCLUDED
#define COMPUTER_H_INCLUDED
#include "../engine.h"
#include "../graphics/image.h"

#define STACK_START 65535
#define STACK_END 65280

#define OPC_HLT 0x01
#define OPC_NOP 0x00
#define OPC_CLS 0x02
#define OPC_FLS 0x03
#define OPC_INT 0x04

#define OPC_MOV_R0_R1 0x05
#define OPC_MOV_R1_R0 0x06

#define OPC_MOV_R0_INT_VALUE 0x07
#define OPC_MOV_R1_INT_VALUE 0x08

#define OPC_MOV_R0_VALUE_AT_ADDRESS 0x09
#define OPC_MOV_R1_VALUE_AT_ADDRESS 0x0A

#define OPC_PUSH_R0 0x0B
#define OPC_PUSH_R1 0x0C

#define OPC_POP_R0 0x0D
#define OPC_POP_R1 0x0E

#define OPC_PUSH_VALUE_AT_ADDRESS 0x0F

#define OPC_MOV_VALUE_AT_ADDRESS_R0 0x10
#define OPC_MOV_VALUE_AT_ADDRESS_R1 0x11

#define OPC_ADD_R0_R1 0x12
#define OPC_ADD_R1_R0 0x13

#define OPC_SUB_R0_R1 0x14
#define OPC_SUB_R1_R0 0x15

#define OPC_MUL_R0_R1 0x16
#define OPC_MUL_R1_R0 0x17

#define OPC_DIV_R0_R1 0x18
#define OPC_DIV_R1_R0 0x19

#define OPC_COLORMODE 0x1A
#define OPC_MONOCHROMEMODE 0x1B

#define OPC_JMP 0x1C
#define OPC_CALL 0x1D
#define OPC_RETURN 0x1E

#define OPC_CMP_R0_R1 0x1F

#define OPC_JNE 0x20
#define OPC_JE 0x21
#define OPC_JG 0x22
#define OPC_JL 0x23
#define OPC_JGE 0x24
#define OPC_JLE 0x25

#define OPC_CNE 0x26
#define OPC_CE 0x27
#define OPC_CG 0x28
#define OPC_CL 0x29
#define OPC_CGE 0x2A
#define OPC_CLE 0x2B

#define OPC_IN_DPAD_UP_R0 0x2C
#define OPC_IN_DPAD_DOWN_R0 0x2D
#define OPC_IN_DPAD_LEFT_R0 0x2E
#define OPC_IN_DPAD_RIGHT_R0 0x2F

#define OPC_IN_A_BUTTON_R0 0x30
#define OPC_IN_B_BUTTON_R0 0x31

#define OPC_IN_KBD_R0 0x32
#define OPC_IN_MOUSE_X_R0 0x33
#define OPC_IN_MOUSE_Y_R0 0x34
#define OPC_IN_MOUSE_CLICK_R0 0x35

#define OPC_JMP_STACK 0x36

#define OPC_AND 0x37
#define OPC_OR 0x38
#define OPC_XOR 0x39

#define INT_SHUTDOWN_COMPUTER 0x00
#define INT_RESTART_COMPUTER 0x01
#define INT_RENDER_PIXEL 0x02
#define INT_RENDER_LINE 0x03
#define INT_RENDER_IMAGE 0x04
#define INT_RENDER_TILEMAP 0x05
#define INT_LOAD_RAM_BANK 0x06
#define INT_SWITCH_RAM_BANK 0x07

struct _TILEMAP
{
    char Name[16];
    unsigned char Bank;
    unsigned short Address;
    unsigned char Cell_Width;
    unsigned char Cell_Height;
};

struct _DBLOCK
{
    char Name[16];
    unsigned char Bank;
    unsigned short Address;
};

struct _VAR
{
    char Name[16];
    unsigned char Bank;
    unsigned short Address;
    unsigned short Value;
};

struct _LABEL
{
    char Name[16];
    unsigned char Bank;
    unsigned short Address;
};

struct _RAM_Bank
{
    unsigned char Bank[65536];
};

struct _New_Processor
{
    uint16_t Register_Zero;
    uint16_t Register_One;
    uint16_t Instruction_Pointer;
    uint16_t Stack_Pointer;

    bool Signed_Int_Mode;
    bool Float_Mode;

    bool Zero_Flag;
    bool Carry_Flag;
    bool Sign_Flag;
    bool Overflow_Flag;
    bool Parity_Flag;
    bool Auxiliary_Carry_Flag;
    bool Direction_Flag;
    bool Maskable_Interrupt_Flag;
    bool Trap_Flag;

    bool Equals_Flag;
    bool Less_Flag;
    bool Greater_Flag;

    bool Processor_Halted;

    bool Division_Error;
};

struct _New_Computer
{
    unsigned char Red_Color_Balance;
    unsigned char Green_Color_Balance;
    unsigned char Blue_Color_Balance;

    bool Color_Mode;

    bool Power_State;

    struct _New_Processor Processor;
    unsigned char Current_ROM_Bank;
    unsigned char Current_RAM_Bank;
    struct _RAM_Bank ROM_Memory_Zero[256];
    struct _RAM_Bank ROM_Memory_One[256];
    struct _RAM_Bank RAM[256];

    unsigned char Controller_D_Pad_RAM[4];
    unsigned char Controller_Button_RAM[2];

    unsigned char Keyboard_RAM[256];
    unsigned char Mouse_RAM[3];

    unsigned char* Front_Video_Buffer;
    unsigned char* Back_Video_Buffer;

    GLuint Texture_ID;
    int RGBA_Image_Size;
    unsigned char Texture_Made;
    int x;
    int y;
    int Width;
    int Height;
    unsigned char* RGB_Canvas;
    unsigned char* RGBA_Canvas;
    float* Vertices;
    float* Texture_Vertices;
};

struct _New_Compiler
{
    unsigned char Current_ROM_Bank;
    unsigned short Current_ROM_Address;
    struct _RAM_Bank ROM_Memory_Zero[256];
};

struct _New_Computer* Create_New_Computer();
void Initialize_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat);

void Write_ROM_Memory_To_Program(const char* Filename);
void Write_Opcode(struct _New_Compiler* New_Compiler, unsigned char Code, unsigned char Bank, unsigned short Address, unsigned char Size, unsigned char Argument_One, unsigned char Argument_Two,unsigned char Argument_Three);
void Write_Program_To_ROM_Memory(const char* Filename, struct _Seat_Object* Seat);
bool Compile_AQ_BASIC_Program(const char* Filename, const char* Output);
void BMPtoSMI(const char* Filename, const char* Output);

void Load_Program(struct _Seat_Object* Seat, const char* Filename);

void Process_New_Computer_Cycles(struct _Engine* Engine, struct _Seat_Object* Seat, int Cycles);
void Process_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat);
void Handle_New_Computer_Inputs(struct _Engine* Engine, struct _Seat_Object* Seat, struct _Keypad Keypad);
void Render_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_NOP(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_HLT(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_CLS(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_FLS(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_INT(struct _Engine* Engine, struct _Seat_Object* Seat);


void Process_Opcode_MOV_R0_INT_VALUE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_PUSH_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_POP_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_MOV_R1_INT_VALUE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_PUSH_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_POP_R1(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_MOV_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_MOV_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_INT_Load_RAM_Bank(struct _Engine* Engine, struct _Seat_Object* Seat);

void Turn_Computer_Off(struct _Engine* Engine, struct _Seat_Object* Seat);
void Turn_Computer_On(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_INT_Shutdown_Computer(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_INT_Restart_Computer(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_INT_Render_Pixel(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_INT_Render_Line(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_INT_Render_Image(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_MOV_VALUE_AT_ADDRESS_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_MOV_VALUE_AT_ADDRESS_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_MOV_R0_VALUE_AT_ADDRESS(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_MOV_R1_VALUE_AT_ADDRESS(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_ADD_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_ADD_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_SUB_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_SUB_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_MUL_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_MUL_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_DIV_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_DIV_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_COLORMODE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_MONOCHROMEMODE(struct _Engine* Engine, struct _Seat_Object* Seat);

bool Handle_Aqueous_Basic_Text_GOTO(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);

bool Handle_Aqueous_Basic_Text_GOIFCTRLUP(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);

bool Handle_Aqueous_Basic_Text_GOIFCTRLDOWN(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);

bool Handle_Aqueous_Basic_Text_GOIFCTRLLEFT(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);
bool Handle_Aqueous_Basic_Text_GOIFCTRLRIGHT(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);

bool Handle_Aqueous_Basic_Text_GOIFCTRLA(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);
bool Handle_Aqueous_Basic_Text_GOIFCTRLB(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);

bool Handle_Aqueous_Basic_Text_ADDVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value);

bool Handle_Aqueous_Basic_Text_SUBVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value);

bool Handle_Aqueous_Basic_Text_SETVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value);
bool Handle_Aqueous_Basic_Text_SETVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo);

bool Handle_Aqueous_Basic_Text_JUMPEX(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank_Bank,unsigned short Bank_Address,unsigned char Address_Bank,unsigned short Address_Address);

bool Handle_Aqueous_Basic_Text_CLS(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address);
bool Handle_Aqueous_Basic_Text_FLS(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address);
bool Handle_Aqueous_Basic_Text_RENDER(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned short xAddress, unsigned char yBank, unsigned short yAddress);

void Process_Opcode_JMP(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_CALL(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_RETURN(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_CMP_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat);

void Process_Opcode_JNE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_JE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_JG(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_JL(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_JGE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_JLE(struct _Engine* Engine, struct _Seat_Object* Seat);


void Process_Opcode_CNE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_CE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_CG(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_CL(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_CGE(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_CLE(struct _Engine* Engine, struct _Seat_Object* Seat);


bool Handle_Aqueous_Basic_Text_GOSUB(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address);

bool Handle_Aqueous_Basic_Text_RETURN(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address);

bool Handle_Aqueous_Basic_Text_COLORMODE(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address);
bool Handle_Aqueous_Basic_Text_MONOCHROMEMODE(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address);

void Process_Opcode_IN_DPAD_UP_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_DPAD_DOWN_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_DPAD_LEFT_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_DPAD_RIGHT_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_A_BUTTON_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_B_BUTTON_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_KBD_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_MOUSE_X_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_MOUSE_Y_R0(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_IN_MOUSE_CLICK_R0(struct _Engine* Engine, struct _Seat_Object* Seat);

bool Handle_Aqueous_Basic_Text_GOIFVAREQVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned char xAddress, unsigned char yBank, unsigned char yAddress);
bool Handle_Aqueous_Basic_Text_GOIFVARLVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned char xAddress, unsigned char yBank, unsigned char yAddress);
bool Handle_Aqueous_Basic_Text_GOIFVARGVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned char xAddress, unsigned char yBank, unsigned char yAddress);


void Process_Opcode_AND(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_OR(struct _Engine* Engine, struct _Seat_Object* Seat);
void Process_Opcode_XOR(struct _Engine* Engine, struct _Seat_Object* Seat);

bool Handle_Aqueous_Basic_Text_RENTILEMAP(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char Bank,unsigned short Address,unsigned char XBank,unsigned short XAddress,unsigned char YBank,unsigned short YAddress, unsigned char EncodingBank,unsigned short EncodingAddress,unsigned char EncodingLengthBank, unsigned short EncodingLengthAddress,unsigned char TPRBank,unsigned short TPRAddress);

#endif // COMPUTER_H_INCLUDED

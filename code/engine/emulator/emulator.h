/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef EMULATOR_H_INCLUDED
#define EMULATOR_H_INCLUDED
#include "../engine.h"
#include "../graphics/image.h"
#include "opcodes.h"

#define VIDEO_MODE_MONOCHROME 0x01 //7-bit color with 1 transparency bit.
#define VIDEO_MODE_COLOR 0x02 //6-bit color with 2 transparency bits

#define HARDWARE_OFFSET_VIDEO_RAM 0x0001
#define HARDWARE_OFFSET_HDD 0x0002
#define HARDWARE_OFFSET_REMOVABLE_DISK 0x0003
#define HARDWARE_OFFSET_SENSOR 0x0004
#define HARDWARE_OFFSET_PERIPHERAL 0x0005
#define HARDWARE_OFFSET_KEYBOARD 0x0006
#define HARDWARE_OFFSET_LEFT_KEYBOARD 0x0007
#define HARDWARE_OFFSET_RIGHT_KEYBOARD 0x0008
#define HARDWARE_OFFSET_HEAD_INDICATORS 0x0009

#define HARDWARE_OFFSET_MOUSE 0x000A
#define HARDWARE_OFFSET_CONTROLLER 0x000B

#define HARDWARE_OFFSET_POWER_SUPPLY 0x000C
#define HARDWARE_OFFSET_FANS 0x000D
#define HARDWARE_OFFSET_NETWORK 0x000E

#define FILE_TYPE_EXECUTABLE 1
#define FILE_TYPE_FOLDER 1
#define FILE_TYPE_FILE 1


//Filename	8 bytes	Name of the file (padded with spaces if shorter)
//Extension	3 bytes	File extension (e.g., TXT, DOC)
//Attributes	1 byte	Flags like read-only, hidden, system, volume label, subdirectory, archive
//Reserved	10 bytes	Reserved for system use or timestamps
//Time	2 bytes	Creation/modification time
//Date	2 bytes	Creation/modification date
//Starting Cluster	2 bytes	Number of the first cluster of the file in the data area
//File Size	4 bytes	Size of the file in bytes



struct _Standard_File_System_File_Or_Directory
{
    unsigned short Name[4*16];
    unsigned short Extension_Name[4*16];
    unsigned char Type; //Executable File, Folder, or File
    unsigned char On_This_Bank;
    unsigned short At_This_Offset;
    unsigned short Size;
    unsigned short Year_Created;
    unsigned char Month_Created;
    unsigned char Day_Created;
    unsigned short Time_Created_In_Seconds;
};


struct _Standard_File_System_Root_Directory
{
    unsigned char Number_Of_Files;
    struct _Standard_File_System_File_Or_Directory Files[256];
};

struct _SixtyFourK_Bank
{
    unsigned char Bank[65536];
};

struct _Memory_Cartridge
{
    unsigned char Number_Of_Banks; //Maximum Cart Size: 16MB.
    unsigned char Offset_To_Banks;
};

struct _x80_Processor
{
    uint16_t Register_Zero;
    uint16_t Register_One;

    uint16_t Stack_Pointer;
    uint16_t Base_Pointer;

    uint16_t Old_Stack_Pointer;
    uint16_t Old_Base_Pointer;

    uint16_t Instruction_Pointer;
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
};

struct _Computer
{
    float Red_Level;
    float Blue_Level;
    float Green_Level;
    struct _x80_Processor Processor;
    struct _SixtyFourK_Bank RAM[256];

    unsigned char Video_RAM_Front_Buffer[65536];
    unsigned char Video_RAM_Back_Buffer[65536];

    unsigned char True_Video_Screen[256*256*4];

    unsigned char HDD_Slot_Status;
    unsigned char Removable_Slot_Status;

    unsigned char Sensor_Link_Port_Status;
    unsigned char Peripheral_Link_Port_Status;
    unsigned char Network_Link_Port_Status;

    unsigned char Monitor_Link_Port_Status;

    unsigned char Keyboard_Link_Port_Status;
    unsigned char Mouse_Link_Port_Status;
    unsigned char Controller_Link_Port_Status;

    unsigned char Power_Status;
    unsigned char Power_Supply_Status;
    unsigned char Fan_Status;

    unsigned char Controller_RAM[5]; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    unsigned char Keyboard_RAM[32]; //256 keys, 1 bit per key

    unsigned char Left_Keyboard_RAM[32]; //256 keys, 1 bit per key
    unsigned char Right_Keyboard_RAM[32]; //256 keys, 1 bit per key

    unsigned char External_Lights_RAM[32];
    unsigned char External_Inputs_RAM[32];

    unsigned char Mouse_RAM[3];



    unsigned char Sensor_Link_Port_RAM[1536]; //256 chars, 6 bytes per char
    unsigned char Peripheral_Link_Port_RAM[1536];
    unsigned char Network_Link_Port_RAM[1536];

    unsigned char CPU_Is_Running;

    unsigned char Color_Mode;
    unsigned char Background_Color;
    unsigned char Current_RAM_Bank;

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

struct _Computer* Create_Computer();
void Initialize_Computer(struct _Computer* Computer);

void Turn_On_Computer(struct _Computer* Computer);
void Turn_Off_Computer(struct _Computer* Computer);
void Load_Program_Into_RAM_And_Start(struct _Computer* Computer, const char* Filename);
void Load_BIOS_Into_RAM_And_Start(struct _Computer* Computer, const char* Filename);



void Process_Computer(struct _Computer* Computer, int Maximum_Cycles, struct _Keypad Keypad);
void Run_A_Processing_Cycle(struct _Computer* Computer, struct _Keypad Keypad);
void Update_Computer_Inputs(struct _Computer* Computer, struct _Keypad Keypad);
void Update_Computer_Video_Card(struct _Computer* Computer);
void Render_Computer(struct _Computer* Computer, float x, float y);

void Render_Computer_Vertices(struct _Computer* Computer, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);

void Create_Cartridge(unsigned char Number_Of_Banks, const char* Output_Filename);
void Convert_Code_To_Cartridge_Data(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset, const char* Filename);
void Convert_BMP_To_Cartridge_Data(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset, const char* Filename);
void Convert_Data_To_Cartridge_Data(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset, const char* Filename);


void Insert_Standard_Filesytem(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset);
void Write_Filesystem_Entry(unsigned short Name[64],unsigned short Extension_Name[64],unsigned char Type,unsigned char On_This_Bank,unsigned short At_This_Offset,unsigned short Size,unsigned short Year_Created,unsigned char Month_Created,unsigned char Day_Created,unsigned short Time_Created_In_Seconds);





void RUN_NOP(struct _Computer* Computer);

void RUN_MOV_REG_IMM(struct _Computer* Computer);

void RUN_MOV_REG_DIR(struct _Computer* Computer);

void RUN_MOV_REG_IND(struct _Computer* Computer);

void RUN_PUSH(struct _Computer* Computer);

void RUN_POP(struct _Computer* Computer);

void RUN_CALL(struct _Computer* Computer);

void RUN_RET(struct _Computer* Computer);

void RUN_CMP_IMM(struct _Computer* Computer);

void RUN_JZ(struct _Computer* Computer);

void RUN_JNZ(struct _Computer* Computer);

void RUN_HALT(struct _Computer* Computer);


void RUN_JE(struct _Computer* Computer);

void RUN_JNE(struct _Computer* Computer);

void RUN_JGE(struct _Computer* Computer);

void RUN_JLE(struct _Computer* Computer);

void RUN_JG(struct _Computer* Computer);

void RUN_JL(struct _Computer* Computer);


void RUN_JC(struct _Computer* Computer);


void RUN_LODSB(struct _Computer* Computer);

void RUN_LODSW(struct _Computer* Computer);

void RUN_STOSB(struct _Computer* Computer);

void RUN_STOSW(struct _Computer* Computer);


void RUN_JMP(struct _Computer* Computer);


void RUN_AND(struct _Computer* Computer);

void RUN_OR(struct _Computer* Computer);

void RUN_XOR(struct _Computer* Computer);

void RUN_NAND(struct _Computer* Computer);

void RUN_NOR(struct _Computer* Computer);

void RUN_XNOR(struct _Computer* Computer);


void RUN_ADD_REG_IMM(struct _Computer* Computer);

void RUN_ADD_REG_DIR(struct _Computer* Computer);

void RUN_ADD_REG_IND(struct _Computer* Computer);


void RUN_SUB_REG_IMM(struct _Computer* Computer);

void RUN_SUB_REG_DIR(struct _Computer* Computer);

void RUN_SUB_REG_IND(struct _Computer* Computer);


void RUN_MUL_REG_IMM(struct _Computer* Computer);

void RUN_MUL_REG_DIR(struct _Computer* Computer);

void RUN_MUL_REG_IND(struct _Computer* Computer);


void RUN_DIV_REG_IMM(struct _Computer* Computer);

void RUN_DIV_REG_DIR(struct _Computer* Computer);

void RUN_DIV_REG_IND(struct _Computer* Computer);








void RUN_PWR_REG_IMM(struct _Computer* Computer);

void RUN_PWR_REG_DIR(struct _Computer* Computer);

void RUN_PWR_REG_IND(struct _Computer* Computer);


void RUN_SQR_REG_IMM(struct _Computer* Computer);

void RUN_SQR_REG_DIR(struct _Computer* Computer);

void RUN_SQR_REG_IND(struct _Computer* Computer);


void RUN_IN(struct _Computer* Computer);

void RUN_OUT(struct _Computer* Computer);


void RUN_REP(struct _Computer* Computer);


//FLAGS
void RUN_SET_ZERO_FLAG(struct _Computer* Computer);

void RUN_SET_CARRY_FLAG(struct _Computer* Computer);

void RUN_SET_SIGN_FLAG(struct _Computer* Computer);

void RUN_SET_OVERFLOW_FLAG(struct _Computer* Computer);

void RUN_SET_PARITY_FLAG(struct _Computer* Computer);

void RUN_SET_AUXILIARY_CARRY_FLAG(struct _Computer* Computer);


void RUN_SET_DIRECTION_FLAG(struct _Computer* Computer);

void RUN_SET_MASKABLE_INTERRUPT_FLAG(struct _Computer* Computer);

void RUN_SET_TRAP_FLAG(struct _Computer* Computer);




void RUN_FUNCTION_ERROR_HANDLER(struct _Computer* Computer);


void RUN_FUNCTION_GRAPHICS_RENDER_POINT(struct _Computer* Computer);

void RUN_FUNCTION_GRAPHICS_RENDER_LINE(struct _Computer* Computer);

void RUN_FUNCTION_GRAPHICS_RENDER_TRIANGLE(struct _Computer* Computer);


void RUN_FUNCTION_GRAPHICS_RENDER_SPRITE(struct _Computer* Computer);

void RUN_FUNCTION_GRAPHICS_RENDER_ANIMATED_SPRITE(struct _Computer* Computer);


void RUN_FUNCTION_SET_TIMER(struct _Computer* Computer);

void RUN_FUNCTION_PAUSE_TIMER(struct _Computer* Computer);

void RUN_FUNCTION_CLEAR_TIMER(struct _Computer* Computer);


void RUN_FUNCTION_SET_CYCLICAL_TIMER(struct _Computer* Computer);

void RUN_FUNCTION_PAUSE_CYCLICAL_TIMER(struct _Computer* Computer);

void RUN_FUNCTION_CLEAR_CYCLICAL_TIMER(struct _Computer* Computer);



void RUN_FUNCTION_READ_TIME(struct _Computer* Computer);

void RUN_FUNCTION_READ_DATE(struct _Computer* Computer);


void RUN_FUNCTION_SET_TIME(struct _Computer* Computer);

void RUN_FUNCTION_SET_DATE(struct _Computer* Computer);


void RUN_FUNCTION_RENDER_TEXT(struct _Computer* Computer);

void RUN_FUNCTION_RENDER_VALUE(struct _Computer* Computer);


void RUN_FUNCTION_INPUT_TEXT(struct _Computer* Computer);

void RUN_FUNCTION_INPUT_VALUE(struct _Computer* Computer);


void RUN_FUNCTION_PLAY_BEEP(struct _Computer* Computer);

void RUN_FUNCTION_PLAY_AUDIO(struct _Computer* Computer);


void RUN_FUNCTION_TEST_AABB_AABB_COLLISION(struct _Computer* Computer);

void RUN_FUNCTION_TEST_AABB_CIRCLE_COLLISION(struct _Computer* Computer);

void RUN_FUNCTION_TEST_AABB_RAY_COLLISION(struct _Computer* Computer);

void RUN_FUNCTION_TEST_AABB_TRIANGLE_COLLISION(struct _Computer* Computer);


void RUN_FUNCTION_TEST_CIRCLE_CIRCLE_COLLISION(struct _Computer* Computer);

void RUN_FUNCTION_TEST_CIRCLE_RAY_COLLISION(struct _Computer* Computer);

void RUN_FUNCTION_TEST_CIRCLE_TRIANGLE_COLLISION(struct _Computer* Computer);


void RUN_FUNCTION_TEST_RAY_RAY_COLLISION(struct _Computer* Computer);

void RUN_FUNCTION_TEST_RAY_TRIANGLE_COLLISION(struct _Computer* Computer);


void RUN_FUNCTION_TEST_TRIANGLE_TRIANGLE_COLLISION(struct _Computer* Computer);


void RUN_FUNCTION_TEST_IF_OFF_SCREEN(struct _Computer* Computer);



void RUN_FUNCTION_SET_SCREEN_MODE(struct _Computer* Computer);

void RUN_FUNCTION_SET_CURSOR_SHAPE(struct _Computer* Computer);

void RUN_FUNCTION_SET_CURSOR_POSITION(struct _Computer* Computer);


void RUN_FUNCTION_SCROLL_SCREEN(struct _Computer* Computer);

void RUN_FUNCTION_CLEAR_SCREEN(struct _Computer* Computer);


void RUN_FUNCTION_READ_CHARACTER_AT_CURSOR(struct _Computer* Computer);

void RUN_FUNCTION_WRITE_CHARACTER_AT_CURSOR(struct _Computer* Computer);


void RUN_FUNCTION_GET_VIDEO_MODE(struct _Computer* Computer);


void RUN_FUNCTION_GET_EQUIPMENT_LIST(struct _Computer* Computer);


void RUN_FUNCTION_GET_RAM_SIZE(struct _Computer* Computer);


void RUN_FUNCTION_SET_BACKGROUND_COLOR(struct _Computer* Computer);





void RUN_FUNCTION_RESET_DISK_DRIVE(struct _Computer* Computer);

void RUN_FUNCTION_CHECK_DISK_DRIVE_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_READ_FROM_DISK_DRIVE(struct _Computer* Computer);

void RUN_FUNCTION_WRITE_TO_DISK_DRIVE(struct _Computer* Computer);

void RUN_FUNCTION_CHECK_DISK_DRIVE_FOR_ERRORS(struct _Computer* Computer);

void RUN_FUNCTION_FORMAT_DISK_DRIVE(struct _Computer* Computer);

void RUN_FUNCTION_GET_DISK_DRIVE_INFORMATION(struct _Computer* Computer);




void RUN_FUNCTION_SEEK_TO_SPECIFIED_TRACK(struct _Computer* Computer);

void RUN_FUNCTION_GET_DRIVE_TYPE(struct _Computer* Computer);

void RUN_FUNCTION_GET_FLOPPY_DRIVE_MEDIA_CHANGE_STATUS(struct _Computer* Computer);


void RUN_FUNCTION_LOCK_DRIVE(struct _Computer* Computer);

void RUN_FUNCTION_EJECT_DRIVE(struct _Computer* Computer);




void RUN_FUNCTION_INITALIZE_SERIAL_PORT(struct _Computer* Computer);

void RUN_FUNCTION_SERIAL_TX(struct _Computer* Computer);

void RUN_FUNCTION_SERIAL_RX(struct _Computer* Computer);

void RUN_FUNCTION_SERIAL_STATUS(struct _Computer* Computer);




void RUN_FUNCTION_KEYBOARD_INTERCEPT(struct _Computer* Computer);

void RUN_FUNCTION_EVENT_WAIT(struct _Computer* Computer);

void RUN_FUNCTION_SYSREQ_KEY_CALLOUT(struct _Computer* Computer);

void RUN_FUNCTION_WAIT(struct _Computer* Computer);





void RUN_FUNCTION_READ_KEYBOARD_CHARACTER(struct _Computer* Computer);

void RUN_FUNCTION_READ_KEYBOARD_INPUT_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_READ_KEYBOARD_SHIFT_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_STORE_KEYSTROKE(struct _Computer* Computer);



void RUN_FUNCTION_READ_MOUSE_CHARACTER(struct _Computer* Computer);

void RUN_FUNCTION_READ_MOUSE_INPUT_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_READ_MOUSE_SHIFT_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_STORE_MOUSESTROKE(struct _Computer* Computer);


void RUN_FUNCTION_READ_CONTROLLER_CHARACTER(struct _Computer* Computer);

void RUN_FUNCTION_READ_CONTROLLER_INPUT_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_READ_CONTROLLER_SHIFT_STATUS(struct _Computer* Computer);

void RUN_FUNCTION_STORE_CONTROLLERSTROKE(struct _Computer* Computer);




void RUN_FUNCTION_PRINT_CHARACTER_ONTO_PRINTER(struct _Computer* Computer);

void RUN_FUNCTION_INITIALIZE_PRINTER(struct _Computer* Computer);

void RUN_FUNCTION_CHECK_PRINTER_STATUS(struct _Computer* Computer);


void RUN_SHIFT_LEFT(struct _Computer* Computer);

void RUN_SHIFT_RIGHT(struct _Computer* Computer);



void RUN_LOG(struct _Computer* Computer);

void RUN_LN(struct _Computer* Computer);


void RUN_SIN(struct _Computer* Computer);

void RUN_COS(struct _Computer* Computer);

void RUN_TAN(struct _Computer* Computer);


void RUN_INVSIN(struct _Computer* Computer);

void RUN_INVCOS(struct _Computer* Computer);

void RUN_INVTAN(struct _Computer* Computer);


void RUN_SET_DECIMAL_FLAG(struct _Computer* Computer);

void RUN_CLEAR_DECIMAL_FLAG(struct _Computer* Computer);


void RUN_CLEAR_ZERO_FLAG(struct _Computer* Computer);

void RUN_CLEAR_CARRY_FLAG(struct _Computer* Computer);

void RUN_CLEAR_SIGN_FLAG(struct _Computer* Computer);

void RUN_CLEAR_OVERFLOW_FLAG(struct _Computer* Computer);

void RUN_CLEAR_PARITY_FLAG(struct _Computer* Computer);

void RUN_CLEAR_AUXILIARY_CARRY_FLAG(struct _Computer* Computer);


void RUN_CLEAR_DIRECTION_FLAG(struct _Computer* Computer);

void RUN_CLEAR_MASKABLE_INTERRUPT_FLAG(struct _Computer* Computer);

void RUN_CLEAR_TRAP_FLAG(struct _Computer* Computer);


void RUN_FUNCTION_REBOOT(struct _Computer* Computer);

void RUN_FUNCTION_SHUTDOWN(struct _Computer* Computer);

void Handle_Computer_Inputs(struct _Computer* Computer, struct _Keypad Keypad);

#endif // EMULATOR_H_INCLUDED

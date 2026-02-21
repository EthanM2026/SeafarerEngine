#ifndef NEW_OPCODES_H_INCLUDED
#define NEW_OPCODES_H_INCLUDED
#include "opcodes.h"
#include "emulator.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//Complete 2-Register CPU Opcode Table
//Opcode	Instruction	Description	Size	PC behavior
//0x00	NOP	Do nothing	1	+1
//0x10	MOV R0, R0	Copy R0 → R0	2	+2
//0x11	MOV R0, R1	Copy R1 → R0	2	+2
//0x12	MOV R1, R0	Copy R0 → R1	2	+2
//0x13	MOV R1, R1	Copy R1 → R1	2	+2
//0x14	MOV R0, [ADDR]	Load 16-bit memory → R0	3	+3
//0x15	MOV R1, [ADDR]	Load 16-bit memory → R1	3	+3
//0x16	MOV [ADDR], R0	Store R0 → memory	3	+3
//0x17	MOV [ADDR], R1	Store R1 → memory	3	+3
//0x20	PUSH R0	Push R0 onto stack	2	+2
//0x21	PUSH R1	Push R1 onto stack	2	+2
//0x22	POP R0	Pop stack → R0	2	+2
//0x23	POP R1	Pop stack → R1	2	+2
//0x30	ADD R0, R0	R0 = R0 + R0	2	+2
//0x31	ADD R0, R1	R0 = R0 + R1	2	+2
//0x32	ADD R1, R0	R1 = R1 + R0	2	+2
//0x33	ADD R1, R1	R1 = R1 + R1	2	+2
//0x34	SUB R0, R0	R0 = R0 - R0	2	+2
//0x35	SUB R0, R1	R0 = R0 - R1	2	+2
//0x36	SUB R1, R0	R1 = R1 - R0	2	+2
//0x37	SUB R1, R1	R1 = R1 - R1	2	+2
//0x50	CMP R0, R0	R0 - R0 → flags	2	+2
//0x51	CMP R0, R1	R0 - R1 → flags	2	+2
//0x52	CMP R1, R0	R1 - R0 → flags	2	+2
//0x53	CMP R1, R1	R1 - R1 → flags	2	+2
//0x40	JMP ADDR16	Unconditional jump	3	PC = ADDR
//0x41	JE ADDR16	Jump if ZF = 1	3	if ZF → PC=ADDR else PC+=3
//0x42	JNE ADDR16	Jump if ZF = 0	3	if !ZF → PC=ADDR else PC+=3
//0x43	CALL ADDR16	Push return, jump to ADDR	3	push PC+3, PC = ADDR
//0x44	RET	Pop return, jump back	1	PC = popped address
//0xFF	HALT	Stop execution	1	stop

#define NEW_OPCODE_NOP 0x00

#define NEW_OPCODE_MOV_RZero_RZero 0x01
#define NEW_OPCODE_MOV_RZero_ROne 0x02
#define NEW_OPCODE_MOV_ROne_RZero 0x03
#define NEW_OPCODE_MOV_ROne_ROne 0x04

#define NEW_OPCODE_MOV_RZero_ADDR 0x05
#define NEW_OPCODE_MOV_ROne_ADDR 0x06

#define NEW_OPCODE_MOV_ADDR_RZero 0x07
#define NEW_OPCODE_MOV_ADDR_ROne 0x08

#define NEW_OPCODE_PUSH_RZero 0x09
#define NEW_OPCODE_PUSH_ROne 0x0A

#define NEW_OPCODE_POP_RZero 0x0B
#define NEW_OPCODE_POP_ROne 0x0C

#define NEW_OPCODE_ADD_RZero_RZero 0x0D
#define NEW_OPCODE_ADD_RZero_ROne 0x0E
#define NEW_OPCODE_ADD_ROne_RZero 0x0F
#define NEW_OPCODE_ADD_ROne_ROne 0x10

#define NEW_OPCODE_SUB_RZero_RZero 0x11
#define NEW_OPCODE_SUB_RZero_ROne 0x12
#define NEW_OPCODE_SUB_ROne_RZero 0x13
#define NEW_OPCODE_SUB_ROne_ROne 0x14

#define NEW_OPCODE_MUL_RZero_RZero 0x15
#define NEW_OPCODE_MUL_RZero_ROne 0x16
#define NEW_OPCODE_MUL_ROne_RZero 0x17
#define NEW_OPCODE_MUL_ROne_ROne 0x18

#define NEW_OPCODE_DIV_RZero_RZero 0x19
#define NEW_OPCODE_DIV_RZero_ROne 0x1A
#define NEW_OPCODE_DIV_ROne_RZero 0x1B
#define NEW_OPCODE_DIV_ROne_ROne 0x1C

#define NEW_OPCODE_PWR_RZero_RZero 0x1D
#define NEW_OPCODE_PWR_RZero_ROne 0x1E
#define NEW_OPCODE_PWR_ROne_RZero 0x1F
#define NEW_OPCODE_PWR_ROne_ROne 0x20

#define NEW_OPCODE_ROOT_RZero_RZero 0x21
#define NEW_OPCODE_ROOT_RZero_ROne 0x22
#define NEW_OPCODE_ROOT_ROne_RZero 0x23
#define NEW_OPCODE_ROOT_ROne_ROne 0x24

#define NEW_OPCODE_CMP_RZero_RZero 0x25
#define NEW_OPCODE_CMP_RZero_ROne 0x26
#define NEW_OPCODE_CMP_ROne_RZero 0x27
#define NEW_OPCODE_CMP_ROne_ROne 0x28

#define NEW_OPCODE_JMP_ADDRSIXTEEN 0x29
#define NEW_OPCODE_JE_ADDRSIXTEEN 0x2A
#define NEW_OPCODE_JNE_ADDRSIXTEEN 0x2B
#define NEW_OPCODE_CALL_ADDRSIXTEEN 0x2C

#define NEW_OPCODE_RET 0x2D
#define NEW_OPCODE_HALT 0x2E

#define NEW_OPCODE_MOV_RZero_Value 0x2F
#define NEW_OPCODE_MOV_ROne_Value 0x30

#define NEW_OPCODE_CMP_RZero_Value 0x31 //3
#define NEW_OPCODE_CMP_ROne_Value 0x32 //3


#define NEW_OPCODE_IN 0x33//STARTED
#define NEW_OPCODE_OUT 0x34//STARTED
#define NEW_OPCODE_REP 0x35//STARTED

//FLAGS
#define NEW_OPCODE_SET_ZERO_FLAG 0x36//STARTED
#define NEW_OPCODE_SET_CARRY_FLAG 0x37//STARTED
#define NEW_OPCODE_SET_SIGN_FLAG 0x38//STARTED
#define NEW_OPCODE_SET_OVERFLOW_FLAG 0x39//STARTED
#define NEW_OPCODE_SET_PARITY_FLAG 0x3A//STARTED
#define NEW_OPCODE_SET_AUXILIARY_CARRY_FLAG 0x3B//STARTED

#define NEW_OPCODE_SET_DIRECTION_FLAG 0x3C//STARTED
#define NEW_OPCODE_SET_MASKABLE_INTERRUPT_FLAG 0x3D//STARTED
#define NEW_OPCODE_SET_TRAP_FLAG 0x3E//STARTED

#define NEW_OPCODE_ERROR_HANDLER 0x3F

#define NEW_OPCODE_RENDER_SPRITE 0x40

#define NEW_OPCODE_PUSH_RZero_HI 0x41
#define NEW_OPCODE_PUSH_ROne_HI 0x42

#define NEW_OPCODE_PUSH_RZero_LO 0x43
#define NEW_OPCODE_PUSH_ROne_LO 0x44

#define NEW_OPCODE_POP_RZero_HI 0x41
#define NEW_OPCODE_POP_ROne_HI 0x42

#define NEW_OPCODE_POP_RZero_LO 0x43
#define NEW_OPCODE_POP_ROne_LO 0x44

#define NEW_OPCODE_CLEAR_SCREEN 0x45

#define NEW_OPCODE_RENDER_POINT 0x46

#define NEW_OPCODE_RENDER_LINE 0x47

#define NEW_OPCODE_RENDER_TRIANGLE 0x48

#define NEW_OPCODE_RENDER_TILEMAP 0x49

#define NEW_OPCODE_FLIP_SCREEN 0x4A




#define OPCODE_FUNCTION_SET_TIMER 0x4B
#define OPCODE_FUNCTION_PAUSE_TIMER 0x4C
#define OPCODE_FUNCTION_CLEAR_TIMER 0x4D
#define OPCODE_FUNCTION_READ_TIMER 0x4E



#define OPCODE_FUNCTION_READ_TIME 0x4F
#define OPCODE_FUNCTION_READ_DATE 0x50

#define OPCODE_FUNCTION_SET_TIME 0x51
#define OPCODE_FUNCTION_SET_DATE 0x52

#define OPCODE_FUNCTION_INPUT_TEXT 0x53


#define OPCODE_FUNCTION_PLAY_AUDIO 0x54

#define OPCODE_FUNCTION_TEST_AABB_AABB_COLLISION 0x55
#define OPCODE_FUNCTION_TEST_AABB_CIRCLE_COLLISION 0x56
#define OPCODE_FUNCTION_TEST_AABB_RAY_COLLISION 0x57

#define OPCODE_FUNCTION_TEST_CIRCLE_CIRCLE_COLLISION 0x58
#define OPCODE_FUNCTION_TEST_CIRCLE_RAY_COLLISION 0x59

#define OPCODE_FUNCTION_TEST_RAY_RAY_COLLISION 0x5A

#define OPCODE_FUNCTION_TEST_TRIANGLE_TRIANGLE_COLLISION 0x5B

#define OPCODE_FUNCTION_TEST_IF_OFF_SCREEN 0x5C




#define OPCODE_FUNCTION_SET_CURSOR_POSITION 0x5D

#define OPCODE_FUNCTION_CLEAR_SCREEN 0x5E

#define OPCODE_FUNCTION_WRITE_CHARACTER_AT_CURSOR 0x5F

#define OPCODE_FUNCTION_SET_BACKGROUND_COLOR 0x60




#define OPCODE_FUNCTION_CHECK_DISK_DRIVE_STATUS 0x61
#define OPCODE_FUNCTION_READ_FROM_DISK_DRIVE 0x62
#define OPCODE_FUNCTION_WRITE_TO_DISK_DRIVE 0x63
#define OPCODE_FUNCTION_FORMAT_DISK_DRIVE 0x64
#define OPCODE_FUNCTION_GET_DISK_DRIVE_INFORMATION 0x65



#define OPCODE_FUNCTION_EJECT_DRIVE 0x66

#define OPCODE_FUNCTION_INITALIZE_SERIAL_PORT 0x67
#define OPCODE_FUNCTION_SERIAL_TX 0x68
#define OPCODE_FUNCTION_SERIAL_RX 0x69
#define OPCODE_FUNCTION_SERIAL_STATUS 0x6A




#define OPCODE_FUNCTION_CHECK_KEYBOARD_PRESS 0x6B
#define OPCODE_FUNCTION_CHECK_MOUSE_XY 0x6C
#define OPCODE_FUNCTION_CHECK_MOUSE_CLICK 0x6D
#define OPCODE_FUNCTION_CHECK_CONTROLLER_PRESS 0x6E

#define OPCODE_FUNCTION_PRINTER_TX 0x6F
#define OPCODE_FUNCTION_INITIALIZE_PRINTER 0x70
#define OPCODE_FUNCTION_CHECK_PRINTER_STATUS 0x71

#define OPCODE_SHIFT_LEFT 0x72
#define OPCODE_SHIFT_RIGHT 0x73

#define OPCODE_LOG 0x74
#define OPCODE_LN 0x75

#define OPCODE_SIN 0x76
#define OPCODE_COS 0x77
#define OPCODE_TAN 0x78

#define OPCODE_INVSIN 0x79
#define OPCODE_INVCOS 0x7A
#define OPCODE_INVTAN 0x7B

#define OPCODE_SET_DECIMAL_FLAG 0x7C
#define OPCODE_CLEAR_DECIMAL_FLAG 0x7D

#define OPCODE_CLEAR_ZERO_FLAG 0x7E
#define OPCODE_CLEAR_CARRY_FLAG 0x7F
#define OPCODE_CLEAR_SIGN_FLAG 0x80
#define OPCODE_CLEAR_OVERFLOW_FLAG 0x81
#define OPCODE_CLEAR_PARITY_FLAG 0x82
#define OPCODE_CLEAR_AUXILIARY_CARRY_FLAG 0x83

#define OPCODE_CLEAR_DIRECTION_FLAG 0x84
#define OPCODE_CLEAR_MASKABLE_INTERRUPT_FLAG 0x85
#define OPCODE_CLEAR_TRAP_FLAG 0x86

#define OPCODE_FUNCTION_REBOOT 0x87
#define OPCODE_FUNCTION_SHUTDOWN 0x88


#define NEW_OPCODE_MOV_RZERO_RANDOM 0x89
#define NEW_OPCODE_MOV_RONE_RANDOM 0x8A

#define NEW_OPCODE_INC_RZero 0x8B
#define NEW_OPCODE_INC_ROne 0x8C

struct _x80_Programmer
{
    struct _SixtyFourK_Bank Banks[256];
    unsigned char Number_Of_Banks;
    unsigned char Current_Bank;
    unsigned short Current_Offset;
    unsigned short Current_Bank_Data_Offset;
};

void New_Create_z80_Program(struct _x80_Programmer* Programmer, const char* Output);
void New_Insert_Opcode(struct _x80_Programmer* Programmer, unsigned char Bank_ID, unsigned short Offset, unsigned char Size, unsigned char Code[3]);



void Run_New_Opcode_NOP(struct _Computer* Computer);

void Run_New_Opcode_MOV_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_MOV_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_MOV_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_MOV_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_MOV_RZero_ADDR(struct _Computer* Computer);
void Run_New_Opcode_MOV_ROne_ADDR(struct _Computer* Computer);

void Run_New_Opcode_MOV_ADDR_RZero(struct _Computer* Computer);
void Run_New_Opcode_MOV_ADDR_ROne(struct _Computer* Computer);

void Run_New_Opcode_PUSH_RZero(struct _Computer* Computer);
void Run_New_Opcode_PUSH_ROne(struct _Computer* Computer);

void Run_New_Opcode_POP_RZero(struct _Computer* Computer);
void Run_New_Opcode_POP_ROne(struct _Computer* Computer);




void Run_New_Opcode_ADD_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_ADD_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_ADD_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_ADD_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_SUB_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_SUB_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_SUB_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_SUB_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_MUL_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_MUL_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_MUL_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_MUL_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_DIV_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_DIV_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_DIV_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_DIV_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_PWR_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_PWR_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_PWR_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_PWR_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_ROOT_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_ROOT_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_ROOT_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_ROOT_ROne_ROne(struct _Computer* Computer);

void Run_New_Opcode_CMP_RZero_RZero(struct _Computer* Computer);
void Run_New_Opcode_CMP_RZero_ROne(struct _Computer* Computer);
void Run_New_Opcode_CMP_ROne_RZero(struct _Computer* Computer);
void Run_New_Opcode_CMP_ROne_ROne(struct _Computer* Computer);


void Run_New_Opcode_JMP(struct _Computer* Computer);
void Run_New_Opcode_JE(struct _Computer* Computer);
void Run_New_Opcode_JNE(struct _Computer* Computer);
void Run_New_Opcode_CALL(struct _Computer* Computer);

void Run_New_Opcode_RET(struct _Computer* Computer);
void Run_New_Opcode_HALT(struct _Computer* Computer);

void Run_New_Opcode_MOV_RZero_Value(struct _Computer* Computer);
void Run_New_Opcode_MOV_ROne_Value(struct _Computer* Computer);

void Run_New_Opcode_CMP_RZero_Value(struct _Computer* Computer);
void Run_New_Opcode_CMP_ROne_Value(struct _Computer* Computer);

void Run_New_Opcode_IN(struct _Computer* Computer);
void Run_New_Opcode_OUT(struct _Computer* Computer);
void Run_New_Opcode_REP(struct _Computer* Computer);

//FLAGS
void Run_New_Opcode_SET_ZERO_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_CARRY_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_SIGN_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_OVERFLOW_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_PARITY_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_AUXILIARY_CARRY_FLAG(struct _Computer* Computer);

void Run_New_Opcode_SET_DIRECTION_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_MASKABLE_INTERRUPT_FLAG(struct _Computer* Computer);
void Run_New_Opcode_SET_TRAP_FLAG(struct _Computer* Computer);

void Run_New_Opcode_Error_Handler(struct _Computer* Computer);

void Run_New_Opcode_Render_Sprite(struct _Computer* Computer);

void Run_New_Opcode_PUSH_RZero_HI(struct _Computer* Computer);
void Run_New_Opcode_PUSH_ROne_HI(struct _Computer* Computer);

void Run_New_Opcode_POP_RZero_LO(struct _Computer* Computer);
void Run_New_Opcode_POP_ROne_LO(struct _Computer* Computer);

void Run_New_Opcode_Clear_Screen(struct _Computer* Computer);

void Run_New_Opcode_Render_Point(struct _Computer* Computer);

void Run_New_Opcode_Render_Line(struct _Computer* Computer);

void Run_New_Opcode_Render_Triangle(struct _Computer* Computer);

void Run_New_Opcode_Render_Tilemap(struct _Computer* Computer);

void Fill_Bank_With_File(struct _x80_Programmer* Programmer, unsigned char Bank_ID, unsigned short Offset, const char* Filename);

void Write_Example_Encoding(const char* Filename);

void Run_New_Opcode_Flip_Screen(struct _Computer* Computer);

void Run_New_Opcode_MOV_RZero_Random(struct _Computer* Computer);
void Run_New_Opcode_MOV_ROne_Random(struct _Computer* Computer);


void Run_NEW_OPCODE_INC_RZero(struct _Computer* Computer);
void Run_NEW_OPCODE_INC_ROne(struct _Computer* Computer);


uint16_t float32_to_float16(float f);
float float16_to_float32(uint16_t h);
double float16_to_double(uint16_t h);
uint16_t double_to_float16_full(double d);
#endif // NEW_OPCODES_H_INCLUDED

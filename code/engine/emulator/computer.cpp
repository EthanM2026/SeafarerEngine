#include "computer.h"

struct _New_Computer* Create_New_Computer()
{
    struct _New_Computer* I = (struct _New_Computer*) calloc(1,sizeof (struct _New_Computer));
    return I;
};

void Initialize_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Power_State = true;

    Seat->New_Computer->Color_Mode = true;

    Seat->New_Computer->Red_Color_Balance = 255;
    Seat->New_Computer->Green_Color_Balance = 255;
    Seat->New_Computer->Blue_Color_Balance = 255;

    Seat->New_Computer->RGB_Canvas = (unsigned char*) calloc (256*256*3,sizeof(unsigned char));
    Seat->New_Computer->RGBA_Canvas = (unsigned char*) calloc (256*256*4,sizeof(unsigned char));

    Seat->New_Computer->Front_Video_Buffer = (unsigned char*) calloc (256*256,sizeof(unsigned char));
    Seat->New_Computer->Back_Video_Buffer = (unsigned char*) calloc (256*256,sizeof(unsigned char));

    Seat->New_Computer->Controller_D_Pad_RAM[0] = 0;
    Seat->New_Computer->Controller_D_Pad_RAM[1] = 0;
    Seat->New_Computer->Controller_D_Pad_RAM[2] = 0;
    Seat->New_Computer->Controller_D_Pad_RAM[3] = 0;

    Seat->New_Computer->Controller_Button_RAM[0] = 0;
    Seat->New_Computer->Controller_Button_RAM[1] = 0;

    for(int j = 0; j < 256; j++)
    {
        Seat->New_Computer->Keyboard_RAM[j] = 0;
    }

    Seat->New_Computer->Mouse_RAM[0] = 0;
    Seat->New_Computer->Mouse_RAM[1] = 0;
    Seat->New_Computer->Mouse_RAM[2] = 0;

    for(int j = 0; j < 256*256; j++)
    {
        Seat->New_Computer->Front_Video_Buffer[j] = 1;
        Seat->New_Computer->Back_Video_Buffer[j] = 1;
    }

    for(int j = 0; j < 256*256*3; j++)
    {
        Seat->New_Computer->RGB_Canvas[j] = 1;
    }

    for(int j = 0; j < 256*256*4; j++)
    {
        Seat->New_Computer->RGBA_Canvas[j] = 255;
    }

    glGenTextures(1, &Seat->New_Computer->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Seat->New_Computer->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256,256, 0, GL_RGB, GL_UNSIGNED_BYTE, Seat->New_Computer->RGB_Canvas);
    glEnable(GL_TEXTURE_2D);




    Seat->New_Computer->Current_ROM_Bank = 0;

    for(int j = 0; j < 256; j++)
    {
        for(int l = 0; l < 65536; l++)
        {
            Seat->New_Computer->ROM_Memory_Zero[j].Bank[l] = 0;
            Seat->New_Computer->ROM_Memory_One[j].Bank[l] = 0;
            Seat->New_Computer->RAM[j].Bank[l] = 0;
        }
    }

    Seat->New_Computer->Processor.Register_Zero = 0;
    Seat->New_Computer->Processor.Register_One = 0;
    Seat->New_Computer->Processor.Instruction_Pointer = 0;
    Seat->New_Computer->Processor.Stack_Pointer = 65535;

    Seat->New_Computer->Processor.Zero_Flag = 0;
    Seat->New_Computer->Processor.Carry_Flag = 0;
    Seat->New_Computer->Processor.Sign_Flag = 0;
    Seat->New_Computer->Processor.Overflow_Flag = 0;
    Seat->New_Computer->Processor.Parity_Flag = 0;
    Seat->New_Computer->Processor.Auxiliary_Carry_Flag = 0;
    Seat->New_Computer->Processor.Direction_Flag = 0;
    Seat->New_Computer->Processor.Maskable_Interrupt_Flag = 0;
    Seat->New_Computer->Processor.Trap_Flag = 0;

    Seat->New_Computer->Processor.Equals_Flag = 0;
    Seat->New_Computer->Processor.Less_Flag = 0;
    Seat->New_Computer->Processor.Greater_Flag = 0;

    Seat->New_Computer->Processor.Processor_Halted = false;

    Turn_Computer_Off(Engine, Seat);
};

void Process_New_Computer_Cycles(struct _Engine* Engine, struct _Seat_Object* Seat, int Cycles)
{
    if(!Seat->New_Computer->Processor.Processor_Halted && Seat->New_Computer->Power_State)
    {
    for(int j = 0; j < Cycles; j++)
    {
        Process_New_Computer(Engine,Seat);
    }
    }
};

void Process_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char Current_Opcode = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer];
    //printf("IP: %d\n", Seat->New_Computer->Processor.Instruction_Pointer);
    //printf("Current RAM Bank: %d\n", Seat->New_Computer->Current_RAM_Bank);
    //printf("Opcode: %d\n", Current_Opcode);
    if(!Seat->New_Computer->Processor.Processor_Halted && Seat->New_Computer->Power_State)
    {
    switch(Current_Opcode)
    {
        case OPC_HLT:
            Process_Opcode_HLT(Engine,Seat);
        break;
        case OPC_NOP:
            Process_Opcode_NOP(Engine,Seat);
        break;
        case OPC_CLS:
            Process_Opcode_CLS(Engine,Seat);
        break;
        case OPC_FLS:
            Process_Opcode_FLS(Engine,Seat);
        break;
        case OPC_INT:
            Process_Opcode_INT(Engine,Seat);
        break;

        case OPC_MOV_R0_R1:
            Process_Opcode_MOV_R0_R1(Engine,Seat);
        break;
        case OPC_MOV_R1_R0:
            Process_Opcode_MOV_R1_R0(Engine,Seat);
        break;

        case OPC_MOV_R0_INT_VALUE:
            Process_Opcode_MOV_R0_INT_VALUE(Engine,Seat);
        break;
        case OPC_MOV_R1_INT_VALUE:
            Process_Opcode_MOV_R1_INT_VALUE(Engine,Seat);
        break;

        case OPC_PUSH_R0:
            Process_Opcode_PUSH_R0(Engine,Seat);
        break;
        case OPC_PUSH_R1:
            Process_Opcode_PUSH_R1(Engine,Seat);
        break;

        case OPC_POP_R0:
            Process_Opcode_POP_R0(Engine,Seat);
        break;
        case OPC_POP_R1:
            Process_Opcode_POP_R1(Engine,Seat);
        break;

        case OPC_PUSH_VALUE_AT_ADDRESS:
//            Process_Opcode_PUSH_VALUE_AT_ADDRESS(Engine,Seat);
        break;

        case OPC_MOV_VALUE_AT_ADDRESS_R0:
            Process_Opcode_MOV_VALUE_AT_ADDRESS_R0(Engine,Seat);
        break;
        case OPC_MOV_VALUE_AT_ADDRESS_R1:
            Process_Opcode_MOV_VALUE_AT_ADDRESS_R1(Engine,Seat);
        break;

        case OPC_MOV_R0_VALUE_AT_ADDRESS:
            Process_Opcode_MOV_R0_VALUE_AT_ADDRESS(Engine,Seat);
        break;
        case OPC_MOV_R1_VALUE_AT_ADDRESS:
            Process_Opcode_MOV_R1_VALUE_AT_ADDRESS(Engine,Seat);
        break;

        case OPC_ADD_R0_R1:
            Process_Opcode_ADD_R0_R1(Engine,Seat);
        break;
        case OPC_ADD_R1_R0:
            Process_Opcode_ADD_R1_R0(Engine,Seat);
        break;

        case OPC_SUB_R0_R1:
            Process_Opcode_SUB_R0_R1(Engine,Seat);
        break;
        case OPC_SUB_R1_R0:
            Process_Opcode_SUB_R1_R0(Engine,Seat);
        break;

        case OPC_MUL_R0_R1:
            Process_Opcode_MUL_R0_R1(Engine,Seat);
        break;
        case OPC_MUL_R1_R0:
            Process_Opcode_MUL_R1_R0(Engine,Seat);
        break;

        case OPC_DIV_R0_R1:
            Process_Opcode_DIV_R0_R1(Engine,Seat);
        break;
        case OPC_DIV_R1_R0:
            Process_Opcode_DIV_R1_R0(Engine,Seat);
        break;




        case OPC_MONOCHROMEMODE:
            Process_Opcode_MONOCHROMEMODE(Engine,Seat);
        break;
        case OPC_COLORMODE:
            Process_Opcode_COLORMODE(Engine,Seat);
        break;

        case OPC_JMP:
            Process_Opcode_JMP(Engine,Seat);
        break;

        case OPC_CALL:
            Process_Opcode_CALL(Engine,Seat);
        break;

        case OPC_RETURN:
            Process_Opcode_RETURN(Engine,Seat);
        break;




        case OPC_CMP_R0_R1:
            Process_Opcode_CMP_R0_R1(Engine,Seat);
        break;
        case OPC_JNE:
            Process_Opcode_JNE(Engine,Seat);
        break;
        case OPC_JE:
            Process_Opcode_JE(Engine,Seat);
        break;
        case OPC_JG:
            Process_Opcode_JG(Engine,Seat);
        break;
        case OPC_JL:
            Process_Opcode_JL(Engine,Seat);
        break;
        case OPC_JGE:
            Process_Opcode_JGE(Engine,Seat);
        break;
        case OPC_JLE:
            Process_Opcode_JLE(Engine,Seat);
        break;

        case OPC_CNE:
            Process_Opcode_CNE(Engine,Seat);
        break;
        case OPC_CE:
            Process_Opcode_CE(Engine,Seat);
        break;
        case OPC_CG:
            Process_Opcode_CG(Engine,Seat);
        break;
        case OPC_CL:
            Process_Opcode_CL(Engine,Seat);
        break;
        case OPC_CGE:
            Process_Opcode_CGE(Engine,Seat);
        break;
        case OPC_CLE:
            Process_Opcode_CLE(Engine,Seat);
        break;


        case OPC_IN_DPAD_UP_R0:
            Process_Opcode_IN_DPAD_UP_R0(Engine,Seat);
        break;
        case OPC_IN_DPAD_DOWN_R0:
            Process_Opcode_IN_DPAD_DOWN_R0(Engine,Seat);
        break;
        case OPC_IN_DPAD_LEFT_R0:
            Process_Opcode_IN_DPAD_LEFT_R0(Engine,Seat);
        break;
        case OPC_IN_DPAD_RIGHT_R0:
            Process_Opcode_IN_DPAD_RIGHT_R0(Engine,Seat);
        break;

        case OPC_IN_A_BUTTON_R0:
            Process_Opcode_IN_A_BUTTON_R0(Engine,Seat);
        break;
        case OPC_IN_B_BUTTON_R0:
            Process_Opcode_IN_B_BUTTON_R0(Engine,Seat);
        break;

        case OPC_IN_KBD_R0:
            Process_Opcode_IN_KBD_R0(Engine,Seat);
        break;
        case OPC_IN_MOUSE_X_R0:
            Process_Opcode_IN_MOUSE_X_R0(Engine,Seat);
        break;
        case OPC_IN_MOUSE_Y_R0:
            Process_Opcode_IN_MOUSE_Y_R0(Engine,Seat);
        break;
        case OPC_IN_MOUSE_CLICK_R0:
            Process_Opcode_IN_MOUSE_CLICK_R0(Engine,Seat);
        break;


        default:
            printf("Invalid opcode at IP: %d %d\n", Seat->New_Computer->Current_RAM_Bank,Seat->New_Computer->Processor.Instruction_Pointer);
            Seat->New_Computer->Processor.Processor_Halted = true;
        break;
    }
    }
};

void Process_Opcode_INT(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char INT_ID = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    if(!Seat->New_Computer->Processor.Processor_Halted)
    {
    switch(INT_ID)
    {
        case INT_SHUTDOWN_COMPUTER:
            Process_INT_Shutdown_Computer(Engine,Seat);
        break;

        case INT_RESTART_COMPUTER:
            Process_INT_Restart_Computer(Engine,Seat);
        break;

        case INT_RENDER_PIXEL:
            Process_INT_Render_Pixel(Engine,Seat);
        break;

        case INT_RENDER_LINE:
            Process_INT_Render_Line(Engine,Seat);
        break;

        case INT_RENDER_IMAGE:
            Process_INT_Render_Image(Engine,Seat);
        break;

        case INT_LOAD_RAM_BANK:
            Process_INT_Load_RAM_Bank(Engine,Seat);
        break;
    }
    }
};

void Process_Opcode_NOP(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_HLT(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Processor_Halted = true;
    printf("Processor Halted!\n");
};

void Process_Opcode_CLS(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    for(int j = 0; j < 256*256; j++)
    {
        Seat->New_Computer->Back_Video_Buffer[j] = 1;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
    printf("Cleared screen\n");
};

void Process_Opcode_FLS(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    for(int j = 0; j < 256*256; j++)
    {
        Seat->New_Computer->Front_Video_Buffer[j] = Seat->New_Computer->Back_Video_Buffer[j];
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
    printf("Flipped screen\n");
};

void Process_Opcode_MONOCHROMEMODE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Color_Mode = false;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_COLORMODE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Color_Mode = true;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_MOV_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Processor.Register_One;
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};

void Process_Opcode_CMP_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Register_Zero == Seat->New_Computer->Processor.Register_One)
    {
        Seat->New_Computer->Processor.Equals_Flag = true;
        printf("RZero Equals ROne!\n");
    }

    if(Seat->New_Computer->Processor.Register_Zero > Seat->New_Computer->Processor.Register_One)
    {
        Seat->New_Computer->Processor.Greater_Flag = true;
        printf("RZero Greater  ROne!\n");
    }

    if(Seat->New_Computer->Processor.Register_Zero < Seat->New_Computer->Processor.Register_One)
    {
        Seat->New_Computer->Processor.Less_Flag = true;

        printf("RZero Less ROne!\n");
    }

    Seat->New_Computer->Processor.Instruction_Pointer += 1;
};

void Process_Opcode_MOV_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_One = Seat->New_Computer->Processor.Register_Zero;
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


static inline bool parity8(uint8_t x)
{
    x ^= x >> 4;
    x &= 0xF;
    return (0x6996 >> x) & 1;
}


void Process_Opcode_SUB_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint32_t full = (uint32_t)Seat->New_Computer->Processor.Register_Zero - (uint32_t)Seat->New_Computer->Processor.Register_One;
    uint16_t result = (uint16_t)full;

    // Carry Flag = borrow
    Seat->New_Computer->Processor.Carry_Flag = Seat->New_Computer->Processor.Register_Zero < Seat->New_Computer->Processor.Register_One;

    // Zero Flag
    Seat->New_Computer->Processor.Zero_Flag = (result == 0);

    // Sign Flag
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;

    // Overflow Flag
   Seat->New_Computer->Processor.Overflow_Flag = (((Seat->New_Computer->Processor.Register_Zero ^ Seat->New_Computer->Processor.Register_One) & (Seat->New_Computer->Processor.Register_Zero ^ result)) >> 15) & 1;

    // Auxiliary Carry Flag (borrow from bit 4)
   Seat->New_Computer->Processor.Auxiliary_Carry_Flag = ((Seat->New_Computer->Processor.Register_Zero ^ Seat->New_Computer->Processor.Register_One ^ result) & 0x10) != 0;

    // Parity Flag
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_Zero=result;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


void Process_Opcode_SUB_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint32_t full = (uint32_t)Seat->New_Computer->Processor.Register_One - (uint32_t)Seat->New_Computer->Processor.Register_Zero;
    uint16_t result = (uint16_t)full;

    // Carry Flag = borrow
    Seat->New_Computer->Processor.Carry_Flag = Seat->New_Computer->Processor.Register_One < Seat->New_Computer->Processor.Register_Zero;

    // Zero Flag
    Seat->New_Computer->Processor.Zero_Flag = (result == 0);

    // Sign Flag
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;

    // Overflow Flag
   Seat->New_Computer->Processor.Overflow_Flag = (((Seat->New_Computer->Processor.Register_One ^ Seat->New_Computer->Processor.Register_Zero) & (Seat->New_Computer->Processor.Register_One ^ result)) >> 15) & 1;

    // Auxiliary Carry Flag (borrow from bit 4)
   Seat->New_Computer->Processor.Auxiliary_Carry_Flag = ((Seat->New_Computer->Processor.Register_One ^ Seat->New_Computer->Processor.Register_Zero ^ result) & 0x10) != 0;

    // Parity Flag
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_One=result;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};



void Process_Opcode_ADD_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
     // Full-width addition for carry detection
    uint32_t full = (uint32_t)Seat->New_Computer->Processor.Register_Zero + (uint32_t)Seat->New_Computer->Processor.Register_One;

    // Truncated 16-bit result
    uint16_t result = (uint16_t)full;

    // Carry Flag (unsigned overflow)
    Seat->New_Computer->Processor.Carry_Flag = (full >> 16) & 1;

    // Zero Flag
    Seat->New_Computer->Processor.Zero_Flag = (result == 0);

    // Sign Flag (bit 15)
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;

    // Overflow Flag (signed overflow)
    Seat->New_Computer->Processor.Overflow_Flag = ((~(Seat->New_Computer->Processor.Register_Zero ^ Seat->New_Computer->Processor.Register_One) & (Seat->New_Computer->Processor.Register_Zero ^ result)) >> 15) & 1;

    // Auxiliary Carry Flag (bit 3 -> bit 4)
    Seat->New_Computer->Processor.Auxiliary_Carry_Flag = ((Seat->New_Computer->Processor.Register_Zero ^ Seat->New_Computer->Processor.Register_One ^ result) & 0x10) != 0;

    // Parity Flag (low byte only)
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_Zero=result;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};



void Process_Opcode_ADD_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
     // Full-width addition for carry detection
    uint32_t full = (uint32_t)Seat->New_Computer->Processor.Register_One + (uint32_t)Seat->New_Computer->Processor.Register_Zero;

    // Truncated 16-bit result
    uint16_t result = (uint16_t)full;

    // Carry Flag (unsigned overflow)
    Seat->New_Computer->Processor.Carry_Flag = (full >> 16) & 1;

    // Zero Flag
    Seat->New_Computer->Processor.Zero_Flag = (result == 0);

    // Sign Flag (bit 15)
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;

    // Overflow Flag (signed overflow)
    Seat->New_Computer->Processor.Overflow_Flag = ((~(Seat->New_Computer->Processor.Register_One ^ Seat->New_Computer->Processor.Register_Zero) & (Seat->New_Computer->Processor.Register_One ^ result)) >> 15) & 1;

    // Auxiliary Carry Flag (bit 3 -> bit 4)
    Seat->New_Computer->Processor.Auxiliary_Carry_Flag = ((Seat->New_Computer->Processor.Register_One ^ Seat->New_Computer->Processor.Register_Zero ^ result) & 0x10) != 0;

    // Parity Flag (low byte only)
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_One=result;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};





void Process_Opcode_MUL_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
     // Unsigned multiplication
    uint32_t result = (uint32_t)Seat->New_Computer->Processor.Register_Zero* (uint32_t)Seat->New_Computer->Processor.Register_One;

    // Store low half
    Seat->New_Computer->Processor.Register_Zero= (uint16_t)(result & 0xFFFF);

    // Store high half
    uint16_t high_half = (uint16_t)(result >> 16);

    bool overflow = (high_half != 0);

    Seat->New_Computer->Processor.Carry_Flag = overflow;
    Seat->New_Computer->Processor.Overflow_Flag = overflow;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


void Process_Opcode_DIV_R0_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    // Division by zero
    if(Seat->New_Computer->Processor.Register_One == 0)
    {
        Seat->New_Computer->Processor.Division_Error = true;
    }

    else if(Seat->New_Computer->Processor.Register_One != 0)
    {
    uint32_t quotient  = Seat->New_Computer->Processor.Register_Zero / Seat->New_Computer->Processor.Register_One;
    uint32_t remainder = Seat->New_Computer->Processor.Register_Zero % Seat->New_Computer->Processor.Register_One;

    // Quotient must fit in AX
    if(quotient > 0xFFFF)
    {
        Seat->New_Computer->Processor.Division_Error = true;
    }

    Seat->New_Computer->Processor.Register_Zero = (uint16_t)quotient;
    Seat->New_Computer->Processor.Register_One = (uint16_t)remainder;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
}




void Process_Opcode_MUL_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
     // Unsigned multiplication
    uint32_t result = (uint32_t)Seat->New_Computer->Processor.Register_One* (uint32_t)Seat->New_Computer->Processor.Register_Zero;

    // Store low half
    Seat->New_Computer->Processor.Register_One= (uint16_t)(result & 0xFFFF);

    // Store high half
    uint16_t high_half = (uint16_t)(result >> 16);

    bool overflow = (high_half != 0);

    Seat->New_Computer->Processor.Carry_Flag = overflow;
    Seat->New_Computer->Processor.Overflow_Flag = overflow;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


void Process_Opcode_DIV_R1_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    // Division by One
    if(Seat->New_Computer->Processor.Register_Zero == 0)
    {
        Seat->New_Computer->Processor.Division_Error = true;
    }

    else if(Seat->New_Computer->Processor.Register_Zero != 0)
    {
    uint32_t quotient  = Seat->New_Computer->Processor.Register_One / Seat->New_Computer->Processor.Register_Zero;
    uint32_t remainder = Seat->New_Computer->Processor.Register_One % Seat->New_Computer->Processor.Register_Zero;

    // Quotient must fit in AX
    if(quotient > 0xFFFF)
    {
        Seat->New_Computer->Processor.Division_Error = true;
    }

    Seat->New_Computer->Processor.Register_One = (uint16_t)quotient;
    Seat->New_Computer->Processor.Register_Zero = (uint16_t)remainder;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
}





void Process_Opcode_MOV_R0_INT_VALUE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint16_t Value;

    Value = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] & 0xff);
    Seat->New_Computer->Processor.Register_Zero = Value;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 3 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 3;
    }
    printf("The VALUE at r0 is now %d\n", Value);
};

void Process_Opcode_MOV_R1_INT_VALUE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint16_t Value;

    Value = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] & 0xff);
    Seat->New_Computer->Processor.Register_One = Value;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 3 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 3;
    }
    printf("The VALUE at r1 is now %d\n", Value);
};



void Process_Opcode_MOV_VALUE_AT_ADDRESS_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint8_t low_byte = Seat->New_Computer->Processor.Register_Zero & 0xFF;
    uint8_t high_byte = (Seat->New_Computer->Processor.Register_Zero >> 8) & 0xFF;

    uint8_t Target_Bank;
    uint16_t Target_Address;

    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address] = low_byte;
    Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address+1] = high_byte;

    printf("Moved Value into Bank %d Address %d\n", Target_Bank, Target_Address);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 4 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};

void Process_Opcode_MOV_VALUE_AT_ADDRESS_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint8_t low_byte = Seat->New_Computer->Processor.Register_One & 0xFF;
    uint8_t high_byte = (Seat->New_Computer->Processor.Register_One >> 8) & 0xFF;

    uint8_t Target_Bank;
    uint16_t Target_Address;

    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address] = low_byte;
    Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address+1] = high_byte;

    printf("Moved Value into Bank %d Address %d\n", Target_Bank, Target_Address);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 4 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};

void Process_Opcode_MOV_R0_VALUE_AT_ADDRESS(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    uint16_t Value = (Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address+1] << 8) | (Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address+0] & 0xff);

    printf("The Value %d is going into Register Zero.\n", Value);

    Seat->New_Computer->Processor.Register_Zero = Value;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 4 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};

void Process_Opcode_MOV_R1_VALUE_AT_ADDRESS(struct _Engine* Engine, struct _Seat_Object* Seat)
{
     //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    printf("MOV R1 Target Bank %d %d\n", Target_Bank, Target_Address);

    uint16_t Value = (Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address+1] << 8) | (Seat->New_Computer->RAM[Target_Bank].Bank[Target_Address+0] & 0xff);

    Seat->New_Computer->Processor.Register_One = Value;

    if(Seat->New_Computer->Processor.Instruction_Pointer + 4 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};


void Process_Opcode_PUSH_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Stack_Pointer - 2 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=2;

    uint8_t low_byte = Seat->New_Computer->Processor.Register_Zero & 0xFF;
    uint8_t high_byte = (Seat->New_Computer->Processor.Register_Zero >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = low_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+1] = high_byte;
    }
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
    printf("Pushed %d\n", Seat->New_Computer->Processor.Register_Zero);
};

void Process_Opcode_PUSH_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Stack_Pointer - 2 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=2;

    uint8_t low_byte = Seat->New_Computer->Processor.Register_One & 0xFF;
    uint8_t high_byte = (Seat->New_Computer->Processor.Register_One >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = low_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+1] = high_byte;
    }
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


void Process_Opcode_POP_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Stack_Pointer + 2 < 65536)
    {
    Seat->New_Computer->Processor.Stack_Pointer += 2;
    Seat->New_Computer->Processor.Register_Zero=(Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+1] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] & 0xff);
    }
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};

void Process_Opcode_POP_R1(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Stack_Pointer + 2 < 65536)
    {
    Seat->New_Computer->Processor.Stack_Pointer += 2;
    Seat->New_Computer->Processor.Register_One=(Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+1] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] & 0xff);
    }
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};

void Process_Opcode_CALL(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;
};




void Process_Opcode_CNE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(!Seat->New_Computer->Processor.Equals_Flag)
    {
    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;

    }

    else if(Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};





void Process_Opcode_CE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Equals_Flag)
    {
    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};



void Process_Opcode_CG(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Greater_Flag)
    {
    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Greater_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};




void Process_Opcode_CL(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Less_Flag)
    {
    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Less_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};





void Process_Opcode_CGE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Greater_Flag ||Seat->New_Computer->Processor.Equals_Flag)
    {
    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Greater_Flag && !Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};



void Process_Opcode_CLE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Less_Flag ||Seat->New_Computer->Processor.Equals_Flag)
    {
    if(Seat->New_Computer->Processor.Stack_Pointer - 4 >= 0)
    {
    Seat->New_Computer->Processor.Stack_Pointer -=4;

    uint8_t current_bank = Seat->New_Computer->Current_RAM_Bank;
    uint16_t current_address = Seat->New_Computer->Processor.Instruction_Pointer+4; //Skip the RETURN command.

    uint8_t low_byte = current_address & 0xFF;
    uint8_t high_byte = (current_address >> 8) & 0xFF;

    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer] = current_bank;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] = high_byte;
    Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] = low_byte;
    }


    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Less_Flag && !Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};


void Process_Opcode_RETURN(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    Target_Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer];
    Target_Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    printf("Returning to %d %d\n", Target_Bank,Target_Address);

    Seat->New_Computer->Processor.Stack_Pointer += 4;
};


void Process_Opcode_JMP(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;
};


void Process_Opcode_JNE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(!Seat->New_Computer->Processor.Equals_Flag)
    {
    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};


void Process_Opcode_JE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Equals_Flag)
    {
    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};


void Process_Opcode_JG(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Greater_Flag)
    {
    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }


    else if(!Seat->New_Computer->Processor.Greater_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};


void Process_Opcode_JL(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Less_Flag)
    {
    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Less_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};

void Process_Opcode_JGE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Greater_Flag || Seat->New_Computer->Processor.Equals_Flag)
    {
    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Greater_Flag && !Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};



void Process_Opcode_JLE(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Move the Value at Bank, Address into RZero!
    uint8_t Target_Bank;
    uint16_t Target_Address;

    if(Seat->New_Computer->Processor.Less_Flag || Seat->New_Computer->Processor.Equals_Flag)
    {
    Target_Bank = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+1];
    Target_Address = (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+2] << 8) | (Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer+3] & 0xff);

    Seat->New_Computer->Current_RAM_Bank = Target_Bank;
    Seat->New_Computer->Processor.Instruction_Pointer = Target_Address;

    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    }

    else if(!Seat->New_Computer->Processor.Less_Flag && !Seat->New_Computer->Processor.Equals_Flag)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 4;
    }
};

void Process_Opcode_PUSH_VALUE_AT_ADDRESS(struct _Engine* Engine, struct _Seat_Object* Seat)
{

};



void Handle_New_Computer_Inputs(struct _Engine* Engine, struct _Seat_Object* Seat, struct _Keypad Keypad)
{
    if(Keypad.Keyboard_Up)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[0] = 255;
        printf("Keypad Up!\n");
    }

    if(Keypad.Keyboard_Down)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[1] = 255;
        printf("Keypad Down!\n");
    }

    if(Keypad.Keyboard_Left)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[2] = 255;
        printf("Keypad Left!\n");
    }

    if(Keypad.Keyboard_Right)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[3] = 255;
        printf("Keypad Right!\n");
    }


    if(Keypad.Keyboard_A)
    {
        Seat->New_Computer->Controller_Button_RAM[0] = 255;
        printf("Keypad A!\n");
    }
    if(Keypad.Keyboard_B)
    {
        Seat->New_Computer->Controller_Button_RAM[1] = 255;
        printf("Keypad B!\n");
    }




    if(!Keypad.Keyboard_A)
    {
        Seat->New_Computer->Controller_Button_RAM[0] = 0;
        printf("Keypad A!\n");
    }
    if(!Keypad.Keyboard_B)
    {
        Seat->New_Computer->Controller_Button_RAM[1] = 0;
        printf("Keypad B!\n");
    }



    if(!Keypad.Keyboard_Up)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[0] = 0;
    }

    if(!Keypad.Keyboard_Down)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[1] = 0;
    }

    if(!Keypad.Keyboard_Left)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[2] = 0;
    }

    if(!Keypad.Keyboard_Right)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[3] = 0;
    }
};

void Turn_Computer_Off(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Power_State = false;
    Seat->New_Computer->Processor.Processor_Halted = true;

    for(int j = 0; j < 256*256; j++)
    {
        Seat->New_Computer->Front_Video_Buffer[j] = 0;
        Seat->New_Computer->Back_Video_Buffer[j] = 0;
    }

    for(int j = 0; j < 256*256*3; j++)
    {
        Seat->New_Computer->RGB_Canvas[j] = 0;
    }

    for(int j = 0; j < 256*256*4; j++)
    {
        Seat->New_Computer->RGBA_Canvas[j] = 0;
    }

    Seat->New_Computer->Current_ROM_Bank = 0;

    for(int j = 0; j < 256; j++)
    {
        for(int l = 0; l < 65536; l++)
        {
            Seat->New_Computer->ROM_Memory_Zero[j].Bank[l] = 0;
            Seat->New_Computer->ROM_Memory_One[j].Bank[l] = 0;
            Seat->New_Computer->RAM[j].Bank[l] = 0;
        }
    }

    Seat->New_Computer->Processor.Register_Zero = 0;
    Seat->New_Computer->Processor.Register_One = 0;
    Seat->New_Computer->Processor.Instruction_Pointer = 0;
    Seat->New_Computer->Processor.Stack_Pointer = 65535;

    Seat->New_Computer->Processor.Zero_Flag = 0;
    Seat->New_Computer->Processor.Carry_Flag = 0;
    Seat->New_Computer->Processor.Sign_Flag = 0;
    Seat->New_Computer->Processor.Overflow_Flag = 0;
    Seat->New_Computer->Processor.Parity_Flag = 0;
    Seat->New_Computer->Processor.Auxiliary_Carry_Flag = 0;
    Seat->New_Computer->Processor.Direction_Flag = 0;
    Seat->New_Computer->Processor.Maskable_Interrupt_Flag = 0;
    Seat->New_Computer->Processor.Trap_Flag = 0;

    Seat->New_Computer->Processor.Equals_Flag = 0;
    Seat->New_Computer->Processor.Less_Flag = 0;
    Seat->New_Computer->Processor.Greater_Flag = 0;

    glBindTexture(GL_TEXTURE_2D, Seat->New_Computer->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int in = 0;
    int r = 0;
    //Convert Video card memory to 24-bit color
    for(int j = 0; j < 65536; j++)
    {
        if(Seat->New_Computer->Front_Video_Buffer[in] < 255)
        {
            memcpy(&Seat->New_Computer->RGB_Canvas[r], &Seat->New_Computer->Front_Video_Buffer[in], 1);
            memcpy(&Seat->New_Computer->RGB_Canvas[r+1], &Seat->New_Computer->Front_Video_Buffer[in], 1);
            memcpy(&Seat->New_Computer->RGB_Canvas[r+2], &Seat->New_Computer->Front_Video_Buffer[in], 1);

            Seat->New_Computer->RGB_Canvas[r]*=Seat->New_Computer->Red_Color_Balance;
            Seat->New_Computer->RGB_Canvas[r+1]*=Seat->New_Computer->Green_Color_Balance;
            Seat->New_Computer->RGB_Canvas[r+2]*=Seat->New_Computer->Blue_Color_Balance;
        }
        in += 1;
        r += 3;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256,256, 0, GL_RGB, GL_UNSIGNED_BYTE, Seat->New_Computer->RGB_Canvas);
    glEnable(GL_TEXTURE_2D);
};

void Turn_Computer_On(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Power_State = true;
    Seat->New_Computer->Processor.Processor_Halted = false;

    Seat->New_Computer->Color_Mode = true;

    Seat->New_Computer->Red_Color_Balance = 1;
    Seat->New_Computer->Green_Color_Balance = 1;
    Seat->New_Computer->Blue_Color_Balance =1;

    for(int j = 0; j < 256*256; j++)
    {
        Seat->New_Computer->Front_Video_Buffer[j] = 1;
        Seat->New_Computer->Back_Video_Buffer[j] = 1;
    }

    for(int j = 0; j < 256*256*3; j++)
    {
        Seat->New_Computer->RGB_Canvas[j] = 1;
    }

    for(int j = 0; j < 256*256*4; j++)
    {
        Seat->New_Computer->RGBA_Canvas[j] = 255;
    }

    Seat->New_Computer->Controller_D_Pad_RAM[0] = 0;
    Seat->New_Computer->Controller_D_Pad_RAM[1] = 0;
    Seat->New_Computer->Controller_D_Pad_RAM[2] = 0;
    Seat->New_Computer->Controller_D_Pad_RAM[3] = 0;

    Seat->New_Computer->Controller_Button_RAM[0] = 0;
    Seat->New_Computer->Controller_Button_RAM[1] = 0;

    for(int j = 0; j < 256; j++)
    {
        Seat->New_Computer->Keyboard_RAM[j] = 0;
    }

    Seat->New_Computer->Mouse_RAM[0] = 0;
    Seat->New_Computer->Mouse_RAM[1] = 0;
    Seat->New_Computer->Mouse_RAM[2] = 0;


    Seat->New_Computer->Current_ROM_Bank = 0;

    for(int j = 0; j < 256; j++)
    {
        for(int l = 0; l < 65536; l++)
        {
            Seat->New_Computer->ROM_Memory_Zero[j].Bank[l] = 0;
            Seat->New_Computer->ROM_Memory_One[j].Bank[l] = 0;
            Seat->New_Computer->RAM[j].Bank[l] = 0;
        }
    }

    Seat->New_Computer->Processor.Register_Zero = 0;
    Seat->New_Computer->Processor.Register_One = 0;
    Seat->New_Computer->Processor.Instruction_Pointer = 0;
    Seat->New_Computer->Processor.Stack_Pointer = 65535;

    Seat->New_Computer->Processor.Zero_Flag = 0;
    Seat->New_Computer->Processor.Carry_Flag = 0;
    Seat->New_Computer->Processor.Sign_Flag = 0;
    Seat->New_Computer->Processor.Overflow_Flag = 0;
    Seat->New_Computer->Processor.Parity_Flag = 0;
    Seat->New_Computer->Processor.Auxiliary_Carry_Flag = 0;
    Seat->New_Computer->Processor.Direction_Flag = 0;
    Seat->New_Computer->Processor.Maskable_Interrupt_Flag = 0;
    Seat->New_Computer->Processor.Trap_Flag = 0;

    Seat->New_Computer->Processor.Equals_Flag = 0;
    Seat->New_Computer->Processor.Less_Flag = 0;
    Seat->New_Computer->Processor.Greater_Flag = 0;

    Load_Program(Seat, "test.rom");
};


void Render_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(!Seat->New_Computer->Processor.Processor_Halted && Seat->New_Computer->Power_State)
    {
    glBindTexture(GL_TEXTURE_2D, Seat->New_Computer->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int in = 0;
    int r = 0;
    //Convert Video card memory to 24-bit color
    unsigned char Red_Value;
    unsigned char Green_Value;
    unsigned char Blue_Value;

    if(Seat->New_Computer->Color_Mode)
    {
    for(int j = 0; j < 65536; j++)
    {
            Red_Value = (Seat->New_Computer->Front_Video_Buffer[in] >> 0) & 0x03;
            Green_Value = (Seat->New_Computer->Front_Video_Buffer[in] >> 2) & 0x03;
            Blue_Value = (Seat->New_Computer->Front_Video_Buffer[in] >> 4) & 0x03;
            //Red_Value =  >> 0;

            //memcpy(&Seat->New_Computer->RGB_Canvas[r], &Seat->New_Computer->Front_Video_Buffer[in], 1);
            //memcpy(&Seat->New_Computer->RGB_Canvas[r+1], &Seat->New_Computer->Front_Video_Buffer[in], 1);
            //memcpy(&Seat->New_Computer->RGB_Canvas[r+2], &Seat->New_Computer->Front_Video_Buffer[in], 1);

            memcpy(&Seat->New_Computer->RGB_Canvas[r], &Blue_Value, 1);
            memcpy(&Seat->New_Computer->RGB_Canvas[r+1], &Green_Value, 1);
            memcpy(&Seat->New_Computer->RGB_Canvas[r+2], &Red_Value, 1);

            Seat->New_Computer->RGB_Canvas[r]*=Seat->New_Computer->Blue_Color_Balance;
            Seat->New_Computer->RGB_Canvas[r+1]*=Seat->New_Computer->Green_Color_Balance;
            Seat->New_Computer->RGB_Canvas[r+2]*=Seat->New_Computer->Red_Color_Balance;

        in += 1;
        r += 3;
    }
    }

    if(!Seat->New_Computer->Color_Mode)
    {
    for(int j = 0; j < 65536; j++)
    {
            Red_Value = (Seat->New_Computer->Front_Video_Buffer[in] >> 0) & 0x03;
            Green_Value = (Seat->New_Computer->Front_Video_Buffer[in] >> 2) & 0x03;
            Blue_Value = (Seat->New_Computer->Front_Video_Buffer[in] >> 4) & 0x03;
            //Red_Value =  >> 0;

            memcpy(&Seat->New_Computer->RGB_Canvas[r], &Seat->New_Computer->Front_Video_Buffer[in], 1);
            memcpy(&Seat->New_Computer->RGB_Canvas[r+1], &Seat->New_Computer->Front_Video_Buffer[in], 1);
            memcpy(&Seat->New_Computer->RGB_Canvas[r+2], &Seat->New_Computer->Front_Video_Buffer[in], 1);

            //memcpy(&Seat->New_Computer->RGB_Canvas[r], &Blue_Value, 1);
           // memcpy(&Seat->New_Computer->RGB_Canvas[r+1], &Green_Value, 1);
           // memcpy(&Seat->New_Computer->RGB_Canvas[r+2], &Red_Value, 1);

            Seat->New_Computer->RGB_Canvas[r]*=Seat->New_Computer->Blue_Color_Balance;
            Seat->New_Computer->RGB_Canvas[r+1]*=Seat->New_Computer->Green_Color_Balance;
            Seat->New_Computer->RGB_Canvas[r+2]*=Seat->New_Computer->Red_Color_Balance;

        in += 1;
        r += 3;
    }
    }



    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256,256, 0, GL_RGB, GL_UNSIGNED_BYTE, Seat->New_Computer->RGB_Canvas);
    glEnable(GL_TEXTURE_2D);
    }
};



void Write_File_Onto_ROM(struct _New_Compiler* New_Compiler, unsigned char Bank, unsigned short Address, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(&Buffer, sizeof(unsigned char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;

    if(File_Size > 0 && File_Size <= 65536)
    {
        if(Address + File_Size < 65536)
        {
        for(int j = 0; j < File_Size; j++)
        {
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+j] = Buffer[j];
        }
        New_Compiler->Current_ROM_Address += File_Size;
        }
    }
    //////printf("Wrote a file \n");
}

void Write_ROM_Memory_To_Program(const char* Filename)
{
    struct _New_Compiler* New_Compiler = (struct _New_Compiler*)calloc(1,sizeof(struct _New_Compiler));

    New_Compiler->Current_ROM_Address = 0;
    New_Compiler->Current_ROM_Bank = 0;

    Write_Opcode(New_Compiler,OPC_CLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,255,255,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,32,0); //Source Bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,1,0); //Dest Bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_LOAD_RAM_BANK,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,32,0); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,32,0); //x
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,0+3,0); //addr(SUB 3 bytes at start!)
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,1,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_IMAGE,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,64,0); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,250,0); //x
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,0+3,0); //addr(SUB 3 bytes at start!)
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,1,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_IMAGE,0,0);



    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_PIXEL,0,0);

    Write_Opcode(New_Compiler,OPC_FLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
    Write_Opcode(New_Compiler,OPC_HLT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);

    Load_Image_And_Convert_To_Bitmap("test.bmp", "test.dat");
    New_Compiler->Current_ROM_Bank += 1;
    New_Compiler->Current_ROM_Address = 0;
    Write_File_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address, "test.dat");

    unsigned char Number_Of_Banks = New_Compiler->Current_ROM_Bank+1;

    FILE* f = fopen(Filename, "wb");
    fwrite(&Number_Of_Banks,1,sizeof(unsigned char),f);
//    fwrite(&Start_Address,1,sizeof(unsigned short),f);
    for(int j = 0; j < New_Compiler->Current_ROM_Bank+1; j++)
    {
        fwrite(&New_Compiler->ROM_Memory_Zero[j], 1, sizeof(struct _RAM_Bank),f);
    }
    fclose(f);
};


void Write_Opcode(struct _New_Compiler* New_Compiler, unsigned char Code, unsigned char Bank, unsigned short Address, unsigned char Size, unsigned char Argument_One, unsigned char Argument_Two,unsigned char Argument_Three)
{
    switch(Size)
    {
        case 1:
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address] = Code;
            New_Compiler->Current_ROM_Address += 1;
        break;

        case 2:
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address] = Code;
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+1] = Argument_One;
            New_Compiler->Current_ROM_Address += 2;
        break;

        case 3:
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address] = Code;
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+1] = Argument_One;
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+2] = Argument_Two;
            New_Compiler->Current_ROM_Address += 3;
        break;

        case 4:
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address] = Code;
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+1] = Argument_One;
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+2] = Argument_Two;
            New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+3] = Argument_Three;
            New_Compiler->Current_ROM_Address += 4;
        break;
    }
};

bool Compile_AQ_BASIC_Program(const char* Filename, const char* Output)
{
    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    char Buffer[File_Size];
    fread(&Buffer, sizeof(char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;

    struct _New_Compiler* New_Compiler;

    New_Compiler = (struct _New_Compiler*)calloc(1,sizeof(struct _New_Compiler));

    New_Compiler->Current_ROM_Bank = 0;
    New_Compiler->Current_ROM_Address = 0;

    unsigned char Start_Bank = 0;
    unsigned short Start_Address = 0;

    struct _DBLOCK* DBLOCKS[65536];
    unsigned int Number_Of_DBLOCKS = 0;

    struct _VAR* VARS[65536];
    unsigned int Number_Of_VARS = 0;

    struct _LABEL* LABELS[65536];
    unsigned int Number_Of_LABELS = 0;

    struct _TILEMAP* TILEMAPS[65536];
    unsigned int Number_Of_TILEMAPS = 0;

    printf("The Current ROM Bank is %d. The Current ROM Address is %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);

    New_Compiler->Current_ROM_Bank = 1;

    for(int j = 0; j < File_Size; j++)
    {
        if(strncmp(&Buffer[j], "DATABLOCK",9) == 0)
        {
            printf("Found a data block.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            printf("After this is %c\n",Buffer[j + 10 + length + 1]);
            if(Buffer[j + 10 + length + 1] != 'B')
            {
                printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 10 + length + 2], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 2];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            printf("After this is %c\n",Buffer[j + 10 + length + banklength + 4]);
            if(Buffer[j + 10 + length + banklength + 4] != 'A')
            {
                printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 10 + length + 2 + banklength + 3], ";");
            printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 10 + x + length + 2 + banklength + 3];
                printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';

            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);

            printf("This DBlock lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            DBLOCKS[Number_Of_DBLOCKS] = (struct _DBLOCK*)calloc(1,sizeof(struct _DBLOCK));

            for(int x = 0; x < 16; x++)
            {
                DBLOCKS[Number_Of_DBLOCKS]->Name[x] = Data_Block_Name[x];
                printf("%c\n",DBLOCKS[Number_Of_DBLOCKS]->Name[x]);
            }
            DBLOCKS[Number_Of_DBLOCKS]->Bank = (unsigned char)Bank_Value;
            DBLOCKS[Number_Of_DBLOCKS]->Address = (unsigned short)Address_Value;
            printf("%d %d\n", DBLOCKS[Number_Of_DBLOCKS]->Bank,DBLOCKS[Number_Of_DBLOCKS]->Address);

            Number_Of_DBLOCKS += 1;
        }


        if(strncmp(&Buffer[j], "TILEMAP",7) == 0) //Bank, Address, Cell Width, Cell Height
        {
            printf("Found a data block.\n");
            size_t length = strcspn(&Buffer[j + 8], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 8 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            printf("After this is %c\n",Buffer[j + 8 + length + 1]);
            if(Buffer[j + 8 + length + 1] != 'B')
            {
                printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 8 + length + 2], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 8 + x + length + 2];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            printf("After this is %c\n",Buffer[j + 8 + length + banklength + 4]);
            if(Buffer[j + 8 + length + banklength + 4] != 'A')
            {
                printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 8 + length + 2 + banklength + 3], ";");
            printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 8 + x + length + 2 + banklength + 3];
                printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';









            size_t CellWidthlength = strcspn(&Buffer[j + 8 + length + 2 + banklength + 3 + Addresslength + 3], ";");
            printf("Its CellWidthlength is %zu\n", CellWidthlength); // Outputs 5
            if(CellWidthlength > 5)
            {
                printf("Build Failed. CellWidth Blocks must be within five digits.");
                return false;
            }

            char Cell_Width[16];
            memset(Cell_Width,0,16);
            for(int x = 0; x < CellWidthlength; x++)
            {
                Cell_Width[x] = Buffer[j + 8 + x + length + 2 + banklength + 3 + Addresslength + 3];
                printf("%c\n", Cell_Width[x]);
            }
            Cell_Width[CellWidthlength] = '\0';





            size_t CellHeightlength = strcspn(&Buffer[j + 8 + length + 2 + banklength + 3 + Addresslength + 3 + CellWidthlength + 3], ";");
            printf("Its CellHeightlength is %zu\n", CellHeightlength); // Outputs 5
            if(CellHeightlength > 5)
            {
                printf("Build Failed. CellHeight Blocks must be within five digits.");
                return false;
            }

            char Cell_Height[16];
            memset(Cell_Height,0,16);
            for(int x = 0; x < CellHeightlength; x++)
            {
                Cell_Height[x] = Buffer[j + 8 + x + length + 2 + banklength + 3 + Addresslength + 3 + CellWidthlength + 3];
                printf("%c\n", Cell_Height[x]);
            }
            Cell_Height[CellHeightlength] = '\0';




            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);
            long Cell_Width_Value = strtol(Cell_Width, NULL, 10);
            long Cell_Height_Value = strtol(Cell_Height, NULL, 10);

            printf("This DBlock lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            TILEMAPS[Number_Of_TILEMAPS] = (struct _TILEMAP*)calloc(1,sizeof(struct _TILEMAP));

            for(int x = 0; x < 16; x++)
            {
                TILEMAPS[Number_Of_TILEMAPS]->Name[x] = Data_Block_Name[x];
                printf("%c\n",TILEMAPS[Number_Of_TILEMAPS]->Name[x]);
            }
            TILEMAPS[Number_Of_TILEMAPS]->Bank = (unsigned char)Bank_Value;
            TILEMAPS[Number_Of_TILEMAPS]->Address = (unsigned short)Address_Value;
            TILEMAPS[Number_Of_TILEMAPS]->Cell_Width = (unsigned char)Cell_Width_Value;
            TILEMAPS[Number_Of_TILEMAPS]->Cell_Height = (unsigned char)Cell_Height_Value;
            printf("%d %d\n", TILEMAPS[Number_Of_TILEMAPS]->Bank,TILEMAPS[Number_Of_TILEMAPS]->Address);

            printf("Width %d Height %d\n", TILEMAPS[Number_Of_TILEMAPS]->Cell_Width,TILEMAPS[Number_Of_TILEMAPS]->Cell_Height);
            Number_Of_TILEMAPS += 1;
        }


        if(strncmp(&Buffer[j], "VALUE",5) == 0)
        {
            printf("Found a value.\n");
            size_t length = strcspn(&Buffer[j + 6], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            printf("After this is %c\n",Buffer[j + 6 + length + 1]);
            if(Buffer[j + 6 + length + 1] != 'B')
            {
                printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 6 + length + 2], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 6 + x + length + 2];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            printf("After this is %c\n",Buffer[j + 6 + length + banklength + 4]);
            if(Buffer[j + 6 + length + banklength + 4] != 'A')
            {
                printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 6 + length + 2 + banklength + 3], ";");
            printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 6 + x + length + 2 + banklength + 3];
                printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';

            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);

            printf("This Var lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            VARS[Number_Of_VARS] = (struct _VAR*)calloc(1,sizeof(struct _VAR));

            for(int x = 0; x < 16; x++)
            {
                VARS[Number_Of_VARS]->Name[x] = Data_Block_Name[x];
                printf("%c\n",VARS[Number_Of_VARS]->Name[x]);
            }
            VARS[Number_Of_VARS]->Bank = (unsigned char)Bank_Value;
            VARS[Number_Of_VARS]->Address = (unsigned short)Address_Value;
            printf("%d %d\n", VARS[Number_Of_VARS]->Bank,VARS[Number_Of_VARS]->Address);

            Number_Of_VARS += 1;
        }





    if(strncmp(&Buffer[j], "SETVARVAL",9) == 0)
        {
            printf("Found a Set Variable to Value.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_SETVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        printf("Error: Could not handle SETVARVAL command.\n");
                        return false;
                    }
                    printf("I will SETVARVAL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }









        if(strncmp(&Buffer[j], "SUBVARVAL",9) == 0)
        {
            printf("Found a Sub Val from Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_SUBVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        printf("Error: Could not handle SUBVVL command.\n");
                        return false;
                    }
                    printf("I will SUBVVL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }



        if(strncmp(&Buffer[j], "RENTILES",8) == 0)
        {
            printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 9], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 9 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 9 + length + 1], " ");
            printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 9 + x + length + 1];
                printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1], " ");
            printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1];
                printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';




            size_t Encvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1 + yvar + 1], " ");
            printf("Its Encvar is %zu\n", Encvar); // Outputs 5

            char EncString[16];
            memset(EncString,0,16);
            for(int x = 0; x < Encvar; x++)
            {
                EncString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1 + yvar + 1];
                printf("%c\n", EncString[x]);
            }
            EncString[Encvar] = '\0';




            size_t EncLengthvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1], " ");
            printf("Its EncLengthvar is %zu\n", EncLengthvar); // Outputs 5

            char EncLengthString[16];
            memset(EncLengthString,0,16);
            for(int x = 0; x < EncLengthvar; x++)
            {
                EncLengthString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1];
                printf("%c\n", EncLengthString[x]);
            }
            EncLengthString[EncLengthvar] = '\0';




            size_t TPRvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1 + EncLengthvar + 1], " ");
            printf("Its TPRvar is %zu\n", TPRvar); // Outputs 5

            char TPRString[16];
            memset(TPRString,0,16);
            for(int x = 0; x < TPRvar; x++)
            {
                TPRString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1 + EncLengthvar + 1];
                printf("%c\n", TPRString[x]);
            }
            TPRString[TPRvar] = '\0';


            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char XBank;
            unsigned short XAddress;

            unsigned char YBank;
            unsigned short YAddress;

            unsigned char EncodingBank;
            unsigned short EncodingAddress;

            unsigned char EncodingLengthBank;
            unsigned short EncodingLengthAddress;

            unsigned char TPRBank;
            unsigned short TPRAddress;

            unsigned char TileWidth;
            unsigned char TileHeight;

            //Find the label.
            for(int r = 0; r < Number_Of_TILEMAPS+1; r++)
            {
                if(r == Number_Of_TILEMAPS)
                {
                    printf("Error: Could not find RENDER's parent TILEMAP.\n");
                    return false;
                }

                if(strncmp(TILEMAPS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found SETVARVAR's first VAR!\n");
                    Bank = TILEMAPS[r]->Bank;
                    Address = TILEMAPS[r]->Address;
                    TileWidth = TILEMAPS[r]->Cell_Width;
                    TileHeight = TILEMAPS[r]->Cell_Height;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    printf("Found RENDER's second VAR!\n");
                    XBank = VARS[r]->Bank;
                    XAddress = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    printf("Found RENDER's second VAR!\n");
                    YBank = VARS[r]->Bank;
                    YAddress = VARS[r]->Address;
                    break;
                }
            }




            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, EncString,16) == 0)
                {
                    printf("Found RENTILES's encoding Var\n");
                    EncodingBank = VARS[r]->Bank;
                    EncodingAddress = VARS[r]->Address;
                    break;
                }
            }









            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, EncLengthString,16) == 0)
                {
                    printf("Found RENTILES's encoding Var\n");
                    EncodingLengthBank = VARS[r]->Bank;
                    EncodingLengthAddress = VARS[r]->Address;
                    break;
                }
            }







            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, TPRString,16) == 0)
                {
                    printf("Found RENTILES's encoding Var\n");
                    TPRBank = VARS[r]->Bank;
                    TPRAddress = VARS[r]->Address;
                    break;
                }
            }

            printf("Bank %d Address %d XBank %d YBank %d EncBank %d EncAddr %d EncLBank %d EncLAddr %d TPRBank %d TPRADDR %d, TileWidth %d TileHeight %d\n",Bank,Address,XBank,XAddress,YBank,YAddress,EncodingBank,EncodingAddress,EncodingLengthBank,EncodingLengthAddress,TPRBank,TPRAddress,TileWidth,TileHeight);



            //if(!Handle_Aqueous_Basic_Text_RENTILEMAP(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,XBank,XAddress,YBank,YAddress, EncodingBank,EncodingAddress,EncodingLengthBank, EncodingLengthAddress,TPRBank,TPRAddress))
            {
                printf("Build failed: Could not insert RENDER\n");
            }
            //printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }


        if(strncmp(&Buffer[j], "ADDVARVAL",9) == 0)
        {
            printf("Found an Add Val to Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_ADDVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        printf("Error: Could not handle ADDVVL command.\n");
                        return false;
                    }
                    printf("I will ADDVVL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }


        if(strncmp(&Buffer[j], "RENDER",6) == 0)
        {
            printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 7], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 7 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 7 + length + 1], " ");
            printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 7 + x + length + 1];
                printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 7 + length + 1 + xvar + 1], ";");
            printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 7 + x + length + 1 + xvar + 1];
                printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';



            unsigned char Bank;
            unsigned short Address;

            unsigned char XBank;
            unsigned short XAddress;

            unsigned char YBank;
            unsigned short YAddress;

            //Find the label.
            for(int r = 0; r < Number_Of_DBLOCKS+1; r++)
            {
                if(r == Number_Of_DBLOCKS)
                {
                    printf("Error: Could not find RENDER's parent DBLOCK.\n");
                    return false;
                }

                if(strncmp(DBLOCKS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found SETVARVAR's first VAR!\n");
                    Bank = DBLOCKS[r]->Bank;
                    Address = DBLOCKS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    printf("Found RENDER's second VAR!\n");
                    XBank = VARS[r]->Bank;
                    XAddress = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    printf("Found RENDER's second VAR!\n");
                    YBank = VARS[r]->Bank;
                    YAddress = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_RENDER(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,XBank,XAddress,YBank,YAddress))
            {
                printf("Build failed: Could not insert RENDER\n");
            }
            //printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }




        if(strncmp(&Buffer[j], "GOIFVAREQVAR",12) == 0)
        {
            printf("Found a Goto if Var Equals Var.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';



            unsigned char Bank;
            unsigned short Address;

            unsigned char XBank;
            unsigned short XAddress;

            unsigned char YBank;
            unsigned short YAddress;

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find RENDER's parent LABEL.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found SETVARVAR's first VAR!\n");
                    Bank = LABELS[r]->Bank;
                    Address = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    printf("Found RENDER's second VAR!\n");
                    XBank = VARS[r]->Bank;
                    XAddress = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    printf("Found RENDER's second VAR!\n");
                    YBank = VARS[r]->Bank;
                    YAddress = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVAREQVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,XBank,XAddress,YBank,YAddress))
            {
                printf("Build failed: Could not insert RENDER\n");
            }
            //printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }





        if(strncmp(&Buffer[j], "JUMPEX",6) == 0)
        {
            printf("Found a Jump Execution to.\n");
            size_t xvar = strcspn(&Buffer[j + 7 + 0 + 1], " ");
            printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 7 + x + 0 + 1];
                printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';

            size_t yvar = strcspn(&Buffer[j + 7 + 0 + 1 + xvar + 1], ";");
            printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 7 + x + 0 + 1 + xvar + 1];
                printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';


            unsigned char Bank_Bank;
            unsigned short Bank_Address;

            unsigned char Address_Bank;
            unsigned short Address_Address;

            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    printf("Found Jumpexes Bank Location!\n");
                    Bank_Bank = VARS[r]->Bank;
                    Bank_Address = VARS[r]->Address;
                    break;
                }
            }


            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    printf("Found Jumpexes Address Location!\n");
                    Address_Bank = VARS[r]->Bank;
                    Address_Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_JUMPEX(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank_Bank,Bank_Address,Address_Bank,Address_Address))
            {
                printf("Build failed: Could not insert JUMPEX\n");
            }
            //printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }




        if(strncmp(&Buffer[j], "SETVARVAR",9) == 0)
        {
            printf("Found a Set Variable to Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            unsigned char ParentOneBank;
            unsigned short ParentOneAddress;

            unsigned char ParentTwoBank;
            unsigned short ParentTwoAddress;

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found SETVARVAR's first VAR!\n");
                    ParentOneBank = VARS[r]->Bank;
                    ParentOneAddress = VARS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    printf("Error: Could not find SETVARVAL's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Bank,16) == 0)
                {
                    printf("Found SETVARVAR's second VAR!\n");
                    ParentTwoBank = VARS[r]->Bank;
                    ParentTwoAddress = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_SETVVR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress))
            {
                printf("Build failed: Could not insert SETVVR\n");
            }
            printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }






        if(strncmp(&Buffer[j], "LABEL",5) == 0)
        {
            printf("Found a Label.\n");
            size_t length = strcspn(&Buffer[j + 6], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Allocate memory.
            LABELS[Number_Of_LABELS] = (struct _LABEL*)calloc(1,sizeof(struct _LABEL));

            for(int x = 0; x < 16; x++)
            {
                LABELS[Number_Of_LABELS]->Name[x] = Data_Block_Name[x];
                printf("%c\n",LABELS[Number_Of_LABELS]->Name[x]);
            }
            LABELS[Number_Of_LABELS]->Bank = New_Compiler->Current_ROM_Bank;
            LABELS[Number_Of_LABELS]->Address = New_Compiler->Current_ROM_Address;
            printf("%d %d\n", LABELS[Number_Of_LABELS]->Bank,LABELS[Number_Of_LABELS]->Address);

            Number_Of_LABELS += 1;
        }


        if(strncmp(&Buffer[j], "STARTLBL",8) == 0)
        {
            printf("Found a Start Label.\n");
            //Allocate memory.
            LABELS[Number_Of_LABELS] = (struct _LABEL*)calloc(1,sizeof(struct _LABEL));

            LABELS[Number_Of_LABELS]->Bank = New_Compiler->Current_ROM_Bank;
            LABELS[Number_Of_LABELS]->Address = New_Compiler->Current_ROM_Address;
            printf("%d %d\n", LABELS[Number_Of_LABELS]->Bank,LABELS[Number_Of_LABELS]->Address);

            Start_Address = LABELS[Number_Of_LABELS]->Address;

            Number_Of_LABELS += 1;
        }


        if(strncmp(&Buffer[j], "GOSUB",5) == 0)
        {
            printf("Found a Gosub.\n");
            size_t length = strcspn(&Buffer[j + 6], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOSUB's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOSUB(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOSUB command.\n");
                        return false;
                    }
                    printf("I will GOSUB Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }




        if(strncmp(&Buffer[j], "GOIFCTRLA",9) == 0)
        {
            printf("Found a Goto if CTRL DOWN pressed.\n");
            size_t length = strcspn(&Buffer[j + 10], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOIFCTRLDOWN's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLA(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOIFCTRLDOWN command.\n");
                        return false;
                    }
                    printf("I will GOIFCTRLDOWN Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }






        if(strncmp(&Buffer[j], "GOIFCTRLB",9) == 0)
        {
            printf("Found a Goto if CTRL DOWN pressed.\n");
            size_t length = strcspn(&Buffer[j + 10], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOIFCTRLDOWN's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLB(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOIFCTRLDOWN command.\n");
                        return false;
                    }
                    printf("I will GOIFCTRLDOWN Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }



        if(strncmp(&Buffer[j], "GOIFCTRLDOWN",12) == 0)
        {
            printf("Found a Goto if CTRL DOWN pressed.\n");
            size_t length = strcspn(&Buffer[j + 13], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOIFCTRLDOWN's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLDOWN(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOIFCTRLDOWN command.\n");
                        return false;
                    }
                    printf("I will GOIFCTRLDOWN Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }

        if(strncmp(&Buffer[j], "GOIFCTRLRIGHT",13) == 0)
        {
            printf("Found a Goto if CTRL RIGHT pressed.\n");
            size_t length = strcspn(&Buffer[j + 14], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOIFCTRLUP's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLRIGHT(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOIFCTRLUP command.\n");
                        return false;
                    }
                    printf("I will GOIFCTRLUP Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }


        if(strncmp(&Buffer[j], "GOIFCTRLLEFT",12) == 0)
        {
            printf("Found a Goto if CTRL LEFT pressed.\n");
            size_t length = strcspn(&Buffer[j + 13], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOIFCTRLUP's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLLEFT(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOIFCTRLUP command.\n");
                        return false;
                    }
                    printf("I will GOIFCTRLUP Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }



if(strncmp(&Buffer[j], "GOIFCTRLUP",10) == 0)
        {
            printf("Found a Goto if CTRL UP pressed.\n");
            size_t length = strcspn(&Buffer[j + 11], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 11 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOIFCTRLUP's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLUP(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOIFCTRLUP command.\n");
                        return false;
                    }
                    printf("I will GOIFCTRLUP Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }

        if(strncmp(&Buffer[j], "GOTO",4) == 0)
        {
            printf("Found a Goto.\n");
            size_t length = strcspn(&Buffer[j + 5], ";");
            printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 5 + x];
                printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    printf("Error: Could not find GOTO's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOTO(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        printf("Error: Could not handle GOTO command.\n");
                        return false;
                    }
                    printf("I will GOTO Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }


        if(strncmp(&Buffer[j], "RETURN",6) == 0)
        {
            printf("Found a RETURN.\n");
            if(!Handle_Aqueous_Basic_Text_RETURN(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                printf("Error: Could not handle Return command.\n");
                return false;
            }
        }



        if(strncmp(&Buffer[j], "FLS",3) == 0)
        {
            printf("Found an FLS.\n");
            if(!Handle_Aqueous_Basic_Text_FLS(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                printf("Error: Could not handle FLS command.\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "CMODE",5) == 0)
        {
            printf("Found a COLOR MODE.\n");
            if(!Handle_Aqueous_Basic_Text_COLORMODE(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                printf("Error: Could not handle CMODE command.\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "MMODE",5) == 0)
        {
            printf("Found a MONOCHROME.\n");
            if(!Handle_Aqueous_Basic_Text_MONOCHROMEMODE(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                printf("Error: Could not handle MMODE command.\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "CLS",3) == 0)
        {
            printf("Found a CLS.\n");
            if(!Handle_Aqueous_Basic_Text_CLS(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                printf("Error: Could not handle CLS command.\n");
                return false;
            }
        }
    }

    New_Compiler->Current_ROM_Bank+=1;
    New_Compiler->Current_ROM_Address = 0;
    Write_File_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address, "test.dat");

    unsigned char Number_Of_Banks = New_Compiler->Current_ROM_Bank+1;



    FILE* ftwo = fopen(Output, "wb");
    fwrite(&Number_Of_Banks,1,sizeof(unsigned char),ftwo);
    fwrite(&Start_Address,1,sizeof(unsigned short),ftwo);
    for(int j = 0; j < New_Compiler->Current_ROM_Bank+1; j++)
    {
        fwrite(&New_Compiler->ROM_Memory_Zero[j], 1, sizeof(struct _RAM_Bank),ftwo);
    }
    fclose(ftwo);

    printf("Compile complete.\n");
    return true;
};

bool Handle_Aqueous_Basic_Text_GOIFCTRLRIGHT(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

    Write_Opcode(New_Compiler,OPC_IN_DPAD_RIGHT_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}


bool Handle_Aqueous_Basic_Text_GOIFCTRLLEFT(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

    Write_Opcode(New_Compiler,OPC_IN_DPAD_LEFT_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}




bool Handle_Aqueous_Basic_Text_GOIFCTRLA(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

    Write_Opcode(New_Compiler,OPC_IN_A_BUTTON_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}



bool Handle_Aqueous_Basic_Text_GOIFCTRLB(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

    Write_Opcode(New_Compiler,OPC_IN_B_BUTTON_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}



bool Handle_Aqueous_Basic_Text_GOIFCTRLUP(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

    Write_Opcode(New_Compiler,OPC_IN_DPAD_UP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}

bool Handle_Aqueous_Basic_Text_GOIFCTRLDOWN(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t high_byte = 0;
    uint8_t low_byte = 0;

    Write_Opcode(New_Compiler,OPC_IN_DPAD_DOWN_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}

bool Handle_Aqueous_Basic_Text_GOTO(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;
    Write_Opcode(New_Compiler,OPC_JMP,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}


bool Handle_Aqueous_Basic_Text_JUMPEX(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank_Bank,unsigned short Bank_Address,unsigned char Address_Bank,unsigned short Address_Address)
{
    uint8_t baddrlow_byte = Bank_Address & 0xFF;
    uint8_t baddrhigh_byte = (Bank_Address >> 8) & 0xFF;

    uint8_t aaddrlow_byte = Address_Address & 0xFF;
    uint8_t aaddrhigh_byte = (Address_Address >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank_Bank,baddrhigh_byte,baddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Address_Bank,aaddrhigh_byte,aaddrlow_byte);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_JMP_STACK,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    return true;
}


bool Handle_Aqueous_Basic_Text_GOSUB(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address)
{
    //Similar to JMP, but pushes the current Bank and Address.
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    printf("I will call %d %d\n", Bank, Address);

    Write_Opcode(New_Compiler,OPC_CALL,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    return true;
}

bool Handle_Aqueous_Basic_Text_RETURN(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address)
{
    //Similar to JMP, but pushes the current Bank and Address.
    Write_Opcode(New_Compiler,OPC_RETURN,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    return true;
}


bool Handle_Aqueous_Basic_Text_SUBVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value)
{
    uint8_t low_byte = New_Value & 0xFF;
    uint8_t high_byte = (New_Value >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_SUB_R1_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y

    printf("%d is going into r0.\n",New_Value);

    return true;
}


bool Handle_Aqueous_Basic_Text_ADDVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value)
{
    uint8_t low_byte = New_Value & 0xFF;
    uint8_t high_byte = (New_Value >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_ADD_R1_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y

    printf("%d is going into r0.\n",New_Value);

    return true;
}

bool Handle_Aqueous_Basic_Text_SETVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value)
{
    uint8_t low_byte = New_Value & 0xFF;
    uint8_t high_byte = (New_Value >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y

    printf("%d is going into r0.\n",New_Value);

    return true;
}

bool Handle_Aqueous_Basic_Text_SETVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t addronelow_byte = Address & 0xFF;
    uint8_t addronehigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrtwolow_byte = AddressTwo & 0xFF;
    uint8_t addrtwohigh_byte = (AddressTwo >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrtwohigh_byte,addrtwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addronehigh_byte,addronelow_byte); //y

    return true;
}

bool Handle_Aqueous_Basic_Text_RENDER(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned short xAddress, unsigned char yBank, unsigned short yAddress)
{
    unsigned char Total_Opcode_Length = 4+2 + 4+2 + 4+2 + 4+2 + 2;

    uint8_t DBLOCKaddrlow_byte = DBLOCKAddr & 0xFF;
    uint8_t DBLOCKaddrhigh_byte = (DBLOCKAddr >> 8) & 0xFF;

    uint8_t xaddrlow_byte = xAddress & 0xFF;
    uint8_t xaddrhigh_byte = (xAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = yAddress & 0xFF;
    uint8_t yaddrhigh_byte = (yAddress >> 8) & 0xFF;

    //y x ADDR Bank

    if(Current_ROM_Address < 65535-Total_Opcode_Length)
    {
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,yBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,xBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,DBLOCKaddrhigh_byte,DBLOCKaddrlow_byte,0); //addr(SUB 3 bytes at start!)
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,DBLOCKBank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_IMAGE,0,0);
    return true;
    }
};

bool Handle_Aqueous_Basic_Text_FLS(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address)
{
    if(Current_ROM_Address < 65535)
    {
        printf("Wrote FLS Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_FLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            printf("ROM Cartridge out of memory!\n");
            return false;
        }
        New_Compiler->Current_ROM_Bank+=1;
        New_Compiler->Current_ROM_Address=0;
        Write_Opcode(New_Compiler,OPC_FLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
}

bool Handle_Aqueous_Basic_Text_CLS(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address)
{
    if(Current_ROM_Address < 65535)
    {
        printf("Wrote CLS Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_CLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            printf("ROM Cartridge out of memory!\n");
            return false;
        }
        New_Compiler->Current_ROM_Bank+=1;
        New_Compiler->Current_ROM_Address=0;
        Write_Opcode(New_Compiler,OPC_CLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
}

bool Handle_Aqueous_Basic_Text_COLORMODE(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address)
{
    if(Current_ROM_Address < 65535)
    {
        printf("Wrote COLORMODE Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_COLORMODE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            printf("ROM Cartridge out of memory!\n");
            return false;
        }
        New_Compiler->Current_ROM_Bank+=1;
        New_Compiler->Current_ROM_Address=0;
        Write_Opcode(New_Compiler,OPC_COLORMODE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
}



bool Handle_Aqueous_Basic_Text_MONOCHROMEMODE(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address)
{
    if(Current_ROM_Address < 65535)
    {
        printf("Wrote MONOCHROMEMODE Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_MONOCHROMEMODE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            printf("ROM Cartridge out of memory!\n");
            return false;
        }
        New_Compiler->Current_ROM_Bank+=1;
        New_Compiler->Current_ROM_Address=0;
        Write_Opcode(New_Compiler,OPC_MONOCHROMEMODE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
}

void BMPtoSMI(const char* Filename, const char* Output)
{
    //SMI is the emulator's image format.
    //It can use:
    //8-bit greyscale, with no alpha channel. 256 colors.
    //7-bit greyscale, with last bit for transparency. 128 colors.
    //6-bit color, with 4 levels of alpha transparency. 64 colors.
    //4-bit
    //2-bit
    //1-bit
};

void Load_Program(struct _Seat_Object* Seat, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(&Buffer, sizeof(unsigned char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;

    unsigned char Number_Of_Banks = Buffer[0];
    unsigned short Start_Address=(Buffer[2] << 8) | (Buffer[1] & 0xff);

    printf("Number of Banks: %d\n", Number_Of_Banks);
    printf("Start Address: %d\n", Start_Address);

    Buffer_Location += 3;

    for(int x = 0; x < Number_Of_Banks; x++)
    {
        for(int l = 0; l < 65536; l++)
        {
            Seat->New_Computer->ROM_Memory_Zero[x].Bank[l] = Buffer[Buffer_Location+l];
            Seat->New_Computer->RAM[x].Bank[l] = Buffer[Buffer_Location+l];
        }
        Buffer_Location += 65536;
    }


    Seat->New_Computer->Current_RAM_Bank = 1;
    Seat->New_Computer->Processor.Instruction_Pointer = Start_Address;

    printf("Starting Execution at %d\n", Start_Address);
};


void Process_INT_Shutdown_Computer(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Turn_Computer_Off(Engine, Seat);
};


void Process_INT_Restart_Computer(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Turn_Computer_Off(Engine, Seat);
//    Set_Universal_Computer_Timer(Engine, Seat);
};


void Process_INT_Render_Pixel(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char x;
    unsigned char y;
    unsigned char Color_Value;
    x = 0;
    y = 0;
    Color_Value = 255;

    if(x < 256 && y < 256)
    {
        Copy_Pixel(1, &Color_Value, Seat->New_Computer->Back_Video_Buffer, 1, 1, 256, 256, 0, 0, x, y);
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }

};


void Process_INT_Render_Line(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};



void Process_INT_Load_RAM_Bank(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char Source_Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2];
    unsigned char Destination_Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+0];

    printf("The Source is %d\n", Source_Bank);
    printf("The Destination is %d\n", Destination_Bank);

    for(int j = 0; j < 65536; j++)
    {
        Seat->New_Computer->RAM[Destination_Bank].Bank[j] = Seat->New_Computer->ROM_Memory_Zero[Source_Bank].Bank[j];
    }

    if(Seat->New_Computer->Processor.Stack_Pointer + 4 < 65536)
    {
    Seat->New_Computer->Processor.Stack_Pointer += 4;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


void Process_INT_Render_Image(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char Bank;
    unsigned short Address;
    unsigned char x;
    unsigned char y;

    //Push Y == SP + 6
    //X = SP + 4 & +5
    //Addr =SP+2,SP+3
    //Bank = SP, SP+1

    Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer];
    Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] & 0xff);
    x = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+4];
    y = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+6];

    printf("The Bank is %d\n", Bank);
    printf("The Address is %d\n", Address);
    printf("The x is %d\n", x);
    printf("The y is %d\n", y);

    unsigned char Width;
    unsigned char Height;
    unsigned char Color_Mode;

    Width = Seat->New_Computer->RAM[Bank].Bank[Address];
    Height = Seat->New_Computer->RAM[Bank].Bank[Address+1];
    Color_Mode = Seat->New_Computer->RAM[Bank].Bank[Address+2];

    printf("The Width is %d\n", Width);
    printf("The Height is %d\n", Height);
    printf("The Color Mode is %d\n", Color_Mode);
    unsigned char New_Width, New_Height;

    unsigned char Actual_Width, Actual_Height;

    bool Only_Copy_A_Piece = false;

    if(x < 256 && y < 256)
    {
    if(x + Width > 256)
    {
        New_Width = (x+Width)-256;
        Actual_Width = New_Width;
        Only_Copy_A_Piece = true;
    }

    if(y + Height > 256)
    {
        New_Height = (y+Height)-256;
        Actual_Height = New_Height;
        Only_Copy_A_Piece = true;
    }

    unsigned char Array[Width*Height];

    for(int j = 0; j < Width*Height; j++)
    {
        if(Address + 3 + j < 65536)
        {
            Array[j] = Seat->New_Computer->RAM[Bank].Bank[Address+3+j];
        }
    }




    if(Only_Copy_A_Piece)
    {
    Copy_Section(1,Array,Seat->New_Computer->Back_Video_Buffer,Width,Height,256,256,0,0,x,y,Width-Actual_Width,Height-Actual_Height);
    }

    else if(!Only_Copy_A_Piece)
    {
    Copy_Section(1,Array,Seat->New_Computer->Back_Video_Buffer,Width,Height,256,256,0,0,x,y,Width,Height);
    }
    }

    if(Seat->New_Computer->Processor.Stack_Pointer + 8 < 65536)
    {
    Seat->New_Computer->Processor.Stack_Pointer += 8;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};



void Process_Opcode_IN_DPAD_UP_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[0];

    printf("Register Zero now equals %d\n", Seat->New_Computer->Processor.Register_Zero);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_DPAD_DOWN_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[1];

    printf("Register Zero now equals %d\n", Seat->New_Computer->Processor.Register_Zero);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_DPAD_LEFT_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[2];

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_DPAD_RIGHT_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[3];

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_A_BUTTON_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_Button_RAM[0];

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_B_BUTTON_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_Button_RAM[1];

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_KBD_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_MOUSE_X_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[0];
    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_MOUSE_Y_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    //Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[0];
    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_MOUSE_CLICK_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
   // Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[0];
   if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};


bool Handle_Aqueous_Basic_Text_GOIFVARGVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned char xAddress, unsigned char yBank, unsigned char yAddress)
{
    unsigned char Total_Opcode_Length = 4+2 + 4+2 + 4+2 + 4+2 + 2;

    uint8_t DBLOCKaddrlow_byte = DBLOCKAddr & 0xFF;
    uint8_t DBLOCKaddrhigh_byte = (DBLOCKAddr >> 8) & 0xFF;

    uint8_t xaddrlow_byte = xAddress & 0xFF;
    uint8_t xaddrhigh_byte = (xAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = yAddress & 0xFF;
    uint8_t yaddrhigh_byte = (yAddress >> 8) & 0xFF;

    //y x ADDR Bank

    if(Current_ROM_Address < 65535-Total_Opcode_Length)
    {
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,yBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,xBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,DBLOCKBank,DBLOCKaddrhigh_byte,DBLOCKaddrlow_byte); //y
    return true;
    }
};



bool Handle_Aqueous_Basic_Text_GOIFVARLVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned char xAddress, unsigned char yBank, unsigned char yAddress)
{
    unsigned char Total_Opcode_Length = 4+2 + 4+2 + 4+2 + 4+2 + 2;

    uint8_t DBLOCKaddrlow_byte = DBLOCKAddr & 0xFF;
    uint8_t DBLOCKaddrhigh_byte = (DBLOCKAddr >> 8) & 0xFF;

    uint8_t xaddrlow_byte = xAddress & 0xFF;
    uint8_t xaddrhigh_byte = (xAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = yAddress & 0xFF;
    uint8_t yaddrhigh_byte = (yAddress >> 8) & 0xFF;

    //y x ADDR Bank

    if(Current_ROM_Address < 65535-Total_Opcode_Length)
    {
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,yBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,xBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CL,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,DBLOCKBank,DBLOCKaddrhigh_byte,DBLOCKaddrlow_byte); //y
    return true;
    }
};




bool Handle_Aqueous_Basic_Text_GOIFVAREQVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char DBLOCKBank, unsigned short DBLOCKAddr, unsigned char xBank, unsigned char xAddress, unsigned char yBank, unsigned char yAddress)
{
    unsigned char Total_Opcode_Length = 4+2 + 4+2 + 4+2 + 4+2 + 2;

    uint8_t DBLOCKaddrlow_byte = DBLOCKAddr & 0xFF;
    uint8_t DBLOCKaddrhigh_byte = (DBLOCKAddr >> 8) & 0xFF;

    uint8_t xaddrlow_byte = xAddress & 0xFF;
    uint8_t xaddrhigh_byte = (xAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = yAddress & 0xFF;
    uint8_t yaddrhigh_byte = (yAddress >> 8) & 0xFF;

    //y x ADDR Bank

    if(Current_ROM_Address < 65535-Total_Opcode_Length)
    {
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,yBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,xBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,DBLOCKBank,DBLOCKaddrhigh_byte,DBLOCKaddrlow_byte); //y
    return true;
    }
};

void Process_Opcode_AND(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint16_t result = Seat->New_Computer->Processor.Register_Zero & Seat->New_Computer->Processor.Register_One;

    Seat->New_Computer->Processor.Carry_Flag = 0;
    Seat->New_Computer->Processor.Overflow_Flag = 0;

    Seat->New_Computer->Processor.Zero_Flag = (result == 0);
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_Zero = result;
};

void Process_Opcode_OR(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint16_t result = Seat->New_Computer->Processor.Register_Zero | Seat->New_Computer->Processor.Register_One;

    Seat->New_Computer->Processor.Carry_Flag = 0;
    Seat->New_Computer->Processor.Overflow_Flag = 0;

    Seat->New_Computer->Processor.Zero_Flag = (result == 0);
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_Zero = result;
};

void Process_Opcode_XOR(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    uint16_t result = Seat->New_Computer->Processor.Register_Zero ^ Seat->New_Computer->Processor.Register_One;

    Seat->New_Computer->Processor.Carry_Flag = 0;
    Seat->New_Computer->Processor.Overflow_Flag = 0;

    Seat->New_Computer->Processor.Zero_Flag = (result == 0);
    Seat->New_Computer->Processor.Sign_Flag = (result >> 15) & 1;
    Seat->New_Computer->Processor.Parity_Flag = parity8(result & 0xFF);

    Seat->New_Computer->Processor.Register_Zero = result;
};














void Process_INT_Render_Tilemap(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char Tiles_Per_Row;
    unsigned short Encoding_Length;

    unsigned char EncodingBank;
    unsigned short EncodingAddress;

    unsigned char Bank;
    unsigned short Address;
    unsigned char x;
    unsigned char y;

    //Push Y == SP + 6
    //X = SP + 4 & +5
    //Addr =SP+2,SP+3
    //Bank = SP, SP+1

    Tiles_Per_Row = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer]; //0,1
    Encoding_Length = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] & 0xff);

    EncodingBank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+4]; //4,5
    EncodingAddress = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+7] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+6] & 0xff);

    Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+8]; //8,9
    Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+11] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+10] & 0xff);
    x = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+12];
    y = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+14];

    printf("The Bank is %d\n", Bank);
    printf("The Address is %d\n", Address);
    printf("The x is %d\n", x);
    printf("The y is %d\n", y);

    unsigned char Width;
    unsigned char Height;
    unsigned char Color_Mode;

    Width = Seat->New_Computer->RAM[Bank].Bank[Address];
    Height = Seat->New_Computer->RAM[Bank].Bank[Address+1];
    Color_Mode = Seat->New_Computer->RAM[Bank].Bank[Address+2];

    printf("The Width is %d\n", Width);
    printf("The Height is %d\n", Height);
    printf("The Color Mode is %d\n", Color_Mode);
    unsigned char New_Width, New_Height;

    unsigned char Actual_Width, Actual_Height;

    bool Only_Copy_A_Piece = false;

    if(x < 256 && y < 256)
    {
    if(x + Width > 256)
    {
        New_Width = (x+Width)-256;
        Actual_Width = New_Width;
        Only_Copy_A_Piece = true;
    }

    if(y + Height > 256)
    {
        New_Height = (x+Height)-256;
        Actual_Height = New_Height;
        Only_Copy_A_Piece = true;
    }

    unsigned char Array[Width*Height];

    for(int j = 0; j < Width*Height; j++)
    {
        if(Address + 3 + j < 65536)
        {
            Array[j] = Seat->New_Computer->RAM[Bank].Bank[Address+3+j];
        }
    }




    if(Only_Copy_A_Piece)
    {
    Copy_Section(1,Array,Seat->New_Computer->Back_Video_Buffer,Width,Height,256,256,0,0,x,y,Width-Actual_Width,Height-Actual_Height);
    }

    else if(!Only_Copy_A_Piece)
    {
    Copy_Section(1,Array,Seat->New_Computer->Back_Video_Buffer,Width,Height,256,256,0,0,x,y,Width,Height);
    }
    }

    if(Seat->New_Computer->Processor.Stack_Pointer + 18 < 65536)
    {
    Seat->New_Computer->Processor.Stack_Pointer += 18;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};













bool Handle_Aqueous_Basic_Text_RENTILEMAP(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char Bank,unsigned short Address,unsigned char XBank,unsigned short XAddress,unsigned char YBank,unsigned short YAddress, unsigned char EncodingBank,unsigned short EncodingAddress,unsigned char EncodingLengthBank, unsigned short EncodingLengthAddress,unsigned char TPRBank,unsigned short TPRAddress)
{
    unsigned char Total_Opcode_Length = 4+2 + 4+2 + 4+2 + 4+2 + 2;

    uint8_t DBLOCKaddrlow_byte = Address & 0xFF;
    uint8_t DBLOCKaddrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t xaddrlow_byte = XAddress & 0xFF;
    uint8_t xaddrhigh_byte = (XAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = YAddress & 0xFF;
    uint8_t yaddrhigh_byte = (YAddress >> 8) & 0xFF;

    uint8_t Encodingaddrlow_byte = EncodingAddress & 0xFF;
    uint8_t Encodingaddrhigh_byte = (EncodingAddress >> 8) & 0xFF;

    uint8_t EncodingLengthaddrlow_byte = EncodingLengthAddress & 0xFF;
    uint8_t EncodingLengthaddrhigh_byte = (EncodingLengthAddress >> 8) & 0xFF;

    uint8_t TPRaddrlow_byte = TPRAddress & 0xFF;
    uint8_t TPRaddrhigh_byte = (TPRAddress >> 8) & 0xFF;



    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,YBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,XBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,DBLOCKaddrhigh_byte,DBLOCKaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,Bank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,Encodingaddrhigh_byte,Encodingaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,EncodingBank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);



    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,EncodingLengthaddrhigh_byte,EncodingLengthaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,EncodingLengthBank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,TPRaddrhigh_byte,TPRaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,TPRBank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);



    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_TILEMAP,0,0);
    return true;

};

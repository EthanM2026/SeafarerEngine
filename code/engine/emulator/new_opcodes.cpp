#include "new_opcodes.h"

void New_Create_z80_Program(struct _x80_Programmer* Programmer, const char* Output)
{
    Programmer->Current_Bank = 0;
    Programmer->Current_Offset = 0;
    Programmer->Number_Of_Banks = 0;

    for(int j = 0; j < 65536; j++)
    {
        Programmer->Banks[0].Bank[j] = 0;
    }

    unsigned char Code[3];

    //0x1000 and 0x1001 IS PERCY's X!

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=0x00;
    Code[2]=0x1F;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);

    Code[0]=NEW_OPCODE_MOV_ADDR_RZero;
    Code[1]=0x10;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);


    Code[0]=NEW_OPCODE_CLEAR_SCREEN;
    Code[1]=0x00;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,1,Code);


    Code[0]=NEW_OPCODE_MOV_RZero_Value; //SPRITE ADDR
    Code[1]=0x20;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);


    Code[0]=NEW_OPCODE_PUSH_RZero; //2 bytes
    Code[1]=0x00;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,2,Code);


    Code[0]=NEW_OPCODE_MOV_RZero_ADDR;
    Code[1]=0x10;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);

    Code[0]=NEW_OPCODE_INC_RZero;
    Code[1]=0x00;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,1,Code);

    Code[0]=NEW_OPCODE_MOV_ADDR_RZero;
    Code[1]=0x10;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);




    Code[0]=NEW_OPCODE_PUSH_RZero; //2 bytes
    Code[1]=0x00;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,2,Code);


    Code[0]=NEW_OPCODE_MOV_RZero_Value; //SPRITE Y
    Code[1]=0;
    Code[2]=0;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);


    Code[0]=NEW_OPCODE_PUSH_RZero; //2 bytes
    Code[1]=0x00;
    Code[2]=0x00;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,2,Code);


    Code[0]=NEW_OPCODE_RENDER_SPRITE; //SPRITE WIDTH
    Code[1]=0;
    Code[2]=0;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,1,Code);









    Code[0]=NEW_OPCODE_FLIP_SCREEN;
    Code[1]=0;
    Code[2]=0;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,1,Code);


    Code[0]=NEW_OPCODE_JMP_ADDRSIXTEEN; //SPRITE WIDTH
    Code[1]=0x00;
    Code[2]=0x06;
    New_Insert_Opcode(Programmer,Programmer->Current_Bank,Programmer->Current_Offset,3,Code);


    Programmer->Current_Bank_Data_Offset = 0x2000;
    Fill_Bank_With_File(Programmer, Programmer->Current_Bank, Programmer->Current_Bank_Data_Offset, "tilemap.3bm");
    //Our example Encoding: ABCDEF


    Programmer->Current_Bank += 1;

    //Programmer->Current_Bank_Data_Offset=0x0000; //0x1FFE +2 = 2000.



    //JMP 0xFEEF

    FILE* f = fopen(Output, "wb");
    unsigned char Header[2];
    Header[0] = Programmer->Current_Bank+1;
    Header[1] = 2; //Offset to the Code

    //printf("Current Bank %d\n", Header[0]);

    fwrite(&Header, 1, 2, f);
    fseek(f, 2, SEEK_SET);

    for(unsigned char j = 0; j < Programmer->Current_Bank+1; j++)
    {
        fwrite(&Programmer->Banks[j], 1, sizeof(struct _SixtyFourK_Bank),f);
    }

    fclose(f);
}

void Fill_Bank_With_File(struct _x80_Programmer* Programmer, unsigned char Bank_ID, unsigned short Offset, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);

    if(File_Size > 0 && File_Size <= 65536)
    {
        for(int j = 0; j < File_Size; j++)
        {
            Programmer->Banks[Bank_ID].Bank[Offset+j] = Buffer[j];
        }
        Programmer->Current_Bank_Data_Offset += File_Size;
    }
    //printf("Wrote a file \n");
}

void New_Insert_Opcode(struct _x80_Programmer* Programmer, unsigned char Bank_ID, unsigned short Offset, unsigned char Size, unsigned char Code[3])
{
    if(Size == 1)
    {
    Programmer->Banks[Bank_ID].Bank[Offset] = Code[0];
    }

    else if(Size == 2)
    {
    Programmer->Banks[Bank_ID].Bank[Offset] = Code[0];
    Programmer->Banks[Bank_ID].Bank[Offset+1] = Code[1];
    }

    else if(Size == 3)
    {
    Programmer->Banks[Bank_ID].Bank[Offset] = Code[0];
    Programmer->Banks[Bank_ID].Bank[Offset+1] = Code[1];
    Programmer->Banks[Bank_ID].Bank[Offset+2] = Code[2];
    }

    Programmer->Current_Offset += Size;
}




void Run_New_Opcode_NOP(struct _Computer* Computer)
{
    Computer->Processor.Instruction_Pointer += 1;
}


void Run_New_Opcode_MOV_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}

void Run_New_Opcode_MOV_RZero_ROne(struct _Computer* Computer)
{
    //MOV R0, R1	Copy R1 → R0	2	+2
    Computer->Processor.Register_Zero = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}

void Run_New_Opcode_MOV_ROne_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}

void Run_New_Opcode_MOV_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}


void Run_New_Opcode_MOV_RZero_ADDR(struct _Computer* Computer)
{
    unsigned short Address;

    Address=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

    Computer->Processor.Register_Zero = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Address+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Address+0] & 0xff);

    //printf("Moved contents of ADDR %d\n into rZero", Address);
    //printf("RZero is NOW %d\n", Computer->Processor.Register_Zero);
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_MOV_ROne_ADDR(struct _Computer* Computer)
{
    unsigned short Address;

    Address=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

    Computer->Processor.Register_One = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Address+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Address+0] & 0xff);

    //printf("Moved contents of ADDR %d\n into rOne", Address);
    //printf("ROne is NOW %d\n", Computer->Processor.Register_One);
    Computer->Processor.Instruction_Pointer += 3;
}


void Run_New_Opcode_MOV_ADDR_RZero(struct _Computer* Computer)
{
    unsigned short Address=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

    uint8_t low_byte = Computer->Processor.Register_Zero & 0xFF;
    uint8_t high_byte = (Computer->Processor.Register_Zero >> 8) & 0xFF;

    Computer->RAM[Computer->Current_RAM_Bank].Bank[Address] = low_byte;
    Computer->RAM[Computer->Current_RAM_Bank].Bank[Address+1] = high_byte;

    //printf("Moved contents of rZero %d\n into ADDR", Address);

    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_MOV_ADDR_ROne(struct _Computer* Computer)
{
    unsigned short Address=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

    uint8_t low_byte = Computer->Processor.Register_One & 0xFF;
    uint8_t high_byte = (Computer->Processor.Register_One >> 8) & 0xFF;

    Computer->RAM[Computer->Current_RAM_Bank].Bank[Address] = low_byte;
    Computer->RAM[Computer->Current_RAM_Bank].Bank[Address+1] = high_byte;

    //printf("Moved contents of rZero %d\n into ADDR", Address);

    Computer->Processor.Instruction_Pointer += 3;
}


void Run_New_Opcode_PUSH_RZero(struct _Computer* Computer)
{
    Computer->Processor.Stack_Pointer -=2;

    uint8_t low_byte = Computer->Processor.Register_Zero & 0xFF;
    uint8_t high_byte = (Computer->Processor.Register_Zero >> 8) & 0xFF;

    Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] = low_byte;
    Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+1] = high_byte;

    Computer->Processor.Instruction_Pointer += 2;
   // //printf("PUSHED!\n");
}
void Run_New_Opcode_PUSH_ROne(struct _Computer* Computer)
{
    Computer->Processor.Stack_Pointer -=2;

    uint8_t low_byte = Computer->Processor.Register_One & 0xFF;
    uint8_t high_byte = (Computer->Processor.Register_One >> 8) & 0xFF;

    Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] = low_byte;
    Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+1] = high_byte;

    Computer->Processor.Instruction_Pointer += 2;
   // //printf("PUSHED!\n");
}

void Run_New_Opcode_POP_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] & 0xff);

    Computer->Processor.Stack_Pointer +=2;
    Computer->Processor.Instruction_Pointer += 2;
    ////printf("POPPED!\n");
}

void Run_New_Opcode_POP_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] & 0xff);

    Computer->Processor.Stack_Pointer +=2;
    Computer->Processor.Instruction_Pointer += 2;
    ////printf("POPPED!\n");
}



void Run_New_Opcode_ADD_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_ADD_RZero_ROne(struct _Computer* Computer)
{
    int Result = Computer->Processor.Register_Zero + Computer->Processor.Register_One;

    if(Result < 0)
    {
        Computer->Processor.Register_Zero = 65535;
    }

    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_Zero = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_Zero += Computer->Processor.Register_One;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_ADD_ROne_RZero(struct _Computer* Computer)
{
    int Result = Computer->Processor.Register_One + Computer->Processor.Register_Zero;

    if(Result < 0)
    {
        Computer->Processor.Register_One = 65535;
    }

    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_One = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_One += Computer->Processor.Register_Zero;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_ADD_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One += Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}






void Run_New_Opcode_SUB_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_SUB_RZero_ROne(struct _Computer* Computer)
{
    int Result = Computer->Processor.Register_Zero - Computer->Processor.Register_One;

    if(Result < 0)
    {
        Computer->Processor.Register_Zero = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_Zero = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_Zero -= Computer->Processor.Register_One;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_SUB_ROne_RZero(struct _Computer* Computer)
{
    int Result = Computer->Processor.Register_One - Computer->Processor.Register_Zero;

    if(Result < 0)
    {
        Computer->Processor.Register_One = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_One = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_One -= Computer->Processor.Register_Zero;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_SUB_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}









void Run_New_Opcode_MUL_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_MUL_RZero_ROne(struct _Computer* Computer)
{
    long Result = Computer->Processor.Register_Zero * Computer->Processor.Register_One;

    if(Result < 0)
    {
        Computer->Processor.Register_Zero = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_Zero = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_Zero = (Computer->Processor.Register_Zero*Computer->Processor.Register_One);
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_MUL_ROne_RZero(struct _Computer* Computer)
{
    long Result = Computer->Processor.Register_One * Computer->Processor.Register_Zero;

    if(Result < 0)
    {
        Computer->Processor.Register_One = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_One = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_One = (Computer->Processor.Register_Zero*Computer->Processor.Register_One);
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_MUL_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}







void Run_New_Opcode_DIV_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_DIV_RZero_ROne(struct _Computer* Computer)
{
    if(Computer->Processor.Register_One != 0)
    {
    float Result = Computer->Processor.Register_Zero / Computer->Processor.Register_One;

    if(Result < 0)
    {
        Computer->Processor.Register_Zero = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_Zero = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        unsigned short Half_Precision_Result = float32_to_float16(Result);
        Computer->Processor.Register_Zero = Half_Precision_Result;
    }
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_DIV_ROne_RZero(struct _Computer* Computer)
{
    if(Computer->Processor.Register_Zero != 0)
    {
    float Result = Computer->Processor.Register_One / Computer->Processor.Register_Zero;

    if(Result < 0)
    {
        Computer->Processor.Register_One = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_One = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        unsigned short Half_Precision_Result = float32_to_float16(Result);
        Computer->Processor.Register_One = Half_Precision_Result;
    }
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_DIV_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}







void Run_New_Opcode_PWR_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_PWR_RZero_ROne(struct _Computer* Computer)
{
    //PWR R0, R1	R0 = R0^R1's power

    float Result = pow(Computer->Processor.Register_Zero, Computer->Processor.Register_One);

   // double base = 2.0, exp = 3.0;
   // double result = pow(base, exp); // Calculates 2^3

    if(Result < 0)
    {
        Computer->Processor.Register_Zero = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_Zero = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_Zero = (unsigned short)Result;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_PWR_ROne_RZero(struct _Computer* Computer)
{
    //PWR R0, R1	R0 = R0^R1's power

    float Result = pow(Computer->Processor.Register_One, Computer->Processor.Register_Zero);

   // double base = 2.0, exp = 3.0;
   // double result = pow(base, exp); // Calculates 2^3

    if(Result < 0)
    {
        Computer->Processor.Register_One = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_One = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_One = (unsigned short)Result;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_PWR_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}












void Run_New_Opcode_ROOT_RZero_RZero(struct _Computer* Computer)
{
    Computer->Processor.Register_Zero = Computer->Processor.Register_Zero;
    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_ROOT_RZero_ROne(struct _Computer* Computer)
{
    //PWR R0, R1	R0 = R0^R1's power

    float Inverse_Register_One = 1/Computer->Processor.Register_One;

    float Result = pow(Computer->Processor.Register_Zero, Inverse_Register_One);

   // double base = 2.0, exp = 3.0;
   // double result = pow(base, exp); // Calculates 2^3

    if(Result < 0)
    {
        Computer->Processor.Register_Zero = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_Zero = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_Zero = (unsigned short)Result;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_ROOT_ROne_RZero(struct _Computer* Computer)
{
    //PWR R0, R1	R0 = R0^R1's power

    float Inverse_Register_Zero = 1/Computer->Processor.Register_Zero;

    float Result = pow(Computer->Processor.Register_One, Inverse_Register_Zero);

   // double base = 2.0, exp = 3.0;
   // double result = pow(base, exp); // Calculates 2^3

    if(Result < 0)
    {
        Computer->Processor.Register_One = 65535;
    }
    if(Result == 0)
    {
        Computer->Processor.Zero_Flag = true;
    }

    if(Result > 65535)
    {
        Computer->Processor.Register_One = 0;
    }

    if(Result >= 0 && Result < 65536)
    {
        Computer->Processor.Register_One = (unsigned short)Result;
    }

    Computer->Processor.Instruction_Pointer += 2;
}
void Run_New_Opcode_ROOT_ROne_ROne(struct _Computer* Computer)
{
    Computer->Processor.Register_One = Computer->Processor.Register_One;
    Computer->Processor.Instruction_Pointer += 2;
}






void Run_New_Opcode_JMP(struct _Computer* Computer)
{
    unsigned short New_Location;

    New_Location=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);
    Computer->Processor.Instruction_Pointer = New_Location;

    //printf("Jumped to %d\n", New_Location);
}

void Run_New_Opcode_JE(struct _Computer* Computer)
{
    if(Computer->Processor.Register_One == Computer->Processor.Register_Zero)
    {
        unsigned short New_Location;

    New_Location=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);
    Computer->Processor.Instruction_Pointer = New_Location;

    //printf("Jumped Equal to %d\n", New_Location);
    }
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_JNE(struct _Computer* Computer)
{
    if(Computer->Processor.Register_One != Computer->Processor.Register_Zero)
    {
        unsigned short New_Location;

    New_Location=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);
    Computer->Processor.Instruction_Pointer = New_Location;

    //printf("Jumped Equal to %d\n", New_Location);
    }
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_CALL(struct _Computer* Computer)
{
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_RET(struct _Computer* Computer)
{
    Computer->Processor.Instruction_Pointer += 1;
}
void Run_New_Opcode_HALT(struct _Computer* Computer)
{
    Computer->CPU_Is_Running = 0;
    //printf("Halted\n");
}

void Run_New_Opcode_MOV_RZero_Value(struct _Computer* Computer)
{
    unsigned short Value;

    Value=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

    Computer->Processor.Register_Zero = Value;
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_MOV_ROne_Value(struct _Computer* Computer)
{
    unsigned short Value;

    Value=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);


    Computer->Processor.Register_One = Value;
    Computer->Processor.Instruction_Pointer += 3;
}


void Run_New_Opcode_CMP_RZero_Value(struct _Computer* Computer)
{
  //  unsigned short Value;

   // Value=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

   // if(Computer->Processor.Register_Zero == Value)
    //{
    //    Computer->Processor.Zero_Flag = true;
    //}

    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_CMP_ROne_Value(struct _Computer* Computer)
{
   // unsigned short Value;

    //Value=(Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer+2] & 0xff);

   // if(Computer->Processor.Register_One == Value)
    //{
   //     Computer->Processor.Zero_Flag = true;
   // }

    Computer->Processor.Instruction_Pointer += 3;
}











void Run_New_Opcode_IN(struct _Computer* Computer)
{
    //Reg Zero: Hardware
    //Reg One: RAM Offset to Read into

    if(Computer->Processor.Register_One + 1536 < 65536) //Do not overflow.

    switch(Computer->Processor.Register_Zero)
    {
        case HARDWARE_OFFSET_VIDEO_RAM:
        break;

        case HARDWARE_OFFSET_HDD:
            Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One] = Computer->HDD_Slot_Status;
        break;

        case HARDWARE_OFFSET_REMOVABLE_DISK:
            Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One] = Computer->Removable_Slot_Status;
        break;

        case HARDWARE_OFFSET_SENSOR:
            for(int j = 0; j < 1536; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Sensor_Link_Port_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_PERIPHERAL:
            for(int j = 0; j < 1536; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Peripheral_Link_Port_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_KEYBOARD:
            for(int j = 0; j < 32; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Keyboard_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_LEFT_KEYBOARD:
            for(int j = 0; j < 32; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Left_Keyboard_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_RIGHT_KEYBOARD:
            for(int j = 0; j < 32; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Right_Keyboard_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_HEAD_INDICATORS:
            for(int j = 0; j < 32; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->External_Lights_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_MOUSE:
            for(int j = 0; j < 3; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Mouse_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_CONTROLLER:
            for(int j = 0; j < 5; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Controller_RAM[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_POWER_SUPPLY:
            Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One] = Computer->Power_Supply_Status;
        break;

        case HARDWARE_OFFSET_FANS:
            Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One] = Computer->Fan_Status;
        break;

        case HARDWARE_OFFSET_NETWORK:
            for(int j = 0; j < 1536; j++)
            {
                Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One] = Computer->Network_Link_Port_RAM[j+Computer->Processor.Register_One];
            }
        break;
    }
}

void Run_New_Opcode_OUT(struct _Computer* Computer)
{
    //Reg Zero: Hardware
    //Reg One: RAM Offset to Read into

    switch(Computer->Processor.Register_Zero)
    {
        case HARDWARE_OFFSET_VIDEO_RAM:
        break;

        case HARDWARE_OFFSET_HDD:
            Computer->HDD_Slot_Status = Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One];
        break;

        case HARDWARE_OFFSET_REMOVABLE_DISK:
            Computer->Removable_Slot_Status = Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One];
        break;

        case HARDWARE_OFFSET_SENSOR:
            for(int j = 0; j < 1536; j++)
            {
                Computer->Sensor_Link_Port_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_PERIPHERAL:
            for(int j = 0; j < 1536; j++)
            {
                 Computer->Peripheral_Link_Port_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_KEYBOARD:
            for(int j = 0; j < 32; j++)
            {
                Computer->Keyboard_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_LEFT_KEYBOARD:
            for(int j = 0; j < 32; j++)
            {
                 Computer->Left_Keyboard_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_RIGHT_KEYBOARD:
            for(int j = 0; j < 32; j++)
            {
                 Computer->Right_Keyboard_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_HEAD_INDICATORS:
            for(int j = 0; j < 32; j++)
            {
                 Computer->External_Lights_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_MOUSE:
            for(int j = 0; j < 3; j++)
            {
                Computer->Mouse_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_CONTROLLER:
            for(int j = 0; j < 5; j++)
            {
                 Computer->Controller_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;

        case HARDWARE_OFFSET_POWER_SUPPLY:
             Computer->Power_Supply_Status = Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One];
        break;

        case HARDWARE_OFFSET_FANS:
             Computer->Fan_Status = Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Register_One];
        break;

        case HARDWARE_OFFSET_NETWORK:
            for(int j = 0; j < 1536; j++)
            {
                Computer->Network_Link_Port_RAM[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[j+Computer->Processor.Register_One];
            }
        break;
    }
}

void Run_New_Opcode_REP(struct _Computer* Computer)
{

}



void Run_New_Opcode_Flip_Screen(struct _Computer* Computer)
{
    memcpy(&Computer->Video_RAM_Back_Buffer, &Computer->Video_RAM_Front_Buffer, 65536);
    Computer->Processor.Instruction_Pointer += 1;
}


//FLAGS
void Run_New_Opcode_SET_ZERO_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Zero_Flag = true;
}

void Run_New_Opcode_SET_CARRY_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Carry_Flag = true;
}

void Run_New_Opcode_SET_SIGN_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Sign_Flag = true;
}

void Run_New_Opcode_SET_OVERFLOW_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Overflow_Flag = true;
}

void Run_New_Opcode_SET_PARITY_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Parity_Flag = true;
}

void Run_New_Opcode_SET_AUXILIARY_CARRY_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Auxiliary_Carry_Flag = true;
}


void Run_New_Opcode_SET_DIRECTION_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Direction_Flag = true;
}

void Run_New_Opcode_SET_MASKABLE_INTERRUPT_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Maskable_Interrupt_Flag = true;
}

void Run_New_Opcode_SET_TRAP_FLAG(struct _Computer* Computer)
{
    Computer->Processor.Trap_Flag = true;
}

void Run_New_Opcode_Error_Handler(struct _Computer* Computer)
{
    //printf("ERROR!\n");
}

void Run_New_Opcode_Render_Sprite(struct _Computer* Computer)
{
    Computer->Processor.Old_Base_Pointer = Computer->Processor.Base_Pointer;
    Computer->Processor.Base_Pointer = Computer->Processor.Stack_Pointer;

    unsigned short y = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] & 0xff);
    unsigned short x = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+3] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+2] & 0xff);

    unsigned short Sprite_RAM_Address = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+5] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+6] & 0xff);

    unsigned short Width = (unsigned char)Computer->RAM[Computer->Current_RAM_Bank].Bank[Sprite_RAM_Address];
    unsigned short Height = (unsigned char)Computer->RAM[Computer->Current_RAM_Bank].Bank[Sprite_RAM_Address+1];
    unsigned char Color_Mode = Computer->RAM[Computer->Current_RAM_Bank].Bank[Sprite_RAM_Address+2];

    if(x < 256 && y < 256)
    {
    if(Width < 257 && Height < 257) //Memory safety
    {

    unsigned int Size_Of_Sprite = Width * Height;

    //printf("Sprite x: %d\n", x);
    //printf("Sprite y: %d\n", y);

    //printf("Sprite Width: %d\n", Width);
    //printf("Sprite Height: %d\n", Height);

    //printf("Size of Sprite: %d\n", Size_Of_Sprite);


    unsigned char Receptacle[Size_Of_Sprite];
    for(int j = 0; j < Size_Of_Sprite+1; j++)
    {
        Receptacle[j] = Computer->RAM[Computer->Current_RAM_Bank].Bank[Sprite_RAM_Address+3+j];
        ////printf("Current Location: %d\n", Computer->RAM[Computer->Current_RAM_Bank].Bank[Sprite_RAM_Address+3+j]);
        ////printf("Bytes Read: %d\n", j);
    }

    unsigned short Corrected_Width = 0;
    unsigned short Corrected_Height = 0;

    unsigned short Render_Width = Width;
    unsigned short Render_Height = Height;

    if(x+Width >= 256)
    {//
        Corrected_Width =(x+Width) - 256;
        Render_Width -= Corrected_Width;
        //printf("Corrected Width: %d\n", Corrected_Width);
    }

    if(y+Height >= 256)
    {
        Corrected_Height =(y+Height) - 256;
        Render_Height -= Corrected_Height;
        //printf("Corrected Height: %d\n", Corrected_Height);
    }


    if(Corrected_Height > 0 || Corrected_Width > 0)
    {
        Copy_Section(1,Receptacle,Computer->Video_RAM_Front_Buffer,(unsigned int)Width,(unsigned int)Height,256,256,0,0,(unsigned int)x,(unsigned int)y,(unsigned int)Render_Width,(unsigned int)Render_Height);
    }

    else if(Corrected_Height == 0 && Corrected_Width == 0)
    {
    Copy_Section(1,Receptacle,Computer->Video_RAM_Front_Buffer,(unsigned int)Width,(unsigned int)Height,256,256,0,0,(unsigned int)x,(unsigned int)y,(unsigned int)Width,(unsigned int)Height);
    }

    }


    //printf("Rendered sprite\n");
    }

    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = Computer->Processor.Old_Base_Pointer;
    Computer->Processor.Instruction_Pointer += 1;
}

void Run_New_Opcode_PUSH_RZero_HI(struct _Computer* Computer)
{
    Computer->Processor.Stack_Pointer -=1;

    uint8_t high_byte = (Computer->Processor.Register_Zero >> 8) & 0xFF;

    Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] = high_byte;

    Computer->Processor.Instruction_Pointer += 2;
    ////printf("PUSHED!\n");
}

void Run_New_Opcode_PUSH_ROne_HI(struct _Computer* Computer)
{
    Computer->Processor.Stack_Pointer -=1;

    uint8_t high_byte = (Computer->Processor.Register_One >> 8) & 0xFF;

    Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] = high_byte;

    Computer->Processor.Instruction_Pointer += 2;
    ////printf("PUSHED!\n");
}


void Run_New_Opcode_POP_RZero_LO(struct _Computer* Computer)
{

}

void Run_New_Opcode_POP_ROne_LO(struct _Computer* Computer)
{

}

void Run_New_Opcode_Clear_Screen(struct _Computer* Computer)
{
    memset(Computer->Video_RAM_Front_Buffer,Computer->Background_Color,65536);
   // Computer->Video_RAM_Front_Buffer[j] = Computer->Background_Color;
    Computer->Processor.Instruction_Pointer += 1;
}


void Run_New_Opcode_Render_Point(struct _Computer* Computer)
{
    Computer->Processor.Old_Base_Pointer = Computer->Processor.Base_Pointer;
    Computer->Processor.Base_Pointer = Computer->Processor.Stack_Pointer;

    unsigned short y = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] & 0xff);
    unsigned short x = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+3] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+2] & 0xff);
    unsigned char Pixel;

    Pixel = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+4] & 0xff);

   ////printf("Sprite x: %d\n", x);
    ////printf("Sprite y: %d\n", y);

    ////printf("Pixel Color: %d\n", Pixel[0]);

    Copy_Section(1,&Pixel,Computer->Video_RAM_Front_Buffer,1,1,256,256,0,0,x,y,1,1);

    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = Computer->Processor.Old_Base_Pointer;
    Computer->Processor.Instruction_Pointer += 1;
}

void Run_New_Opcode_Render_Line(struct _Computer* Computer)
{
    Computer->Processor.Old_Base_Pointer = Computer->Processor.Base_Pointer;
    Computer->Processor.Base_Pointer = Computer->Processor.Stack_Pointer;

    unsigned short yTwo = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] & 0xff);
    unsigned short xTwo = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+3] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+2] & 0xff);

    unsigned short y = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+5] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+4] & 0xff);
    unsigned short x = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+7] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+6] & 0xff);

    unsigned char Pixel;

    Pixel = (Computer->RAM[0].Bank[Computer->Processor.Stack_Pointer+8] & 0xff);

    float xone = x;
    float yone = y;
    float xtwo = xTwo;
    float ytwo = yTwo;

    bool IsXTwoXoneNegative = false;
    bool IsYTwoYoneNegative = false;

    if(xtwo - xone < 0)
    {
        IsXTwoXoneNegative = true;
    }

    if(ytwo - yone < 0)
    {
        IsYTwoYoneNegative = true;
    }

    float Slope = (xtwo-xone/ytwo-yone);

    float Cursor_x = x;
    float Cursor_y = y;

    if(IsXTwoXoneNegative && IsYTwoYoneNegative)
    {
        while(Cursor_x >= xtwo && Cursor_y >= ytwo)
        {
            Cursor_x += (xtwo-xone)/256;
            Cursor_y += (ytwo-yone)/256;
            Copy_Section(1,&Pixel,Computer->Video_RAM_Front_Buffer,1,1,256,256,0,0,(int)Cursor_x,(int)Cursor_y,1,1);
        }
    }

    else if(IsXTwoXoneNegative && !IsYTwoYoneNegative)
    {
        while(Cursor_x >= xtwo && Cursor_y <= ytwo)
        {
            Cursor_x += (xtwo-xone)/256;
            Cursor_y += (ytwo-yone)/256;
            Copy_Section(1,&Pixel,Computer->Video_RAM_Front_Buffer,1,1,256,256,0,0,(int)Cursor_x,(int)Cursor_y,1,1);
        }
    }

    else if(!IsXTwoXoneNegative && IsYTwoYoneNegative)
    {
        while(Cursor_x <= xtwo && Cursor_y >= ytwo)
        {
            Cursor_x += (xtwo-xone)/256;
            Cursor_y += (ytwo-yone)/256;
            Copy_Section(1,&Pixel,Computer->Video_RAM_Front_Buffer,1,1,256,256,0,0,(int)Cursor_x,(int)Cursor_y,1,1);
        }
    }

    else if(!IsXTwoXoneNegative && !IsYTwoYoneNegative)
    {
        while(Cursor_x <= xtwo && Cursor_y <= ytwo)
        {
            Cursor_x += (xtwo-xone)/256;
            Cursor_y += (ytwo-yone)/256;
            Copy_Section(1,&Pixel,Computer->Video_RAM_Front_Buffer,1,1,256,256,0,0,(int)Cursor_x,(int)Cursor_y,1,1);
        }
    }




    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = Computer->Processor.Old_Base_Pointer;
    Computer->Processor.Instruction_Pointer += 1;
}

void Run_New_Opcode_Render_Triangle(struct _Computer* Computer)
{

    Computer->Processor.Old_Base_Pointer = Computer->Processor.Base_Pointer;
    Computer->Processor.Base_Pointer = Computer->Processor.Stack_Pointer;

    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = Computer->Processor.Old_Base_Pointer;
    Computer->Processor.Instruction_Pointer += 1;
}

void Pull_Tile_Encoding_Information(struct _Computer* Computer)
{

};

void Run_New_Opcode_MOV_RZero_Random(struct _Computer* Computer)
{
    int Value = rand() % 256;
    Computer->Processor.Register_Zero = (unsigned short)Value;
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_MOV_ROne_Random(struct _Computer* Computer)
{
    int Value = rand() % 256;
    Computer->Processor.Register_One = (unsigned short)Value;
    Computer->Processor.Instruction_Pointer += 3;
}

void Run_New_Opcode_Render_Tilemap(struct _Computer* Computer)
{
    Computer->Processor.Old_Base_Pointer = Computer->Processor.Base_Pointer;
    Computer->Processor.Base_Pointer = Computer->Processor.Stack_Pointer;

    unsigned short y = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+1] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer] & 0xff);

    unsigned short x = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+3] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+2] & 0xff);

    unsigned short Height_To_Render_In_Tiles = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+5] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+4] & 0xff);

    unsigned short Width_To_Render_In_Tiles = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+7] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+6] & 0xff);

    unsigned short Encoding_Start_Location = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+9] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+8] & 0xff);

    unsigned short Encoding_RAM_Bank = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+11] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+10] & 0xff);

    unsigned short Encoding_RAM_Address = (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+13] << 8) | (Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Stack_Pointer+12] & 0xff);

   // //printf("Tilemap: x %d\n y %d\n Height to render %d\n Width to render %d\n Start location %d\n Bank %d\n Addr %d\n",x,y,Height_To_Render_In_Tiles,Width_To_Render_In_Tiles,Encoding_Start_Location,Encoding_RAM_Bank,Encoding_RAM_Address);

    unsigned char Tilemap[128*128];
    memset(Tilemap, 56, 128*128);

    if(Width_To_Render_In_Tiles * 16 < 257 && Height_To_Render_In_Tiles * 16 < 257)
    {
        unsigned char RAM_Bank;
        unsigned short Address;
        unsigned short Encoding[4];
        unsigned short Current_Encoding = 0;
        unsigned short Number_Of_Encodings = (Width_To_Render_In_Tiles * Height_To_Render_In_Tiles);
        unsigned short sx,sy,dx,dy;
        dx = x;
        dy = y;

        ////printf("Number of Encodings: %d\n", Number_Of_Encodings);

        while(Current_Encoding < Number_Of_Encodings)
        {
            Encoding[0] = (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+1+(Current_Encoding*8)] << 8) | (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+(Current_Encoding*8)] & 0xff);
            Encoding[1] = (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+3+(Current_Encoding*8)] << 8) | (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+2+(Current_Encoding*8)] & 0xff);
            Encoding[2] =(Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+5+(Current_Encoding*8)] << 8) | (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+4+(Current_Encoding*8)] & 0xff);
            Encoding[3] = (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+7+(Current_Encoding*8)] << 8) | (Computer->RAM[(unsigned char)Encoding_RAM_Bank].Bank[Encoding_RAM_Address+6+(Current_Encoding*8)] & 0xff);
            ////printf("Encodings: \n BANK %d\n ADDR %d\n X %d\n Y %d\n", Encoding[0],Encoding[1],Encoding[2],Encoding[3]);
            ////printf("RAM_BANK %d\n ADDRESS %d\n", RAM_Bank, Address);

            Encoding[1] += 3;

            for(int l = 0; l < 16384; l++)
            {
                Tilemap[l] = Computer->RAM[Encoding[0]].Bank[Encoding[1]+l];
               // //printf("Value: %d\n", Tilemap[l]);
            }

            Copy_Section(1,Tilemap,Computer->Video_RAM_Front_Buffer,128,128,256,256,(int)Encoding[2],(int)Encoding[3],(int)dx,(int)dy,16,16);
            dx += 16;
            if(dx >= Width_To_Render_In_Tiles * 16)
            {
                dx = 0;
                dy += 16;
            }

            Current_Encoding += 1;
        }


    }

    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = Computer->Processor.Old_Base_Pointer;
    Computer->Processor.Instruction_Pointer += 1;
}

void Run_NEW_OPCODE_INC_RZero(struct _Computer* Computer)
{
    if(Computer->Processor.Register_Zero + 1 < 65535)
    {
        Computer->Processor.Register_Zero +=1;
            //printf("INCREMENTED RZero!\n");
    }
    Computer->Processor.Instruction_Pointer += 1;
}
void Run_NEW_OPCODE_INC_ROne(struct _Computer* Computer)
{
    if(Computer->Processor.Register_One +1 < 65535)
    {
        Computer->Processor.Register_One +=1;
        //printf("INCREMENTED ROne!\n");
    }

    Computer->Processor.Instruction_Pointer += 1;
}

void Write_Example_Encoding(const char* Filename)
{
    unsigned int Sequence_Size = 16;
    char Character_Sequence[65536];

    sprintf(Character_Sequence, "THE FOX JUMPS NO");

    unsigned short Encoding[4];

    unsigned short Encoding_Sequence[4*Sequence_Size];

    int Encoding_Position = 0;
    for(int j = 0; j < Sequence_Size*8; j++)
    {
        switch(Character_Sequence[j])
        {
            case ' ': //SPACE
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 0; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case '`': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 16; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case 'A': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 32; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case 'B': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 48; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case 'C': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 64; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case 'D': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 80; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case 'E': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 96; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;

            case 'F': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 112; //x 0
                Encoding_Sequence[Encoding_Position+3] = 0; //y-0
                Encoding_Position += 4;
            break;









            case 'G': //SPACE
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 0; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'H': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 16; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'I': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 32; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'J': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 48; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'K': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 64; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'L': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 80; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'M': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 96; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;

            case 'N': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 112; //x 0
                Encoding_Sequence[Encoding_Position+3] = 16; //y-0
                Encoding_Position += 4;
            break;









            case 'O': //SPACE
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 0; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'P': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 16; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'Q': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 32; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'R': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 48; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'S': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 64; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'T': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 80; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'U': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 96; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;

            case 'V': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 112; //x 0
                Encoding_Sequence[Encoding_Position+3] = 32; //y-0
                Encoding_Position += 4;
            break;












            case 'W': //SPACE
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 0; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case 'X': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 16; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case 'Y': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 32; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case 'Z': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 48; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case '0': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 64; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case '1': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 80; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case '2': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 96; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;

            case '3': //New Line
                Encoding_Sequence[Encoding_Position] = 0x0001; //Bank 1
                Encoding_Sequence[Encoding_Position+1] = 0x0000; //Address 0
                Encoding_Sequence[Encoding_Position+2] = 112; //x 0
                Encoding_Sequence[Encoding_Position+3] = 48; //y-0
                Encoding_Position += 4;
            break;
        }
    }

    FILE* f = fopen(Filename, "wb");
    fwrite(&Encoding_Sequence,1,Sequence_Size*8,f);
    fclose(f);
}


uint16_t float32_to_float16(float f) {
    // Use a union to access the bits of the float
    union { float f; uint32_t i; } u = {f};
    uint32_t f32 = u.i;

    // Extract Sign, Exponent, and Mantissa
    uint16_t sign = (f32 >> 16) & 0x8000;              // Shift 31 to 15
    int16_t exponent = ((f32 >> 23) & 0xFF) - 127;     // Unbias the 8-bit exponent
    uint32_t mantissa = f32 & 0x007FFFFF;              // Last 23 bits

    uint16_t res;

    // Case 1: Zero or Denormal (too small for half-precision)
    if (exponent <= -15) {
        res = sign; // Return signed zero
    }
    // Case 2: Infinity or NaN
    else if (exponent > 15) {
        res = sign | 0x7C00; // Set exponent to all 1s (Infinity)
        if (mantissa != 0 && exponent == 128) res |= 0x01; // Keep NaN status
    }
    // Case 3: Normal conversion
    else {
        exponent += 15; // Re-bias for 5 bits
        res = sign | (exponent << 10) | (mantissa >> 13);
    }

    return res;
}





float float16_to_float32(uint16_t h) {
    uint32_t sign = (h & 0x8000) << 16;       // Shift sign to bit 31
    uint32_t exp  = (h & 0x7C00) >> 10;       // Extract 5-bit exponent
    uint32_t mant = (h & 0x03FF) << 13;       // Shift 10-bit mantissa to 23-bit range

    uint32_t res;

    // Case 1: Zero
    if (exp == 0 && mant == 0) {
        res = sign;
    }
    // Case 2: Subnormals (very small numbers)
    else if (exp == 0 && mant != 0) {
        // Convert subnormal half to normal single
        // This requires normalizing the mantissa
        while (!(mant & 0x00800000)) {
            mant <<= 1;
            exp--;
        }
        exp++;
        res = sign | ((exp + (127 - 15)) << 23) | (mant & 0x007FFFFF);
    }
    // Case 3: Infinity or NaN
    else if (exp == 0x1F) {
        res = sign | 0x7F800000 | mant;
    }
    // Case 4: Normal Numbers
    else {
        res = sign | ((exp + (127 - 15)) << 23) | mant;
    }

    // Use memcpy to avoid aliasing issues when converting bits to float
    float f;
    memcpy(&f, &res, sizeof(f));
    return f;
}



double float16_to_double(uint16_t h) {
    uint64_t sign = (uint64_t)(h & 0x8000) << 48; // Shift sign to bit 63
    uint32_t exp  = (h & 0x7C00) >> 10;          // Extract 5-bit exponent
    uint64_t mant = (uint64_t)(h & 0x03FF) << 42; // Shift 10-bit mantissa to 52-bit range

    uint64_t res;

    // Case 1: Zero
    if (exp == 0 && mant == 0) {
        res = sign;
    }
    // Case 2: Subnormals (Denormals)
    else if (exp == 0 && mant != 0) {
        // Convert subnormal half to normal double
        // We normalize by shifting the mantissa until the implicit leading 1 is at bit 52
        while (!(mant & 0x0010000000000000ULL)) {
            mant <<= 1;
            exp--;
        }
        // Unbias the half-float (-14) and re-bias for double (+1023)
        // Since we shifted until the bit hit the 52nd position, we adjust exp accordingly
        res = sign | ((uint64_t)(exp + (1023 - 15 + 1)) << 52) | (mant & 0x000FFFFFFFFFFFFFULL);
    }
    // Case 3: Infinity or NaN
    else if (exp == 0x1F) {
        // Exponent becomes all 1s (0x7FF for double)
        res = sign | 0x7FF0000000000000ULL | mant;
    }
    // Case 4: Normal Numbers
    else {
        // Re-bias: (exp - 15) + 1023
        res = sign | ((uint64_t)(exp + 1008) << 52) | mant;
    }

    double d;
    memcpy(&d, &res, sizeof(d));
    return d;
}



uint16_t double_to_float16_full(double d) {
    uint64_t u64;
    memcpy(&u64, &d, sizeof(d));

    uint16_t sign = (uint16_t)((u64 >> 48) & 0x8000);
    int32_t exp = (int32_t)((u64 >> 52) & 0x7FF) - 1023;
    uint64_t mant = u64 & 0x000FFFFFFFFFFFFFULL;

    // Handle Infinity and NaN
    if (exp == 1024) {
        return sign | 0x7C00 | (mant ? 0x0200 : 0);
    }

    // Handle Overflow
    if (exp > 15) {
        return sign | 0x7C00;
    }

    uint32_t res_exp = 0;
    uint64_t final_mant = 0;
    int32_t shift = 42; // Base shift from 52-bit to 10-bit

    if (exp < -14) {
        // --- Subnormal Logic ---
        // Distance below the minimum exponent (-14)
        int32_t sub_shift = -14 - exp;

        if (sub_shift > 11) {
            return sign; // Truly too small for even subnormal 16-bit
        }

        // Add the implicit leading 1 back into the mantissa
        mant |= 0x0010000000000000ULL;
        shift += sub_shift;
        res_exp = 0;
    } else {
        // --- Normal Logic ---
        res_exp = (uint32_t)(exp + 15);
    }

    // Round to Nearest Even (RTNE)
    // L = Least Significant Bit, G = Guard, S = Sticky
    uint64_t L = (mant >> shift) & 1;
    uint64_t G = (mant >> (shift - 1)) & 1;
    uint64_t S = (mant << (64 - (shift - 1))) ? 1 : 0;

    final_mant = (mant >> shift);

    if (G && (S || L)) {
        final_mant++;
        // If mantissa overflowed, it might turn a subnormal into a normal
        // or increment the exponent of a normal number.
        if (final_mant & 0x0400) {
            final_mant = 0;
            res_exp++;
        }
    }

    if (res_exp >= 31) return sign | 0x7C00; // Final overflow check

    return sign | (res_exp << 10) | (uint16_t)final_mant;
}

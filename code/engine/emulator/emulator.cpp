#include "emulator.h"
#include "new_opcodes.h"
#include "time.h"
struct _Computer* Create_Computer()
{
    struct _Computer* Computer = (struct _Computer*) calloc(1,sizeof (struct _Computer));
	return Computer;
};

void Initialize_Computer(struct _Computer* Computer)
{
    srand(time(NULL));

    Computer->Red_Level = 0.1;
    Computer->Blue_Level = 1;
    Computer->Green_Level = 0.1;

    Computer->Processor.Register_Zero = 0;
    Computer->Processor.Register_One = 0;

    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = 65535;

    Computer->Processor.Instruction_Pointer = 0;
    Computer->Processor.Zero_Flag = false;

    for(int j = 0; j < 65536; j++)
    {
        Computer->Video_RAM_Front_Buffer[j] = 0;
        Computer->Video_RAM_Back_Buffer[j] = 0;
    }

    for(int l = 0; l < 256; l++)
    {
        for(int j = 0; j < 65536; j++)
        {
            Computer->RAM[l].Bank[j] = 0;
        }
    }

    Computer->HDD_Slot_Status = 1;
    Computer->Removable_Slot_Status = 0;

    Computer->Sensor_Link_Port_Status = 0;
    Computer->Peripheral_Link_Port_Status = 0;

    Computer->Network_Link_Port_Status = 0;

    Computer->Monitor_Link_Port_Status = 0;

    Computer->Keyboard_Link_Port_Status = 0;
    Computer->Mouse_Link_Port_Status = 0;
    Computer->Controller_Link_Port_Status = 0;

    Computer->Power_Status = 0;
    Computer->Power_Supply_Status = 1;
    Computer->Fan_Status = 0;


    Computer->Controller_RAM[0] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[1] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[2] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[3] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[4] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.

    for(int j = 0; j < 32; j++)
    {
        Computer->Keyboard_RAM[j] = 0;
        Computer->External_Lights_RAM[j] = 0;
        Computer->External_Inputs_RAM[j] = 0;
    }

    Computer->Mouse_RAM[0] = 0;
    Computer->Mouse_RAM[1] = 0;
    Computer->Mouse_RAM[2] = 0;


    for(int j = 0; j < 256*256*4; j++)
    {
        Computer->True_Video_Screen[j] = 0;
    }

    Computer->Color_Mode = VIDEO_MODE_MONOCHROME;
    Computer->Background_Color = 128;

    glGenTextures(1, &Computer->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Computer->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Computer->True_Video_Screen);
    glEnable(GL_TEXTURE_2D);

};


void Turn_On_Computer(struct _Computer* Computer)
{
    if(Computer->Power_Status != 1)
    {
    if(Computer->Power_Supply_Status == 1)
    {
        Computer->Power_Status = 1;
    }
    if(Computer->HDD_Slot_Status == 1)
    {
        Load_Program_Into_RAM_And_Start(Computer, "z802.rom");
    }
    else if(Computer->HDD_Slot_Status == 0)
    {
        //Load_BIOS_Into_RAM_And_Start(Computer, "test.rom");
    }
    }
};

void Turn_Off_Computer(struct _Computer* Computer)
{
    if(Computer->Power_Status == 1)
    {
    Computer->Power_Status = 0;

    Computer->Processor.Register_Zero = 0;
    Computer->Processor.Register_One = 0;

    Computer->Processor.Stack_Pointer = 65535;
    Computer->Processor.Base_Pointer = 65535;

    Computer->Processor.Instruction_Pointer = 0;
    Computer->Processor.Zero_Flag = false;

    for(int j = 0; j < 65536; j++)
    {
        Computer->Video_RAM_Front_Buffer[j] = 0;
        Computer->Video_RAM_Back_Buffer[j] = 0;
    }

    for(int l = 0; l < 256; l++)
    {
        for(int j = 0; j < 65536; j++)
        {
            Computer->RAM[l].Bank[j] = 0;
        }
    }

    Computer->HDD_Slot_Status = 1;
    Computer->Removable_Slot_Status = 0;

    Computer->Sensor_Link_Port_Status = 0;
    Computer->Peripheral_Link_Port_Status = 0;

    Computer->Network_Link_Port_Status = 0;

    Computer->Monitor_Link_Port_Status = 0;

    Computer->Keyboard_Link_Port_Status = 0;
    Computer->Mouse_Link_Port_Status = 0;
    Computer->Controller_Link_Port_Status = 0;

    Computer->Power_Status = 0;
    Computer->Power_Supply_Status = 1;
    Computer->Fan_Status = 0;


    Computer->Controller_RAM[0] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[1] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[2] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[3] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.
    Computer->Controller_RAM[4] = 0; //X and Y hats, 4 bytes. 1 byte for 4 buttons and a DPAD.

    for(int j = 0; j < 32; j++)
    {
        Computer->Keyboard_RAM[j] = 0;
        Computer->External_Lights_RAM[j] = 0;
        Computer->External_Inputs_RAM[j] = 0;
    }

    Computer->Mouse_RAM[0] = 0;
    Computer->Mouse_RAM[1] = 0;
    Computer->Mouse_RAM[2] = 0;


    for(int j = 0; j < 256*256*4; j++)
    {
        Computer->True_Video_Screen[j] = 0;
    }

    }
};

void Load_Program_Into_RAM_And_Start(struct _Computer* Computer, const char* Filename)
{
    Computer->Processor.Instruction_Pointer = 0x0000;

    FILE* f = fopen(Filename, "rb");

    unsigned char Header[2];
    fread(Header, 1, 2, f);

    unsigned char Banks_On_Disk = *(unsigned char*)&Header[0];
    unsigned int Bytes_To_Load = *(unsigned char*)&Header[0] * sizeof(struct _SixtyFourK_Bank); //NUmber of banks * sizeof Bank

    //printf("Need to load %d bytes.\n", Bytes_To_Load);

    fseek(f, 2, SEEK_SET);

    //This is wrong. You're not reading all banks.

    Computer->Current_RAM_Bank = 0;

    for(unsigned char j = 0; j < Banks_On_Disk; j++)
    {
        //printf("Bank: %d\n", j);
        fread(&Computer->RAM[j],1,sizeof(struct _SixtyFourK_Bank),f);
        //printf("Loaded a Bank");
    }
    fclose(f);
    //printf("Loaded ROM");
    Computer->CPU_Is_Running = 1;
};


void Load_BIOS_Into_RAM_And_Start(struct _Computer* Computer, const char* Filename)
{

};

void Process_Computer(struct _Computer* Computer, int Maximum_Cycles)
{
    if(Computer->Power_Status == 1)
    {
    if(Computer->CPU_Is_Running == 1)
    {
        int Cycles = 0;
        while(Cycles < Maximum_Cycles)
        {
        //Run_New_Opcode_Clear_Screen(Computer);
        Run_A_Processing_Cycle(Computer);
        Update_Computer_Video_Card(Computer);
       // Computer->Cycles += 1;
        ////printf("Cycles: %d\n", Cycles);
        Cycles += 1;
        //60 cycles/second 60Hz
        };
    }
    }
}

void Run_A_Processing_Cycle(struct _Computer* Computer)
{
    unsigned char Current_Opcode = Computer->RAM[Computer->Current_RAM_Bank].Bank[Computer->Processor.Instruction_Pointer];
    ////printf("r0: %d r1: %d\n", Computer->Processor.Register_Zero, Computer->Processor.Register_One);
    switch(Current_Opcode)
    {
        case NEW_OPCODE_NOP:
            Run_New_Opcode_NOP(Computer);
        break;

        case NEW_OPCODE_MOV_RZero_RZero:
            Run_New_Opcode_MOV_RZero_RZero(Computer);
        break;

        case NEW_OPCODE_MOV_RZero_ROne:
            Run_New_Opcode_MOV_RZero_ROne(Computer);
        break;

        case NEW_OPCODE_MOV_ROne_RZero:
            Run_New_Opcode_MOV_ROne_RZero(Computer);
        break;

        case NEW_OPCODE_MOV_ROne_ROne:
            Run_New_Opcode_MOV_ROne_ROne(Computer);
        break;

        case NEW_OPCODE_MOV_RZero_ADDR:
            Run_New_Opcode_MOV_RZero_ADDR(Computer);
        break;
        case NEW_OPCODE_MOV_ROne_ADDR:
            Run_New_Opcode_MOV_ROne_ADDR(Computer);
        break;

        case NEW_OPCODE_MOV_ADDR_RZero:
            Run_New_Opcode_MOV_ADDR_RZero(Computer);
        break;
        case NEW_OPCODE_MOV_ADDR_ROne:
            Run_New_Opcode_MOV_ADDR_ROne(Computer);
        break;

        case NEW_OPCODE_PUSH_RZero:
            Run_New_Opcode_PUSH_RZero(Computer);
        break;
        case NEW_OPCODE_PUSH_ROne:
            Run_New_Opcode_PUSH_ROne(Computer);
        break;

        case NEW_OPCODE_POP_RZero:
            Run_New_Opcode_POP_RZero(Computer);
        break;
        case NEW_OPCODE_POP_ROne:
            Run_New_Opcode_POP_ROne(Computer);
        break;

        case NEW_OPCODE_ADD_RZero_RZero:
            Run_New_Opcode_ADD_RZero_RZero(Computer);
        break;
        case NEW_OPCODE_ADD_RZero_ROne:
            Run_New_Opcode_ADD_RZero_ROne(Computer);
        break;
        case NEW_OPCODE_ADD_ROne_RZero:
            Run_New_Opcode_ADD_ROne_RZero(Computer);
        break;
        case NEW_OPCODE_ADD_ROne_ROne:
            Run_New_Opcode_ADD_ROne_ROne(Computer);
        break;

        case NEW_OPCODE_SUB_RZero_RZero:
            Run_New_Opcode_SUB_RZero_RZero(Computer);
        break;
        case NEW_OPCODE_SUB_RZero_ROne:
            Run_New_Opcode_SUB_RZero_ROne(Computer);
        break;
        case NEW_OPCODE_SUB_ROne_RZero:
            Run_New_Opcode_SUB_ROne_RZero(Computer);
        break;
        case NEW_OPCODE_SUB_ROne_ROne:
            Run_New_Opcode_SUB_ROne_ROne(Computer);
        break;
        case NEW_OPCODE_MUL_RZero_RZero:
            Run_New_Opcode_MUL_RZero_RZero(Computer);
        break;
        case NEW_OPCODE_MUL_RZero_ROne:
            Run_New_Opcode_MUL_RZero_ROne(Computer);
        break;
        case NEW_OPCODE_MUL_ROne_RZero:
            Run_New_Opcode_MUL_ROne_RZero(Computer);
        break;
        case NEW_OPCODE_MUL_ROne_ROne:
            Run_New_Opcode_MUL_ROne_ROne(Computer);
        break;

        case NEW_OPCODE_DIV_RZero_RZero:
            Run_New_Opcode_DIV_RZero_RZero(Computer);
        break;
        case NEW_OPCODE_DIV_RZero_ROne:
            Run_New_Opcode_DIV_RZero_ROne(Computer);
        break;
        case NEW_OPCODE_DIV_ROne_RZero:
            Run_New_Opcode_DIV_ROne_RZero(Computer);
        break;
        case NEW_OPCODE_DIV_ROne_ROne:
            Run_New_Opcode_DIV_ROne_ROne(Computer);
        break;

        case NEW_OPCODE_PWR_RZero_RZero:
            Run_New_Opcode_PWR_RZero_RZero(Computer);
        break;
        case NEW_OPCODE_PWR_RZero_ROne:
            Run_New_Opcode_PWR_RZero_ROne(Computer);
        break;
        case NEW_OPCODE_PWR_ROne_RZero:
            Run_New_Opcode_PWR_ROne_RZero(Computer);
        break;
        case NEW_OPCODE_PWR_ROne_ROne:
            Run_New_Opcode_PWR_ROne_ROne(Computer);
        break;

        case NEW_OPCODE_ROOT_RZero_RZero:
            Run_New_Opcode_ROOT_RZero_RZero(Computer);
        break;
        case NEW_OPCODE_ROOT_RZero_ROne:
            Run_New_Opcode_ROOT_RZero_ROne(Computer);
        break;
        case NEW_OPCODE_ROOT_ROne_RZero:
            Run_New_Opcode_ROOT_ROne_RZero(Computer);
        break;
        case NEW_OPCODE_ROOT_ROne_ROne:
            Run_New_Opcode_ROOT_ROne_ROne(Computer);
        break;

        case NEW_OPCODE_JMP_ADDRSIXTEEN:
            Run_New_Opcode_JMP(Computer);
        break;
        case NEW_OPCODE_JE_ADDRSIXTEEN:
            Run_New_Opcode_JE(Computer);
        break;
        case NEW_OPCODE_JNE_ADDRSIXTEEN:
            Run_New_Opcode_JNE(Computer);
        break;
        case NEW_OPCODE_CALL_ADDRSIXTEEN:
            Run_New_Opcode_CALL(Computer);
        break;

        case NEW_OPCODE_RET:
            Run_New_Opcode_RET(Computer);
        break;
        case NEW_OPCODE_HALT:
            Run_New_Opcode_HALT(Computer);
        break;


        case NEW_OPCODE_MOV_RZero_Value:
            Run_New_Opcode_MOV_RZero_Value(Computer);
        break;
        case NEW_OPCODE_MOV_ROne_Value:
            Run_New_Opcode_MOV_ROne_Value(Computer);
        break;


        case NEW_OPCODE_CMP_RZero_Value:
            Run_New_Opcode_CMP_RZero_Value(Computer);
        break;
        case NEW_OPCODE_CMP_ROne_Value:
            Run_New_Opcode_CMP_ROne_Value(Computer);
        break;

        case NEW_OPCODE_IN:
            Run_New_Opcode_IN(Computer);
        break;
        case NEW_OPCODE_OUT:
            Run_New_Opcode_OUT(Computer);
        break;
        case NEW_OPCODE_REP:
            Run_New_Opcode_REP(Computer);
        break;

//FLAGS
        case NEW_OPCODE_SET_ZERO_FLAG:
            Run_New_Opcode_SET_ZERO_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_CARRY_FLAG:
            Run_New_Opcode_SET_CARRY_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_SIGN_FLAG:
            Run_New_Opcode_SET_SIGN_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_OVERFLOW_FLAG:
            Run_New_Opcode_SET_OVERFLOW_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_PARITY_FLAG:
            Run_New_Opcode_SET_PARITY_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_AUXILIARY_CARRY_FLAG:
            Run_New_Opcode_SET_AUXILIARY_CARRY_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_DIRECTION_FLAG:
            Run_New_Opcode_SET_DIRECTION_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_MASKABLE_INTERRUPT_FLAG:
            Run_New_Opcode_SET_MASKABLE_INTERRUPT_FLAG(Computer);
        break;
        case NEW_OPCODE_SET_TRAP_FLAG:
            Run_New_Opcode_SET_TRAP_FLAG(Computer);
        break;

        case NEW_OPCODE_ERROR_HANDLER:
            Run_New_Opcode_Error_Handler(Computer);
        break;

        case NEW_OPCODE_RENDER_SPRITE:
            Run_New_Opcode_Render_Sprite(Computer);
        break;



        case NEW_OPCODE_PUSH_RZero_HI:
            Run_New_Opcode_PUSH_RZero_HI(Computer);
        break;
        case NEW_OPCODE_PUSH_ROne_HI:
            Run_New_Opcode_PUSH_ROne_HI(Computer);
        break;

        case NEW_OPCODE_POP_RZero_LO:
            Run_New_Opcode_POP_RZero_LO(Computer);
        break;
        case NEW_OPCODE_POP_ROne_LO:
            Run_New_Opcode_POP_ROne_LO(Computer);
        break;

        case NEW_OPCODE_RENDER_POINT:
            Run_New_Opcode_Render_Point(Computer);
        break;

        case NEW_OPCODE_RENDER_LINE:
            Run_New_Opcode_Render_Line(Computer);
        break;

        case NEW_OPCODE_RENDER_TRIANGLE:
            Run_New_Opcode_Render_Triangle(Computer);
        break;

        case NEW_OPCODE_RENDER_TILEMAP:
            Run_New_Opcode_Render_Tilemap(Computer);
        break;

         case NEW_OPCODE_CLEAR_SCREEN:
            Run_New_Opcode_Clear_Screen(Computer);
        break;

        case NEW_OPCODE_FLIP_SCREEN:
            Run_New_Opcode_Flip_Screen(Computer);
        break;


        case NEW_OPCODE_MOV_RONE_RANDOM:
            Run_New_Opcode_MOV_ROne_Random(Computer);
        break;

        case NEW_OPCODE_MOV_RZERO_RANDOM:
            Run_New_Opcode_MOV_RZero_Random(Computer);
        break;



        case NEW_OPCODE_INC_RZero:
            Run_NEW_OPCODE_INC_RZero(Computer);
        break;

        case NEW_OPCODE_INC_ROne:
            Run_NEW_OPCODE_INC_ROne(Computer);
        break;

        default:
            Run_New_Opcode_Error_Handler(Computer);
        break;
    }
};

void Update_Computer_Inputs(struct _Computer* Computer)
{

};

void Update_Computer_Video_Card(struct _Computer* Computer)
{
    //glGenTextures(1, &Computer->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Computer->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int in = 0;
    int r = 0;
    //Image->RGBA_Canvas[0] = Image->RGB_Canvas[0];

    unsigned char Fourth_Color = 255;

    unsigned char Red_mask = 0xC0;
    unsigned char Green_mask = 0x30;
    unsigned char Blue_mask = 0x0C;
    unsigned char Transparency_Level_Mask = 0x03;


    if(Computer->Color_Mode == VIDEO_MODE_MONOCHROME)
    {
    for(int j = 0; j < 65536; j++)
    {
       // printf("Render");

        if(Computer->Video_RAM_Back_Buffer[in] < 255)
        {
        memcpy(&Computer->True_Video_Screen[r], &Computer->Video_RAM_Back_Buffer[in], 1);
        memcpy(&Computer->True_Video_Screen[r+1], &Computer->Video_RAM_Back_Buffer[in], 1);
        memcpy(&Computer->True_Video_Screen[r+2], &Computer->Video_RAM_Back_Buffer[in], 1);

        Computer->True_Video_Screen[r] = Computer->True_Video_Screen[r]*Computer->Red_Level;
        Computer->True_Video_Screen[r+1] = Computer->True_Video_Screen[r+1]*Computer->Green_Level;
        Computer->True_Video_Screen[r+2] = Computer->True_Video_Screen[r+2]*Computer->Blue_Level;
        }

        in += 1;
        r += 4;
    }
    }



    int Shade_Of_Red;
    int Shade_Of_Green;
    int Shade_Of_Blue;
    int Transparency;

    if(Computer->Color_Mode == VIDEO_MODE_COLOR)
    {
    for(int j = 0; j < ((256*256*4)/4); j++)
    {
        Shade_Of_Red = Computer->Video_RAM_Back_Buffer[in] >> 6;
        Shade_Of_Green = Computer->Video_RAM_Back_Buffer[in] >> 4;
        Shade_Of_Blue = Computer->Video_RAM_Back_Buffer[in] >> 2;
        Transparency = Computer->Video_RAM_Back_Buffer[in] & 0x03;

        //unsigned char Transparency_Level_Mask = masked_Transparent_bits >> 0;

        //printf("R %u\n G %u\n B %u\n", Shade_Of_Red, Shade_Of_Green, Shade_Of_Blue);


        Computer->True_Video_Screen[r] = (unsigned char)Shade_Of_Red;
        Computer->True_Video_Screen[r + 1] = (unsigned char)Shade_Of_Green;
        Computer->True_Video_Screen[r + 2] = (unsigned char)Shade_Of_Blue;
        Computer->True_Video_Screen[r + 3] = (unsigned char)Transparency;

        //Computer->True_Video_Screen[r + 3] = Fourth_Color;
        in += 1;
        r += 4;
    }
    }


    //Copy_Section(4,Section,Computer->True_Video_Screen,24,48,256,256,0,0,0,32,24,48);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Computer->True_Video_Screen);
    glEnable(GL_TEXTURE_2D);
};

void Render_Computer(struct _Computer* Computer, float x, float y)
{
    glBindTexture(GL_TEXTURE_2D, Computer->Texture_ID);

    float Vertices[] = {x, y, x + 256, y, x + 256, y + 256, x, y + 256};
    float Texture_Vertices[] = {0, 0, 1, 0, 1, -1, 0, -1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(2, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
};


void Render_Computer_Vertices(struct _Computer* Computer, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
{
    glBindTexture(GL_TEXTURE_2D, Computer->Texture_ID);

    float Vertices[] = {x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4};
    float Texture_Vertices[] = {0, 0, 1, 0, 1, -1, 0, -1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(3, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
};



void Create_Cartridge(unsigned char Number_Of_Banks, const char* Output_Filename)
{
    struct _Memory_Cartridge Cartridge;

    Cartridge.Number_Of_Banks = Number_Of_Banks;
    Cartridge.Offset_To_Banks = 2;

    struct _SixtyFourK_Bank Bank;

    for(int j = 0; j < 65536; j++)
    {
        Bank.Bank[j] = 0x00;
    }


    FILE* f;
    f = fopen(Output_Filename,"wb");
    fwrite(&Cartridge,1,sizeof(struct _Memory_Cartridge),f);
    rewind(f);

    fseek(f, Cartridge.Offset_To_Banks, SEEK_SET);

    for(int j = 0; j < Cartridge.Number_Of_Banks; j++)
    {
        fwrite(&Bank, 1, sizeof(struct _SixtyFourK_Bank), f);
    }
    fclose(f);
};

void Convert_Code_To_Cartridge_Data(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset, const char* Filename)
{

};

void Convert_BMP_To_Cartridge_Data(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset, const char* Filename)
{

};

void Convert_Data_To_Cartridge_Data(struct _Memory_Cartridge* Cartridge, unsigned char Bank_ID, unsigned short Bank_Offset, const char* Filename)
{

};

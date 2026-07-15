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

void Process_New_Computer_Cycles(struct _Engine* Engine, struct _Seat_Object* Seat, int Cycles, struct _Audio_Chip* Audio_Chip)
{
    if(!Seat->New_Computer->Processor.Processor_Halted && Seat->New_Computer->Power_State)
    {
        for(int j = 0; j < Cycles; j++)
        {
            Process_New_Computer(Engine,Seat,Audio_Chip);
        }
    }
};

void Process_New_Computer(struct _Engine* Engine, struct _Seat_Object* Seat, struct _Audio_Chip* Audio_Chip)
{
    double x = Get_Ticks(Seat->New_Computer->Universal_Timer);
    if(x >= Seat->New_Computer->Timer && !Seat->New_Computer->Timer_Triggered)
    {
        if(Seat->New_Computer->Timer_Set)
        {
            Seat->New_Computer->Timer_Triggered = true;
            Seat->New_Computer->Timer_Set = false;
            //printf("Timer Triggered!\n");
        }
    }

    if(x >= Seat->New_Computer->Last_Time_Cursor_Was_Visible + 0.5)
    {
        if(Seat->New_Computer->Cursor_Visible == false)
        {
            Seat->New_Computer->Cursor_Visible = true;
            //printf("Cursor is visible!\n");
        }

        else if(Seat->New_Computer->Cursor_Visible == true)
        {
            Seat->New_Computer->Cursor_Visible = false;
        }
        Seat->New_Computer->Last_Time_Cursor_Was_Visible = x;
    }

    if(!Seat->New_Computer->Timer_Set) //Delay timer
    {
    unsigned char Current_Opcode = Seat->New_Computer->RAM[Seat->New_Computer->Current_RAM_Bank].Bank[Seat->New_Computer->Processor.Instruction_Pointer];
    //////printf("IP: %d\n", Seat->New_Computer->Processor.Instruction_Pointer);
    //////printf("Current RAM Bank: %d\n", Seat->New_Computer->Current_RAM_Bank);
    //////printf("Opcode: %d\n", Current_Opcode);
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
            Process_Opcode_INT(Engine,Seat,Audio_Chip);
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
            ////printf("Invalid opcode at IP: %d %d\n", Seat->New_Computer->Current_RAM_Bank,Seat->New_Computer->Processor.Instruction_Pointer);
            Seat->New_Computer->Processor.Processor_Halted = true;
        break;
    }


    }

    if(!Seat->New_Computer->Processor.Audio_Processor.Tracking_On)
    {
        sid_gate_off(Audio_Chip,0);
        sid_gate_off(Audio_Chip,1);
        sid_gate_off(Audio_Chip,2);
    }

    else if(Seat->New_Computer->Processor.Audio_Processor.Tracking_On)
    {
    //Get 3 steps from memory. 1 for each channel
    Step Channel_Zero;
    Step Channel_One;
    Step Channel_Two;

    Channel_Zero.note = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+0];
    Channel_Zero.instrument_id = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+1];
    Channel_Zero.gate = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+2];

    printf("Ch 0 Note %d Instrum %d Gate %d\n", Channel_Zero.note,Channel_Zero.instrument_id,Channel_Zero.gate);

    Channel_One.note = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+3];
    Channel_One.instrument_id = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+4];
    Channel_One.gate = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+5];
    printf("Ch 1 Note %d Instrum %d Gate %d\n", Channel_One.note,Channel_One.instrument_id,Channel_One.gate);

    Channel_Two.note = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+6];
    Channel_Two.instrument_id = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+7];
    Channel_Two.gate = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Track_Bank].Bank[Seat->New_Computer->Processor.Audio_Processor.Track_Address+8];

    printf("Ch 2 Note %d Instrum %d Gate %d\n", Channel_Two.note,Channel_Two.instrument_id,Channel_Two.gate);

    if(!Channel_Zero.gate)
    {
        sid_gate_off(Audio_Chip, 0);
        printf("Channel 0 off!\n");
    }

    if(!Channel_One.gate)
    {
        sid_gate_off(Audio_Chip, 1);
        printf("Channel 1 off!\n");
    }

    if(!Channel_Two.gate)
    {
        sid_gate_off(Audio_Chip, 2);
        printf("Channel 2 off!\n");
    }

    else if(Channel_Zero.gate)
    {
        if(Channel_Zero.note > 0)
        {
            printf("Channel 0 on!\n");
            uint16_t freq_reg = note_to_sid_freq(Channel_Zero.note);
            unsigned char This_Instrument = Channel_Zero.instrument_id;
            sid_gate_on(Audio_Chip, 0, freq_reg, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instwaveform, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instattack, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instdecay, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instsustain, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instrelease);
            if (Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instwaveform == WAVE_PULSE)
            {
                Audio_Chip->channels[0].pulse_width = Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instpulse_width;
            }
        }
    }





    else if(Channel_One.gate)
    {
        if(Channel_One.note > 0)
        {
            printf("Channel 1 on!\n");
            uint16_t freq_reg = note_to_sid_freq(Channel_One.note);
            unsigned char This_Instrument = Channel_One.instrument_id;
            sid_gate_on(Audio_Chip, 1, freq_reg, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instwaveform, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instattack, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instdecay, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instsustain, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instrelease);
            if (Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instwaveform == WAVE_PULSE)
            {
                Audio_Chip->channels[1].pulse_width = Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instpulse_width;
            }
        }
    }



    else if(Channel_Two.gate)
    {
        if(Channel_Two.note > 0)
        {
            printf("Channel 2 on!\n");
            uint16_t freq_reg = note_to_sid_freq(Channel_Two.note);
            unsigned char This_Instrument = Channel_Two.instrument_id;
            sid_gate_on(Audio_Chip, 2, freq_reg, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instwaveform, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instattack, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instdecay, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instsustain, Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instrelease);
            if (Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instwaveform == WAVE_PULSE)
            {
                Audio_Chip->channels[2].pulse_width = Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[This_Instrument].Instpulse_width;
            }
        }
    }


    double x = Get_Ticks(Seat->New_Computer->Universal_Timer);
        printf("x: %f\n",x);
        printf("Step Spacing: %f\n", Seat->New_Computer->Processor.Audio_Processor.Step_Spacing);
        printf("Last Time Step: %f\n", Seat->New_Computer->Processor.Audio_Processor.Last_Time_Step_Triggered);
        printf("Track Address: %d\n", Seat->New_Computer->Processor.Audio_Processor.Track_Address);
        if(x >= Seat->New_Computer->Processor.Audio_Processor.Last_Time_Step_Triggered + (Seat->New_Computer->Processor.Audio_Processor.Step_Spacing/1000))
        {
            Seat->New_Computer->Processor.Audio_Processor.Track_Address += 9;

            if(Seat->New_Computer->Processor.Audio_Processor.Track_Address >= Seat->New_Computer->Processor.Audio_Processor.Track_End_Address)
            {
                printf("Track Address exceeds End address. Looping.\n");
                Seat->New_Computer->Processor.Audio_Processor.Track_Address = Seat->New_Computer->Processor.Audio_Processor.Track_Start_Address;
            }
            Seat->New_Computer->Processor.Audio_Processor.Last_Time_Step_Triggered = x;
        }

    }

    }
};

void Process_Opcode_INT(struct _Engine* Engine, struct _Seat_Object* Seat, struct _Audio_Chip* Audio_Chip)
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

        case INT_RENDER_TILEMAP:
            Process_INT_Render_Tilemap(Engine,Seat);
        break;

        case INT_RENDER_CURSOR:
            Process_INT_Render_Cursor(Engine,Seat);
        break;

        case INT_SET_TIMER:
            Process_INT_Set_Timer(Engine,Seat);
        break;

        case INT_PLAY_INSTRUMENT:
            Process_INT_Play_Instrument(Engine,Seat, Audio_Chip);
        break;

        case INT_PLAY_TRACK:
            Process_INT_Play_Track(Engine,Seat);
        break;



        case INT_STOP_INSTRUMENT:
            Process_INT_Stop_Instrument(Engine,Seat, Audio_Chip);
        break;

        case INT_STOP_TRACK:
//            Process_INT_Stop_Track(Engine,Seat);
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
    ////printf("Processor Halted!\n");
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
    ////printf("Cleared screen\n");
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
    ////printf("Flipped screen\n");
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
    Seat->New_Computer->Processor.Equals_Flag = false;
    Seat->New_Computer->Processor.Greater_Flag = false;
    Seat->New_Computer->Processor.Less_Flag = false;

    if(Seat->New_Computer->Processor.Register_Zero == Seat->New_Computer->Processor.Register_One)
    {
        Seat->New_Computer->Processor.Equals_Flag = true;
        ////printf("RZero Equals ROne!\n");
    }

    if(Seat->New_Computer->Processor.Register_Zero > Seat->New_Computer->Processor.Register_One)
    {
        Seat->New_Computer->Processor.Greater_Flag = true;
        ////printf("RZero Greater  ROne!\n");
    }

    if(Seat->New_Computer->Processor.Register_Zero < Seat->New_Computer->Processor.Register_One)
    {
        Seat->New_Computer->Processor.Less_Flag = true;
        ////printf("RZero Less ROne!\n");
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
    //printf("The result is %d\n", Seat->New_Computer->Processor.Register_Zero);

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
        //printf("Divide by Zero error!\n");
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
    //printf("The quotient is %d and the remainder is %d\n", quotient, remainder);
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
    //printf("The result is %d\n", Seat->New_Computer->Processor.Register_One);

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
        //printf("Divide by Zero error!\n");
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

    //printf("The quotient is %d and the remainder is %d\n", quotient, remainder);
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
    ////printf("The VALUE at r0 is now %d\n", Value);
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
    ////printf("The VALUE at r1 is now %d\n", Value);
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

    ////printf("Moved Value into Bank %d Address %d\n", Target_Bank, Target_Address);

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

    ////printf("Moved Value into Bank %d Address %d\n", Target_Bank, Target_Address);

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

    ////printf("The Value %d is going into Register Zero.\n", Value);

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

    ////printf("MOV R1 Target Bank %d %d\n", Target_Bank, Target_Address);

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
    ////printf("Pushed %d\n", Seat->New_Computer->Processor.Register_Zero);
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
        if(!Seat->New_Computer->Processor.Greater_Flag && !Seat->New_Computer->Processor.Less_Flag)
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
    }

    else if(!Seat->New_Computer->Processor.Equals_Flag || Seat->New_Computer->Processor.Greater_Flag || Seat->New_Computer->Processor.Less_Flag)
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

    else if(!Seat->New_Computer->Processor.Greater_Flag || !Seat->New_Computer->Processor.Equals_Flag)
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

    else if(!Seat->New_Computer->Processor.Less_Flag || !Seat->New_Computer->Processor.Equals_Flag)
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

    ////printf("Returning to %d %d\n", Target_Bank,Target_Address);

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
        ////printf("Keypad Up!\n");
    }

    if(Keypad.Keyboard_Down)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[1] = 255;
        ////printf("Keypad Down!\n");
    }

    if(Keypad.Keyboard_Left)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[2] = 255;
        ////printf("Keypad Left!\n");
    }

    if(Keypad.Keyboard_Right)
    {
        Seat->New_Computer->Controller_D_Pad_RAM[3] = 255;
        ////printf("Keypad Right!\n");
    }


    if(Keypad.Keyboard_A)
    {
        Seat->New_Computer->Controller_Button_RAM[0] = 255;
        ////printf("Keypad A!\n");
    }
    if(Keypad.Keyboard_B)
    {
        Seat->New_Computer->Controller_Button_RAM[1] = 255;
        ////printf("Keypad B!\n");
    }




    if(!Keypad.Keyboard_A)
    {
        Seat->New_Computer->Controller_Button_RAM[0] = 0;
        ////printf("Keypad A!\n");
    }
    if(!Keypad.Keyboard_B)
    {
        Seat->New_Computer->Controller_Button_RAM[1] = 0;
        ////printf("Keypad B!\n");
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
    //sid_gate_off(Audio_Chip,0);
    //sid_gate_off(Audio_Chip,1);
    //sid_gate_off(Audio_Chip,2);

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
   Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank = 0;

    Seat->New_Computer->Processor.Audio_Processor.Track_Bank = 0;
    Seat->New_Computer->Processor.Audio_Processor.Track_Address = 0;
    Seat->New_Computer->Processor.Audio_Processor.Track_Start_Address = 0;
    Seat->New_Computer->Processor.Audio_Processor.Track_End_Address = 0;
    Seat->New_Computer->Processor.Audio_Processor.Current_Step = 0;

    Seat->New_Computer->Processor.Audio_Processor.Last_Time_Step_Triggered = 0;
    Seat->New_Computer->Processor.Audio_Processor.Step_Spacing = 0;
    Seat->New_Computer->Processor.Audio_Processor.Tracking_On = false;

    //sid_gate_off(Audio_Chip,0);
   // sid_gate_off(Audio_Chip,1);
  //  sid_gate_off(Audio_Chip,2);



    Seat->New_Computer->Power_State = true;
    Seat->New_Computer->Processor.Processor_Halted = false;

    Seat->New_Computer->Universal_Timer = Create_Timer();
    Set_Timer(Seat->New_Computer->Universal_Timer);
    Seat->New_Computer->Last_Time_Cursor_Was_Visible = 0;
    Seat->New_Computer->Timer = 0;
    Seat->New_Computer->Timer_Triggered = false;
    Seat->New_Computer->Timer_Set = false;
    Seat->New_Computer->Cursor_Visible = false;

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

            Seat->New_Computer->RGB_Canvas[r]*=255;
            Seat->New_Computer->RGB_Canvas[r+1]*=255;
            Seat->New_Computer->RGB_Canvas[r+2]*=255;

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


void Write_Instrument_Onto_ROM(struct _New_Compiler* New_Compiler, unsigned char Bank, unsigned short Address)
{
    Instrument Ins;

    Ins.Instwaveform = 0x20;
    Ins.Instattack = 1;
    Ins.Instdecay = 4;
    Ins.Instsustain = 12;
    Ins.Instrelease = 4;
    Ins.Instpulse_width = 2048;

    New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+0] = Ins.Instwaveform;
    New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+1] = Ins.Instattack;
    New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+2] = Ins.Instdecay;
    New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+3] = Ins.Instsustain;
    New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+4] = Ins.Instrelease;
    New_Compiler->ROM_Memory_Zero[Bank].Bank[Address+5] = Ins.Instpulse_width;

    //printf("Wrote instrument to Bank %d Address %d\n", Bank, Address);
}



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
        //printf("Wrote a file to Bank %d Address %d\n", Bank,Address);
        }

    }

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

    struct _ENCODING* ENCODINGS[65536];
    unsigned int Number_Of_ENCODINGS = 0;

    struct _TRACK* TRACKS[65536];
    unsigned int Number_Of_TRACKS = 0;

    unsigned char InstrumentBank;
    unsigned short InstrumentAddress;

    ////printf("The Current ROM Bank is %d. The Current ROM Address is %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);

    New_Compiler->Current_ROM_Bank = 1;

    for(int j = 0; j < File_Size; j++)
    {
        if(strncmp(&Buffer[j], "ENCODING",8) == 0 && Buffer[j+8] == ' ')
        {
            //printf("Found a Encoding.\n");
            size_t length = strcspn(&Buffer[j + 9], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 9 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            //printf("After this is %c\n",Buffer[j + 9 + length + 1]);
            if(Buffer[j + 9 + length + 1] != 'B')
            {
                //printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 9 + length + 2], ";");
            //printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                //printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 9 + x + length + 2];
                //printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            //printf("After this is %c\n",Buffer[j + 9 + length + banklength + 4]);
            if(Buffer[j + 9 + length + banklength + 4] != 'A')
            {
                //printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 9 + length + 2 + banklength + 3], ";");
            //printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                //printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 9 + x + length + 2 + banklength + 3];
                //printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';

            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);

            //printf("This Encoding lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                //printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                //printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            ENCODINGS[Number_Of_ENCODINGS] = (struct _ENCODING*)calloc(1,sizeof(struct _ENCODING));

            for(int x = 0; x < 16; x++)
            {
                ENCODINGS[Number_Of_ENCODINGS]->Name[x] = Data_Block_Name[x];
                //printf("%c\n",ENCODINGS[Number_Of_ENCODINGS]->Name[x]);
            }
            ENCODINGS[Number_Of_ENCODINGS]->Bank = (unsigned char)Bank_Value;
            ENCODINGS[Number_Of_ENCODINGS]->Address = (unsigned short)Address_Value;
            //printf("%d %d\n", ENCODINGS[Number_Of_ENCODINGS]->Bank,ENCODINGS[Number_Of_ENCODINGS]->Address);

            Number_Of_ENCODINGS += 1;
        }




        if(strncmp(&Buffer[j], "DATABLOCK",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a data block.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            ////printf("After this is %c\n",Buffer[j + 10 + length + 1]);
            if(Buffer[j + 10 + length + 1] != 'B')
            {
                ////printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 10 + length + 2], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                ////printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 2];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            ////printf("After this is %c\n",Buffer[j + 10 + length + banklength + 4]);
            if(Buffer[j + 10 + length + banklength + 4] != 'A')
            {
                ////printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 10 + length + 2 + banklength + 3], ";");
            ////printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                ////printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 10 + x + length + 2 + banklength + 3];
                ////printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';

            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);

            ////printf("This DBlock lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                ////printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                ////printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            DBLOCKS[Number_Of_DBLOCKS] = (struct _DBLOCK*)calloc(1,sizeof(struct _DBLOCK));

            for(int x = 0; x < 16; x++)
            {
                DBLOCKS[Number_Of_DBLOCKS]->Name[x] = Data_Block_Name[x];
                ////printf("%c\n",DBLOCKS[Number_Of_DBLOCKS]->Name[x]);
            }
            DBLOCKS[Number_Of_DBLOCKS]->Bank = (unsigned char)Bank_Value;
            DBLOCKS[Number_Of_DBLOCKS]->Address = (unsigned short)Address_Value;
            ////printf("%d %d\n", DBLOCKS[Number_Of_DBLOCKS]->Bank,DBLOCKS[Number_Of_DBLOCKS]->Address);

            Number_Of_DBLOCKS += 1;
        }


        if(strncmp(&Buffer[j], "TILEMAP",7) == 0 && Buffer[j+7] == ' ') //Bank, Address, Cell Width, Cell Height
        {
            ////printf("Found a data block.\n");
            size_t length = strcspn(&Buffer[j + 8], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 8 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            ////printf("After this is %c\n",Buffer[j + 8 + length + 1]);
            if(Buffer[j + 8 + length + 1] != 'B')
            {
                ////printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 8 + length + 2], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                ////printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 8 + x + length + 2];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            ////printf("After this is %c\n",Buffer[j + 8 + length + banklength + 4]);
            if(Buffer[j + 8 + length + banklength + 4] != 'A')
            {
                ////printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 8 + length + 2 + banklength + 3], ";");
            ////printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                ////printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 8 + x + length + 2 + banklength + 3];
                ////printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';









            size_t CellWidthlength = strcspn(&Buffer[j + 8 + length + 2 + banklength + 3 + Addresslength + 3], ";");
            ////printf("Its CellWidthlength is %zu\n", CellWidthlength); // Outputs 5
            if(CellWidthlength > 5)
            {
                ////printf("Build Failed. CellWidth Blocks must be within five digits.");
                return false;
            }

            char Cell_Width[16];
            memset(Cell_Width,0,16);
            for(int x = 0; x < CellWidthlength; x++)
            {
                Cell_Width[x] = Buffer[j + 8 + x + length + 2 + banklength + 3 + Addresslength + 3];
                ////printf("%c\n", Cell_Width[x]);
            }
            Cell_Width[CellWidthlength] = '\0';





            size_t CellHeightlength = strcspn(&Buffer[j + 8 + length + 2 + banklength + 3 + Addresslength + 3 + CellWidthlength + 3], ";");
            ////printf("Its CellHeightlength is %zu\n", CellHeightlength); // Outputs 5
            if(CellHeightlength > 5)
            {
                ////printf("Build Failed. CellHeight Blocks must be within five digits.");
                return false;
            }

            char Cell_Height[16];
            memset(Cell_Height,0,16);
            for(int x = 0; x < CellHeightlength; x++)
            {
                Cell_Height[x] = Buffer[j + 8 + x + length + 2 + banklength + 3 + Addresslength + 3 + CellWidthlength + 3];
                ////printf("%c\n", Cell_Height[x]);
            }
            Cell_Height[CellHeightlength] = '\0';




            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);
            long Cell_Width_Value = strtol(Cell_Width, NULL, 10);
            long Cell_Height_Value = strtol(Cell_Height, NULL, 10);

            ////printf("This DBlock lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                ////printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                ////printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            TILEMAPS[Number_Of_TILEMAPS] = (struct _TILEMAP*)calloc(1,sizeof(struct _TILEMAP));

            for(int x = 0; x < 16; x++)
            {
                TILEMAPS[Number_Of_TILEMAPS]->Name[x] = Data_Block_Name[x];
                ////printf("%c\n",TILEMAPS[Number_Of_TILEMAPS]->Name[x]);
            }
            TILEMAPS[Number_Of_TILEMAPS]->Bank = (unsigned char)Bank_Value;
            TILEMAPS[Number_Of_TILEMAPS]->Address = (unsigned short)Address_Value;
            TILEMAPS[Number_Of_TILEMAPS]->Cell_Width = (unsigned char)Cell_Width_Value;
            TILEMAPS[Number_Of_TILEMAPS]->Cell_Height = (unsigned char)Cell_Height_Value;
            ////printf("%d %d\n", TILEMAPS[Number_Of_TILEMAPS]->Bank,TILEMAPS[Number_Of_TILEMAPS]->Address);

            ////printf("Width %d Height %d\n", TILEMAPS[Number_Of_TILEMAPS]->Cell_Width,TILEMAPS[Number_Of_TILEMAPS]->Cell_Height);
            Number_Of_TILEMAPS += 1;
        }


        if(strncmp(&Buffer[j], "TRACK",5) == 0 && Buffer[j+5] == ' ')
        {
            ////printf("Found a data block.\n");
            size_t length = strcspn(&Buffer[j + 6], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            ////printf("After this is %c\n",Buffer[j + 6 + length + 1]);
            if(Buffer[j + 6 + length + 1] != 'B')
            {
                ////printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 6 + length + 2], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                ////printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 6 + x + length + 2];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            ////printf("After this is %c\n",Buffer[j + 6 + length + banklength + 4]);
            if(Buffer[j + 6 + length + banklength + 4] != 'A')
            {
                ////printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 6 + length + 2 + banklength + 3], ";");
            ////printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                ////printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 6 + x + length + 2 + banklength + 3];
                ////printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';









            size_t CellWidthlength = strcspn(&Buffer[j + 6 + length + 2 + banklength + 3 + Addresslength + 3], ";");
            ////printf("Its CellWidthlength is %zu\n", CellWidthlength); // Outputs 5
            if(CellWidthlength > 5)
            {
                ////printf("Build Failed. CellWidth Blocks must be within five digits.");
                return false;
            }

            char Cell_Width[16];
            memset(Cell_Width,0,16);
            for(int x = 0; x < CellWidthlength; x++)
            {
                Cell_Width[x] = Buffer[j + 6 + x + length + 2 + banklength + 3 + Addresslength + 3];
                ////printf("%c\n", Cell_Width[x]);
            }
            Cell_Width[CellWidthlength] = '\0';





            size_t CellHeightlength = strcspn(&Buffer[j + 6 + length + 2 + banklength + 3 + Addresslength + 3 + CellWidthlength + 3], ";");
            ////printf("Its CellHeightlength is %zu\n", CellHeightlength); // Outputs 5
            if(CellHeightlength > 5)
            {
                ////printf("Build Failed. CellHeight Blocks must be within five digits.");
                return false;
            }

            char Cell_Height[16];
            memset(Cell_Height,0,16);
            for(int x = 0; x < CellHeightlength; x++)
            {
                Cell_Height[x] = Buffer[j + 6 + x + length + 2 + banklength + 3 + Addresslength + 3 + CellWidthlength + 3];
                ////printf("%c\n", Cell_Height[x]);
            }
            Cell_Height[CellHeightlength] = '\0';




            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);
            long Cell_Width_Value = strtol(Cell_Width, NULL, 10);
            long Cell_Height_Value = strtol(Cell_Height, NULL, 10);

            ////printf("This DBlock lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                ////printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                ////printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            TRACKS[Number_Of_TRACKS] = (struct _TRACK*)calloc(1,sizeof(struct _TRACK));

            for(int x = 0; x < 16; x++)
            {
                TRACKS[Number_Of_TRACKS]->Name[x] = Data_Block_Name[x];
                ////printf("%c\n",TRACKS[Number_Of_TRACKS]->Name[x]);
            }
            TRACKS[Number_Of_TRACKS]->Bank = (unsigned char)Bank_Value;
            TRACKS[Number_Of_TRACKS]->Address = (unsigned short)Address_Value;
            TRACKS[Number_Of_TRACKS]->Spacing = (unsigned char)Cell_Width_Value;
            TRACKS[Number_Of_TRACKS]->Length = (unsigned char)Cell_Height_Value;
            ////printf("%d %d\n", TRACKS[Number_Of_TRACKS]->Bank,TRACKS[Number_Of_TRACKS]->Address);

            ////printf("Width %d Height %d\n", TRACKS[Number_Of_TRACKS]->Cell_Width,TRACKS[Number_Of_TRACKS]->Cell_Height);
            Number_Of_TRACKS += 1;
        }


        if(strncmp(&Buffer[j], "VALUE",5) == 0 && Buffer[j+5] == ' ')
        {
            ////printf("Found a value.\n");
            size_t length = strcspn(&Buffer[j + 6], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            ////printf("After this is %c\n",Buffer[j + 6 + length + 1]);
            if(Buffer[j + 6 + length + 1] != 'B')
            {
                ////printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 6 + length + 2], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                ////printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 6 + x + length + 2];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            ////printf("After this is %c\n",Buffer[j + 6 + length + banklength + 4]);
            if(Buffer[j + 6 + length + banklength + 4] != 'A')
            {
                ////printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 6 + length + 2 + banklength + 3], ";");
            ////printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                ////printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 6 + x + length + 2 + banklength + 3];
                ////printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';

            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);

            ////printf("This Var lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                ////printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                ////printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            //Allocate memory.
            VARS[Number_Of_VARS] = (struct _VAR*)calloc(1,sizeof(struct _VAR));

            for(int x = 0; x < 16; x++)
            {
                VARS[Number_Of_VARS]->Name[x] = Data_Block_Name[x];
                ////printf("%c\n",VARS[Number_Of_VARS]->Name[x]);
            }
            VARS[Number_Of_VARS]->Bank = (unsigned char)Bank_Value;
            VARS[Number_Of_VARS]->Address = (unsigned short)Address_Value;
            ////printf("%d %d\n", VARS[Number_Of_VARS]->Bank,VARS[Number_Of_VARS]->Address);

            Number_Of_VARS += 1;
        }





        if(strncmp(&Buffer[j], "SETVARVAL",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a Set Variable to Value.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_SETVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        ////printf("Error: Could not handle SETVARVAL command.\n");
                        return false;
                    }
                    ////printf("I will SETVARVAL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }




if(strncmp(&Buffer[j], "MULVARVAL",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a Sub Val from Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_MULVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        ////printf("Error: Could not handle SUBVVL command.\n");
                        return false;
                    }
                    ////printf("I will SUBVVL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }






        if(strncmp(&Buffer[j], "DIVVARVAL",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a Sub Val from Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_DIVVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        ////printf("Error: Could not handle SUBVVL command.\n");
                        return false;
                    }
                    ////printf("I will SUBVVL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }




        if(strncmp(&Buffer[j], "SUBVARVAL",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a Sub Val from Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_SUBVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        ////printf("Error: Could not handle SUBVVL command.\n");
                        return false;
                    }
                    ////printf("I will SUBVVL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }




        if(strncmp(&Buffer[j], "ADDVARVAR",9) == 0 && Buffer[j+9] == ' ')
        {
            //printf("Found a ADD Var Var.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 10 + length + 1], ";");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 10 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';


            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char BankTwo;
            unsigned short AddressTwo;

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found X Var\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }

            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }


            //printf("I will set the value at Bank %d Address %d to the one in %d Address %d\n", Bank, Address, BankTwo, AddressTwo);
            if(!Handle_Aqueous_Basic_Text_ADDVVR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,BankTwo,AddressTwo))
            {
               //printf("Build failed: Could not insert SetVarVar\n");
                return false;
            }
        }



        if(strncmp(&Buffer[j], "DIVVARVAR",9) == 0 && Buffer[j+9] == ' ')
        {
            //printf("Found a DIV Var Var.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 10 + length + 1], ";");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 10 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';


            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char BankTwo;
            unsigned short AddressTwo;

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found X Var\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }

            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }


            //printf("I will set the value at Bank %d Address %d to the one in %d Address %d\n", Bank, Address, BankTwo, AddressTwo);
            if(!Handle_Aqueous_Basic_Text_DIVVVR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,BankTwo,AddressTwo))
            {
               //printf("Build failed: Could not insert SetVarVar\n");
                return false;
            }
        }





        if(strncmp(&Buffer[j], "MULVARVAR",9) == 0 && Buffer[j+9] == ' ')
        {
            //printf("Found a MUL Var Var.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 10 + length + 1], ";");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 10 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';


            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char BankTwo;
            unsigned short AddressTwo;

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found X Var\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }

            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }


            //printf("I will set the value at Bank %d Address %d to the one in %d Address %d\n", Bank, Address, BankTwo, AddressTwo);
            if(!Handle_Aqueous_Basic_Text_MULVVR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,BankTwo,AddressTwo))
            {
               //printf("Build failed: Could not insert SetVarVar\n");
                return false;
            }
        }






        if(strncmp(&Buffer[j], "SUBVARVAR",9) == 0 && Buffer[j+9] == ' ')
        {
            //printf("Found a Sub Var Var.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 10 + length + 1], ";");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 10 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';


            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char BankTwo;
            unsigned short AddressTwo;

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found X Var\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }

            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }


            //printf("I will set the value at Bank %d Address %d to the one in %d Address %d\n", Bank, Address, BankTwo, AddressTwo);
            if(!Handle_Aqueous_Basic_Text_SUBVVR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,BankTwo,AddressTwo))
            {
               //printf("Build failed: Could not insert SetVarVar\n");
                return false;
            }
        }




        if(strncmp(&Buffer[j], "SETVARVAR",9) == 0 && Buffer[j+9] == ' ')
        {
            //printf("Found a Set Var Var.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 10 + length + 1], ";");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 10 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';


            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char BankTwo;
            unsigned short AddressTwo;

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found X Var\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }

            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }


            //printf("I will set the value at Bank %d Address %d to the one in %d Address %d\n", Bank, Address, BankTwo, AddressTwo);
            if(!Handle_Aqueous_Basic_Text_SETVVR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,BankTwo,AddressTwo))
            {
               //printf("Build failed: Could not insert SetVarVar\n");
                return false;
            }
        }




        if(strncmp(&Buffer[j], "RENCURSOR",9) == 0 && Buffer[j+9] == ' ')
        {
            //printf("Found a Render Cursor.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 10 + length + 1], " ");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 10 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 10 + length + 1 + xvar + 1], ";");
            //printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 10 + x + length + 1 + xvar + 1];
                //printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';



            //Need encoding, encoding length, tiles per row.


            unsigned char Bank;
            unsigned short Address;

            unsigned char XBank;
            unsigned short XAddress;

            unsigned char YBank;
            unsigned short YAddress;

            unsigned char TileWidth;
            unsigned char TileHeight;

            //Find the label.
            for(int r = 0; r < Number_Of_TILEMAPS+1; r++)
            {
                if(r == Number_Of_TILEMAPS)
                {
                    //printf("Error: Could not find RENDER's parent TILEMAP.\n");
                    return false;
                }

                if(strncmp(TILEMAPS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found Tilemap's data block!\n");
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
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found X Var\n");
                    XBank = VARS[r]->Bank;
                    XAddress = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    YBank = VARS[r]->Bank;
                    YAddress = VARS[r]->Address;
                    break;
                }
            }

            //printf("The Tilemap's Bank is %d and its Address is %d\n", Bank, Address);
            //printf("Its width is %d and its height is %d\n", TileWidth, TileHeight);

            //printf("The Tilemap's X Coordinate Bank is %d and its Addr is %d\n", XBank,XAddress);
            //printf("The Tilemap's Y Coordinate Bank is %d and its Addr is %d\n", YBank,YAddress);

            if(!Handle_Aqueous_Basic_Text_RENCURSOR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank, Address,XBank,XAddress,YBank,YAddress,TileWidth,TileHeight))
            {
               //printf("Build failed: Could not insert RENDER Cursor\n");
                return false;
            }
        }




        if(strncmp(&Buffer[j], "GOIFVARGEQVAR",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 14], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 14 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 14 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 14 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 14 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            unsigned char GotoBank, Bank, BankTwo;
            unsigned short GotoAddress, Address, AddressTwo;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarEqVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVARGEQVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,BankTwo,AddressTwo))
            {
                //printf("Build failed: Could not insert GoifVarGEqVar\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }


    if(strncmp(&Buffer[j], "GOIFVARLEQVAR",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 14], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 14 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 14 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 14 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 14 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            unsigned char GotoBank, Bank, BankTwo;
            unsigned short GotoAddress, Address, AddressTwo;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarEqVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVARLEQVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,BankTwo,AddressTwo))
            {
                //printf("Build failed: Could not insert GoifVarLEQVar\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }



        if(strncmp(&Buffer[j], "GOIFVARNEQVAR",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 14], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 14 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 14 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 14 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 14 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            unsigned char GotoBank, Bank, BankTwo;
            unsigned short GotoAddress, Address, AddressTwo;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarEqVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVARNEQVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,BankTwo,AddressTwo))
            {
                //printf("Build failed: Could not insert GoifVarNEQVar\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }





        if(strncmp(&Buffer[j], "GOIFVARLSVAR",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            unsigned char GotoBank, Bank, BankTwo;
            unsigned short GotoAddress, Address, AddressTwo;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarLSVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVARLSVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,BankTwo,AddressTwo))
            {
                //printf("Build failed: Could not insert GoifVarLSVar\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }




        if(strncmp(&Buffer[j], "GOIFVARGRVAR",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            unsigned char GotoBank, Bank, BankTwo;
            unsigned short GotoAddress, Address, AddressTwo;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarGRVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVARGRVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,BankTwo,AddressTwo))
            {
                //printf("Build failed: Could not insert GoifVarGRVar\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }



        if(strncmp(&Buffer[j], "GOIFVAREQVAR",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            unsigned char GotoBank, Bank, BankTwo;
            unsigned short GotoAddress, Address, AddressTwo;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarEqVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }



            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    BankTwo = VARS[r]->Bank;
                    AddressTwo = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_GOIFVAREQVAR(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,BankTwo,AddressTwo))
            {
                //printf("Build failed: Could not insert GoifVarEqVar\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }



        if(strncmp(&Buffer[j], "RENTILES",8) == 0 && Buffer[j+8] == ' ')
        {
            //printf("Found a Render tiles.\n");
            size_t length = strcspn(&Buffer[j + 9], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 9 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 9 + length + 1], " ");
            //printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 9 + x + length + 1];
                //printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1], " ");
            //printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1];
                //printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';




            size_t Encvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1 + yvar + 1], " ");
            //printf("Its Encvar is %zu\n", Encvar); // Outputs 5

            char EncString[16];
            memset(EncString,0,16);
            for(int x = 0; x < Encvar; x++)
            {
                EncString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1 + yvar + 1];
                //printf("%c\n", EncString[x]);
            }
            EncString[Encvar] = '\0';




            size_t EncLengthvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1], " ");
            //printf("Its EncLengthvar is %zu\n", EncLengthvar); // Outputs 5

            char EncLengthString[16];
            memset(EncLengthString,0,16);
            for(int x = 0; x < EncLengthvar; x++)
            {
                EncLengthString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1];
                //printf("%c\n", EncLengthString[x]);
            }
            EncLengthString[EncLengthvar] = '\0';




            size_t TPRvar = strcspn(&Buffer[j + 9 + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1 + EncLengthvar + 1], ";");
            //printf("Its TPRvar is %zu\n", TPRvar); // Outputs 5

            char TPRString[16];
            memset(TPRString,0,16);
            for(int x = 0; x < TPRvar; x++)
            {
                TPRString[x] = Buffer[j + 9 + x + length + 1 + xvar + 1 + yvar + 1 + Encvar + 1 + EncLengthvar + 1];
                //printf("%c\n", TPRString[x]);
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
                    //printf("Error: Could not find RENDER's parent TILEMAP.\n");
                    return false;
                }

                if(strncmp(TILEMAPS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found Tilemap's data block!\n");
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
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found X Var\n");
                    XBank = VARS[r]->Bank;
                    XAddress = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    //printf("Found Y VAR!\n");
                    YBank = VARS[r]->Bank;
                    YAddress = VARS[r]->Address;
                    break;
                }
            }




            for(int r = 0; r < Number_Of_ENCODINGS+1; r++)
            {
                if(r == Number_Of_ENCODINGS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(ENCODINGS[r]->Name, EncString,16) == 0)
                {
                    //printf("Found Encoding\n");
                    EncodingBank = ENCODINGS[r]->Bank;
                    EncodingAddress = ENCODINGS[r]->Address;
                    break;
                }
            }









            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, EncLengthString,16) == 0)
                {
                    ////printf("Found RENTILES's encoding Var\n");
                    EncodingLengthBank = VARS[r]->Bank;
                    EncodingLengthAddress = VARS[r]->Address;
                    break;
                }
            }







            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, TPRString,16) == 0)
                {
                    ////printf("Found RENTILES's encoding Var\n");
                    TPRBank = VARS[r]->Bank;
                    TPRAddress = VARS[r]->Address;
                    break;
                }
            }

            //printf("The Tilemap's Bank is %d and its Address is %d\n", Bank, Address);
            //printf("Its width is %d and its height is %d\n", TileWidth, TileHeight);

            //printf("The Tilemap's X Coordinate Bank is %d and its Addr is %d\n", XBank,XAddress);
            //printf("The Tilemap's Y Coordinate Bank is %d and its Addr is %d\n", YBank,YAddress);

            //printf("The Encoding is on Bank %d Addr %d\n", EncodingBank, EncodingAddress);

            //printf("The Encoding Length is on Bank %d Addr %d\n", EncodingLengthBank, EncodingLengthAddress);

            //printf("The Tiles Per Row is on Bank %d Addr %d\n", TPRBank, TPRAddress);


            if(!Handle_Aqueous_Basic_Text_RENTILEMAP(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank, Address,XBank,XAddress,YBank,YAddress,EncodingBank,EncodingAddress,EncodingLengthBank,EncodingLengthAddress,TPRBank,TPRAddress,TileWidth,TileHeight))
            {
               //printf("Build failed: Could not insert RENDER TILES\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "ADDVARVAL",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found an Add Val to Variable.\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t banklength = strcspn(&Buffer[j + 10 + length + 1], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 10 + x + length + 1];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            long Set_Value = strtol(Data_Bank, NULL, 10);

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find SETVARVAL's parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its VAR!\n");
                    unsigned char Bank = VARS[r]->Bank;
                    unsigned short Address = VARS[r]->Address;
                    VARS[r]->Value = (unsigned short)Set_Value;
                    if(!Handle_Aqueous_Basic_Text_ADDVVL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,(unsigned short)Set_Value))
                    {
                        ////printf("Error: Could not handle ADDVVL command.\n");
                        return false;
                    }
                    ////printf("I will ADDVVL Bank %d Address %d to %d\n", Bank,Address,(unsigned short)Set_Value);
                    break;
                }
            }
        }


        if(strncmp(&Buffer[j], "RENDER",6) == 0 && Buffer[j+6] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 7], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 7 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 7 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 7 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 7 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 7 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
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
                    ////printf("Error: Could not find RENDER's parent DBLOCK.\n");
                    return false;
                }

                if(strncmp(DBLOCKS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found SETVARVAR's first VAR!\n");
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
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    ////printf("Found RENDER's second VAR!\n");
                    XBank = VARS[r]->Bank;
                    XAddress = VARS[r]->Address;
                    break;
                }
            }



            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    ////printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, YString,16) == 0)
                {
                    ////printf("Found RENDER's second VAR!\n");
                    YBank = VARS[r]->Bank;
                    YAddress = VARS[r]->Address;
                    break;
                }
            }

            if(!Handle_Aqueous_Basic_Text_RENDER(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,XBank,XAddress,YBank,YAddress))
            {
                ////printf("Build failed: Could not insert RENDER\n");
            }
            //////printf("I will SETVARVAR Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }

        if(strncmp(&Buffer[j], "PLAYINSTRUMENT",14) == 0 && Buffer[j+14] == ' ')
        {
            //printf("Found a Play Instrument.\n");
            size_t length = strcspn(&Buffer[j + 15], ";");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 15 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            unsigned char Bank;
            unsigned short Address;


            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find DELAY's parent value.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found Delay's var!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }


            if(!Handle_Aqueous_Basic_Text_PLAYINSTRUMENT(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
            {
                //printf("Build failed: Could not insert PLAYINSTRUMENT\n");
            }
        }


        if(strncmp(&Buffer[j], "STOPINSTRUMENT",14) == 0 && Buffer[j+14] == ' ')
        {
            //printf("Found a Stop Instrument.\n");
            size_t length = strcspn(&Buffer[j + 15], ";");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 15 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            unsigned char Bank;
            unsigned short Address;


            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find DELAY's parent value.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found Delay's var!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }


            if(!Handle_Aqueous_Basic_Text_STOPINSTRUMENT(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
            {
                //printf("Build failed: Could not insert PLAYINSTRUMENT\n");
            }
        }


         if(strncmp(&Buffer[j], "PLAYTRK",7) == 0 && Buffer[j+7] == ' ')
        {
            //printf("Found a PLAYTRK.\n");
            size_t length = strcspn(&Buffer[j + 8], ";");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 8 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            unsigned char Bank;
            unsigned short Address;
            unsigned short Spacing;
            unsigned short Length;


            //Find the label.
            for(int r = 0; r < Number_Of_TRACKS+1; r++)
            {
                if(r == Number_Of_TRACKS)
                {
                    //printf("Error: Could not find PLAYTRK's parent value.\n");
                    return false;
                }

                if(strncmp(TRACKS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found PLAYTRK's TRACK!\n");
                    Bank = TRACKS[r]->Bank;
                    Address = TRACKS[r]->Address;
                    Spacing = TRACKS[r]->Spacing;
                    Length = TRACKS[r]->Length;
                    break;
                }
            }


         if(!Handle_Aqueous_Basic_Text_PLAYTRACK(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address,Spacing,Length))
            {
                //printf("Build failed: Could not insert PLAYTRACK\n");
            }
            //printf("Inserted Track");
        }


        if(strncmp(&Buffer[j], "DELAY",5) == 0 && Buffer[j+5] == ' ')
        {
            //printf("Found a Delay.\n");
            size_t length = strcspn(&Buffer[j + 6], ";");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            unsigned char Bank;
            unsigned short Address;


            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find DELAY's parent value.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found Delay's var!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }


            if(!Handle_Aqueous_Basic_Text_DELAY(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
            {
                //printf("Build failed: Could not insert DELAY\n");
            }
        }



        if(strncmp(&Buffer[j], "LABEL",5) == 0 && Buffer[j+5] == ' ')
        {
            ////printf("Found a Label.\n");
            size_t length = strcspn(&Buffer[j + 6], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Allocate memory.
            LABELS[Number_Of_LABELS] = (struct _LABEL*)calloc(1,sizeof(struct _LABEL));

            for(int x = 0; x < 16; x++)
            {
                LABELS[Number_Of_LABELS]->Name[x] = Data_Block_Name[x];
                ////printf("%c\n",LABELS[Number_Of_LABELS]->Name[x]);
            }
            LABELS[Number_Of_LABELS]->Bank = New_Compiler->Current_ROM_Bank;
            LABELS[Number_Of_LABELS]->Address = New_Compiler->Current_ROM_Address;
            ////printf("%d %d\n", LABELS[Number_Of_LABELS]->Bank,LABELS[Number_Of_LABELS]->Address);

            Number_Of_LABELS += 1;
        }


        if(strncmp(&Buffer[j], "STARTLBL",8) == 0 && Buffer[j+8] == ';')
        {
            ////printf("Found a Start Label.\n");
            //Allocate memory.
            LABELS[Number_Of_LABELS] = (struct _LABEL*)calloc(1,sizeof(struct _LABEL));

            LABELS[Number_Of_LABELS]->Bank = New_Compiler->Current_ROM_Bank;
            LABELS[Number_Of_LABELS]->Address = New_Compiler->Current_ROM_Address;
            ////printf("%d %d\n", LABELS[Number_Of_LABELS]->Bank,LABELS[Number_Of_LABELS]->Address);

            Start_Address = LABELS[Number_Of_LABELS]->Address;

            Number_Of_LABELS += 1;
        }


        if(strncmp(&Buffer[j], "GOSUB",5) == 0 && Buffer[j+5] == ' ')
        {
            ////printf("Found a Gosub.\n");
            size_t length = strcspn(&Buffer[j + 6], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 6 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOSUB's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOSUB(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOSUB command.\n");
                        return false;
                    }
                    ////printf("I will GOSUB Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }




        if(strncmp(&Buffer[j], "GOIFCTRLA",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a Goto if CTRL DOWN pressed.\n");
            size_t length = strcspn(&Buffer[j + 10], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOIFCTRLDOWN's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLA(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOIFCTRLDOWN command.\n");
                        return false;
                    }
                    ////printf("I will GOIFCTRLDOWN Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }






        if(strncmp(&Buffer[j], "GOIFCTRLB",9) == 0 && Buffer[j+9] == ' ')
        {
            ////printf("Found a Goto if CTRL DOWN pressed.\n");
            size_t length = strcspn(&Buffer[j + 10], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 10 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOIFCTRLDOWN's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLB(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOIFCTRLDOWN command.\n");
                        return false;
                    }
                    ////printf("I will GOIFCTRLDOWN Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }



        if(strncmp(&Buffer[j], "GOIFCTRLDOWN",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Goto if CTRL DOWN pressed.\n");
            size_t length = strcspn(&Buffer[j + 13], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOIFCTRLDOWN's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLDOWN(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOIFCTRLDOWN command.\n");
                        return false;
                    }
                    ////printf("I will GOIFCTRLDOWN Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }

        if(strncmp(&Buffer[j], "GOIFCTRLRIGHT",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Goto if CTRL RIGHT pressed.\n");
            size_t length = strcspn(&Buffer[j + 14], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOIFCTRLUP's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLRIGHT(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOIFCTRLUP command.\n");
                        return false;
                    }
                    ////printf("I will GOIFCTRLUP Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }


        if(strncmp(&Buffer[j], "GOIFCTRLLEFT",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Goto if CTRL LEFT pressed.\n");
            size_t length = strcspn(&Buffer[j + 13], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOIFCTRLUP's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLLEFT(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOIFCTRLUP command.\n");
                        return false;
                    }
                    ////printf("I will GOIFCTRLUP Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }



        if(strncmp(&Buffer[j], "GOIFCTRLUP",10) == 0 && Buffer[j+10] == ' ')
        {
            ////printf("Found a Goto if CTRL UP pressed.\n");
            size_t length = strcspn(&Buffer[j + 11], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 11 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOIFCTRLUP's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOIFCTRLUP(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOIFCTRLUP command.\n");
                        return false;
                    }
                    ////printf("I will GOIFCTRLUP Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }

        if(strncmp(&Buffer[j], "GOTO",4) == 0 && Buffer[j+4] == ' ')
        {
            ////printf("Found a Goto.\n");
            size_t length = strcspn(&Buffer[j + 5], ";");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 5 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';
            ////printf("\n");

            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    ////printf("Error: Could not find GOTO's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    ////printf("Found its label!\n");
                    unsigned char Bank = LABELS[r]->Bank;
                    unsigned short Address = LABELS[r]->Address;
                    if(!Handle_Aqueous_Basic_Text_GOTO(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,Bank,Address))
                    {
                        ////printf("Error: Could not handle GOTO command.\n");
                        return false;
                    }
                    ////printf("I will GOTO Bank %d Address %d\n", Bank,Address);
                    break;
                }
            }
        }


        if(strncmp(&Buffer[j], "RETURN",6) == 0 && Buffer[j+6] == ';')
        {
            ////printf("Found a RETURN.\n");
            if(!Handle_Aqueous_Basic_Text_RETURN(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                ////printf("Error: Could not handle Return command.\n");
                return false;
            }
        }



        if(strncmp(&Buffer[j], "FLS",3) == 0 && Buffer[j+3] == ';')
        {
            ////printf("Found an FLS.\n");
            if(!Handle_Aqueous_Basic_Text_FLS(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                ////printf("Error: Could not handle FLS command.\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "CMODE",5) == 0 && Buffer[j+5] == ';')
        {
            ////printf("Found a COLOR MODE.\n");
            if(!Handle_Aqueous_Basic_Text_COLORMODE(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                ////printf("Error: Could not handle CMODE command.\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "MMODE",5) == 0 && Buffer[j+5] == ';')
        {
            ////printf("Found a MONOCHROME.\n");
            if(!Handle_Aqueous_Basic_Text_MONOCHROMEMODE(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                ////printf("Error: Could not handle MMODE command.\n");
                return false;
            }
        }


        if(strncmp(&Buffer[j], "CLS",3) == 0 && Buffer[j+3] == ';')
        {
            ////printf("Found a CLS.\n");
            if(!Handle_Aqueous_Basic_Text_CLS(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address))
            {
                ////printf("Error: Could not handle CLS command.\n");
                return false;
            }
        }


if(strncmp(&Buffer[j], "INSTRUMENTS",11) == 0 && Buffer[j+11] == ' ')
        {
            //printf("Found a instruments.\n");
            size_t length = strcspn(&Buffer[j + 12], " ");
            //printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 12 + x];
                //printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            ////printf("After this is %c\n",Buffer[j + 12 + length + 1]);
            if(Buffer[j + 12 + length + 1] != 'B')
            {
                ////printf("Build Failed. Data Block is not prefaced with 'B'.");
                return false;
            }

            size_t banklength = strcspn(&Buffer[j + 12 + length + 2], ";");
            ////printf("Its banklength is %zu\n", banklength); // Outputs 5
            if(banklength > 3)
            {
                ////printf("Build Failed. Data Blocks must be within three digits.");
                return false;
            }

            char Data_Bank[16];
            memset(Data_Bank,0,16);
            for(int x = 0; x < banklength; x++)
            {
                Data_Bank[x] = Buffer[j + 12 + x + length + 2];
                ////printf("%c\n", Data_Bank[x]);
            }
            Data_Bank[banklength] = '\0';

            ////printf("After this is %c\n",Buffer[j + 12 + length + banklength + 4]);
            if(Buffer[j + 12 + length + banklength + 4] != 'A')
            {
                ////printf("Build Failed. Data Address is not prefaced with 'A'.");
                return false;
            }

            size_t Addresslength = strcspn(&Buffer[j + 12 + length + 2 + banklength + 3], ";");
            ////printf("Its Addresslength is %zu\n", Addresslength); // Outputs 5
            if(Addresslength > 5)
            {
                ////printf("Build Failed. Address Blocks must be within five digits.");
                return false;
            }

            char Data_Address[16];
            memset(Data_Address,0,16);
            for(int x = 0; x < Addresslength; x++)
            {
                Data_Address[x] = Buffer[j + 12 + x + length + 2 + banklength + 3];
                ////printf("%c\n", Data_Address[x]);
            }
            Data_Address[Addresslength] = '\0';

            long Bank_Value = strtol(Data_Bank, NULL, 10);
            long Address_Value = strtol(Data_Address, NULL, 10);

            ////printf("This Var lives on %d %d.\n", Bank_Value, Address_Value);

            if(Bank_Value > 255)
            {
                ////printf("COMPLIATION FAILED: Bank value must not exceed 255.\n");
                return false;
            }

            if(Address_Value > 65535)
            {
                ////printf("COMPLIATION FAILED: Address value must not exceed 65535.\n");
                return false;
            }

            InstrumentBank = (unsigned char)Bank_Value;
            InstrumentAddress = (unsigned short)Address_Value;
        }


        if(strncmp(&Buffer[j], "GOIFVARGRVAL",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            long Value = strtol(YString, NULL, 10);

            //SetVarVal sets values at compile time! Fix that!


            unsigned char Bank, GotoBank;
            unsigned short Address, GotoAddress;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarGRVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_GOIFVARGRVAL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,Value))
            {
                //printf("Build failed: Could not insert GoifVarGRVAL\n");
            }
            //////printf("I will SETVALVAL Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }


        if(strncmp(&Buffer[j], "GOIFVARLSVAL",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            long Value = strtol(YString, NULL, 10);

            //SetVarVal sets values at compile time! Fix that!


            unsigned char Bank, GotoBank;
            unsigned short Address, GotoAddress;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarLSVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_GOIFVARLSVAL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,Value))
            {
                //printf("Build failed: Could not insert GoifVarLSVAL\n");
            }
            //////printf("I will SETVALVAL Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }


        if(strncmp(&Buffer[j], "GOIFVAREQVAL",12) == 0 && Buffer[j+12] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 13], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 13 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 13 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 13 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 13 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 13 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            long Value = strtol(YString, NULL, 10);

            //SetVarVal sets values at compile time! Fix that!


            unsigned char Bank, GotoBank;
            unsigned short Address, GotoAddress;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarEqVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_GOIFVAREQVAL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,Value))
            {
                //printf("Build failed: Could not insert GoifVarEqVAL\n");
            }
            //////printf("I will SETVALVAL Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }



if(strncmp(&Buffer[j], "GOIFVARGEQVAL",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 14], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 14 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 14 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 14 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 14 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            long Value = strtol(YString, NULL, 10);

            //SetVarVal sets values at compile time! Fix that!


            unsigned char Bank, GotoBank;
            unsigned short Address, GotoAddress;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarGEQVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_GOIFVARGEQVAL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,Value))
            {
                //printf("Build failed: Could not insert GoifVarGEQVAL\n");
            }
            //////printf("I will SETVALVAL Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }





        if(strncmp(&Buffer[j], "GOIFVARLEQVAL",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 14], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 14 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 14 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 14 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 14 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            long Value = strtol(YString, NULL, 10);

            //SetVarVal sets values at compile time! Fix that!


            unsigned char Bank, GotoBank;
            unsigned short Address, GotoAddress;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarLEQVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_GOIFVARLEQVAL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,Value))
            {
                //printf("Build failed: Could not insert GoifVarLEQVAL\n");
            }
            //////printf("I will SETVALVAL Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }



        if(strncmp(&Buffer[j], "GOIFVARNEQVAL",13) == 0 && Buffer[j+13] == ' ')
        {
            ////printf("Found a Render.\n");
            size_t length = strcspn(&Buffer[j + 14], " ");
            ////printf("Its length is %zu\n", length); // Outputs 5

            char Data_Block_Name[16];
            memset(Data_Block_Name,0,16);
            for(int x = 0; x < length; x++)
            {
                Data_Block_Name[x] = Buffer[j + 14 + x];
                ////printf("%c\n", Data_Block_Name[x]);
            }
            Data_Block_Name[length] = '\0';

            size_t xvar = strcspn(&Buffer[j + 14 + length + 1], " ");
            ////printf("Its xvar is %zu\n", xvar); // Outputs 5

            char XString[16];
            memset(XString,0,16);
            for(int x = 0; x < xvar; x++)
            {
                XString[x] = Buffer[j + 14 + x + length + 1];
                ////printf("%c\n", XString[x]);
            }
            XString[xvar] = '\0';





            size_t yvar = strcspn(&Buffer[j + 14 + length + 1 + xvar + 1], ";");
            ////printf("Its yvar is %zu\n", yvar); // Outputs 5

            char YString[16];
            memset(YString,0,16);
            for(int x = 0; x < yvar; x++)
            {
                YString[x] = Buffer[j + 14 + x + length + 1 + xvar + 1];
                ////printf("%c\n", YString[x]);
            }
            YString[yvar] = '\0';

            long Value = strtol(YString, NULL, 10);

            //SetVarVal sets values at compile time! Fix that!


            unsigned char Bank, GotoBank;
            unsigned short Address, GotoAddress;


            //Find the label.
            for(int r = 0; r < Number_Of_LABELS+1; r++)
            {
                if(r == Number_Of_LABELS)
                {
                    //printf("Error: Could not find GoIfVarNEQVar's parent label.\n");
                    return false;
                }

                if(strncmp(LABELS[r]->Name, Data_Block_Name,16) == 0)
                {
                    //printf("Found SETLABELLABEL's first LABEL!\n");
                    GotoBank = LABELS[r]->Bank;
                    GotoAddress = LABELS[r]->Address;
                    break;
                }
            }

            //Find the label.
            for(int r = 0; r < Number_Of_VARS+1; r++)
            {
                if(r == Number_Of_VARS)
                {
                    //printf("Error: Could not find RENDER's second parent VAR.\n");
                    return false;
                }

                if(strncmp(VARS[r]->Name, XString,16) == 0)
                {
                    //printf("Found RENDER's second VAR!\n");
                    Bank = VARS[r]->Bank;
                    Address = VARS[r]->Address;
                    break;
                }
            }



            if(!Handle_Aqueous_Basic_Text_GOIFVARNEQVAL(New_Compiler,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,GotoBank, GotoAddress,Bank,Address,Value))
            {
                //printf("Build failed: Could not insert GoifVarNEQVAL\n");
            }
            //////printf("I will SETVALVAL Bank %d Address %d to the one in Bank %d Address %d \n", ParentOneBank,ParentOneAddress,ParentTwoBank,ParentTwoAddress);
        }
    }

    New_Compiler->Current_ROM_Bank+=1;
    New_Compiler->Current_ROM_Address = 0;
    Write_File_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address, "test.dat");
    Write_File_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address, "test2.dat");

    New_Compiler->Current_ROM_Bank+=1;
    New_Compiler->Current_ROM_Address=0;
    Write_File_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address, "eng_text_tilemap.dat");

    New_Compiler->Current_ROM_Bank+=1;
    New_Compiler->Current_ROM_Address=0;
    Write_File_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address, "test.aqbenc");

    New_Compiler->Current_ROM_Bank+=1;
    New_Compiler->Current_ROM_Address=0;
    for(int w = 0; w < 65536; w++)
    {
        New_Compiler->ROM_Memory_Zero[New_Compiler->Current_ROM_Bank].Bank[New_Compiler->Current_ROM_Address+w] = rand() % 256;
    }
    //Write Track

    New_Compiler->Current_ROM_Bank+=1;
    New_Compiler->Current_ROM_Address=0;
    //Write instruments
    //Write_Instrument_Onto_ROM(New_Compiler, New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
    for(int w = 0; w < 65536; w++)
    {
        New_Compiler->ROM_Memory_Zero[New_Compiler->Current_ROM_Bank].Bank[New_Compiler->Current_ROM_Address+w] = rand() % 256;
    }

    unsigned char Number_Of_Banks = New_Compiler->Current_ROM_Bank+1;
    //printf("The Number of Banks is %d\n", Number_Of_Banks);

    FILE* ftwo = fopen(Output, "wb");
    fwrite(&Number_Of_Banks,1,sizeof(unsigned char),ftwo);
    fwrite(&Start_Address,1,sizeof(unsigned short),ftwo);
    fwrite(&InstrumentBank,1,sizeof(unsigned char),ftwo);
    fwrite(&InstrumentAddress,1,sizeof(unsigned short),ftwo);
    for(int j = 0; j < New_Compiler->Current_ROM_Bank+1; j++)
    {
        fwrite(&New_Compiler->ROM_Memory_Zero[j], 1, sizeof(struct _RAM_Bank),ftwo);
    }
    fclose(ftwo);

    ////printf("Compile complete.\n");
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

    ////printf("I will call %d %d\n", Bank, Address);

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

    ////printf("%d is going into r0.\n",New_Value);

    return true;
}




bool Handle_Aqueous_Basic_Text_MULVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value)
{
    uint8_t low_byte = New_Value & 0xFF;
    uint8_t high_byte = (New_Value >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_MUL_R1_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y

    ////printf("%d is going into r0.\n",New_Value);

    return true;
}




bool Handle_Aqueous_Basic_Text_DIVVVL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned short New_Value)
{
    uint8_t low_byte = New_Value & 0xFF;
    uint8_t high_byte = (New_Value >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,high_byte,low_byte,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_DIV_R1_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y

    ////printf("%d is going into r0.\n",New_Value);

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

    ////printf("%d is going into r0.\n",New_Value);

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

    ////printf("%d is going into r0.\n",New_Value);

    return true;
}

bool Handle_Aqueous_Basic_Text_SETVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t addronelow_byte = Address & 0xFF;
    uint8_t addronehigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrtwolow_byte = AddressTwo & 0xFF;
    uint8_t addrtwohigh_byte = (AddressTwo >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrtwohigh_byte,addrtwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addronehigh_byte,addronelow_byte); //y

    return true;
}


bool Handle_Aqueous_Basic_Text_ADDVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t addronelow_byte = Address & 0xFF;
    uint8_t addronehigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrtwolow_byte = AddressTwo & 0xFF;
    uint8_t addrtwohigh_byte = (AddressTwo >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrtwohigh_byte,addrtwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_ADD_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addronehigh_byte,addronelow_byte); //y

    return true;
}



bool Handle_Aqueous_Basic_Text_SUBVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t addronelow_byte = Address & 0xFF;
    uint8_t addronehigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrtwolow_byte = AddressTwo & 0xFF;
    uint8_t addrtwohigh_byte = (AddressTwo >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrtwohigh_byte,addrtwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_SUB_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addronehigh_byte,addronelow_byte); //y

    return true;
}


bool Handle_Aqueous_Basic_Text_MULVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t addronelow_byte = Address & 0xFF;
    uint8_t addronehigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrtwolow_byte = AddressTwo & 0xFF;
    uint8_t addrtwohigh_byte = (AddressTwo >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrtwohigh_byte,addrtwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MUL_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_MOV_VALUE_AT_ADDRESS_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addronehigh_byte,addronelow_byte); //y

    return true;
}


bool Handle_Aqueous_Basic_Text_DIVVVR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t addronelow_byte = Address & 0xFF;
    uint8_t addronehigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrtwolow_byte = AddressTwo & 0xFF;
    uint8_t addrtwohigh_byte = (AddressTwo >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrtwohigh_byte,addrtwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_DIV_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //y
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
        ////printf("Wrote FLS Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_FLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            ////printf("ROM Cartridge out of memory!\n");
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
        ////printf("Wrote CLS Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_CLS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            ////printf("ROM Cartridge out of memory!\n");
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
        ////printf("Wrote COLORMODE Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_COLORMODE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            ////printf("ROM Cartridge out of memory!\n");
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
        ////printf("Wrote MONOCHROMEMODE Command to Bank %d Address %d\n", New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address);
        Write_Opcode(New_Compiler,OPC_MONOCHROMEMODE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0);
        return true;
    }
    else if(Current_ROM_Address >= 65535)
    {
        if(New_Compiler->Current_ROM_Bank + 1 > 255)
        {
            ////printf("ROM Cartridge out of memory!\n");
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
    Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank = 0;

    Seat->New_Computer->Processor.Audio_Processor.Track_Bank = 0;
    Seat->New_Computer->Processor.Audio_Processor.Track_Address = 0;
    Seat->New_Computer->Processor.Audio_Processor.Track_Start_Address = 0;
    Seat->New_Computer->Processor.Audio_Processor.Track_End_Address = 0;
    Seat->New_Computer->Processor.Audio_Processor.Current_Step = 0;

    Seat->New_Computer->Processor.Audio_Processor.Last_Time_Step_Triggered = 0;
    Seat->New_Computer->Processor.Audio_Processor.Step_Spacing = 0;
    Seat->New_Computer->Processor.Audio_Processor.Tracking_On = false;

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

    unsigned char Instruments_Bank = Buffer[3];
    unsigned short Instruments_Address=(Buffer[4] << 8) | (Buffer[5] & 0xff);

    //printf("Instruments located on Bank %d Address %d\n", Instruments_Bank, Instruments_Address);

    Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank = Instruments_Bank;

    ////printf("Number of Banks: %d\n", Number_Of_Banks);
    ////printf("Start Address: %d\n", Start_Address);

    Buffer_Location += 6;

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


    int Current_Instrument = 0;
    int i = 0;

    while(Current_Instrument < 256)
    {
        Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[Current_Instrument].Instwaveform = Seat->New_Computer->RAM[Instruments_Bank].Bank[i];
        Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[Current_Instrument].Instattack = Seat->New_Computer->RAM[Instruments_Bank].Bank[i+1];
        Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[Current_Instrument].Instdecay = Seat->New_Computer->RAM[Instruments_Bank].Bank[i+2];
        Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[Current_Instrument].Instsustain = Seat->New_Computer->RAM[Instruments_Bank].Bank[i+3];
        Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[Current_Instrument].Instrelease = Seat->New_Computer->RAM[Instruments_Bank].Bank[i+4];
        Seat->New_Computer->Processor.Audio_Processor.Track_Instruments[Current_Instrument].Instpulse_width = (Seat->New_Computer->RAM[Instruments_Bank].Bank[i+6] << 8) | (Seat->New_Computer->RAM[Instruments_Bank].Bank[i+5] & 0xff);
        i += 7;
        Current_Instrument += 1;
    }


    ////printf("Starting Execution at %d\n", Start_Address);
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

    ////printf("The Source is %d\n", Source_Bank);
    ////printf("The Destination is %d\n", Destination_Bank);

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

    ////printf("The Bank is %d\n", Bank);
    ////printf("The Address is %d\n", Address);
    ////printf("The x is %d\n", x);
    ////printf("The y is %d\n", y);

    unsigned char Width;
    unsigned char Height;
    unsigned char Color_Mode;

    Width = Seat->New_Computer->RAM[Bank].Bank[Address];
    Height = Seat->New_Computer->RAM[Bank].Bank[Address+1];
    Color_Mode = Seat->New_Computer->RAM[Bank].Bank[Address+2];

    ////printf("The Width is %d\n", Width);
    ////printf("The Height is %d\n", Height);
    ////printf("The Color Mode is %d\n", Color_Mode);
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

    ////printf("Register Zero now equals %d\n", Seat->New_Computer->Processor.Register_Zero);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 1 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 1;
    }
};

void Process_Opcode_IN_DPAD_DOWN_R0(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Register_Zero = Seat->New_Computer->Controller_D_Pad_RAM[1];

    ////printf("Register Zero now equals %d\n", Seat->New_Computer->Processor.Register_Zero);

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


bool Handle_Aqueous_Basic_Text_GOIFVARNEQVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrTwolow_byte = AddressTwo & 0xFF;
    uint8_t addrTwohigh_byte = (AddressTwo >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrTwohigh_byte,addrTwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CNE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};



bool Handle_Aqueous_Basic_Text_GOIFVARLEQVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrTwolow_byte = AddressTwo & 0xFF;
    uint8_t addrTwohigh_byte = (AddressTwo >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrTwohigh_byte,addrTwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CLE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};



bool Handle_Aqueous_Basic_Text_GOIFVARGEQVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrTwolow_byte = AddressTwo & 0xFF;
    uint8_t addrTwohigh_byte = (AddressTwo >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrTwohigh_byte,addrTwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CGE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};


bool Handle_Aqueous_Basic_Text_GOIFVARLSVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrTwolow_byte = AddressTwo & 0xFF;
    uint8_t addrTwohigh_byte = (AddressTwo >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrTwohigh_byte,addrTwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CL,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};



bool Handle_Aqueous_Basic_Text_GOIFVARGRVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrTwolow_byte = AddressTwo & 0xFF;
    uint8_t addrTwohigh_byte = (AddressTwo >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrTwohigh_byte,addrTwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};




bool Handle_Aqueous_Basic_Text_GOIFVAREQVAR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned char BankTwo,unsigned short AddressTwo)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t addrTwolow_byte = AddressTwo & 0xFF;
    uint8_t addrTwohigh_byte = (AddressTwo >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,BankTwo,addrTwohigh_byte,addrTwolow_byte); //y
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
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






void Process_INT_Render_Cursor(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char Bank;
    unsigned short Address;
    unsigned char TileWidth;
    unsigned char TileHeight;
    unsigned char x,y;
    Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer]; //MOV int value
    Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] & 0xff); //MOV int value
    x = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+4];  //MOV value at address
    y = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+6];  //MOV value at address
    TileWidth = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+8]; //MOV int value
    TileHeight = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+10]; //MOV int value

    x*=TileWidth;
    y*=TileHeight;

    unsigned char Width;
    unsigned char Height;
    unsigned char Color_Mode;
    Width = Seat->New_Computer->RAM[Bank].Bank[Address];
    Height = Seat->New_Computer->RAM[Bank].Bank[Address+1];
    Color_Mode = Seat->New_Computer->RAM[Bank].Bank[Address+2];

    //printf("The Bank is %d\n", Bank);
    //printf("The Adress is %d\n", Address);
    //printf("The x is %d\n", x);
    //printf("The y is %d\n", y);

    //printf("The Width is %d\n", Width);
    //printf("The Height is %d\n", Height);

    //printf("The TileWidth is %d\n", TileWidth);
    //printf("The TileHeight is %d\n", TileHeight);


    unsigned char Array[Width*Height];
    for(int j = 0; j < Width*Height; j++)
    {
        if(Address + 3 + j < 65536)
        {
            Array[j] = Seat->New_Computer->RAM[Bank].Bank[Address+3+j];
        }
    }

    unsigned char Tilemap_Canvas[TileWidth*TileHeight];
    memset(Tilemap_Canvas,0,TileWidth*TileHeight);

    Copy_Section(1,Array,Tilemap_Canvas,Width,Height,TileWidth,TileHeight,20*TileWidth,2*TileHeight,0,0,TileWidth,TileHeight);

    if(Seat->New_Computer->Cursor_Visible)
    {
        Copy_Section(1,Tilemap_Canvas,Seat->New_Computer->Back_Video_Buffer,TileWidth,TileHeight,256,256,0,0,x,y,TileWidth,TileHeight);
    }


    if(Seat->New_Computer->Processor.Stack_Pointer + 12 < 65536)
    {
        Seat->New_Computer->Processor.Stack_Pointer += 12;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};


bool Handle_Aqueous_Basic_Text_DELAY(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char XBank,unsigned short XAddress)
{
    uint8_t timelow_byte = XAddress & 0xFF;
    uint8_t timehigh_byte = (XAddress >> 8) & 0xFF;

    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,XBank,timehigh_byte,timelow_byte); //y
    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_SET_TIMER,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    //INT SET TIMER MUST POP R0!

    return true;
};

void Process_INT_Set_Timer(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }

    Seat->New_Computer->Timer_Set = true;
    Seat->New_Computer->Timer_Triggered = false;

    double Delay_Timer = Seat->New_Computer->Processor.Register_Zero/1000; //No need to delay 18 hours!

    Seat->New_Computer->Timer = Delay_Timer + Get_Ticks(Seat->New_Computer->Universal_Timer);
    //printf("Timer set to %d\n", Seat->New_Computer->Timer);
}

void Process_INT_Render_Tilemap(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    unsigned char Bank;
    unsigned short Address;
    unsigned char EncodingBank;
    unsigned short EncodingAddress;
    unsigned char EncodingLengthBank;
    unsigned short EncodingLengthAddress;
    unsigned char TileWidth;
    unsigned char TileHeight;
    unsigned char x,y;
    unsigned short EncodingLength;
    unsigned char Tiles_Per_Row;
    Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer]; //MOV int value
    Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] & 0xff); //MOV int value
    x = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+4];  //MOV value at address
    y = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+6];  //MOV value at address
    EncodingBank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+8]; //MOV int value
    EncodingAddress = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+11] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+10] & 0xff); //MOV int value
    EncodingLength = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+13] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+12] & 0xff); //MOV int value
    Tiles_Per_Row = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+14]; //MOV int value
    TileWidth = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+16]; //MOV int value
    TileHeight = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+18]; //MOV int value

    //EncodingLength += 1; //If Enc is 13, then it goes from 0 to 12.

    unsigned char Width;
    unsigned char Height;
    unsigned char Color_Mode;

    Width = Seat->New_Computer->RAM[Bank].Bank[Address];
    Height = Seat->New_Computer->RAM[Bank].Bank[Address+1];
    Color_Mode = Seat->New_Computer->RAM[Bank].Bank[Address+2];

    unsigned char Array[Width*Height];
    for(int j = 0; j < Width*Height; j++)
    {
        if(Address + 3 + j < 65536)
        {
            Array[j] = Seat->New_Computer->RAM[Bank].Bank[Address+3+j];
        }
    }

    unsigned char Tilemap_Encodings[EncodingLength * 4];
    for(int j = 0; j < EncodingLength * 4; j++)
    {
        Tilemap_Encodings[j] = Seat->New_Computer->RAM[EncodingBank].Bank[EncodingAddress+j];
    }
    //printf("The encoding length is %d\n", EncodingLength);

    int Tilemap_Canvas_Width;

    if(EncodingLength <= Tiles_Per_Row)
    {
        Tilemap_Canvas_Width = EncodingLength * TileWidth;
        //printf("EncLength is less than or equal to TPR.\n");
    }

    else if(EncodingLength > Tiles_Per_Row)
    {
        Tilemap_Canvas_Width = Tiles_Per_Row * TileWidth;
        //printf("EncLength is greater than TPR.\n");
    }

    //printf("The Tilemap Canvas Width is %d\n", Tilemap_Canvas_Width);
    //This is correct.

    //Now, find all of the newlines.
    int i = 0;
    int Current_Tile = 0;
    int Tilemap_Canvas_X = 0;
    int Tilemap_Canvas_Y = 0;
    int Number_Of_Columns = 1;

    while(Current_Tile < EncodingLength)
    {
        unsigned char CodepageH = Tilemap_Encodings[i];
        unsigned char CodepageL = Tilemap_Encodings[i+1];
        unsigned char xC = Tilemap_Encodings[i+2];
        unsigned char yC = Tilemap_Encodings[i+3];

        if(xC == 15 && yC == 3)
        {
            Number_Of_Columns += 1;
            //printf("Found newline\n");
        }

        i += 4;
        Current_Tile += 1;
    }

    //printf("The number of columns is %d\n", Number_Of_Columns);

    int Tilemap_Canvas_Height = Number_Of_Columns * TileHeight;

    if(Tilemap_Canvas_Width < 256 && Tilemap_Canvas_Height < 256) //Stop memory errors before they appear!
    {
        if(x < 256 && y < 256)
        {

    unsigned char Tilemap_Canvas[Tilemap_Canvas_Width * Tilemap_Canvas_Height];
    memset(Tilemap_Canvas,0,Tilemap_Canvas_Width * Tilemap_Canvas_Height);


    //printf("The tilemap Canvas is %d by %d\n", Tilemap_Canvas_Width, Tilemap_Canvas_Height);





    int CodepageH = 0;
    int CodepageL = 0;
    int TilemapX = 0;
    int TilemapY = 0;

    Current_Tile = 0;
    i = 0;

    int Current_Tilemap_Canvas_x= 0;
    int Current_Tilemap_Canvas_y= 0;
    int Times_Tiles_Multiplied = 1;

    while(Current_Tile < EncodingLength)
    {
        //printf("The current tile is %d\n", Current_Tile);
        CodepageH = Tilemap_Encodings[i];
        CodepageL = Tilemap_Encodings[i+1];
        TilemapX = Tilemap_Encodings[i+2];
        TilemapY = Tilemap_Encodings[i+3];

        if(TilemapX != 16 || TilemapY != 3)
        {
            Copy_Section(1,Array,Tilemap_Canvas,Width,Height,Tilemap_Canvas_Width,Tilemap_Canvas_Height,TilemapX*TileWidth,TilemapY*TileHeight,Current_Tilemap_Canvas_x,Current_Tilemap_Canvas_y,TileWidth,TileHeight);
            Current_Tilemap_Canvas_x += TileWidth;
        }

        if(TilemapX == 15)
        {
            if(TilemapY == 3)
            {
                Current_Tilemap_Canvas_y += TileHeight;
                Current_Tilemap_Canvas_x = 0;
                //printf("Encountered newline\n");
            }
        }

        if(TilemapX == 16)
        {
            if(TilemapY == 3)
            {
                Current_Tilemap_Canvas_x += TileWidth;
                //printf("Encountered space\n");
            }
        }



        Current_Tile += 1;
        i+=4;

       // if(Current_Tile >= (Tiles_Per_Row - 1) * Times_Tiles_Multiplied)
      //  {
       //     Times_Tiles_Multiplied += 1;
       //     Current_Tilemap_Canvas_y += TileHeight;
       //     Current_Tilemap_Canvas_x = 0;
       //     //printf("Encountered newline\n");
       // }
    }

    int xCutoff = 0; //This is if the tilemap's coords exceed 256.

    xCutoff = x + Tilemap_Canvas_Width - 256;
    if(xCutoff <= 0)
    {
        //printf("Nothing to cut off. Tilemap within x bounds.\n");
        xCutoff = 0;
    }

    if(xCutoff > 0)
    {
        //printf("Tilemap exceeds x bounds by %d\n", xCutoff);
    }


    int yCutoff = 0; //This is if the tilemap's coords eyceed 256.

    yCutoff = y + Tilemap_Canvas_Height - 256;
    if(yCutoff <= 0)
    {
        //printf("Nothing to cut off. Tilemap within y bounds.\n");
        yCutoff = 0;
    }

    if(yCutoff > 0)
    {
        //printf("Tilemap exceeds y bounds by %d\n", yCutoff);
    }

    int TileCompensation = 0;
    if(EncodingLength <= Tiles_Per_Row)
    {
        TileCompensation = TileWidth;
    }


    if(EncodingLength > Tiles_Per_Row)
    {
    Copy_Section(1,Tilemap_Canvas,Seat->New_Computer->Back_Video_Buffer,Tilemap_Canvas_Width,Tilemap_Canvas_Height,256,256,0,0,x,y,Tilemap_Canvas_Width-TileCompensation-xCutoff,Tilemap_Canvas_Height-yCutoff);
    }

    else if(EncodingLength <= Tiles_Per_Row)
    {
    Copy_Section(1,Tilemap_Canvas,Seat->New_Computer->Back_Video_Buffer,Tilemap_Canvas_Width,Tilemap_Canvas_Height,256,256,0,0,x,y,Tilemap_Canvas_Width-xCutoff,Tilemap_Canvas_Height-yCutoff);
    }

    }
    }

    if(Seat->New_Computer->Processor.Stack_Pointer + 20 < 65536)
    {
        Seat->New_Computer->Processor.Stack_Pointer += 20;
    }

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};





bool Handle_Aqueous_Basic_Text_RENCURSOR(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank, unsigned short Address,unsigned char XBank,unsigned short XAddress,unsigned char YBank,unsigned short YAddress,unsigned char TileWidth,unsigned char TileHeight)
{

     //printf("\nThe Tilemap's Bank is %d and its Address is %d\n", Bank, Address);
    //printf("Its width is %d and its height is %d\n", TileWidth, TileHeight);

    //printf("The Tilemap's X Coordinate Bank is %d and its Addr is %d\n", XBank,XAddress);
    //printf("The Tilemap's Y Coordinate Bank is %d and its Addr is %d\n", YBank,YAddress);

    uint8_t tileaddrlow_byte = Address & 0xFF;
    uint8_t tileaddrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t xaddrlow_byte = XAddress & 0xFF;
    uint8_t xaddrhigh_byte = (XAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = YAddress & 0xFF;
    uint8_t yaddrhigh_byte = (YAddress >> 8) & 0xFF;


    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,TileHeight,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,TileWidth,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,YBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,XBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,tileaddrhigh_byte,tileaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,Bank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_CURSOR,0,0);
    return true;
};







bool Handle_Aqueous_Basic_Text_RENTILEMAP(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char Bank, unsigned short Address,unsigned char XBank,unsigned short XAddress,unsigned char YBank,unsigned short YAddress,unsigned char EncodingBank,unsigned short EncodingAddress,unsigned char EncodingLengthBank,unsigned short EncodingLengthAddress,unsigned char TPRBank,unsigned short TPRAddress,unsigned char TileWidth,unsigned char TileHeight)
{

     //printf("\nThe Tilemap's Bank is %d and its Address is %d\n", Bank, Address);
            //printf("Its width is %d and its height is %d\n", TileWidth, TileHeight);

            //printf("The Tilemap's X Coordinate Bank is %d and its Addr is %d\n", XBank,XAddress);
            //printf("The Tilemap's Y Coordinate Bank is %d and its Addr is %d\n", YBank,YAddress);

            //printf("The Encoding is on Bank %d Addr %d\n", EncodingBank, EncodingAddress);

            //printf("The Encoding Length is on Bank %d Addr %d\n", EncodingLengthBank, EncodingLengthAddress);

            //printf("The Tiles Per Row is on Bank %d Addr %d\n", TPRBank, TPRAddress);


    uint8_t tileaddrlow_byte = Address & 0xFF;
    uint8_t tileaddrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t xaddrlow_byte = XAddress & 0xFF;
    uint8_t xaddrhigh_byte = (XAddress >> 8) & 0xFF;

    uint8_t yaddrlow_byte = YAddress & 0xFF;
    uint8_t yaddrhigh_byte = (YAddress >> 8) & 0xFF;

    uint8_t encaddrlow_byte = EncodingAddress & 0xFF;
    uint8_t encaddrhigh_byte = (EncodingAddress >> 8) & 0xFF;

    uint8_t enclenaddrlow_byte = EncodingLengthAddress & 0xFF;
    uint8_t enclenaddrhigh_byte = (EncodingLengthAddress >> 8) & 0xFF;

    uint8_t tpraddrlow_byte = TPRAddress & 0xFF;
    uint8_t tpraddrhigh_byte = (TPRAddress >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,TileHeight,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,TileWidth,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);



    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,TPRBank,tpraddrhigh_byte,tpraddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);



    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,EncodingLengthBank,enclenaddrhigh_byte,enclenaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);



    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,encaddrhigh_byte,encaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,EncodingBank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);


    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,YBank,yaddrhigh_byte,yaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,XBank,xaddrhigh_byte,xaddrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,tileaddrhigh_byte,tileaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,Bank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_RENDER_TILEMAP,0,0);
    return true;
};

void Encode_Text_As_File(char Text[256], unsigned short Encoding_Length, const char* Output_File)
{
    char Output[65536];
    memset(Output,0,65536);
    int Current_Output_Pos = 0;

    int Current_x = 0;
    int Current_y = 0;

    //The font is 8 by 16.

    //Codepage, X, Y

    for(int j = 0; j < Encoding_Length; j++)
    {
        switch(Text[j])
        {
            case '`': //Newline
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 15;
                Output[Current_Output_Pos+3] = 3;
            break;

            case ' ':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 16;
                Output[Current_Output_Pos+3] = 3;
            break;


            case 'A':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 0;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'B':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 1;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'C':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 2;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'D':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 3;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'E':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 4;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'F':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 5;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'G':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 6;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'H':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 7;
                Output[Current_Output_Pos+3] = 0;
            break;










            case 'I':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 8;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'J':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 9;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'K':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 10;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'L':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 11;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'M':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 12;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'N':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 13;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'O':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 14;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'P':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 15;
                Output[Current_Output_Pos+3] = 0;
            break;











            case 'Q':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 16;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'R':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 17;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'S':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 18;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'T':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 19;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'U':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 20;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'V':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 21;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'W':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 22;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'X':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 23;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'Y':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 24;
                Output[Current_Output_Pos+3] = 0;
            break;

            case 'Z':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 25;
                Output[Current_Output_Pos+3] = 0;
            break;















            case 'a':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 0;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'b':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 1;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'c':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 2;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'd':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 3;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'e':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 4;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'f':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 5;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'g':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 6;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'h':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 7;
                Output[Current_Output_Pos+3] = 1;
            break;










            case 'i':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 8;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'j':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 9;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'k':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 10;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'l':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 11;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'm':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 12;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'n':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 13;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'o':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 14;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'p':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 15;
                Output[Current_Output_Pos+3] = 1;
            break;











            case 'q':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 16;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'r':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 17;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 's':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 18;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 't':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 19;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'u':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 20;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'v':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 21;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'w':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 22;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'x':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 23;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'y':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 24;
                Output[Current_Output_Pos+3] = 1;
            break;

            case 'z':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 25;
                Output[Current_Output_Pos+3] = 1;
            break;
















            case '1':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 0;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '2':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 1;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '3':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 2;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '4':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 3;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '5':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 4;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '6':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 5;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '7':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 6;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '8':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 7;
                Output[Current_Output_Pos+3] = 2;
            break;










            case '9':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 8;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '0':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 9;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '!':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 10;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '@':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 11;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '#':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 12;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '$':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 13;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '%':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 14;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '^':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 15;
                Output[Current_Output_Pos+3] = 2;
            break;











            case '&':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 16;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '*':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 17;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '(':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 18;
                Output[Current_Output_Pos+3] = 2;
            break;

            case ')':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 19;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '_':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 20;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '-':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 21;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '+':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 22;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '=':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 23;
                Output[Current_Output_Pos+3] = 2;
            break;

            case '|':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 24;
                Output[Current_Output_Pos+3] = 2;
            break;










            case '{':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 0;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '[':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 1;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '}':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 2;
                Output[Current_Output_Pos+3] = 3;
            break;

            case ']':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 3;
                Output[Current_Output_Pos+3] = 3;
            break;

            case ':':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 4;
                Output[Current_Output_Pos+3] = 3;
            break;

            case ';':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 5;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '"':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 6;
                Output[Current_Output_Pos+3] = 3;
            break;











            case '<':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 8;
                Output[Current_Output_Pos+3] = 3;
            break;

            case ',':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 9;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '>':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 10;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '.':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 11;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '?':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 12;
                Output[Current_Output_Pos+3] = 3;
            break;

            case '/':
                Output[Current_Output_Pos] = 0;
                Output[Current_Output_Pos+1] = 0;
                Output[Current_Output_Pos+2] = 13;
                Output[Current_Output_Pos+3] = 3;
            break;

        }
        Current_Output_Pos += 4;
    }


    FILE* f = fopen(Output_File, "wb");
    fwrite(&Output,1,Encoding_Length*4,f);
    fclose(f);

}




bool Handle_Aqueous_Basic_Text_GOIFVARLEQVAL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned short Value)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t vallow_byte = Value & 0xFF;
    uint8_t valhigh_byte = (Value >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,valhigh_byte,vallow_byte,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CLE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};


bool Handle_Aqueous_Basic_Text_GOIFVARGEQVAL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned short Value)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t vallow_byte = Value & 0xFF;
    uint8_t valhigh_byte = (Value >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,valhigh_byte,vallow_byte,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CGE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};

bool Handle_Aqueous_Basic_Text_GOIFVARNEQVAL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned short Value)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t vallow_byte = Value & 0xFF;
    uint8_t valhigh_byte = (Value >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,valhigh_byte,vallow_byte,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CNE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};

bool Handle_Aqueous_Basic_Text_GOIFVAREQVAL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned short Value)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t vallow_byte = Value & 0xFF;
    uint8_t valhigh_byte = (Value >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,valhigh_byte,vallow_byte,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};



bool Handle_Aqueous_Basic_Text_GOIFVARGRVAL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned short Value)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t vallow_byte = Value & 0xFF;
    uint8_t valhigh_byte = (Value >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,valhigh_byte,vallow_byte,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CG,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};


bool Handle_Aqueous_Basic_Text_GOIFVARLSVAL(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address,unsigned char GotoBank, unsigned short Goto_Address, unsigned char Bank,unsigned short Address,unsigned short Value)
{
    uint8_t Gotoaddrlow_byte = Goto_Address & 0xFF;
    uint8_t Gotoaddrhigh_byte = (Goto_Address >> 8) & 0xFF;

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t vallow_byte = Value & 0xFF;
    uint8_t valhigh_byte = (Value >> 8) & 0xFF;

    //y x ADDR Bank
    Write_Opcode(New_Compiler,OPC_MOV_R1_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,valhigh_byte,vallow_byte,0);
    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_CMP_R0_R1,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,1,0,0,0); //y
    Write_Opcode(New_Compiler,OPC_CL,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,GotoBank,Gotoaddrhigh_byte,Gotoaddrlow_byte); //y
    return true;
};

bool Handle_Aqueous_Basic_Text_PLAYINSTRUMENT(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    //Play the instrument whose id is at Bank, Addr

    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //Preserve old value

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_PLAY_INSTRUMENT,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    return true;
};

bool Handle_Aqueous_Basic_Text_PLAYTRACK(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char Bank,unsigned short Address, unsigned short Spacing, unsigned short Length)
{

    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    uint8_t spaceaddrlow_byte = Spacing & 0xFF;
    uint8_t spaceaddrhigh_byte = (Spacing >> 8) & 0xFF;

    uint8_t lenaddrlow_byte = Length & 0xFF;
    uint8_t lenaddrhigh_byte = (Length >> 8) & 0xFF;

    //y x ADDR Bank

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,lenaddrhigh_byte,lenaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,spaceaddrhigh_byte,spaceaddrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,addrhigh_byte,addrlow_byte,0);
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_MOV_R0_INT_VALUE,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,3,0,Bank,0); //bank
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_PLAY_TRACK,0,0);

    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    return true;
};


bool Handle_Aqueous_Basic_Text_STOPINSTRUMENT(struct _New_Compiler* New_Compiler,unsigned char Current_ROM_Bank,unsigned short Current_ROM_Address, unsigned char Bank,unsigned short Address)
{
    uint8_t addrlow_byte = Address & 0xFF;
    uint8_t addrhigh_byte = (Address >> 8) & 0xFF;

    //Play the instrument whose id is at Bank, Addr

    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0); //Preserve old value

    Write_Opcode(New_Compiler,OPC_MOV_R0_VALUE_AT_ADDRESS,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,4,Bank,addrhigh_byte,addrlow_byte); //y
    Write_Opcode(New_Compiler,OPC_PUSH_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_INT,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,INT_STOP_INSTRUMENT,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);
    Write_Opcode(New_Compiler,OPC_POP_R0,New_Compiler->Current_ROM_Bank,New_Compiler->Current_ROM_Address,2,0,0,0);

    return true;
};



void Process_INT_Play_Instrument(struct _Engine* Engine, struct _Seat_Object* Seat, struct _Audio_Chip* Audio_Chip)
{
    unsigned short Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+1] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+0] & 0xff);

    //printf("Playing Sound %d\n", Address);

    int Offset = Address * 6;

    uint8_t Instwaveform = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank].Bank[Offset];
    uint8_t Instattack = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank].Bank[Offset+1];
    uint8_t Instdecay = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank].Bank[Offset+2];
    uint8_t Instsustain = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank].Bank[Offset+3];
    uint8_t Instrelease = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank].Bank[Offset+4];
    uint16_t Instpulse_width = Seat->New_Computer->RAM[Seat->New_Computer->Processor.Audio_Processor.Instruments_Bank].Bank[Offset+5];

    //printf("%d %d %d %d %d %d\n", Instwaveform, Instattack, Instdecay, Instsustain, Instrelease, Instpulse_width);


    uint16_t freq_reg = note_to_sid_freq(40);


    sid_gate_on(Audio_Chip,0,freq_reg,Instwaveform,Instattack,Instdecay,Instsustain,Instrelease);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};





void Process_INT_Stop_Instrument(struct _Engine* Engine, struct _Seat_Object* Seat, struct _Audio_Chip* Audio_Chip)
{
    sid_gate_off(Audio_Chip, 0);
    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};



void Process_INT_Play_Track(struct _Engine* Engine, struct _Seat_Object* Seat)
{
    Seat->New_Computer->Processor.Audio_Processor.Tracking_On = true;

    unsigned char Bank = Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer]; //MOV int value
    unsigned short Address = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+3] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+2] & 0xff); //MOV int value
    unsigned short Spacing = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+5] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+4] & 0xff); //MOV int value
    unsigned short Length = (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+7] << 8) | (Seat->New_Computer->RAM[0].Bank[Seat->New_Computer->Processor.Stack_Pointer+6] & 0xff); //MOV int value

    Seat->New_Computer->Processor.Audio_Processor.Track_Bank = Bank;
    Seat->New_Computer->Processor.Audio_Processor.Track_Address = Address;
    Seat->New_Computer->Processor.Audio_Processor.Track_Start_Address = Address;
    Seat->New_Computer->Processor.Audio_Processor.Track_End_Address = Length;
    Seat->New_Computer->Processor.Audio_Processor.Step_Spacing = Spacing;

    printf("Playing Track Bank %d Start %d\n with Spacing %d\n", Bank, Address, Spacing);
    printf("This track ends at %d\n", Length);

    if(Seat->New_Computer->Processor.Instruction_Pointer + 2 < 65536)
    {
        Seat->New_Computer->Processor.Instruction_Pointer += 2;
    }
};



void Write_File_System(const char* Filename)
{
    BootSector BS;

    BS.boot_jmp[0] = OPC_JMP;
    BS.boot_jmp[1] = 0x00;
    BS.boot_jmp[2] = 0x00;
    BS.boot_jmp[3] = 0x00;

    memset(BS.oem_name,0,16);
    sprintf(BS.oem_name, "Test Name");
    BS.root_dir_entries = 1;

    DirEntry DE;

    memset(DE.File_Name_Encodings,0,64);
    memset(DE.Extension_Encodings,0,16);

    sprintf(DE.File_Name_Encodings,"TEST");
    sprintf(DE.Extension_Encodings,"TEST");
    DE.Filename_Length = 4;
    DE.File_Attributes = ATTRIBUTE_FILE;

    DE.Year_Created = 0;
    DE.Month_Created = 0;
    DE.Day_Created = 0;
    DE.Time_Created_Seconds = 0;

    DE.Year_Accessed = 0;
    DE.Month_Accessed = 0;
    DE.Day_Accessed = 0;
    DE.Time_Accessed_Seconds = 0;

    DE.Year_Modified = 0;
    DE.Month_Modified = 0;
    DE.Day_Modified = 0;
    DE.Time_Modified_Seconds = 0;

    DE.On_This_Bank = 0;
//    DE.At_This_Offset = 0;
  //  DE.Ends_At_This_Offset = 0;

    FILE* f = fopen(Filename, "wb");
    fwrite(&BS, 1, sizeof(BootSector),f);
    for(int u = 0; u < 590; u++)
    {
        fwrite(&DE, 1, sizeof(DirEntry),f);
    }
    unsigned char x;
    fwrite(&x, 25, sizeof(unsigned char),f);
    fclose(f);
};

void Write_Boot_Sector(FILE* f)
{

};

void Write_Directory(FILE* f)
{



};


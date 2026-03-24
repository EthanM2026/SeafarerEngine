/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "basic.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "new_opcodes.h"
#include "opcodes.h"
#include "emulator.h"
struct _AQ_BASIC_Compiler* Create_AQ_BASIC_Compiler()
{
    struct _AQ_BASIC_Compiler* I = (struct _AQ_BASIC_Compiler*) calloc(1,sizeof (struct _AQ_BASIC_Compiler));
    return I;
};

void AQ_Fill_Bank_With_File(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank_ID, unsigned short Offset, const char* Filename)
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

    if(File_Size > 0 && File_Size <= 65536)
    {
        for(int j = 0; j < File_Size; j++)
        {
            Compiler->Banks[Bank_ID].Bank[Offset+j] = Buffer[j];
        }
        Compiler->Current_Bank_Data_Offset += File_Size;
    }
    ////printf("Wrote a file \n");
}

void Initialize_AQ_BASIC_Compiler(struct _AQ_BASIC_Compiler* Compiler)
{
    Compiler->Number_Of_Banks = 3;
    Compiler->Start_Bank = 1;

    Compiler->Current_ROM_Bank = 1;
    Compiler->Current_ROM_Address = 0;

    Compiler->Current_RAM_Bank = 0;
    Compiler->Current_RAM_Address = 0;

    Compiler->Number_Of_RAM_Counters = 0;
    Compiler->Number_Of_ROM_Counters = 0;

    Compiler->Number_Of_Numericals = 0;
    Compiler->Number_Of_Equations = 0;
    Compiler->Number_Of_Strings = 0;
    Compiler->Number_Of_Labels = 0;

    Compiler->Number_Of_Ifs = 0;
    Compiler->Number_Of_Fors = 0;
    Compiler->Number_Of_Whiles = 0;

    Compiler->Number_Of_Images = 0;
    Compiler->Number_Of_Tilemaps = 0;
    Compiler->Number_Of_Encodings = 0;
};

int Compile_This_Text(struct _AQ_BASIC_Compiler* Compiler, const char* Filename, const char* Output)
{
    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    char Buffer[File_Size];
    fread(&Buffer, sizeof(unsigned char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;

    printf("Compiling\n");

    unsigned char Main_Bank = 0;
    unsigned short Main_Address = 0;

    for(int j = 0; j < File_Size; j++)
    {
        if(strncmp(&Buffer[j], "NUMBER",6) == 0)
        {
            printf("Found a NUMBER!\n");
            Compiler->Number_Of_Numericals += 1;
            Compiler->Numericals[Compiler->Number_Of_Numericals-1] = (struct _Numerical*)calloc(1,sizeof(struct _Numerical));

            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Address = Compiler->Current_RAM_Address;
            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Bank_Location = 0;
            memset(Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Name,0,16);
            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->This_Is_Contents_Of_RAM_Address = false;
            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Type = 1;
            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Value = 0;

            size_t length = strcspn(&Buffer[j + 7], " ");
            printf("Its length is %zu\n", length); // Outputs 5
            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Length = length;
            size_t value = strcspn(&Buffer[j + 7 + length + 3], ":");
            printf("Its value is %zu long.\n", value);
            if(value > 5)
            {
                printf("COMPILATION ABORTED: Number at %d is too long.\n", j);
                return -1;
            }

            int m = 0;
            while(m < length)
            {
                Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Name[m] = Buffer[j + 7 + m];
                printf("%c\n", Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Name[m]);
                m += 1;
            }
            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Name[length] = '\0';

            char Value[value];
            int n = 0;
            while(n < value)
            {
                Value[n] = Buffer[j + 7 + length + 3 + n];
                n+=1;
            }

            long temp_long;
            temp_long = strtol(Value, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }

            Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Value = (unsigned short)temp_long;
            printf("NUMBER Value: %d\n", Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Value);

            int process = Process_AQ_BASIC_Syntax_NUMVAR(Compiler,Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Value,Compiler->Numericals[Compiler->Number_Of_Numericals-1]->Address);
            if(process == -1)
            {
                printf("COMPILATION ABORTED: Could not insert sequence.");
                return -1;
            }

            Compiler->Current_RAM_Address += 2;

            printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }


        if(strncmp(&Buffer[j], "LBL",3) == 0)
        {
            printf("Found a LABEL!\n");
            Compiler->Number_Of_Labels += 1;
            Compiler->Labels[Compiler->Number_Of_Labels-1] = (struct _Label*)calloc(1,sizeof(struct _Label));

            Compiler->Labels[Compiler->Number_Of_Labels-1]->Address = Compiler->Current_ROM_Address;
            Compiler->Labels[Compiler->Number_Of_Labels-1]->Bank_Location = Compiler->Current_ROM_Bank;
            memset(Compiler->Labels[Compiler->Number_Of_Labels-1]->Name,0,16);
            Compiler->Labels[Compiler->Number_Of_Labels-1]->Type = 0;
            Compiler->Labels[Compiler->Number_Of_Labels-1]->Value = 0;

            size_t length = strcspn(&Buffer[j + 4], ":");
            printf("Its length is %zu\n", length); // Outputs 5

            int m = 0;
            while(m < length)
            {
                Compiler->Labels[Compiler->Number_Of_Labels-1]->Name[m] = Buffer[j + 4 + m];
                printf("%c\n", Compiler->Labels[Compiler->Number_Of_Labels-1]->Name[m]);
                m += 1;
            }
            Compiler->Labels[Compiler->Number_Of_Labels-1]->Name[length] = '\0';

            if(strncmp(Compiler->Labels[Compiler->Number_Of_Labels-1]->Name, "MAIN",4) == 0)
            {
                printf("Found Main Bank!\n");
                Main_Bank = Compiler->Labels[Compiler->Number_Of_Labels-1]->Bank_Location;
                Main_Address = Compiler->Labels[Compiler->Number_Of_Labels-1]->Address;
            }

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }


        if(strncmp(&Buffer[j], "CLS",3) == 0)
        {
            printf("Found a CLS!\n");
            int process = Process_AQ_BASIC_Syntax_CLS(Compiler, Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address);
            if(process == -1)
            {
                printf("COMPILATION ABORTED: Could not insert sequence.");
                return -1;
            }
           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }

        if(strncmp(&Buffer[j], "FLS",3) == 0)
        {
            printf("Found an FLS!\n");
            int process = Process_AQ_BASIC_Syntax_FLS(Compiler, Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address);
            if(process == -1)
            {
                printf("COMPILATION ABORTED: Could not insert sequence.");
                return -1;
            }
           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }


if(strncmp(&Buffer[j], "DECONDOWN",9) == 0)
        {
            printf("Found an Decrement Variable on Down Key\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t increment_length = strcspn(&Buffer[j + 10 + length + 1], ":");
            printf("Its length is %zu\n", increment_length); // Outputs 5

            char Variable[length];

            int m = 0;
            while(m < length)
            {
                Variable[m] = Buffer[j + 10 + m];
                printf("%c\n", Variable[m]);
                m += 1;
            }
            Variable[length] = '\0';




            char IncVariable[increment_length];

            m = 0;
            while(m < increment_length)
            {
                IncVariable[m] = Buffer[j + 10 + m + length + 1];
                printf("%c\n", IncVariable[m]);
                m += 1;
            }
            IncVariable[increment_length] = '\0';




            unsigned short Variable_Address, Amount_To_Increment;


            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, Variable,length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Variable_Address = Compiler->Numericals[r]->Address;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, IncVariable,increment_length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Amount_To_Increment = Compiler->Numericals[r]->Value;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            int Process = Process_AQ_BASIC_Syntax_DECONDOWN(Compiler, Variable_Address, Amount_To_Increment);

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }




        if(strncmp(&Buffer[j], "INCONRIGHT",10) == 0)
        {
            printf("Found an Decrement Variable on Left Key\n");
            size_t length = strcspn(&Buffer[j + 11], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t increment_length = strcspn(&Buffer[j + 11 + length + 1], ":");
            printf("Its length is %zu\n", increment_length); // Outputs 5

            char Variable[length];

            int m = 0;
            while(m < length)
            {
                Variable[m] = Buffer[j + 11 + m];
                printf("%c\n", Variable[m]);
                m += 1;
            }
            Variable[length] = '\0';




            char IncVariable[increment_length];

            m = 0;
            while(m < increment_length)
            {
                IncVariable[m] = Buffer[j + 11 + m + length + 1];
                printf("%c\n", IncVariable[m]);
                m += 1;
            }
            IncVariable[increment_length] = '\0';




            unsigned short Variable_Address, Amount_To_Increment;


            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, Variable,length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Variable_Address = Compiler->Numericals[r]->Address;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, IncVariable,increment_length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Amount_To_Increment = Compiler->Numericals[r]->Value;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            int Process = Process_AQ_BASIC_Syntax_INCONRIGHT(Compiler, Variable_Address, Amount_To_Increment);

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }




        if(strncmp(&Buffer[j], "DECONLEFT",9) == 0)
        {
            printf("Found an Decrement Variable on Left Key\n");
            size_t length = strcspn(&Buffer[j + 10], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t increment_length = strcspn(&Buffer[j + 10 + length + 1], ":");
            printf("Its length is %zu\n", increment_length); // Outputs 5

            char Variable[length];

            int m = 0;
            while(m < length)
            {
                Variable[m] = Buffer[j + 10 + m];
                printf("%c\n", Variable[m]);
                m += 1;
            }
            Variable[length] = '\0';




            char IncVariable[increment_length];

            m = 0;
            while(m < increment_length)
            {
                IncVariable[m] = Buffer[j + 10 + m + length + 1];
                printf("%c\n", IncVariable[m]);
                m += 1;
            }
            IncVariable[increment_length] = '\0';




            unsigned short Variable_Address, Amount_To_Increment;


            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, Variable,length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Variable_Address = Compiler->Numericals[r]->Address;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, IncVariable,increment_length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Amount_To_Increment = Compiler->Numericals[r]->Value;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            int Process = Process_AQ_BASIC_Syntax_DECONLEFT(Compiler, Variable_Address, Amount_To_Increment);

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }




        if(strncmp(&Buffer[j], "INCONUP",7) == 0)
        {
            printf("Found an Increment Variable on Up Key\n");
            size_t length = strcspn(&Buffer[j + 8], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t increment_length = strcspn(&Buffer[j + 8 + length + 1], ":");
            printf("Its length is %zu\n", increment_length); // Outputs 5

            char Variable[length];

            int m = 0;
            while(m < length)
            {
                Variable[m] = Buffer[j + 8 + m];
                printf("%c\n", Variable[m]);
                m += 1;
            }
            Variable[length] = '\0';




            char IncVariable[increment_length];

            m = 0;
            while(m < increment_length)
            {
                IncVariable[m] = Buffer[j + 8 + m + length + 1];
                printf("%c\n", IncVariable[m]);
                m += 1;
            }
            IncVariable[increment_length] = '\0';




            unsigned short Variable_Address, Amount_To_Increment;


            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, Variable,length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Variable_Address = Compiler->Numericals[r]->Address;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, IncVariable,increment_length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Amount_To_Increment = Compiler->Numericals[r]->Value;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }

            int Process = Process_AQ_BASIC_Syntax_INCONUP(Compiler, Variable_Address, Amount_To_Increment);

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }




        if(strncmp(&Buffer[j], "GOTO",4) == 0)
        {
            printf("Found a GOTO!\n");
            size_t length = strcspn(&Buffer[j + 5], ":");
            printf("Its length is %zu\n", length); // Outputs 5

            char Goto_This_Label[16];
            memset(Goto_This_Label,0,16);

            int m = 0;
            while(m < length)
            {
                Goto_This_Label[m] = Buffer[j + 5 + m];
                printf("%c\n", Goto_This_Label[m]);
                m += 1;
            }

            Goto_This_Label[length] = '\0';

            int r = 0;
            while(r < Compiler->Number_Of_Labels + 1)
            {
                if(r >= Compiler->Number_Of_Labels)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent label.");
                    return -1;
                }
                if(strncmp(Compiler->Labels[r]->Name, Goto_This_Label,length) == 0)
                {
                    printf("Found its label!\n");
                    int x = Process_AQ_BASIC_Syntax_GOTO(Compiler, Compiler->Labels[r]->Bank_Location, Compiler->Labels[r]->Address);
                    r = Compiler->Number_Of_Labels;
                }
                r+=1;
            }

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }


        if(strncmp(&Buffer[j], "GOSUB",5) == 0)
        {
            printf("Found a GOSUB!\n");
            size_t length = strcspn(&Buffer[j + 6], ":");
            printf("Its length is %zu\n", length); // Outputs 5

            char Goto_This_Label[16];
            memset(Goto_This_Label,0,16);

            int m = 0;
            while(m < length)
            {
                Goto_This_Label[m] = Buffer[j + 6 + m];
                printf("%c\n", Goto_This_Label[m]);
                m += 1;
            }

            Goto_This_Label[length] = '\0';

            int r = 0;
            while(r < Compiler->Number_Of_Labels + 1)
            {
                if(r >= Compiler->Number_Of_Labels)
                {
                    printf("COMPILATION ABORTED: GOSUB command has no parent label.");
                    return -1;
                }
                if(strncmp(Compiler->Labels[r]->Name, Goto_This_Label,length) == 0)
                {
                    printf("Found its label!\n");
                    int x = Process_AQ_BASIC_Syntax_GOSUB(Compiler, Compiler->Labels[r]->Bank_Location, Compiler->Labels[r]->Address);
                    r = Compiler->Number_Of_Labels;
                }
                r+=1;
            }

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }

        if(strncmp(&Buffer[j], "GBNK",4) == 0) //Go to Bank and Address
        {
            printf("Found a GBNK!\n");
            size_t length = strcspn(&Buffer[j + 5], ":");
            printf("Its length is %zu\n", length); // Outputs 5

            char Goto_This_Label[16];
            memset(Goto_This_Label,0,16);

            int m = 0;
            while(m < length)
            {
                Goto_This_Label[m] = Buffer[j + 5 + m];
                printf("%c\n", Goto_This_Label[m]);
                m += 1;
            }

            Goto_This_Label[length] = '\0';

            int r = 0;
            while(r < Compiler->Number_Of_Labels + 1)
            {
                if(r >= Compiler->Number_Of_Labels)
                {
                    printf("COMPILATION ABORTED: GOTO command has no parent label.");
                    return -1;
                }
                if(strncmp(Compiler->Labels[r]->Name, Goto_This_Label,length) == 0)
                {
                    printf("Found its label!\n");
                    int x = Process_AQ_BASIC_Syntax_GOTO(Compiler, Compiler->Labels[r]->Bank_Location, Compiler->Labels[r]->Address);
                    r = Compiler->Number_Of_Labels;
                }
                r+=1;
            }

           printf("Update:\n");
            printf("ROM Bank: %d, ROM Address: %d, RAM Address: %d\n", Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address,Compiler->Current_RAM_Address);
        }

        if(strncmp(&Buffer[j], "RIMG",4) == 0)
        {
            printf("Found an Render Image!\n");
            size_t length = strcspn(&Buffer[j + 5], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            char Render_This[16];
            memset(Render_This,0,16);

            int m = 0;
            while(m < length)
            {
                Render_This[m] = Buffer[j + 5 + m];
                printf("%c\n", Render_This[m]);
                m += 1;
            }

            Render_This[length] = '\0';

            size_t xlength = strcspn(&Buffer[j + 5 + length + 1], " ");
            printf("Its x length is %zu\n", xlength); // Outputs 5

            char xValue[xlength];
            int n = 0;
            while(n < xlength)
            {
                xValue[n] = Buffer[j + 5 + length + 1 + n];
                n+=1;
            }

            long temp_longx;
            temp_longx = strtol(xValue, NULL, 10);

            if(temp_longx > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("x Value: %d\n", (unsigned short)temp_longx);
            unsigned short RIMGx = (unsigned short)temp_longx;












            size_t ylength = strcspn(&Buffer[j + 5 + length + 1 + xlength + 1], ":");
            printf("Its y length is %zu\n", ylength); // Outputs 5

            char yValue[ylength];
            n = 0;
            while(n < ylength)
            {
                yValue[n] = Buffer[j + 5 + length + 1 + xlength + 1 + n];
                n+=1;
            }

            long temp_longy;
            temp_longy = strtol(yValue, NULL, 10);

            if(temp_longy > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("y Value: %d\n", (unsigned short)temp_longy);
            unsigned short RIMGy = (unsigned short)temp_longy;



            int r = 0;
            while(r < Compiler->Number_Of_Images + 1)
            {
                if(r >= Compiler->Number_Of_Images)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent image.");
                    return -1;
                }
                if(strncmp(Compiler->Images[r]->Name, Render_This,length) == 0)
                {
                    printf("Found its image!\n");
                    int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Images[r]->Bank_Location, Compiler->Images[r]->Address, RIMGx, RIMGy);
                    r = Compiler->Number_Of_Images;
                }
                r+=1;
            }
        }








        if(strncmp(&Buffer[j], "RENAT",5) == 0) //FIXED
        {
            printf("Found an Render Image At Variables!\n");
            size_t length = strcspn(&Buffer[j + 6], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t xVarlength = strcspn(&Buffer[j + 6 + length + 1], " ");
            printf("Its xVar length is %zu\n", xVarlength); // Outputs 5

            size_t yVarlength = strcspn(&Buffer[j + 6 + length + 1 + xVarlength + 1], ":");
            printf("Its yVar length is %zu\n", yVarlength); // Outputs 5

            char Render_This[16];
            memset(Render_This,0,16);

            int m = 0;
            while(m < length)
            {
                Render_This[m] = Buffer[j + 6 + m];
                printf("%c\n", Render_This[m]);
                m += 1;
            }
            Render_This[length] = '\0';
            printf("Render this\n");


            char AtThisX[16];
            memset(AtThisX,0,16);

            m = 0;
            while(m < xVarlength)
            {
                AtThisX[m] = Buffer[j + 6 + m + length + 1];
                printf("%c\n", AtThisX[m]);
                m += 1;
            }

            AtThisX[xVarlength] = '\0';
            printf("Render this\n");





            char AtThisY[16];
            memset(AtThisY,0,16);

            m = 0;
            while(m < yVarlength)
            {
                AtThisY[m] = Buffer[j + 6 + m + length + 1 + xVarlength + 1];
                printf("%c\n", AtThisY[m]);
                m += 1;
            }

            AtThisY[yVarlength] = '\0';
            printf("Render this\n");

            unsigned short x, y;

            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent x var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, AtThisX,length) == 0)
                {
                    printf("Found its xVar!\n");
                    x = Compiler->Numericals[r]->Address;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[r]->Bank_Location, Compiler->Numericals[r]->Address, RIMGx, RIMGy);
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }







            int s = 0;
            while(s < Compiler->Number_Of_Numericals + 1)
            {
                if(s >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent y var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[s]->Name, AtThisY,length) == 0)
                {
                    printf("Found its yVar!\n");
                    y = Compiler->Numericals[s]->Address;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[s]->Bank_Location, Compiler->Numericals[s]->Address, RIMGx, RIMGy);
                    s = Compiler->Number_Of_Numericals;
                }
                s+=1;
            }




            int u = 0;
            while(u < Compiler->Number_Of_Images + 1)
            {
                if(u >= Compiler->Number_Of_Images)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent image.");
                    return -1;
                }
                if(strncmp(Compiler->Images[u]->Name, Render_This,length) == 0)
                {
                    printf("Found its image!\n");
                    int v = Process_AQ_BASIC_Syntax_RENAT(Compiler, Compiler->Images[u]->Bank_Location, Compiler->Images[u]->Address, x,y);
                    u = Compiler->Number_Of_Images;
                }
                u+=1;
            }
        }




        if(strncmp(&Buffer[j], "SETV",4) == 0)
        {
            printf("Found a Set Variable to!\n");
            size_t length = strcspn(&Buffer[j + 5], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t xVarlength = strcspn(&Buffer[j + 5 + length + 1], ":");
            printf("Its Set To length is %zu\n", xVarlength); // Outputs 5



            char Set_This[16];
            memset(Set_This,0,16);

            int m = 0;
            while(m < length)
            {
                Set_This[m] = Buffer[j + 5 + m];
                printf("%c\n", Set_This[m]);
                m += 1;
            }
            Set_This[length] = '\0';
            printf("Render this\n");


            char SetToThis[16];
            memset(SetToThis,0,16);

            m = 0;
            while(m < xVarlength)
            {
                SetToThis[m] = Buffer[j + 5 + m + length + 1];
                printf("%c\n", SetToThis[m]);
                m += 1;
            }
            printf("Render this\n");

            long temp_longTT;
            temp_longTT = strtol(SetToThis, NULL, 10);

            if(temp_longTT > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("y Value: %d\n", (unsigned short)temp_longTT);



            unsigned short New_Value = (unsigned short)temp_longTT;



            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
               {
                    printf("COMPILATION ABORTED: SETV command has no parent x var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name,Set_This,xVarlength) == 0)
                {
                    printf("Found its xVar!\n");
                    Compiler->Numericals[r]->Value = New_Value;
                    printf("Num new value is %d\n", New_Value);
                    int process = Process_AQ_BASIC_Syntax_SETV(Compiler, Compiler->Numericals[r]->Address, New_Value);

                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }


           // int u = 0;
           // while(u < Compiler->Number_Of_Images + 1)
           // {
              //  if(u >= Compiler->Number_Of_Images)
              //  {
            //        printf("COMPILATION ABORTED: RIMG command has no parent image.");
             //       return -1;
            //    }
            //    if(strncmp(Compiler->Images[u]->Name, Render_This,length) == 0)
            //    {
            //        printf("Found its image!\n");
             //       int v = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Images[u]->Bank_Location, Compiler->Images[u]->Address, x,y);
            //        u = Compiler->Number_Of_Images;
            //    }
           //     u+=1;
            //}
        }






        if(strncmp(&Buffer[j], "ADDV",4) == 0)
        {
            printf("Found a Set Variable to!\n");
            size_t length = strcspn(&Buffer[j + 5], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t xVarlength = strcspn(&Buffer[j + 5 + length + 1], ":");
            printf("Its Set To length is %zu\n", xVarlength); // Outputs 5



            char Set_This[16];
            memset(Set_This,0,16);

            int m = 0;
            while(m < length)
            {
                Set_This[m] = Buffer[j + 5 + m];
                printf("%c\n", Set_This[m]);
                m += 1;
            }
            Set_This[length] = '\0';
            printf("Render this\n");


            char SetToThis[16];
            memset(SetToThis,0,16);

            m = 0;
            while(m < xVarlength)
            {
                SetToThis[m] = Buffer[j + 5 + m + length + 1];
                printf("%c\n", SetToThis[m]);
                m += 1;
            }
            printf("Render this\n");

            long temp_longTT;
            temp_longTT = strtol(SetToThis, NULL, 10);

            if(temp_longTT > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("y Value: %d\n", (unsigned short)temp_longTT);



            unsigned short New_Value = (unsigned short)temp_longTT;



            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
               {
                    printf("COMPILATION ABORTED: SETV command has no parent x var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name,Set_This,xVarlength) == 0)
                {
                    printf("Found its xVar!\n");
                    Compiler->Numericals[r]->Value += New_Value;
                    printf("Num new value is %d\n", New_Value);
                    int process = Process_AQ_BASIC_Syntax_ADDV(Compiler, Compiler->Numericals[r]->Address, New_Value);

                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }


           // int u = 0;
           // while(u < Compiler->Number_Of_Images + 1)
           // {
              //  if(u >= Compiler->Number_Of_Images)
              //  {
            //        printf("COMPILATION ABORTED: RIMG command has no parent image.");
             //       return -1;
            //    }
            //    if(strncmp(Compiler->Images[u]->Name, Render_This,length) == 0)
            //    {
            //        printf("Found its image!\n");
             //       int v = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Images[u]->Bank_Location, Compiler->Images[u]->Address, x,y);
            //        u = Compiler->Number_Of_Images;
            //    }
           //     u+=1;
            //}
        }








        if(strncmp(&Buffer[j], "IMAGE",5) == 0)
        {
            printf("Found an IMAGE!\n");
            Compiler->Number_Of_Images += 1;
            Compiler->Images[Compiler->Number_Of_Images-1] = (struct AQ_Image*)calloc(1,sizeof(struct AQ_Image));
            Compiler->Images[Compiler->Number_Of_Images-1]->Address = Compiler->Current_RAM_Address;
            Compiler->Images[Compiler->Number_Of_Images-1]->Bank_Location = Compiler->Current_RAM_Bank;
            memset(Compiler->Images[Compiler->Number_Of_Images-1]->Name,0,16);
            Compiler->Images[Compiler->Number_Of_Images-1]->This_Is_Contents_Of_RAM_Address = false;
            Compiler->Images[Compiler->Number_Of_Images-1]->Type = 1;
            Compiler->Images[Compiler->Number_Of_Images-1]->Value = 0;

            size_t length = strcspn(&Buffer[j + 6], " ");
            printf("Its length is %zu\n", length); // Outputs 5
            Compiler->Images[Compiler->Number_Of_Images-1]->Length = length;

            int m = 0;
            while(m < length)
            {
                Compiler->Images[Compiler->Number_Of_Images-1]->Name[m] = Buffer[j + 6 + m];
                printf("%c\n", Compiler->Images[Compiler->Number_Of_Images-1]->Name[m]);
                m += 1;
            }
            Compiler->Images[Compiler->Number_Of_Images-1]->Name[length] = '\0';

            size_t bank_length = strcspn(&Buffer[j + 6 + length + 4], " ");
            printf("Its bank length is %zu\n", bank_length); // Outputs 5

            char BankValue[bank_length];
            int n = 0;
            while(n < bank_length)
            {
                BankValue[n] = Buffer[j + 6 + length + 4 + n];
                n+=1;
            }

            long temp_long;
            temp_long = strtol(BankValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Bank Value: %d\n", (unsigned char)temp_long);
            Compiler->Images[Compiler->Number_Of_Images-1]->Bank_Location = (unsigned char)temp_long;



            size_t addr_length = strcspn(&Buffer[j + 6 + length + 4 + bank_length + 2], ":");
            printf("Its addr length is %zu\n", addr_length); // Outputs 5

            char AddrValue[addr_length];
            n = 0;
            while(n < addr_length)
            {
                AddrValue[n] = Buffer[j + 6 + length + 4 + bank_length + 2 + n];
                n+=1;
            }

            long temp_longtwo;
            temp_longtwo = strtol(AddrValue, NULL, 10);

            if(temp_longtwo > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Addr Value: %d\n", (unsigned char)temp_longtwo);
            Compiler->Images[Compiler->Number_Of_Images-1]->Address = (unsigned short)temp_longtwo;
        }













        if(strncmp(&Buffer[j], "TILEMAP",7) == 0)
        {
            printf("Found an TILEMAP!\n");
            Compiler->Number_Of_Tilemaps += 1;
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1] = (struct AQ_Tilemap*)calloc(1,sizeof(struct AQ_Tilemap));
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Address = Compiler->Current_RAM_Address;
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Bank_Location = Compiler->Current_RAM_Bank;
            memset(Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Name,0,16);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->This_Is_Contents_Of_RAM_Address = false;
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Type = 1;
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Value = 0;

            size_t length = strcspn(&Buffer[j + 8], " ");
            printf("Its length is %zu\n", length); // Outputs 5
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Length = length;

            int m = 0;
            while(m < length)
            {
                Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Name[m] = Buffer[j + 8 + m];
                printf("%c\n", Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Name[m]);
                m += 1;
            }
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Name[length] = '\0';




            size_t bank_length = strcspn(&Buffer[j + 8 + length + 4], " ");
            printf("Its bank length is %zu\n", bank_length); // Outputs 5

            char BankValue[bank_length];
            int n = 0;
            while(n < bank_length)
            {
                BankValue[n] = Buffer[j + 8 + length + 4 + n];
                printf("%c\n", BankValue[n]);
                n+=1;
            }

            long temp_long;
            temp_long = strtol(BankValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Bank Value: %d\n", (unsigned char)temp_long);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Bank_Location = (unsigned char)temp_long;














            size_t addr_length = strcspn(&Buffer[j + 8 + length + 4 + bank_length + 2], " ");
            printf("Its addr length is %zu\n", addr_length); // Outputs 5

            char AddrValue[addr_length];
            n = 0;
            while(n < addr_length)
            {
                AddrValue[n] = Buffer[j + 8 + length + 4 + bank_length + 2 + n];
                printf("%c\n", AddrValue[n]);
                n+=1;
            }

            temp_long = strtol(AddrValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Addr Value: %d\n", (unsigned char)temp_long);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Address = (unsigned char)temp_long;













            size_t width_length = strcspn(&Buffer[j + 8 + length + 4 + bank_length + 2 +addr_length + 2], " ");
            printf("Its width length is %zu\n", width_length); // Outputs 5

            char WidthValue[width_length];
            n = 0;
            while(n < width_length)
            {
                WidthValue[n] = Buffer[j + 8 + length + 4 + bank_length + 2 + n + addr_length + 2];
                printf("%c\n", WidthValue[n]);
                n+=1;
            }

            temp_long = strtol(WidthValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Width Value: %d\n", (unsigned char)temp_long);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Width = (unsigned char)temp_long;














            size_t height_length = strcspn(&Buffer[j + 8 + length + 4 + bank_length + 2 +addr_length + 2 + width_length + 2], " ");
            printf("Its height length is %zu\n", height_length); // Outputs 5

            char HeightValue[height_length];
            n = 0;
            while(n < height_length)
            {
                HeightValue[n] = Buffer[j + 8 + length + 4 + bank_length + 2 + n + addr_length + 2 + width_length + 2];
                printf("%c\n", HeightValue[n]);
                n+=1;
            }

            temp_long = strtol(HeightValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Height Value: %d\n", (unsigned char)temp_long);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Height = (unsigned char)temp_long;















            size_t tile_width_length = strcspn(&Buffer[j + 8 + length + 4 + bank_length + 2 +addr_length + 2 + width_length + 2 + height_length + 2], " ");
            printf("Its height length is %zu\n", tile_width_length); // Outputs 5

            char TileWidthValue[tile_width_length];
            n = 0;
            while(n < tile_width_length)
            {
                TileWidthValue[n] = Buffer[j + 8 + length + 4 + bank_length + 2 + n + addr_length + 2 + width_length + 2 + height_length + 2];
                printf("%c\n", TileWidthValue[n]);
                n+=1;
            }

            temp_long = strtol(TileWidthValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("TileWidth Value: %d\n", (unsigned char)temp_long);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Tile_Width = (unsigned char)temp_long;















            size_t tile_height_length = strcspn(&Buffer[j + 8 + length + 4 + bank_length + 2 +addr_length + 2 + width_length + 2 + height_length + 2 + tile_width_length + 2], ":");
            printf("Its height length is %zu\n", tile_height_length); // Outputs 5

            char TileHeightValue[tile_height_length];
            n = 0;
            while(n < tile_height_length)
            {
                TileHeightValue[n] = Buffer[j + 8 + length + 4 + bank_length + 2 + n + addr_length + 2 + height_length + 2 + height_length + 2 + tile_width_length + 2];
                printf("%c\n", TileHeightValue[n]);
                n+=1;
            }

            temp_long = strtol(TileHeightValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("TileHeight Value: %d\n", (unsigned char)temp_long);
            Compiler->Tilemaps[Compiler->Number_Of_Tilemaps-1]->Tile_Height = (unsigned char)temp_long;
        }



        if(strncmp(&Buffer[j], "ENCODING",8) == 0)
        {
            printf("Found an ENCODING!\n");
            Compiler->Number_Of_Encodings += 1;
            Compiler->Encodings[Compiler->Number_Of_Encodings-1] = (struct AQ_Encoding*)calloc(1,sizeof(struct AQ_Encoding));
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Address = Compiler->Current_RAM_Address;
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Bank_Location = Compiler->Current_RAM_Bank;
            memset(Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Name,0,16);
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->This_Is_Contents_Of_RAM_Address = false;
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Type = 1;
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Value = 0;

            size_t length = strcspn(&Buffer[j + 9], " ");
            printf("Its length is %zu\n", length); // Outputs 5
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Length = length;

            int m = 0;
            while(m < length)
            {
                Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Name[m] = Buffer[j + 9 + m];
                printf("%c\n", Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Name[m]);
                m += 1;
            }
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Name[length] = '\0';

            size_t bank_length = strcspn(&Buffer[j + 9 + length + 4], " ");
            printf("Its bank length is %zu\n", bank_length); // Outputs 5

            char BankValue[bank_length];
            int n = 0;
            while(n < bank_length)
            {
                BankValue[n] = Buffer[j + 9 + length + 4 + n];
                n+=1;
            }

            long temp_long;
            temp_long = strtol(BankValue, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Bank Value: %d\n", (unsigned char)temp_long);
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Bank_Location = (unsigned char)temp_long;



            size_t addr_length = strcspn(&Buffer[j + 9 + length + 4 + bank_length + 2], ":");
            printf("Its addr length is %zu\n", addr_length); // Outputs 5

            char AddrValue[addr_length];
            n = 0;
            while(n < addr_length)
            {
                AddrValue[n] = Buffer[j + 9 + length + 4 + bank_length + 2 + n];
                n+=1;
            }

            long temp_longtwo;
            temp_longtwo = strtol(AddrValue, NULL, 10);

            if(temp_longtwo > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Addr Value: %d\n", (unsigned char)temp_longtwo);
            Compiler->Encodings[Compiler->Number_Of_Encodings-1]->Address = (unsigned short)temp_longtwo;
        }


        if(strncmp(&Buffer[j], "RENTILESAT",10) == 0)
        {
            printf("Found an RENTILESAT!\n");
            unsigned short yRenderLength;
            unsigned short xRenderLength;
            unsigned short y;
            unsigned short x;
            unsigned short Encoding_Start_Location;
            unsigned short Encoding_RAM_Bank;
            unsigned short Encoding_RAM_Address;
            unsigned short Tilemap_Width;
            unsigned short Tilemap_Height;
            unsigned short Tilemap_RAM_Bank;
            unsigned short Tilemap_RAM_Address;
            unsigned short Tile_Width;
            unsigned short Tile_Height;



            size_t length = strcspn(&Buffer[j + 11], " ");
            printf("Its length is %zu\n", length);

            char Tilemap_To_Use[length];

            int m = 0;
            while(m < length)
            {
                Tilemap_To_Use[m] = Buffer[j + 11 + m];
                printf("%c\n", Tilemap_To_Use[m]);
                m += 1;
            }
            Tilemap_To_Use[length] = '\0';








            size_t encoding_length = strcspn(&Buffer[j + 11 + length + 1], " ");
            printf("Its encoding length is %zu\n", encoding_length);

            char Encoding_To_Use[encoding_length];

            m = 0;
            while(m < encoding_length)
            {
                Encoding_To_Use[m] = Buffer[j + 11 + length + 1 + m];
                printf("%c\n", Encoding_To_Use[m]);
                m += 1;
            }
            Encoding_To_Use[encoding_length] = '\0';









            size_t encoding_offset_length = strcspn(&Buffer[j + 11 + length + 1 + encoding_length + 2], " ");
            printf("Its encoding_offset length is %zu\n", encoding_offset_length);

            char Length_To_Use[encoding_offset_length];

            m = 0;
            while(m < encoding_offset_length)
            {
                Length_To_Use[m] = Buffer[j + 11 + length + 1 + m + encoding_length + 2];
                printf("%c\n", Length_To_Use[m]);
                m += 1;
            }
            Length_To_Use[encoding_offset_length] = '\0';


            long temp_long;
            temp_long = strtol(Length_To_Use, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Bank Value: %d\n", (unsigned short)temp_long);
            Encoding_Start_Location = (unsigned short)temp_long;










            size_t xvar_length = strcspn(&Buffer[j + 11 + length + 1 + encoding_length + 2 + encoding_offset_length + 1], " ");
            printf("Its xvar length is %zu\n", xvar_length);

            char XVLength_To_Use[xvar_length];

            m = 0;
            while(m < xvar_length)
            {
                XVLength_To_Use[m] = Buffer[j + 11 + length + 1 + m + encoding_length + 2 + encoding_offset_length + 1];
                printf("%c\n", XVLength_To_Use[m]);
                m += 1;
            }
            XVLength_To_Use[xvar_length] = '\0';
            //Now, find the XVariable.
            int u = 0;
            while(u < Compiler->Number_Of_Numericals + 1)
            {
                if(u >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RENTILES command has no parent XVar.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[u]->Name, XVLength_To_Use,length) == 0)
                {
                    printf("Found its numerical\n");
                    x = Compiler->Numericals[u]->Value;
                    //int v = Process_AQ_BASIC_Syntax_RENAT(Compiler, Compiler->Numericals[u]->Bank_Location, Compiler->Numericals[u]->Address, x,y);
                    u = Compiler->Number_Of_Numericals;
                }
                u+=1;
            }



            //temp_long = strtol(XVLength_To_Use, NULL, 10);

            //if(temp_long > 65535)
           // {
           //     printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
           //     return -1;
          //  }
          //  printf("Bank Value: %d\n", (unsigned short)temp_long);
           // x = (unsigned short)temp_long;













            size_t yvar_length = strcspn(&Buffer[j + 11 + length + 1 + encoding_length + 2 + encoding_offset_length + 1 + xvar_length + 1], " ");
            printf("Its yvar length is %zu\n", yvar_length);

            char YVLength_To_Use[yvar_length];

            m = 0;
            while(m < yvar_length)
            {
                YVLength_To_Use[m] = Buffer[j + 11 + length + 1 + m + encoding_length + 2 + encoding_offset_length + 1 + xvar_length + 1];
                printf("%c\n", YVLength_To_Use[m]);
                m += 1;
            }
            YVLength_To_Use[yvar_length] = '\0';
            //Now, find the XVariable.
            u = 0;
            while(u < Compiler->Number_Of_Numericals + 1)
            {
                if(u >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RENTILES command has no parent XVar.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[u]->Name, YVLength_To_Use,length) == 0)
                {
                    printf("Found its numerical\n");
                    y = Compiler->Numericals[u]->Value;
                    //int v = Process_AQ_BASIC_Syntax_RENAT(Compiler, Compiler->Numericals[u]->Bank_Location, Compiler->Numericals[u]->Address, x,y);
                    u = Compiler->Number_Of_Numericals;
                }
                u+=1;
            }








            size_t xrender_length = strcspn(&Buffer[j + 11 + length + 1 + encoding_length + 2 + encoding_offset_length + 1 + xvar_length + 1 + yvar_length + 1], " ");
            printf("Its xrender length is %zu\n", xrender_length);

            char XRLength_To_Use[xrender_length];

            m = 0;
            while(m < xrender_length)
            {
                XRLength_To_Use[m] = Buffer[j + 11 + length + 1 + m + encoding_length + 2 + encoding_offset_length + 1 + xvar_length + 1 + yvar_length + 1];
                printf("%c\n", XRLength_To_Use[m]);
                m += 1;
            }
            XRLength_To_Use[xrender_length] = '\0';

            temp_long = strtol(XRLength_To_Use, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Bank Value: %d\n", (unsigned short)temp_long);
            xRenderLength = (unsigned short)temp_long;












            size_t yrender_length = strcspn(&Buffer[j + 11 + length + 1 + encoding_length + 2 + encoding_offset_length + 1 + xvar_length + 1 + yvar_length + 1 + xrender_length + 1], ":");
            printf("Its yrender length is %zu\n", yrender_length);

            char YRLength_To_Use[yrender_length];

            m = 0;
            while(m < yrender_length)
            {
                YRLength_To_Use[m] = Buffer[j + 11 + length + 1 + m + encoding_length + 2 + encoding_offset_length + 1 + xvar_length + 1 + yvar_length + 1 + xrender_length + 1];
                printf("%c\n", YRLength_To_Use[m]);
                m += 1;
            }
            YRLength_To_Use[yrender_length] = '\0';

            temp_long = strtol(YRLength_To_Use, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("Bank Value: %d\n", (unsigned short)temp_long);
            yRenderLength = (unsigned short)temp_long;





            printf("Search for tilemaps\n");
            u = 0;
            while(u < Compiler->Number_Of_Tilemaps + 1)
            {
                if(u >= Compiler->Number_Of_Tilemaps)
                {
                    printf("COMPILATION ABORTED: RENTILES command has no parent Tilemap.");
                    return -1;
                }
                printf("Length, %d\n", Compiler->Tilemaps[u]->Length);
                if(strncmp(Compiler->Tilemaps[u]->Name, Tilemap_To_Use,length) == 0)
                {
                    printf("Found its tilemap\n");
                    Tilemap_Width = Compiler->Tilemaps[u]->Width;
                    Tilemap_Height = Compiler->Tilemaps[u]->Height;
                    Tilemap_RAM_Bank = Compiler->Tilemaps[u]->Bank_Location;
                    Tilemap_RAM_Address = Compiler->Tilemaps[u]->Address;
                    Tile_Width = Compiler->Tilemaps[u]->Tile_Width;
                    Tile_Height = Compiler->Tilemaps[u]->Tile_Height;

                    printf("Tilemap Width: %d\n", Tilemap_Width);
                    printf("Tilemap Height: %d\n", Tilemap_Height);

                    //int v = Process_AQ_BASIC_Syntax_RENAT(Compiler, Compiler->Tilemaps[u]->Bank_Location, Compiler->Tilemaps[u]->Address, x,y);
                    u = Compiler->Number_Of_Tilemaps;
                }
                u+=1;
            }










            u = 0;
            while(u < Compiler->Number_Of_Encodings + 1)
            {
                if(u >= Compiler->Number_Of_Encodings)
                {
                    printf("COMPILATION ABORTED: RENTILES command has no parent Encoding.");
                    return -1;
                }
                if(strncmp(Compiler->Encodings[u]->Name, Encoding_To_Use,encoding_length) == 0)
                {
                    printf("Found its encoding\n");

                    Encoding_RAM_Bank = Compiler->Encodings[u]->Bank_Location;
                    Encoding_RAM_Address = Compiler->Encodings[u]->Address;

                    printf("Encoding Bank: %d, Encoding Address: %d\n", Encoding_RAM_Bank, Encoding_RAM_Address);

                    //int v = Process_AQ_BASIC_Syntax_RENAT(Compiler, Compiler->Encodings[u]->Bank_Location, Compiler->Encodings[u]->Address, x,y);
                    u = Compiler->Number_Of_Encodings;
                }
                u+=1;
            }

            printf("yRender Length %d xRender Length %d x %d y %d Encode Start %d Encode RAM Bank %d Encode Address %d Tilemap Widht %d Tilemap Height %d Tilemap Ram B %d Tilemap RAM A %d Tile W %d Tile H %d \n", yRenderLength,xRenderLength,y,x,Encoding_Start_Location,Encoding_RAM_Bank,Encoding_RAM_Address,Tilemap_Width,Tilemap_Height,Tilemap_RAM_Bank,Tilemap_RAM_Address,Tile_Width,Tile_Height);
            printf("Tilemap Width: %d\n", Tilemap_Width);

            int Process = Process_AQ_BASIC_Syntax_RENTILESAT(Compiler, Tilemap_RAM_Bank,Tilemap_RAM_Address,Tilemap_Width,Tilemap_Height, Tile_Width, Tile_Height, Encoding_RAM_Bank, Encoding_RAM_Address, Encoding_Start_Location, x,y,xRenderLength,yRenderLength);

        }


        if(strncmp(&Buffer[j], "IFVAR",5) == 0) //IFVAR PERCY EQ 7 THENGO DIE
        {
            printf("Found an IF This Variable Is!\n"); //EQ, NEQ, GE, LE, G, L
            size_t length = strcspn(&Buffer[j + 6], " ");
            printf("Its length is %zu\n", length); // Outputs 5

            size_t cond_length = strcspn(&Buffer[j + 6 + length + 1], " ");
            printf("Its condition length is %zu\n", cond_length); // Outputs 5

            size_t var_length = strcspn(&Buffer[j + 6 + length + 1 + cond_length + 1], " ");
            printf("Its var length is %zu\n", var_length); // Outputs 5

            size_t goto_label_length = strcspn(&Buffer[j + 6 + length + 1 + cond_length + 1 + var_length + 8], ":");
            printf("Its goto label length is %zu\n", goto_label_length); // Outputs 5

            char Variable[length];
            char Condition[cond_length];
            char Var[var_length];
            char Goto_Label[goto_label_length];

            unsigned short Value;

            int m = 0;
            while(m < length)
            {
                Variable[m] = Buffer[j + 6 + m];
                printf("%c\n", Variable[m]);
                m += 1;
            }
            Variable[length] = '\0';

            printf("\n");

            m = 0;
            while(m < cond_length)
            {
                Condition[m] = Buffer[j + 6 + m + length + 1];
                printf("%c\n", Condition[m]);
                m += 1;
            }
            Condition[cond_length] = '\0';


            printf("\n");




            m = 0;
            while(m < var_length)
            {
               Var[m] = Buffer[j + 6 + m + length + 1 + cond_length + 1];
                printf("%c\n", Var[m]);
                m += 1;
            }
            Var[var_length] = '\0';
            printf("\n");




             m = 0;
            while(m < goto_label_length)
            {
               Goto_Label[m] = Buffer[j + 6 + m + length + 1 + cond_length + 1 + var_length + 8];
                printf("%c\n", Goto_Label[m]);
                m += 1;
            }
            Goto_Label[goto_label_length] = '\0';
            printf("\n");





            long temp_long;
            temp_long = strtol(Var, NULL, 10);

            if(temp_long > 65535)
            {
                printf("COMPILATION ABORTED: Number at %d is too large.\n", j);
                return -1;
            }
            printf("y Value: %d\n", (unsigned short)temp_long);
            Value = (unsigned short)temp_long;

            unsigned short Contents_Of_This_RAM_Address;


            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
               {
                    printf("COMPILATION ABORTED: IFVAR command has no parent var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name,Variable,length) == 0)
                {
                    printf("Found its xVar!\n");
                    Contents_Of_This_RAM_Address = Compiler->Numericals[r]->Address;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }





            unsigned char Goto_Label_Bank;
            unsigned short Goto_Label_Address;


            r = 0;
            while(r < Compiler->Number_Of_Labels + 1)
            {
                if(r >= Compiler->Number_Of_Labels)
               {
                    printf("COMPILATION ABORTED: IFVAR command has no parent label.");
                    return -1;
                }
                if(strncmp(Compiler->Labels[r]->Name,Goto_Label,length) == 0)
                {
                    printf("Found its label!\n");
                    Goto_Label_Address = Compiler->Labels[r]->Address;
                    Goto_Label_Bank = Compiler->Labels[r]->Bank_Location;
                    r = Compiler->Number_Of_Labels;
                }
                r+=1;
            }

            int Process = Process_AQ_BASIC_Syntax_IFVAR(Compiler, Contents_Of_This_RAM_Address, Value, Goto_Label_Bank, Goto_Label_Address);
        }

        if(strncmp(&Buffer[j], "IFVARVAR",8) == 0) //FIXED
        {
            printf("Found an IF This Variable Is Equal to another Variable!\n"); //EQ, NEQ, GE, LE, G, L
        }


        if(strncmp(&Buffer[j], "IFINPUT",7) == 0) //FIXED
        {
            printf("Found an IF This Input Is!\n"); //EQ, NEQ, GE, LE, G, L
        }

        if(strncmp(&Buffer[j], "IFINPUTVAR",10) == 0) //FIXED
        {
            printf("Found an IF This INPUT Is Equal to a Variable!\n"); //EQ, NEQ, GE, LE, G, L
        }




        if(strncmp(&Buffer[j], "RETURN",6) == 0) //FIXED
        {
            printf("Found a Return!\n"); //EQ, NEQ, GE, LE, G, L
            int Process = Process_AQ_BASIC_Syntax_RETURN(Compiler,Compiler->Current_ROM_Bank, Compiler->Current_ROM_Address);
        }



        if(strncmp(&Buffer[j], "REPEAT",6) == 0) //FIXED
        {
            printf("Found a Repeat this!\n"); //EQ, NEQ, GE, LE, G, L
            Compiler->Number_Of_Repeats += 1;
            Compiler->Repeats[Compiler->Number_Of_Repeats-1] = (struct _Label*)calloc(1,sizeof(struct _Label));

            Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Address = Compiler->Current_ROM_Address;
            Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Bank_Location = Compiler->Current_ROM_Bank;
            memset(Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Name,0,16);
            Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Type = 0;
            Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Value = 0;

            size_t length = strcspn(&Buffer[j + 7], " "); //Then TIMES:
            printf("Its length is %zu\n", length); // Outputs 5

            char Variable_To_Repeat[length];

            int m = 0;
            while(m < length)
            {
                Variable_To_Repeat[m] = Buffer[j + 7 + m];
                printf("%c\n", Variable_To_Repeat[m]);
                m += 1;
            }
            Variable_To_Repeat[length] = '\0';

            unsigned short Times_To_Repeat;

            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
               {
                    printf("COMPILATION ABORTED: REPEAT command has no parent Numerical.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name,Variable_To_Repeat,length) == 0)
                {
                    printf("Found its Numerical!\n");
                    Times_To_Repeat = Compiler->Numericals[r]->Value;
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }
            printf("I will repeat this %d times.", Times_To_Repeat);

            int Process = Process_AQ_BASIC_Syntax_REPEAT(Compiler, Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Bank_Location, Compiler->Repeats[Compiler->Number_Of_Repeats-1]->Address, Times_To_Repeat);
        }

        if(strncmp(&Buffer[j], "ENDREP",6) == 0) //FIXED
        {
            printf("Found an END Repeat this!\n"); //EQ, NEQ, GE, LE, G, L
            int Process = Process_AQ_BASIC_Syntax_ENDREP(Compiler);
        }



        if(strncmp(&Buffer[j], "RENLINEAT",9) == 0) //FIXED
        {
            printf("Found an Render Line At Variables!\n");

            size_t xVarlength = strcspn(&Buffer[j + 10], " ");
            printf("Its xVar length is %zu\n", xVarlength); // Outputs 5



            size_t yVarlength = strcspn(&Buffer[j + 10 + xVarlength + 1], " ");
            printf("Its yVar length is %zu\n", yVarlength); // Outputs 5


            size_t xtwoVarlength = strcspn(&Buffer[j + 10 + xVarlength + 1 + yVarlength + 1], " ");
            printf("Its xVartwo length is %zu\n", xtwoVarlength); // Outputs 5

            size_t ytwoVarlength = strcspn(&Buffer[j + 10 + xVarlength + 1 + yVarlength + 1 + xtwoVarlength + 1], " ");
            printf("Its yVartwo length is %zu\n", ytwoVarlength); // Outputs 5



            size_t ColorVarlength = strcspn(&Buffer[j + 10 + xVarlength + 1 + yVarlength + 1 + xtwoVarlength + 1 + ytwoVarlength + 1], ":");
            printf("Its Color length is %zu\n", ColorVarlength); // Outputs 5


            char Render_This[16];
            memset(Render_This,0,16);

            char AtThisX[16];
            memset(AtThisX,0,16);

            int m = 0;
            while(m < xVarlength)
            {
                AtThisX[m] = Buffer[j + 10 + m];
                printf("%c\n", AtThisX[m]);
                m += 1;
            }

            AtThisX[xVarlength] = '\0';
            printf("Render this\n");


            char AtThisY[16];
            memset(AtThisY,0,16);

            m = 0;
            while(m < yVarlength)
            {
                AtThisY[m] = Buffer[j + 10 + m + xVarlength + 1];
                printf("%c\n", AtThisY[m]);
                m += 1;
            }

            AtThisY[yVarlength] = '\0';
            printf("Render this\n");






            char AtThisXtwo[16];
            memset(AtThisXtwo,0,16);

            m = 0;
            while(m < xtwoVarlength)
            {
                AtThisXtwo[m] = Buffer[j + 10 + m + xVarlength + 1 + yVarlength + 1];
                printf("%c\n", AtThisXtwo[m]);
                m += 1;
            }

            AtThisXtwo[xtwoVarlength] = '\0';
            printf("Render this\n");






            char AtThisYtwo[16];
            memset(AtThisYtwo,0,16);

            m = 0;
            while(m < ytwoVarlength)
            {
                AtThisYtwo[m] = Buffer[j + 10 + m + xVarlength + 1 + yVarlength + 1 + xtwoVarlength + 1];
                printf("%c\n", AtThisYtwo[m]);
                m += 1;
            }

            AtThisYtwo[ytwoVarlength] = '\0';
            printf("Render this\n");





            char This_Color[16];
            memset(This_Color,0,16);

            m = 0;
            while(m < ColorVarlength)
            {
                This_Color[m] = Buffer[j + 10 + m + xVarlength + 1 + yVarlength + 1 + xtwoVarlength + 1 + ytwoVarlength + 1];
                printf("%c\n", This_Color[m]);
                m += 1;
            }

            This_Color[ColorVarlength] = '\0';
            printf("Render this\n");



            unsigned short x, y, xTwo, yTwo, Color;

            int r = 0;
            while(r < Compiler->Number_Of_Numericals + 1)
            {
                if(r >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent x var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[r]->Name, AtThisX,xVarlength) == 0)
                {
                    printf("Found its xVar!\n");
                    x = Compiler->Numericals[r]->Value;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[r]->Bank_Location, Compiler->Numericals[r]->Address, RIMGx, RIMGy);
                    r = Compiler->Number_Of_Numericals;
                }
                r+=1;
            }



            int s = 0;
            while(s < Compiler->Number_Of_Numericals + 1)
            {
                if(s >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent y var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[s]->Name, AtThisY,yVarlength) == 0)
                {
                    printf("Found its yVar!\n");
                    y = Compiler->Numericals[s]->Value;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[s]->Bank_Location, Compiler->Numericals[s]->Address, RIMGx, RIMGy);
                    s = Compiler->Number_Of_Numericals;
                }
                s+=1;
            }




            int v = 0;
            while(v < Compiler->Number_Of_Numericals + 1)
            {
                if(v >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent y var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[v]->Name, AtThisXtwo,xtwoVarlength) == 0)
                {
                    printf("Found its xtwo Var!\n");
                    xTwo = Compiler->Numericals[v]->Value;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[v]->Bank_Location, Compiler->Numericals[v]->Address, RIMGx, RIMGy);
                    v = Compiler->Number_Of_Numericals;
                }
                v+=1;
            }








            int w = 0;
            while(w < Compiler->Number_Of_Numericals + 1)
            {
                if(w >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent y var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[w]->Name, AtThisYtwo,ytwoVarlength) == 0)
                {
                    printf("Found its ytwo Var!\n");
                    yTwo = Compiler->Numericals[w]->Value;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[w]->Bank_Location, Compiler->Numericals[w]->Address, RIMGx, RIMGy);
                    w = Compiler->Number_Of_Numericals;
                }
                w+=1;
            }




            int a = 0;
            while(a < Compiler->Number_Of_Numericals + 1)
            {
                if(a >= Compiler->Number_Of_Numericals)
                {
                    printf("COMPILATION ABORTED: RIMG command has no parent y var.");
                    return -1;
                }
                if(strncmp(Compiler->Numericals[a]->Name, This_Color,ColorVarlength) == 0)
                {
                    printf("Found its ytwo Var!\n");
                    Color = Compiler->Numericals[a]->Value;
                   // int x = Process_AQ_BASIC_Syntax_RIMG(Compiler, Compiler->Numericals[a]->Bank_Location, Compiler->Numericals[a]->Address, RIMGx, RIMGy);
                    a = Compiler->Number_Of_Numericals;
                }
                a+=1;
            }

            printf("x %d y %d xT %d yT %d Color %d", x,y,xTwo,yTwo,Color);
            int b = Process_AQ_BASIC_Syntax_RENLINEAT(Compiler, x,y, xTwo, yTwo,Color);

        }
    }



    unsigned char Code[3];
    Code[0]=NEW_OPCODE_HALT;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);

    Load_Image_And_Convert_To_Bitmap("cursor.bmp", "cursor.3bm");
    AQ_Fill_Bank_With_File(Compiler, 2, 0, "cursor.3bm");






    FILE* file = fopen(Output, "wb");
    unsigned char Header[4];
    Header[0] = Compiler->Number_Of_Banks; //Number of Banks
    Header[1] = Main_Bank; //Number of Banks
    Header[2] = Main_Address & 0xFF; //Number of Banks
    Header[3] = (Main_Address >> 8) & 0xFF;

    fwrite(&Header, 1, 4, file);

   // fwrite(&Header, 1, 1, file);
   // fseek(file, 1, SEEK_SET);

    for(unsigned char j = 0; j < Compiler->Number_Of_Banks; j++)
    {
        fwrite(&Compiler->Banks[j], 1, sizeof(struct _SixtyFourK_Bank),file);
    }


    fclose(file);
    printf("Compile complete!\n");
    return 1;
};

int Process_AQ_BASIC_Syntax_IFVAR(struct _AQ_BASIC_Compiler* Compiler, unsigned short Contents_Of_This_RAM_Address, unsigned short Value, unsigned char Label_Bank, unsigned short Label_Address)
{
    uint8_t contentslow_byte = Contents_Of_This_RAM_Address & 0xFF;
    uint8_t contentshigh_byte = (Contents_Of_This_RAM_Address >> 8) & 0xFF;

    uint8_t valuelow_byte = Value & 0xFF;
    uint8_t valuehigh_byte = (Value >> 8) & 0xFF;

    unsigned char LabelBank = Label_Bank;

    uint8_t addrlow_byte = Value & 0xFF;
    uint8_t addrhigh_byte = (Value >> 8) & 0xFF;
    unsigned char Code[3];

    printf("Do the Contents of %d Equal %d\n", Contents_Of_This_RAM_Address, Value);

    Code[0]=NEW_OPCODE_MOV_RZero_ADDR;
    Code[1]=contentshigh_byte;
    Code[2]=contentslow_byte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_MOV_ROne_Value;
    Code[1]=valuehigh_byte;
    Code[2]=valuelow_byte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_CMP_ROne_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


    Code[0]=NEW_OPCODE_JE_BANK_ADDRSIXTEEN;
    Code[1]=LabelBank;
    Code[2]=addrhigh_byte;
    Code[3]=addrlow_byte;
    AQ_BASIC_Insert_Large_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,4,Code);
    Compiler->Current_ROM_Address += 4;

    //printf("Processed IFVAR \n\n\n\n");
    return 1;
};

void AQ_BASIC_Insert_Opcode(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank_ID, unsigned short Offset, unsigned char Size, unsigned char Code[3])
{
    if(Size == 1)
    {
    Compiler->Banks[Bank_ID].Bank[Offset] = Code[0];
    }

    else if(Size == 2)
    {
    Compiler->Banks[Bank_ID].Bank[Offset] = Code[0];
    Compiler->Banks[Bank_ID].Bank[Offset+1] = Code[1];
    }

    else if(Size == 3)
    {
    Compiler->Banks[Bank_ID].Bank[Offset] = Code[0];
    Compiler->Banks[Bank_ID].Bank[Offset+1] = Code[1];
    Compiler->Banks[Bank_ID].Bank[Offset+2] = Code[2];
    }

    else if(Size == 4)
    {
    Compiler->Banks[Bank_ID].Bank[Offset] = Code[0];
    Compiler->Banks[Bank_ID].Bank[Offset+1] = Code[1];
    Compiler->Banks[Bank_ID].Bank[Offset+2] = Code[2];
    Compiler->Banks[Bank_ID].Bank[Offset+3] = Code[3];
    }
};


void AQ_BASIC_Insert_Large_Opcode(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank_ID, unsigned short Offset, unsigned char Size, unsigned char Code[4])
{
    Compiler->Banks[Bank_ID].Bank[Offset] = Code[0];
    Compiler->Banks[Bank_ID].Bank[Offset+1] = Code[1];
    Compiler->Banks[Bank_ID].Bank[Offset+2] = Code[2];
    Compiler->Banks[Bank_ID].Bank[Offset+3] = Code[3];
};

int Process_AQ_BASIC_Syntax_NUMVAR(struct _AQ_BASIC_Compiler* Compiler, unsigned short This_Value, unsigned short This_Address)
{
    uint8_t low_byte = This_Value & 0xFF;
    uint8_t high_byte = (This_Value >> 8) & 0xFF;

    uint8_t Addrlow_byte = This_Address & 0xFF;
    uint8_t Addrhigh_byte = (This_Address >> 8) & 0xFF;

    unsigned char Code[3];

    if(Compiler->Current_ROM_Address + 6 > 65535 || Compiler->Current_ROM_Address > 65535)
    {
        printf("Too many variables\n");
        return -1;

    }

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=high_byte;
    Code[2]=low_byte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_MOV_ADDR_RZero;
    Code[1]=Addrhigh_byte;
    Code[2]=Addrlow_byte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    return 1;
};

int Process_AQ_BASIC_Syntax_GOSUB(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address)
{
    //One More Command: Place a PUSH BANK ADDR SIXTEEN BEFORE THE JUMP
    uint8_t Addrlow_byte = ROM_Address & 0xFF;
    uint8_t Addrhigh_byte = (ROM_Address >> 8) & 0xFF;

    unsigned short Old_Current_ROM_Address = Compiler->Current_ROM_Address;
    Old_Current_ROM_Address += 8; //Go beyond this,

    uint8_t CurrentAddrlow_byte = Old_Current_ROM_Address & 0xFF;
    uint8_t CurrentAddrhigh_byte = (Old_Current_ROM_Address >> 8) & 0xFF;

    unsigned char Code[4];

    unsigned char CodeTwo[3];

    Code[0]=NEW_OPCODE_PUSH_BANK_ADDRSIXTEEN;
    Code[1]=Compiler->Current_ROM_Bank;
    Code[2]=CurrentAddrhigh_byte;
    Code[3]=CurrentAddrlow_byte;
    AQ_BASIC_Insert_Large_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,4,Code);
    Compiler->Current_ROM_Address += 4;

    printf("The Compiler will push Bank %d ADDR %d %d onto the stack.\n", Compiler->Current_ROM_Bank, CurrentAddrhigh_byte, CurrentAddrlow_byte);

    Code[0]=NEW_OPCODE_JMP_BANK_ADDRSIXTEEN;
    Code[1]=ROM_Bank;
    Code[2]=Addrhigh_byte;
    Code[3]=Addrlow_byte;
    AQ_BASIC_Insert_Large_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,4,Code);
    Compiler->Current_ROM_Address += 4;

    return 1;
};
//Fixed
int Process_AQ_BASIC_Syntax_GOTO(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address)
{
    //One More Command: Place a PUSH BANK ADDR SIXTEEN BEFORE THE JUMP
    uint8_t Addrlow_byte = ROM_Address & 0xFF;
    uint8_t Addrhigh_byte = (ROM_Address >> 8) & 0xFF;

    unsigned char Code[4];

    Code[0]=NEW_OPCODE_JMP_BANK_ADDRSIXTEEN;
    Code[1]=ROM_Bank;
    Code[2]=Addrhigh_byte;
    Code[3]=Addrlow_byte;
    AQ_BASIC_Insert_Large_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,4,Code);
    Compiler->Current_ROM_Address += 4;

    return 1;
};


int Process_AQ_BASIC_Syntax_CLS(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address)
{
    unsigned char Code[3];
    Code[0]=NEW_OPCODE_CLEAR_SCREEN;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
};

int Process_AQ_BASIC_Syntax_FLS(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address)
{
    unsigned char Code[3];
    Code[0]=NEW_OPCODE_FLIP_SCREEN;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
};


void Process_AQ_BASIC_Syntax_ADDNUMVAR(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank, unsigned short Address, unsigned short Value)
{

};




int Process_AQ_BASIC_Syntax_RETURN(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address)
{
    unsigned char Code[3];
    Code[0]=NEW_OPCODE_JMP_STACK;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
};






//I want for all variables to be on the same bank.
int Process_AQ_BASIC_Syntax_SETV(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short New_Variable_Value)
{
    unsigned char addrlowbyte, addrhighbyte, vallowbyte, valhighbyte;

    addrlowbyte = Variable_Address & 0xFF;
    addrhighbyte = (Variable_Address >> 8) & 0xFF;

    vallowbyte = New_Variable_Value & 0xFF;
    valhighbyte = (New_Variable_Value >> 8) & 0xFF;

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=valhighbyte;
    Code[2]=vallowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;


    Code[0]=NEW_OPCODE_MOV_ADDR_RZero;
    Code[1]=addrhighbyte;
    Code[2]=addrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;
    return 1;
};



//I want for all variables to be on the same bank.
int Process_AQ_BASIC_Syntax_ADDV(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short New_Variable_Value)
{
    unsigned char addrlowbyte, addrhighbyte, vallowbyte, valhighbyte;

    addrlowbyte = Variable_Address & 0xFF;
    addrhighbyte = (Variable_Address >> 8) & 0xFF;

    vallowbyte = New_Variable_Value & 0xFF;
    valhighbyte = (New_Variable_Value >> 8) & 0xFF;

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_ROne_Value;
    Code[1]=valhighbyte;
    Code[2]=vallowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;


    Code[0]=NEW_OPCODE_MOV_RZero_ADDR;
    Code[1]=addrhighbyte;
    Code[2]=addrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;






    Code[0]=NEW_OPCODE_ADD_RZero_ROne;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;




    Code[0]=NEW_OPCODE_MOV_ADDR_RZero;
    Code[1]=addrhighbyte;
    Code[2]=addrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;
    return 1;
};


int Process_AQ_BASIC_Syntax_RENAT(struct _AQ_BASIC_Compiler* Compiler, unsigned char Image_ROM_Bank, unsigned short Image_ROM_Address, unsigned short xAddr,unsigned short yAddr)
{
    //One of these arguments is incorrect.
    unsigned char xlowbyte, xhighbyte, ylowbyte, yhighbyte, addrlowbyte, addrhighbyte, banklowbyte, bankhighbyte;

    ////printf("Bank: %d Address: %d x: %d y: %d\n", ROM_Bank, ROM_Address, x,y);

    addrlowbyte = Image_ROM_Address & 0xFF;
    addrhighbyte = (Image_ROM_Address >> 8) & 0xFF;

    banklowbyte = Image_ROM_Bank & 0xFF;
    bankhighbyte = (Image_ROM_Bank >> 8) & 0xFF;

    xlowbyte = xAddr & 0xFF;
    xhighbyte = (xAddr >> 8) & 0xFF;

    ylowbyte = yAddr & 0xFF;
    yhighbyte = (yAddr >> 8) & 0xFF;


    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=0;
    Code[2]=Image_ROM_Bank;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;




    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=addrhighbyte;
    Code[2]=addrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_ADDR;
    Code[1]=xhighbyte;
    Code[2]=xlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_ADDR;
    Code[1]=yhighbyte;
    Code[2]=ylowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;



    Code[0]=NEW_OPCODE_RENDER_SPRITE;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
    //Push adddress, x and y
};

int Process_AQ_BASIC_Syntax_REPEAT(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank, unsigned short Address, unsigned short Times_To_Repeat)
{
    Address += 20; //Skip past the REPEAT
    unsigned char addrhighbyte, addrlowbyte, timeshighbyte, timeslowbyte, bankhighbyte, banklowbyte;

    addrhighbyte = Address & 0xFF;
    addrlowbyte = (Address >> 8) & 0xFF;

    timeshighbyte = Times_To_Repeat & 0xFF;
    timeslowbyte = (Times_To_Repeat >> 8) & 0xFF;

    unsigned short Bankshort = (unsigned short)Bank;

    bankhighbyte = Bankshort & 0xFF;
    banklowbyte = (Bankshort >> 8) & 0xFF;

    printf("Repeat instructions at Bank %d Address %d\n", Bank, Address);

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=bankhighbyte;
    Code[2]=banklowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=addrhighbyte;
    Code[2]=addrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=timeshighbyte;
    Code[2]=timeslowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


    Code[0]=NEW_OPCODE_MOV_RZero_Value; //THIS IS HOW MANY TIMES I REPEATED
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;

    return 1;
};

int Process_AQ_BASIC_Syntax_ENDREP(struct _AQ_BASIC_Compiler* Compiler)
{
    unsigned char Code[3];

    Code[0]=NEW_OPCODE_CMP_STACK; //An all-in-one command which checks SP + 5 and 6, sees if they're the same, and if so, JMP STACKS and resets the stack pointer.
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;

    return 1;
};

int Process_AQ_BASIC_Syntax_RENLINEAT(struct _AQ_BASIC_Compiler* Compiler, unsigned short x, unsigned short y, unsigned short xTwo, unsigned short yTwo, unsigned short Color)
{
    //One of these arguments is incorrect.
    unsigned char xlowbyte, xhighbyte, ylowbyte, yhighbyte, xtwolowbyte, xtwohighbyte, ytwolowbyte, ytwohighbyte, colorlowbyte, colorhighbyte;

    ////printf("Bank: %d Address: %d x: %d y: %d\n", ROM_Bank, ROM_Address, x,y);

    xtwolowbyte = xTwo & 0xFF;
    xtwohighbyte = (xTwo >> 8) & 0xFF;

    ytwolowbyte = yTwo & 0xFF;
    ytwohighbyte = (yTwo >> 8) & 0xFF;

    xlowbyte = x & 0xFF;
    xhighbyte = (x >> 8) & 0xFF;

    ylowbyte = y & 0xFF;
    yhighbyte = (y >> 8) & 0xFF;

    colorlowbyte = Color & 0xFF;
    colorhighbyte = (Color >> 8) & 0xFF;

    unsigned char Code[3];

    //Color, x, y, xTwo, yTwo


    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=yhighbyte;
    Code[2]=ylowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=xhighbyte;
    Code[2]=xlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=ytwohighbyte;
    Code[2]=ytwolowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;




    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=xtwohighbyte;
    Code[2]=xtwolowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;



    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=colorhighbyte;
    Code[2]=colorlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;


    Code[0]=NEW_OPCODE_RENDER_LINE;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
    //Push adddress, x and y
};


//FIXED
int Process_AQ_BASIC_Syntax_RIMG(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address, unsigned short x, unsigned short y)
{
    unsigned char xlowbyte, xhighbyte, ylowbyte, yhighbyte, addrlowbyte, addrhighbyte, banklowbyte, bankhighbyte;

    //printf("Bank: %d Address: %d x: %d y: %d\n", ROM_Bank, ROM_Address, x,y);

    addrlowbyte = ROM_Address & 0xFF;
    addrhighbyte = (ROM_Address >> 8) & 0xFF;

    banklowbyte = ROM_Bank & 0xFF;
    bankhighbyte = (ROM_Bank >> 8) & 0xFF;

    xlowbyte = x & 0xFF;
    xhighbyte = (x >> 8) & 0xFF;

    ylowbyte = y & 0xFF;
    yhighbyte = (y >> 8) & 0xFF;


    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=0;
    Code[2]=ROM_Bank;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;




    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=addrhighbyte;
    Code[2]=addrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=xhighbyte;
    Code[2]=xlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=yhighbyte;
    Code[2]=ylowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);

    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);

    Compiler->Current_ROM_Address += 2;



    Code[0]=NEW_OPCODE_RENDER_SPRITE;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
    //Push adddress, x and y
};


//Fixed
int Process_AQ_BASIC_Syntax_RENTILESAT(struct _AQ_BASIC_Compiler* Compiler, unsigned short Tilemap_RAM_Bank, unsigned short Tilemap_RAM_Address, unsigned short Tilemap_Width,unsigned short Tilemap_Height, unsigned short Tile_Width, unsigned short Tile_Height, unsigned short Encoding_RAM_Bank, unsigned short Encoding_RAM_Address, unsigned short Encoding_Start_Location, unsigned short x,unsigned short y,unsigned short xRenderLength,unsigned short yRenderLength)
{
    unsigned char Code[3];

    unsigned char tilemaprambanklowbyte, tilemaprambankhighbyte, tilemapramaddrlowbyte,tilemapramaddrhighbyte, encodingrambankhighbyte, encodingrambanklowbyte, encodingramaddrhighbyte, encodingramaddrlowbyte, encodingstarthighbyte, encodingstartlowbyte, mapwidthhighbyte, mapwidthlowbyte,mapheighthighbyte, mapheightlowbyte,widthhighbyte, widthlowbyte,heighthighbyte, heightlowbyte, widthtorenderhighbyte, widthtorenderlowbyte, heighttorenderhighbyte, heighttorenderlowbyte, xlowbyte, xhighbyte, ylowbyte, yhighbyte;

    tilemaprambanklowbyte = Tilemap_RAM_Bank & 0xFF;
    tilemaprambankhighbyte = (Tilemap_RAM_Bank >> 8) & 0xFF;

    tilemapramaddrlowbyte = Tilemap_RAM_Address & 0xFF;
    tilemapramaddrhighbyte = (Tilemap_RAM_Address >> 8) & 0xFF;

    encodingrambanklowbyte = Encoding_RAM_Bank & 0xFF;
    encodingrambankhighbyte = (Encoding_RAM_Bank >> 8) & 0xFF;

    encodingramaddrlowbyte = Encoding_RAM_Address & 0xFF;
    encodingramaddrhighbyte = (Encoding_RAM_Address >> 8) & 0xFF;

    encodingstartlowbyte = Encoding_Start_Location & 0xFF;
    encodingstarthighbyte = (Encoding_Start_Location >> 8) & 0xFF;

    mapwidthlowbyte = Tilemap_Width & 0xFF;
    mapwidthhighbyte = (Tilemap_Width >> 8) & 0xFF;

    mapheightlowbyte = Tilemap_Height & 0xFF;
    mapheighthighbyte = (Tilemap_Height >> 8) & 0xFF;


    widthlowbyte = Tile_Width & 0xFF;
    widthhighbyte = (Tile_Width >> 8) & 0xFF;


    heightlowbyte = Tile_Height & 0xFF;
    heighthighbyte = (Tile_Height >> 8) & 0xFF;



    widthtorenderlowbyte = xRenderLength & 0xFF;
    widthtorenderhighbyte = (xRenderLength >> 8) & 0xFF;


    heighttorenderlowbyte = yRenderLength & 0xFF;
    heighttorenderhighbyte = (yRenderLength >> 8) & 0xFF;

    xlowbyte = x & 0xFF;
    xhighbyte =(x >> 8) & 0xFF;

    ylowbyte = y & 0xFF;
    yhighbyte = (y >> 8) & 0xFF;

    //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=tilemaprambankhighbyte;
    Code[2]=tilemaprambanklowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;

    //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=tilemapramaddrhighbyte;
    Code[2]=tilemapramaddrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;

    //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=encodingrambankhighbyte;
    Code[2]=encodingrambanklowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;



    //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=encodingramaddrhighbyte;
    Code[2]=encodingramaddrlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=encodingstarthighbyte;
    Code[2]=encodingstartlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;



     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=mapwidthhighbyte;
    Code[2]=mapwidthlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;



     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=mapheighthighbyte;
    Code[2]=mapheightlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;









     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=widthhighbyte;
    Code[2]=widthlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;



     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=heighthighbyte;
    Code[2]=heightlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;













     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=widthtorenderhighbyte;
    Code[2]=widthtorenderlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;



     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=heighttorenderhighbyte;
    Code[2]=heighttorenderlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;



     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=xhighbyte;
    Code[2]=xlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


     //1: PUSH TILEMAP RAM BANK
    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=yhighbyte;
    Code[2]=ylowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;
    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;


    Code[0]=NEW_OPCODE_RENDER_TILEMAP;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;
    return 1;
};

int Process_AQ_BASIC_Syntax_INCONUP(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment)
{
    unsigned char varlowbyte, varhighbyte, incvarlowbyte, incvarhighbyte;

    varlowbyte = Variable_Address & 0xFF;
    varhighbyte = (Variable_Address >> 8) & 0xFF;

    incvarlowbyte = Amount_To_Increment & 0xFF;
    incvarhighbyte = (Amount_To_Increment >> 8) & 0xFF;

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=varhighbyte;
    Code[2]=varlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=incvarhighbyte;
    Code[2]=incvarlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;





    Code[0]=NEW_OPCODE_INC_ON_UP_KEY;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;

    return 1;
}



int Process_AQ_BASIC_Syntax_DECONLEFT(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment)
{
    unsigned char varlowbyte, varhighbyte, incvarlowbyte, incvarhighbyte;

    varlowbyte = Variable_Address & 0xFF;
    varhighbyte = (Variable_Address >> 8) & 0xFF;

    incvarlowbyte = Amount_To_Increment & 0xFF;
    incvarhighbyte = (Amount_To_Increment >> 8) & 0xFF;

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=varhighbyte;
    Code[2]=varlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=incvarhighbyte;
    Code[2]=incvarlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;





    Code[0]=NEW_OPCODE_DEC_ON_LEFT_KEY;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;

    return 1;
}


int Process_AQ_BASIC_Syntax_DECONDOWN(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment)
{
    unsigned char varlowbyte, varhighbyte, incvarlowbyte, incvarhighbyte;

    varlowbyte = Variable_Address & 0xFF;
    varhighbyte = (Variable_Address >> 8) & 0xFF;

    incvarlowbyte = Amount_To_Increment & 0xFF;
    incvarhighbyte = (Amount_To_Increment >> 8) & 0xFF;

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=varhighbyte;
    Code[2]=varlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=incvarhighbyte;
    Code[2]=incvarlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;





    Code[0]=NEW_OPCODE_DEC_ON_DOWN_KEY;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;

    return 1;
}






int Process_AQ_BASIC_Syntax_INCONRIGHT(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment)
{
    unsigned char varlowbyte, varhighbyte, incvarlowbyte, incvarhighbyte;

    varlowbyte = Variable_Address & 0xFF;
    varhighbyte = (Variable_Address >> 8) & 0xFF;

    incvarlowbyte = Amount_To_Increment & 0xFF;
    incvarhighbyte = (Amount_To_Increment >> 8) & 0xFF;

    unsigned char Code[3];

    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=varhighbyte;
    Code[2]=varlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;






    Code[0]=NEW_OPCODE_MOV_RZero_Value;
    Code[1]=incvarhighbyte;
    Code[2]=incvarlowbyte;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,3,Code);
    Compiler->Current_ROM_Address += 3;

    Code[0]=NEW_OPCODE_PUSH_RZero;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,2,Code);
    Compiler->Current_ROM_Address += 2;





    Code[0]=NEW_OPCODE_INC_ON_RIGHT_KEY;
    Code[1]=0;
    Code[2]=0;
    AQ_BASIC_Insert_Opcode(Compiler,Compiler->Current_ROM_Bank,Compiler->Current_ROM_Address,1,Code);
    Compiler->Current_ROM_Address += 1;

    return 1;
}





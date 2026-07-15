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

struct _Aqueous_BASIC_Compiler* Create_Aqueous_BASIC_Compiler()
{
    struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler = (struct _Aqueous_BASIC_Compiler*) calloc(1,sizeof (struct _Aqueous_BASIC_Compiler));
	return Aqueous_BASIC_Compiler;
}

void Initialize_Aqueous_BASIC_Compiler(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler)
{
    Aqueous_BASIC_Compiler->Number_Of_Banks = 2;

    Aqueous_BASIC_Compiler->Start_Bank = 1; //All variables occupy Bank 0.
    Aqueous_BASIC_Compiler->Start_Address = 0;

    Aqueous_BASIC_Compiler->Current_Bank = 1;
    Aqueous_BASIC_Compiler->Current_Address = 0;
}


void Load_N_Encode_Image(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address)
{
    if(This_Bank < Aqueous_BASIC_Compiler->Number_Of_Banks)
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

        if(This_Address + File_Size < 65536)
        {
            for(int j = This_Address; j < 65536; j++)
            {
                //Aqueous_BASIC_Compiler->Banks[This_Bank].
            }
        }
    }
}

void Load_N_Encode_Tilemap(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address)
{
    if(This_Bank < Aqueous_BASIC_Compiler->Number_Of_Banks)
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

        if(This_Address + File_Size < 65535)
        {

        }
    }
}

void Load_N_Encode_Encoding(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address)
{
    if(This_Bank < Aqueous_BASIC_Compiler->Number_Of_Banks)
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

        if(This_Address + File_Size < 65535)
        {

        }
    }
}

void Load_N_Encode_Sound(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address)
{
    if(This_Bank < Aqueous_BASIC_Compiler->Number_Of_Banks)
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

        if(This_Address + File_Size < 65535)
        {

        }
    }
}

void Load_N_Encode_File(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address)
{
    if(This_Bank < Aqueous_BASIC_Compiler->Number_Of_Banks)
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

        if(This_Address + File_Size < 65535)
        {

        }
    }
}

void Compile_Aqueous_BASIC_Source_Code(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address, const char* Output_Filename, unsigned char Main_Bank, unsigned char Main_Address)
{
    FILE* file = fopen(Output_Filename, "wb");
    unsigned char Header[4];
    Header[0] = Aqueous_BASIC_Compiler->Number_Of_Banks; //Number of Banks
    Header[1] = Main_Bank; //Number of Banks
    Header[2] = Main_Address & 0xFF; //Number of Banks
    Header[3] = (Main_Address >> 8) & 0xFF;

    fwrite(&Header, 1, 4, file);
    for(unsigned char j = 0; j < Aqueous_BASIC_Compiler->Number_Of_Banks; j++)
    {
        fwrite(&Aqueous_BASIC_Compiler->Banks[j], 1, sizeof(struct _SixtyFourK_Bank),file);
    }
    fclose(file);
    printf("Compile complete!\n");
}






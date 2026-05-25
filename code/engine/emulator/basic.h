/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BASIC_H_INCLUDED
#define BASIC_H_INCLUDED
#include "emulator.h"

struct _Encodable_Image
{
    unsigned char* Image_Buffer;
};

struct _Encodable_Tilemap
{
    unsigned char* Tilemap_Buffer;
};

struct _Encodable_Encoding
{
    unsigned short* Encoding_Buffer;
};

struct _Encodable_Var
{
    unsigned short Value;
};

struct _Encodable_Array
{
    unsigned short Array_Length;
};


struct _Aqueous_BASIC_Compiler
{
    unsigned char Number_Of_Banks;

    unsigned char Start_Bank;
    unsigned short Start_Address;

    unsigned char Current_Bank;
    unsigned short Current_Address;

    unsigned short Number_Of_Images;
    struct _Encodable_Image* Images_To_Encode[65536];

    unsigned short Number_Of_Tilemaps;
    struct _Encodable_Tilemap* Tilemaps_To_Encode[65536];

    unsigned short Number_Of_Encodings;
    struct _Encodable_Encoding* Encodings_To_Encode[65536];

    unsigned short Number_Of_Vars;
    struct _Encodable_Var* Vars_To_Encode[65536];

    unsigned short Number_Of_Arrays;
    struct _Encodable_Array* Arrays_To_Encode[65536];

    struct _SixtyFourK_Bank Banks[256];
};

struct _Aqueous_BASIC_Compiler* Create_Aqueous_BASIC_Compiler();
void Initialize_Aqueous_BASIC_Compiler(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler);

void Load_N_Encode_Image(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address);
void Load_N_Encode_Tilemap(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address);
void Load_N_Encode_Encoding(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address);
void Load_N_Encode_Sound(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address);
void Load_N_Encode_File(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address);

void Compile_Aqueous_BASIC_Source_Code(struct _Aqueous_BASIC_Compiler* Aqueous_BASIC_Compiler, const char* Filename, unsigned char This_Bank, unsigned short This_Address, const char* Output_Filename, unsigned char Main_Bank, unsigned char Main_Address);

#endif // BASIC_H_INCLUDED

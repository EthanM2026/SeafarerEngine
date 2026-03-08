/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BASIC_H_INCLUDED
#define BASIC_H_INCLUDED
#include "emulator.h"

//ALl Ram is 128KB, Banks 0 and 1
//Programs are loaded into Bank 2

struct _String
{
    unsigned short Values[6*256];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Type; //CONST, Variable, float Const, float Variable
};

struct AQ_Encoding
{
    char Name[16];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Length;
    unsigned char Type; //CONST, Variable, float Const, float Variable
    bool This_Is_Contents_Of_RAM_Address;

};

struct AQ_Tilemap
{
    char Name[16];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Length;
    unsigned char Type; //CONST, Variable, float Const, float Variable
    bool This_Is_Contents_Of_RAM_Address;

   // W208 H144 I8 E16:

   unsigned short Tile_Width;
    unsigned short Tile_Height;

    unsigned short Width;
    unsigned short Height;
    unsigned char Color_Mode;
};

struct AQ_Image
{
    char Name[16];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Length;
    unsigned char Type; //CONST, Variable, float Const, float Variable
    bool This_Is_Contents_Of_RAM_Address;

   // B0 A0 W12 H24 C1:

    unsigned short Width;
    unsigned short Height;
    unsigned char Color_Mode;
};

struct _Numerical
{
    char Name[16];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Length;
    unsigned char Type; //CONST, Variable, float Const, float Variable
    bool This_Is_Contents_Of_RAM_Address;
};

struct _Equation //2 Ram Addresses, operation and result
{
    char Name[16];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Type; //CONST, Variable, float Const, float Variable
};

struct _Label
{
    char Name[16];
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;
    unsigned char Type;
};

struct _If
{
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;

    unsigned char If_This;

    unsigned char Bank_Location_To_Go_To;
    unsigned short Address_To_Go_To;
    unsigned short Value_Two;
};

struct _For
{
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;

    unsigned char For_This;

    unsigned char Other_Bank_Location;
    unsigned short Other_Address;
    unsigned short Other_Value;

    unsigned short For_Addition_Value;

    unsigned char Bank_Location_To_Go_To;
    unsigned short Address_To_Go_To;
    unsigned short Value_Two;
};

struct _While
{
    unsigned char Bank_Location;
    unsigned short Address;
    unsigned short Value;

    unsigned char While_This;

    unsigned char Bank_Location_To_Go_To;
    unsigned short Address_To_Go_To;
    unsigned short Value_Two;
};

struct _AQ_BASIC_Compiler
{
    unsigned short* Current_RAM_Counter;
    unsigned short* Current_ROM_Counter;

    unsigned char Current_ROM_Bank;
    unsigned char Current_ROM_Address;

    unsigned char Current_RAM_Bank;
    unsigned char Current_RAM_Address;

    unsigned int Number_Of_RAM_Counters;
    unsigned int Number_Of_ROM_Counters;

    unsigned int Number_Of_Numericals;
    unsigned int Number_Of_Equations;
    unsigned int Number_Of_Strings;
    unsigned int Number_Of_Labels;
    unsigned int Number_Of_Repeats;

    unsigned int Number_Of_Images;
    unsigned int Number_Of_Tilemaps;
    unsigned int Number_Of_Encodings;

    unsigned char Number_Of_Banks;
    unsigned char Start_Bank;

    unsigned int Number_Of_Ifs;
    unsigned int Number_Of_Fors;
    unsigned int Number_Of_Whiles;

    struct _Numerical* Numericals[8192];
    struct _Equation* Equations[8192];
    struct _String* Strings[8192];
    struct _Label* Labels[8192];
    struct _Label* Repeats[8192];

    struct _If* Ifs[8192];
    struct _For* Fors[8192];
    struct _While* Whiles[8192];
    struct AQ_Image* Images[8192];
    struct AQ_Tilemap* Tilemaps[8192];
    struct AQ_Encoding* Encodings[8192];


    struct _SixtyFourK_Bank Banks[256];

    unsigned char Current_Writing_Bank;
    unsigned short Current_Bank_Data_Offset;
};

struct _AQ_BASIC_Compiler* Create_AQ_BASIC_Compiler();
void Initialize_AQ_BASIC_Compiler(struct _AQ_BASIC_Compiler* Compiler);
int Compile_This_Text(struct _AQ_BASIC_Compiler* Compiler, const char* Filename, const char* Output);

void AQ_BASIC_Insert_Opcode(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank_ID, unsigned short Offset, unsigned char Size, unsigned char Code[3]);
int Process_AQ_BASIC_Syntax_NUMVAR(struct _AQ_BASIC_Compiler* Compiler, unsigned short This_Value, unsigned short This_Address);

int Process_AQ_BASIC_Syntax_GOTO(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address);

int Process_AQ_BASIC_Syntax_CLS(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address);

int Process_AQ_BASIC_Syntax_FLS(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address);

int Process_AQ_BASIC_Syntax_RIMG(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address, unsigned short x, unsigned short y);

void AQ_Fill_Bank_With_File(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank_ID, unsigned short Offset, const char* Filename);


int Process_AQ_BASIC_Syntax_SETV(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short New_Variable_Value);
int Process_AQ_BASIC_Syntax_ADDV(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short New_Variable_Value);

int Process_AQ_BASIC_Syntax_RENAT(struct _AQ_BASIC_Compiler* Compiler, unsigned char Image_ROM_Bank, unsigned short Image_ROM_Address, unsigned short xAddr,unsigned short yAddr);

int Process_AQ_BASIC_Syntax_RENTILESAT(struct _AQ_BASIC_Compiler* Compiler, unsigned short Tilemap_RAM_Bank, unsigned short Tilemap_RAM_Address, unsigned short Tilemap_Width,unsigned short Tilemap_Height, unsigned short Tile_Width, unsigned short Tile_Height, unsigned short Encoding_RAM_Bank, unsigned short Encoding_RAM_Address, unsigned short Encoding_Start_Location, unsigned short x,unsigned short y,unsigned short xRenderLength,unsigned short yRenderLength);
int Process_AQ_BASIC_Syntax_IFVAR(struct _AQ_BASIC_Compiler* Compiler, unsigned short Contents_Of_This_RAM_Address, unsigned short Value, unsigned char Label_Bank, unsigned short Label_Address);
void AQ_BASIC_Insert_Large_Opcode(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank_ID, unsigned short Offset, unsigned char Size, unsigned char Code[4]);
int Process_AQ_BASIC_Syntax_RETURN(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address);
int Process_AQ_BASIC_Syntax_GOSUB(struct _AQ_BASIC_Compiler* Compiler, unsigned char ROM_Bank, unsigned short ROM_Address);
int Process_AQ_BASIC_Syntax_RENLINEAT(struct _AQ_BASIC_Compiler* Compiler, unsigned short x, unsigned short y, unsigned short xTwo, unsigned short yTwo, unsigned short Color);

int Process_AQ_BASIC_Syntax_REPEAT(struct _AQ_BASIC_Compiler* Compiler, unsigned char Bank, unsigned short Address, unsigned short Times_To_Repeat);

int Process_AQ_BASIC_Syntax_ENDREP(struct _AQ_BASIC_Compiler* Compiler);

int Process_AQ_BASIC_Syntax_INCONUP(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment);
int Process_AQ_BASIC_Syntax_DECONDOWN(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment);
int Process_AQ_BASIC_Syntax_DECONLEFT(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment);
int Process_AQ_BASIC_Syntax_INCONRIGHT(struct _AQ_BASIC_Compiler* Compiler, unsigned short Variable_Address, unsigned short Amount_To_Increment);
#endif // BASIC_H_INCLUDED

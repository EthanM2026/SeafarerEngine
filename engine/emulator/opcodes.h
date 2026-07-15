/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>





















struct _Codepage
{
    unsigned char Data[8192]; //256 x 256, 2 colors.
};

// --- Configuration ---
struct _Symbol
{
    char name[32];
    int address;
};

struct _Compiler
{
    struct _Symbol symbol_table[256];
    int symbol_count = 0;
};



void add_symbol(struct _Compiler* Compiler, char *name, int addr);
int get_symbol(struct _Compiler* Compiler, char *name);
void write16(FILE *f, uint16_t val);
void process(struct _Compiler* Compiler, FILE *src, FILE *dest, int pass);
void Compile_Source_Code(const char* In, const char* Out);




#endif // OPCODES_H_INCLUDED

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

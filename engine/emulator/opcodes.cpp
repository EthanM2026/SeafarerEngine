/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "opcodes.h"


// --- Helpers ---
void add_symbol(struct _Compiler* Compiler, char *name, int addr) {
    strcpy(Compiler->symbol_table[Compiler->symbol_count].name, name);
    Compiler->symbol_table[Compiler->symbol_count++].address = addr;
};

int get_symbol(struct _Compiler* Compiler, char *name) {
    for (int i = 0; i < Compiler->symbol_count; i++)
        if (strcmp(Compiler->symbol_table[i].name, name) == 0) return Compiler->symbol_table[i].address;
    return -1;
};

void write16(FILE *f, uint16_t val) {
    uint8_t b[2];

    b[0]=val & 0xFF;
    b[1]=(val >> 8) & 0xFF;

    fwrite(b, 1, 2, f);
};

// --- The Core Engine ---
void process(struct _Compiler* Compiler, FILE *src, FILE *dest, int pass) {

};


void Compile_Source_Code(const char* In, const char* Out)
{
    struct _Compiler* Compiler = (struct _Compiler*)calloc(1,sizeof(struct _Compiler));
    FILE *s = fopen(In, "r");
    FILE *d = fopen(Out, "wb");

    process(Compiler, s, NULL, 1); // Pass 1: Symbols
    process(Compiler, s, d, 2);    // Pass 2: Binary
    fclose(s);
    fclose(d);
};



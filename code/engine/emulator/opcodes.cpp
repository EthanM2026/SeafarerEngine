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



#include "text.h"

struct _Text_Engine* Create_Text_Engine()
{
    struct _Text_Engine* I = (struct _Text_Engine*) calloc(1,sizeof (struct _Text_Engine));
    return I;
};
void Initialize_Text_Engine(struct _Engine* Engine)
{
    Engine->Text_Engine->Default_Codepage = Create_Image();
    Load_Image(Engine->Text_Engine->Default_Codepage, "resources/artwork/gui/font_w_16_48_2.bmp");

};

void Write_Language(const char* Filename)
{
    struct _Language_Header Header;
    memset(Header.Name,0,256);
    sprintf(Header.Name, "English(USA)");
    Header.Name[12] = '\0';

    Header.ID = ID_LANGUAGE_ENGLISH;

    Header.Number_Of_Encodings = 53+26+14;
    Header.Number_Of_Codepages_Required = 1;
    Header.Codepage_Width = 416;
    Header.Codepage_Height = 288;
    Header.Character_Width = 16;
    Header.Character_Height = 32;

    memset(Header.Encoding_File,0,256);
    sprintf(Header.Encoding_File, "resources/data/languages/english_usa.enc");
    Header.Encoding_File[40] = '\0';

    struct _Filepath Codepage_One_Filepath;
    memset(Codepage_One_Filepath.Filepath,0,256);
    sprintf(Codepage_One_Filepath.Filepath, "resources/artwork/gui/font_w_16_48_2.bmp");
    Codepage_One_Filepath.Filepath[40] = '\0';

    FILE* f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _Language_Header), f);
    fwrite(&Codepage_One_Filepath,1,sizeof(struct _Filepath),f);
    fclose(f);
}

void Load_Language(struct _Text_Engine* Text_Engine, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

    for(int j = 0; j < 256; j++)
    {
        Text_Engine->Language.Name[j] = Buffer[Buffer_Location+j];
        Text_Engine->Language.Encoding_File[j] = Buffer[Buffer_Location+j+256];
    }

    Text_Engine->Language.ID = *(int*)&(Buffer[Buffer_Location+512]);
    Text_Engine->Language.Number_Of_Encodings = *(int*)&(Buffer[Buffer_Location+516]);
    Text_Engine->Language.Number_Of_Codepages_Required = *(int*)&(Buffer[Buffer_Location+520]);

    Text_Engine->Language.Codepage_Width = *(int*)&(Buffer[Buffer_Location+524]);
    Text_Engine->Language.Codepage_Height = *(int*)&(Buffer[Buffer_Location+528]);

    Text_Engine->Language.Character_Width = *(int*)&(Buffer[Buffer_Location+532]);
    Text_Engine->Language.Character_Height = *(int*)&(Buffer[Buffer_Location+536]);

    Buffer_Location = sizeof(struct _Language_Header); //Now, look for the codepage filepaths.

    Text_Engine->Codepages = (struct _Image*)calloc(Text_Engine->Language.Number_Of_Codepages_Required, sizeof(struct _Image));

    printf("Language info\n");
    printf(Text_Engine->Language.Name);
    printf("\n");
    printf(Text_Engine->Language.Encoding_File);

    Text_Engine->Encodings = (struct _Language_Encoding*)calloc(Text_Engine->Language.Number_Of_Encodings, sizeof(_Language_Encoding));

    for(int j = 0; j < Text_Engine->Language.Number_Of_Codepages_Required; j++)
    {
        Buffer_Location = sizeof(struct _Language_Header) + (256 * j);
        char Filepath[256];

        for(int l = 0; l < 256; l++)
        {
            Filepath[l] = Buffer[Buffer_Location+l];
        }
        printf("Need to Load ");
        printf(Filepath);
        Load_Image(&Text_Engine->Codepages[j], Filepath);
    }
    Load_Encoding(Text_Engine, Text_Engine->Language.Encoding_File);
}

void Load_Encoding(struct _Text_Engine* Text_Engine, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;
    printf("There are %d\n encodings.\n", Text_Engine->Language.Number_Of_Encodings);
    for(int j = 0; j < Text_Engine->Language.Number_Of_Encodings; j++)
    {
        Buffer_Location = (sizeof(struct _Language_Encoding) * j);
        Text_Engine->Encodings[j].Unicode_Character = *(int*)&(Buffer[Buffer_Location]);
        Text_Engine->Encodings[j].Codepage = *(int*)&(Buffer[Buffer_Location+4]);
        Text_Engine->Encodings[j].x = *(int*)&(Buffer[Buffer_Location+8]);
        Text_Engine->Encodings[j].y = *(int*)&(Buffer[Buffer_Location+12]);
        Text_Engine->Encodings[j].Width = *(int*)&(Buffer[Buffer_Location+16]);
        Text_Engine->Encodings[j].Height = *(int*)&(Buffer[Buffer_Location+20]);

        printf("Unicode Character: ");
        printf("%c",Text_Engine->Encodings[j].Unicode_Character);
        printf("\n");
    }
}

void Add_Encoding(FILE * f, int Unicode_Character,
    int Codepage,
    int x,
    int y,
    int Width,
    int Height)
{
    struct _Language_Encoding Encoding;
    Encoding.Unicode_Character = Unicode_Character;
    Encoding.Codepage = Codepage;
    Encoding.x = x;
    Encoding.y = y;
    Encoding.Width = Width;
    Encoding.Height = Height;

    fwrite(&Encoding, 1, sizeof(struct _Language_Encoding), f);

}

void Create_English_Encoding_File()
{
    FILE* f = fopen("resources/data/languages/english_usa.enc", "wb");
    Add_Encoding(f, '~',-1,16*0,0,16,32);
    Add_Encoding(f, '`',-2,16*0,0,16,32);
    Add_Encoding(f, 'A',0,16*0,0,16,32);
    Add_Encoding(f, 'B',0,16*1,0,16,32);
    Add_Encoding(f, 'C',0,16*2,0,16,32);
    Add_Encoding(f, 'D',0,16*3,0,16,32);
    Add_Encoding(f, 'E',0,16*4,0,16,32);
    Add_Encoding(f, 'F',0,16*5,0,16,32);
    Add_Encoding(f, 'G',0,16*6,0,16,32);
    Add_Encoding(f, 'H',0,16*7,0,16,32);
    Add_Encoding(f, 'I',0,16*8,0,16,32);
    Add_Encoding(f, 'J',0,16*9,0,16,32);
    Add_Encoding(f, 'K',0,16*10,0,16,32);
    Add_Encoding(f, 'L',0,16*11,0,16,32);
    Add_Encoding(f, 'M',0,16*12,0,16,32);
    Add_Encoding(f, 'N',0,16*13,0,16,32);
    Add_Encoding(f, 'O',0,16*14,0,16,32);
    Add_Encoding(f, 'P',0,16*15,0,16,32);
    Add_Encoding(f, 'Q',0,16*16,0,16,32);
    Add_Encoding(f, 'R',0,16*17,0,16,32);
    Add_Encoding(f, 'S',0,16*18,0,16,32);
    Add_Encoding(f, 'T',0,16*19,0,16,32);
    Add_Encoding(f, 'U',0,16*20,0,16,32);
    Add_Encoding(f, 'V',0,16*21,0,16,32);
    Add_Encoding(f, 'W',0,16*22,0,16,32);
    Add_Encoding(f, 'X',0,16*23,0,16,32);
    Add_Encoding(f, 'Y',0,16*24,0,16,32);
    Add_Encoding(f, 'Z',0,16*25,0,16,32);


    Add_Encoding(f, 'a',0,16*0,32,16,32);
    Add_Encoding(f, 'b',0,16*1,32,16,32);
    Add_Encoding(f, 'c',0,16*2,32,16,32);
    Add_Encoding(f, 'd',0,16*3,32,16,32);
    Add_Encoding(f, 'e',0,16*4,32,16,32);
    Add_Encoding(f, 'f',0,16*5,32,16,32);
    Add_Encoding(f, 'g',0,16*6,32,16,32);
    Add_Encoding(f, 'h',0,16*7,32,16,32);
    Add_Encoding(f, 'i',0,16*8,32,16,32);
    Add_Encoding(f, 'j',0,16*9,32,16,32);
    Add_Encoding(f, 'k',0,16*10,32,16,32);
    Add_Encoding(f, 'l',0,16*11,32,16,32);
    Add_Encoding(f, 'm',0,16*12,32,16,32);
    Add_Encoding(f, 'n',0,16*13,32,16,32);
    Add_Encoding(f, 'o',0,16*14,32,16,32);
    Add_Encoding(f, 'p',0,16*15,32,16,32);
    Add_Encoding(f, 'q',0,16*16,32,16,32);
    Add_Encoding(f, 'r',0,16*17,32,16,32);
    Add_Encoding(f, 's',0,16*18,32,16,32);
    Add_Encoding(f, 't',0,16*19,32,16,32);
    Add_Encoding(f, 'u',0,16*20,32,16,32);
    Add_Encoding(f, 'v',0,16*21,32,16,32);
    Add_Encoding(f, 'w',0,16*22,32,16,32);
    Add_Encoding(f, 'x',0,16*23,32,16,32);
    Add_Encoding(f, 'y',0,16*24,32,16,32);
    Add_Encoding(f, 'z',0,16*25,32,16,32);










    Add_Encoding(f, '1',0,16*0,64,16,32);
    Add_Encoding(f, '2',0,16*1,64,16,32);
    Add_Encoding(f, '3',0,16*2,64,16,32);
    Add_Encoding(f, '4',0,16*3,64,16,32);
    Add_Encoding(f, '5',0,16*4,64,16,32);
    Add_Encoding(f, '6',0,16*5,64,16,32);
    Add_Encoding(f, '7',0,16*6,64,16,32);
    Add_Encoding(f, '8',0,16*7,64,16,32);
    Add_Encoding(f, '9',0,16*8,64,16,32);
    Add_Encoding(f, '0',0,16*9,64,16,32);
    Add_Encoding(f, '!',0,16*10,64,16,32);
    Add_Encoding(f, '@',0,16*11,64,16,32);
    Add_Encoding(f, '#',0,16*12,64,16,32);
    Add_Encoding(f, '$',0,16*13,64,16,32);
    Add_Encoding(f, '%',0,16*14,64,16,32);
    Add_Encoding(f, '^',0,16*15,64,16,32);
    Add_Encoding(f, '&',0,16*16,64,16,32);
    Add_Encoding(f, '*',0,16*17,64,16,32);
    Add_Encoding(f, '(',0,16*18,64,16,32);
    Add_Encoding(f, ')',0,16*19,64,16,32);
    Add_Encoding(f, '_',0,16*20,64,16,32);
    Add_Encoding(f, '-',0,16*21,64,16,32);
    Add_Encoding(f, '+',0,16*22,64,16,32);
    Add_Encoding(f, '=',0,16*23,64,16,32);
    Add_Encoding(f, '|',0,16*24,64,16,32);
    Add_Encoding(f, 92,0,16*25,64,16,32);












    Add_Encoding(f, '{',0,16*0,96,16,32);
    Add_Encoding(f, '[',0,16*1,96,16,32);
    Add_Encoding(f, '}',0,16*2,96,16,32);
    Add_Encoding(f, ']',0,16*3,96,16,32);
    Add_Encoding(f, ':',0,16*4,96,16,32);
    Add_Encoding(f, ';',0,16*5,96,16,32);
    Add_Encoding(f, '"',0,16*6,96,16,32);
    Add_Encoding(f, 39,0,16*7,96,16,32); //
    Add_Encoding(f, '<',0,16*8,96,16,32);
    Add_Encoding(f, ',',0,16*9,96,16,32);
    Add_Encoding(f, '>',0,16*10,96,16,32);
    Add_Encoding(f, '.',0,16*11,96,16,32);
    Add_Encoding(f, '?',0,16*12,96,16,32);
    Add_Encoding(f, '/',0,16*13,96,16,32);
    //Add_Encoding(f,"\",0,16*25,64,16,32);
    fclose(f);
}

struct _Image* Create_Text_Image(struct _Text_Engine* Text_Engine, char Text[256], int Text_Length, int Width, int Height)
{
    struct _Image* Image = (struct _Image*) calloc(1,sizeof (struct _Image));
    Image->Width = Width;
    Image->Height = Height;
    int RGBA_Image_Size = ((Image->Width) * (Image->Height) * 4);
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);

    int Start_X = 0;
    int Start_Y = 0;

    int Current_X = 0;
    int Current_Y = 0;
    for(int j = 0; j < Text_Length; j++)
    {
        switch(Text[j])
        {
            case '~':
            Current_Y += 24;
            Current_X = Start_X;
            break;

            case ' ':
            Current_X += 17;
            break;

            case 'A':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,0,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'B':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,16,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'C':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,32,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'D':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,48,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'E':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,64,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'F':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,80,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'G':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,96,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'H':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,112,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'I':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,128,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'J':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,144,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'K':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,160,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'L':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,176,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'M':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,192,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'N':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,208,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'O':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,224,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'P':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,240,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'Q':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,256,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'R':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,272,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'S':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,288,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'T':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,304,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'U':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,320,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'V':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,336,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'W':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,352,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'X':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,368,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'Y':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,384,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'Z':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,400,0,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;


            case 'a':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,0,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'b':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,16,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'c':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,32,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'd':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,48,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'e':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,64,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'f':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,80,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'g':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,96,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'h':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,112,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'i':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,128,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'j':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,144,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'k':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,160,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'l':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,176,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'm':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,192,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'n':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,208,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'o':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,224,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'p':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,240,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'q':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,256,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'r':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,272,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 's':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,288,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 't':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,304,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'u':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,320,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'v':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,336,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'w':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,352,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'x':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,368,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'y':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,384,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case 'z':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,400,16,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;





            case '0':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,0,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '1':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,16,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '2':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,32,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '3':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,48,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '4':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,64,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '5':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,80,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '6':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,96,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '7':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,112,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '8':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,128,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '9':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,144,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '.':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,160,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '!':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,176,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '?':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,192,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case ',':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,208,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '"':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,224,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case ':':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,240,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case ';':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,256,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '_':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,272,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '+':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,288,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '-':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,304,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '*':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,320,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '/':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,336,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '(':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,352,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case ')':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,368,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '[':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,384,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case ']':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,400,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;
        }
    }

    //Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,64,16,0,0,0,0,16,16);


    glGenTextures(1, &Image->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return Image;
};


void Create_Encoded_Text(struct _Text_Engine* Text_Engine, const char* Input_Filepath, const char* Filepath)
{
    FILE* fOne;
    fOne = fopen(Input_Filepath, "rb");
    int File_Size;
    fseek(fOne, 0, SEEK_END);
    File_Size = ftell(fOne);
    rewind(fOne);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, fOne);
    fclose(fOne);
    int Buffer_Location = 0;


    int This_Character[6];
    FILE* f = fopen(Filepath, "wb");
    for(int j = 0; j < File_Size; j++)
    {
        for(int l = 0; l < Text_Engine->Language.Number_Of_Encodings; l++)
        {
            if(Buffer[j] == Text_Engine->Encodings[l].Unicode_Character)
            {
                This_Character[0] = Text_Engine->Encodings[l].Unicode_Character;
                This_Character[1] = Text_Engine->Encodings[l].Codepage;
                printf("Character's Codepage, %d\n", This_Character[1]);
                This_Character[2] = Text_Engine->Encodings[l].x;
                This_Character[3] = Text_Engine->Encodings[l].y;
                This_Character[4] = Text_Engine->Encodings[l].Width;
                This_Character[5] = Text_Engine->Encodings[l].Height;
                fwrite(&This_Character,1,6*sizeof(int),f);
            }

           // else if(Text[j] == ' ')
            {
                //This_Character[0] = ' ';
               // This_Character[1] = 0;
               // printf("Character's Codepage, %d\n", This_Character[1]);
               // This_Character[2] = 0;
               // This_Character[3] = 0;
               // This_Character[4] = 16;
               // This_Character[5] = 32;
               // fwrite(&This_Character,1,6*sizeof(int),f);
            }

            //else if(Text[j] == ' ')
           // {
             //   This_Character[0] = 0;
             //   This_Character[1] = 0;
             ///   This_Character[2] = -1;
             //   This_Character[3] = 0;
             //   This_Character[4] = 0;
             //   This_Character[5] = 0;
             //   fwrite(&This_Character,1,6*sizeof(int),f);
            //}
        }
    }
    fclose(f);
}





void Initialize_Text_Image_From_Encoded_Text(struct _Text_Engine* Text_Engine, struct _Image* Image, int Text_Length, int Height, const char* Filename)
{
    FILE* f;
    f = fopen(Filename, "rb");
    int File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(Buffer, 1, File_Size, f);
    fclose(f);
    int Buffer_Location = 0;

    //Encodings are just Codepage, x, y, Width, Height.
    int This_Character[6];


    Image->Width = (Text_Length*(Text_Engine->Language.Character_Width))+1;
    printf("Width: %d\n", Image->Width);
    Image->Height = Height;
    int RGBA_Image_Size = ((Image->Width) * (Image->Height) * 4);
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);

    int Start_X = 0;
    int Start_Y = 0;

    int Current_X = 0;
    int Current_Y = 0;

    printf("Need to decode %d Characters\n", Text_Length);

    for(int j = 0; j < Text_Length-1; j++)
    {
        printf("Decoding \n");
        Buffer_Location = 24 * j;
        This_Character[0] = *(int*)&(Buffer[Buffer_Location]);
        This_Character[1] = *(int*)&(Buffer[Buffer_Location+4]);
        This_Character[2] = *(int*)&(Buffer[Buffer_Location+8]);
        This_Character[3] = *(int*)&(Buffer[Buffer_Location+12]);
        This_Character[4] = *(int*)&(Buffer[Buffer_Location+16]);
        This_Character[5] = *(int*)&(Buffer[Buffer_Location+20]);

        printf("Info: %c\n", This_Character[0]);

        if(This_Character[0] == '~')
        {
            Current_X += Text_Engine->Language.Character_Width;
            printf("Encounted space.\n");
        }

        else if(This_Character[0] == '`')
        {
            Current_Y += Text_Engine->Language.Character_Height;
            Current_X = 0;
            printf("Encounted newline.\n");
        }

        else if(This_Character[0] != '`' && This_Character[0] != '~')
        {
            if(This_Character[2] < 0) //For Newline, bold italic underline and scripts.
            {

            }

            else if(This_Character[2] >= 0) //For Newline, bold italic underline and scripts.
            {
                Copy_Section(4,Text_Engine->Codepages[This_Character[1]].RGBA_Canvas,Image->RGBA_Canvas,Text_Engine->Language.Codepage_Width,Text_Engine->Language.Codepage_Height,Image->Width,Image->Height,This_Character[2],This_Character[3],Current_X,Current_Y,This_Character[4],This_Character[5]);
                Current_X += Text_Engine->Language.Character_Width;
                printf("Decoded: %c\n", This_Character[0]);
            }
        }

        //Buffer_Location += 24;
    }

    //Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,64,16,0,0,0,0,16,16);


    // Generate a texture ID
    glGenTextures(1, &Image->Texture_ID);
    // Bind the texture to GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);

    // Set texture parameters
    // GL_LINEAR for smooth scaling, GL_NEAREST for pixelated look
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Load the image data into the texture
    // GL_BGR is used because BMP stores pixels in BGR order
    // GL_UNSIGNED_BYTE indicates each color component is an unsigned byte
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->Width, Image->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
}




void Initialize_Text_Image(struct _Text_Engine* Text_Engine, struct _Image* Image, char Text[256], int Text_Length, int Width, int Height)
{
    Image->Width = Width;
    Image->Height = Height;
    int RGBA_Image_Size = ((Image->Width) * (Image->Height) * 4);
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);

    int Start_X = 0;
    int Start_Y = 0;

    int Current_X = 0;
    int Current_Y = 0;
    for(int j = 0; j < Text_Length; j++)
    {
        switch(Text[j])
        {
            case '~':
            Current_Y += 32;
            Current_X = Start_X;
            break;

            case ' ':
            Current_X += 17;
            break;

            case 'A':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,0,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;

            case 'B':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,16,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;





            case 'C':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,32,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'D':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,48,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'E':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,64,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;







            case 'F':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,80,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'G':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,96,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'H':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,112,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'I':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,128,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'J':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,144,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'K':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,160,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'L':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,176,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'M':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,192,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'N':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,208,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;







            case 'O':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,224,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'P':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,240,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'Q':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,256,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'R':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,272,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'S':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,288,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'T':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,304,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'U':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,320,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'V':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,336,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'W':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,352,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;


            case 'X':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,368,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'Y':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,384,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;
            case 'Z':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,288,Width,Height,400,0,Current_X,Current_Y,16,32);
            Current_X += 17;
            break;

        }
    }

    //Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,64,16,0,0,0,0,16,16);


    // Generate a texture ID
    glGenTextures(1, &Image->Texture_ID);
    // Bind the texture to GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);

    // Set texture parameters
    // GL_LINEAR for smooth scaling, GL_NEAREST for pixelated look
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Load the image data into the texture
    // GL_BGR is used because BMP stores pixels in BGR order
    // GL_UNSIGNED_BYTE indicates each color component is an unsigned byte
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->Width, Image->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);

};




































void Initialize_Number_Image(struct _Text_Engine* Text_Engine, struct _Image* Image, float Number, int Text_Length, int Width, int Height)
{
    Image->Width = Width;
    Image->Height = Height;
    int RGBA_Image_Size = ((Image->Width) * (Image->Height) * 4);
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);

    int Start_X = 0;
    int Start_Y = 0;

    int Current_X = 0;
    int Current_Y = 0;

    char Text[256];

    sprintf(Text, "%f", Number);

    for(int j = 0; j < Text_Length; j++)
    {
        switch(Text[j])
        {
            case '0':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,0,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '1':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,16,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '2':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,32,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '3':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,48,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '4':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,64,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '5':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,80,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '6':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,96,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '7':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,112,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '8':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,128,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;

            case '9':
            Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,Width,Height,144,32,Current_X,Current_Y,16,16);
            Current_X += 17;
            break;
        }
    }

    //Copy_Section(4,Text_Engine->Default_Codepage->RGBA_Canvas,Image->RGBA_Canvas,416,144,64,16,0,0,0,0,16,16);


    // Generate a texture ID
    glGenTextures(1, &Image->Texture_ID);
    // Bind the texture to GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);

    // Set texture parameters
    // GL_LINEAR for smooth scaling, GL_NEAREST for pixelated look
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Load the image data into the texture
    // GL_BGR is used because BMP stores pixels in BGR order
    // GL_UNSIGNED_BYTE indicates each color component is an unsigned byte
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->Width, Image->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);

};

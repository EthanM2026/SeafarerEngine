/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED
#include "stdio.h"
#include "../engine.h"
#include "../graphics/image.h"

#define ID_LANGUAGE_ENGLISH 0
#define ID_LANGUAGE_ICELANDIC 1
#define ID_LANGUAGE_FRENCH 2
#define ID_LANGUAGE_SPANISH 3
#define ID_LANGUAGE_PORTUGESE 4
#define ID_LANGUAGE_NORWEGIAN 5
#define ID_LANGUAGE_SWEDISH 6
#define ID_LANGUAGE_FINNISH 7
#define ID_LANGUAGE_DANISH 8
#define ID_LANGUAGE_DUTCH 9
#define ID_LANGUAGE_GERMAN 10
#define ID_LANGUAGE_POLISH 11
#define ID_LANGUAGE_ITALIAN 12
#define ID_LANGUAGE_GREEK 13
#define ID_LANGUAGE_TURKISH 14
#define ID_LANGUAGE_UKRAINIAN 15
#define ID_LANGUAGE_RUSSIAN 16
#define ID_LANGUAGE_CHINESE 17
#define ID_LANGUAGE_KOREAN 18
#define ID_LANGUAGE_JAPANESE 19
#define ID_LANGUAGE_THAI 20
#define ID_LANGUAGE_VIETNAMESE 21
#define ID_LANGUAGE_FILIPINO 22
#define ID_LANGUAGE_INDONESIAN 23
#define ID_LANGUAGE_ARABIC 24
#define ID_LANGUAGE_HINDI 25
#define ID_LANGUAGE_BENGALI 26
#define ID_LANGUAGE_URDU 27
#define ID_LANGUAGE_PUNJABI 28
#define ID_LANGUAGE_FARSI 29
#define ID_LANGUAGE_UADL 30
#define ID_LANGUAGE_LATIN 31
#define ID_LANGUAGE_ESPERANTO 32
#define ID_LANGUAGE_DARI 33
#define ID_LANGUAGE_PASHTO 34
#define ID_LANGUAGE_SERBOCROATIAN 35
#define ID_LANGUAGE_ROMANIAN 36

struct _Language_Header
{
    char Name[256];
    char Encoding_File[256];
    int ID;
    int Number_Of_Encodings;
    int Number_Of_Codepages_Required;
    int Codepage_Width;
    int Codepage_Height;
    int Character_Width;
    int Character_Height;
};

struct _Language_Encoding
{
    int Unicode_Character;
    int Codepage;
    int x;
    int y;
    int Width;
    int Height;
};

void Create_Language(unsigned char Name[256],
    unsigned int Number_Of_Encodings,
    unsigned int Number_Of_Codepages_Required,
    unsigned char Encoding_File[256], const char* Filename);

void Add_Encoding(FILE * f, int Unicode_Character,
    int Codepage,
    int x,
    int y,
    int Width,
    int Height);



//START: ENGLISH-US,

struct _Text_Engine
{
    unsigned int Number_Of_Codepages_Loaded;
    struct _Image* Default_Codepage;
    struct _Image* Codepages;

    struct _Language_Header Language;
    struct _Language_Encoding* Encodings;

    int Number_Of_Text_Images;
    struct _Image* Text_Images;
};

void Load_Language(struct _Text_Engine* Text_Engine, const char* Filename);

void Write_Language(const char* Filename);
void Encode_Text(struct _Text_Engine* Text_Engine, int Text[256], int Text_Length, int Width, int Height, const char* Output);

struct _Text_Engine* Create_Text_Engine();
void Initialize_Text_Engine(struct _Engine* Engine); //Load Default codepage

struct _Image* Create_Text_Image(struct _Engine* Engine, char Text[256], int Text_Length, int Width, int Height);
void Initialize_Text_Image(struct _Text_Engine* Text_Engine, struct _Image* Image, char Text[256], int Text_Length, int Width, int Height);

void Create_English_Encoding_File();
void Load_Encoding(struct _Text_Engine* Text_Engine, const char* Filename);

void Create_Encoded_Text(struct _Text_Engine* Text_Engine, const char* Input_Filepath, const char* Filepath);

void Initialize_Text_Image_From_Encoded_Text(struct _Text_Engine* Text_Engine, struct _Image* Image, int Text_Length, int Height, const char* Filename);
#endif // TEXT_H_INCLUDED












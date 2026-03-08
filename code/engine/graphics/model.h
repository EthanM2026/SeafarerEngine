/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

typedef float vec3_t[3];

struct _Texture_Coordinate
{
    short x;
    short y;
};

struct _Triangle
{
    unsigned short Indices[3];
    unsigned short Texture_Indices[3];
};

struct _Vertex
{
    unsigned short Coordinates[3];
    unsigned char Normal_Index;
};

struct _Frame
{
    vec3_t Scale;
    vec3_t Translation;
    char Name[16];
    struct _Vertex* Vertices;
};

struct _Model
{
    int ident;      // Magic number "IDP2"
    int version;    // Version, must be 8
    int skinwidth;
    int skinheight;
    int framesize;
    int num_skins;
    int num_xyz;    // Number of vertices
    int num_st;     // Number of texture coordinates
    int num_tris;   // Number of triangles
    int num_glcmds;
    int num_frames; // Number of frames
    int offset_skins;
    int offset_st;
    int offset_tris;
    int offset_frames;
    int offset_glcmds;
    int offset_end;
};

struct _Model* Create_Model();
void Load_Model(struct _Model* Model, const char* Filename);
void Load_Texture(struct _Model* Model, const char* Filename);
void Extract_Actual_Vertex_Coordinates(struct _Model* Model);
void Render_Model(struct _Model* Model);

#endif // MODEL_H_INCLUDED

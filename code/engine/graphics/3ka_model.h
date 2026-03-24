/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef 3KA_MODEL_H_INCLUDED
#define 3KA_MODEL_H_INCLUDED

struct _8BT_File
{
    unsigned char Width;
    unsigned char Height;
};

struct _3KA_Texture_Vertex
{
    short x;
    short y;
};

struct _3KA_Triangle
{
    unsigned short Vertex_Indices[3];
    unsigned short Texture_Vertex_Indices[3]
};

struct _3KA_Vertex
{
    unsigned short Coordinates[3];
    unsigned short Normals[3];
}

struct _3KA_Frame
{
    struct _3KA_Vertex* Frame_Vertices;
};

struct _3D_Keyframe_Animation_Model_Header
{
    float X_Scale;
    float Y_Scale;
    float Z_Scale;

    float X_Offset;
    float Y_Offset;
    float Z_Offset;

    int Number_Of_Vertices;
    int Number_Of_Texture_Vertices;
    int Number_Of_Triangles;
    int Number_Of_Frames;

    int Offset_To_Texture_Vertices;
    int Offset_To_Triangles;
    int Offset_To_Frames;
};


struct _3KA_Model
{
    struct _3D_Keyframe_Animation_Model_Header;


};

struct _3KA_Model* Create_3KA_Model();
void Load_3KA_Model(struct _3KA_Model* Model, const char* Filename);
void Load_3KA_Texture(struct _3KA_Model* Model, const char* Filename);
void Render_3KA_Model (struct _3KA_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B);




#endif // 3KA_MODEL_H_INCLUDED

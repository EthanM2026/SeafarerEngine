/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef NEW_MODEL_H
#define NEW_MODEL_H
#include "../engine.h"
#include "stdio.h"
#include "../physics/vector.h"
#include "image.h"

#define NUMVERTEXNORMALS        162
#define SHADEDOT_QUANT          16
#define MAX_SE3_VERTS 2048

#pragma pack(push, 1)
// vertex
struct _Animation_File_Animation
{
    char Name[32];
    unsigned char Keyframes[2];
    float Speed;
    bool Loop;
};

struct _Animation_File
{
    //.ANF
     struct _Animation_File_Animation Animations[21];
};

struct Vertex
{
    unsigned short  Coordinates[3];                // compressed vertex (x, y, z) coordinates
    unsigned char  Normal_Index;    // index to a normal vector for the lighting

};

struct Texture_Coordinate
{
    short x;
    short y;
};

struct Frame
{
    float  Frame_Scale[3];       // scale values
    float  Frame_Translation_Vector[3];   // translation vector
    char   Frame_Name[16];       // frame name
    struct Vertex*    First_Vertex;       // first vertex of this frame
};

struct Triangle
{
    short  Triangle_Vertex_IDs[3];    // indexes to triangle's vertices
    short  Triangle_Texture_Vertex_IDs[3];     // indexes to vertices' texture coorinates

};

typedef struct {
    float pos[3];
    float normal[3];
    float uv[2];
} gl_vertex_t;



struct Texture_Triangle
{
    short  Triangle_Texture_Vertex_IDs[3];     // indexes to vertices' texture coorinates

};

struct Stripped_Triangle
{
    short  Triangle_Vertex_IDs[3];    // indexes to triangle's vertices

};

// animation
struct _Animation
{
    int     First_Frame_ID;            // first frame of the animation
    int     Last_Frame_ID;             // number of frames
    int     FPS;                    // number of frames per second

};

// animation state
struct _Animation_State
{
    int     Start_Frame_ID;              // first frame
    int     End_Frame_ID;                // last frame
    int     FPS;                     // frame per second for this animation

    float   Current_Time;               // current time
    float   Old_Time;                // old time
    float   Interpolation_Percentage;                // percent of interpolation

    int     Animation_Type;                    // animation type

    int     Current_Frame_ID;              // current frame
    int     Next_Frame_ID;              // next frame

};

//Populate anorms and anorms dots

struct Texture_Name
{
    char Name[64];
};

/////////////////////////////////////////////////

struct _Draw_Array
{
    float* Triangle_Coordinates;
    float* Texture_Triangle_Coordinates;
    float* Triangle_Normal_Coordinates;
};

struct _Normal_Table_Entry
{
    float x;
    float y;
    float z;
};

struct _Normal_Table
{
    struct _Normal_Table_Entry Entries[240];
};

struct _SE3_Model
{
    struct _Draw_Array* Draw_Arrays;
    gl_vertex_t* glDraw_Vertices;
    struct _Animation_File* Animation_File;
    unsigned char Current_Start_Keyframe;
    unsigned char Current_End_Keyframe;
    unsigned char Current_Keyframe = 0;

    int Frame_Size;
    int Number_Of_Textures;
    int Number_Of_Vertices;
    int Number_Of_Texture_Vertices;
    int Number_Of_Triangles;
    int Number_Of_Commands;
    int Number_Of_Frames;
    unsigned int Texture_ID;
    float Model_Scale;

    struct _Animation_State Animation_State;


    Vector* Vertex_Array;
    int* Command_Array;
    int* Light_Normals;
    struct _Animation* Animation_List[21];







    Texture_Name* Texture_Names;
    struct Texture_Coordinate* Texture_Coordinates;
    struct Frame* Frame_Data;

    struct Triangle* Triangle_Indices;
    struct Texture_Triangle* Texture_Triangle_Indices;
    struct Stripped_Triangle* Stripped_Triangle_Indices;


        // member variables
    static Vector   anorms[ NUMVERTEXNORMALS ];
    static float    anorms_dots[ SHADEDOT_QUANT ][256];





/* Table of precalculated normals */
Vector anorms_table[240] = {
#include "normal_table.h"
};




struct _Normal_Table NT;

//    unsigned int Texture_ID;

    struct _Image* Image;

    int RGBA_Image_Size;
        unsigned char Texture_Made;
        int x;
        int y;
        int Width;
        int Height;
        unsigned char* RGB_Canvas;
        unsigned char* RGBA_Canvas;
        float Vertices[8];
        //float Texture_Vertices[8];
};

struct SE3_glcmd_t
{
  float s;                    /* s texture coord. */
  float t;                    /* t texture coord. */
  int index;                  /* vertex index */
};

struct SE3_vertex_t
{
  unsigned short v[3];         /* position */
  unsigned char normalIndex;  /* normal vector index */
};

struct SE3_frame_t
{
  Vector scale;               /* scale factor */
  Vector translate;           /* translation vector */
  char name[16];              /* frame name */
  struct SE3_vertex_t *verts; /* list of frame's vertices */
};

#pragma pack(pop)

struct _SE3_Model* Create_SE3_Model();
void Load_SE3_Model(struct _SE3_Model* Model, const char* Filename);
void Load_SE3_Texture(struct _SE3_Model* Model, const char* Filename);

void Load_Transparent_SE3_Texture(struct _SE3_Model* Image, const char* Filename, const char* Transparency_Map_Filename);

void Load_SE3_Animation_Array(struct _SE3_Model* Model, const char* Filename);


void Render_SE3_Model (struct _SE3_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B);

struct _Animation* Register_Animation(int First_Frame_ID, int Last_Frame_ID, int FPS);


void Add_Animation(struct _Animation_File* A, unsigned char Index, char Name[32], unsigned char Keyframe_One, unsigned char Keyframe_Two, float Speed, bool Loop);
void Create_Animation_File(const char* Filename);


void Render_Wrapper(struct _SE3_Model* Model, unsigned char Index, unsigned char Keyframe_One, unsigned char Keyframe_Two, float Speed, bool Loop, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation); //for animations

void Set_Model_Sequeance(struct _SE3_Model* Model, unsigned char Keyframe_One, unsigned char Keyframe_Two);

void Render_SE3_Model_GLDraw(struct _SE3_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B);
void update_gl_vertices_for_frame(struct _SE3_Model* Model, int frame_index);

void Extract_All_Frames_To_Arrays(struct _SE3_Model* Model);

void Render_SE3_Model_Dynamic_Texture(struct _SE3_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B, int Texture_ID);

//void Dump_SE3_


#endif /* NEW_MODEL_H */

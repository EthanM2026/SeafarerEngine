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

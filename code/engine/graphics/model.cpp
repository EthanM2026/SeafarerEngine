#include "model.h"
#include "stdio.h"
#include <iostream>

struct _Model* Create_Model()
{
    struct _Model* Model = (struct _Model*)calloc(1,sizeof(struct _Model));
    return Model;
};

void Load_Model(struct _Model* Model, const char* Filename)
{
    FILE* f = fopen(Filename, "wb");
};

void Load_Texture(struct _Model* Model, const char* Filename)
{

};

void Extract_Actual_Vertex_Coordinates(struct _Model* Model)
{

};

void Render_Model(struct _Model* Model)
{

};

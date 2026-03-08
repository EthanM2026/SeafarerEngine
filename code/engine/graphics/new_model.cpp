/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "new_model.h"
#include "string.h"
#include "math.h"



struct _MD2_Model* Create_MD2_Model()
{
    struct _MD2_Model* M = (struct _MD2_Model*) calloc(1,sizeof(struct _MD2_Model));
    return M;
}

void Load_MD2_Model(struct _MD2_Model* Model, const char* Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_model.dat", "rb");
        printf("ERROR: COULD NOT LOAD FILE: ");
        printf(Filename);
        printf("\n");
    }

    unsigned char header[68];

    fread(header, 1, 68, file);

    Model->Width = *(int*)&(header[8]);
    Model->Height =*(int*)&(header[12]);
    Model->Frame_Size =*(int*)&(header[16]);
    Model->Number_Of_Textures =*(int*)&(header[20]);
    Model->Number_Of_Vertices =*(int*)&(header[24]);
    Model->Number_Of_Texture_Vertices =*(int*)&(header[28]);
    Model->Number_Of_Triangles =*(int*)&(header[32]);
    Model->Number_Of_Commands =*(int*)&(header[36]);
    Model->Number_Of_Frames =*(int*)&(header[40]);

    int Texture_Name_Offset=*(int*)&(header[44]);
    int Texture_Vertex_Offset=*(int*)&(header[48]);
    int Triangle_Offset=*(int*)&(header[52]);
    int Frame_Data_Offset=*(int*)&(header[56]);
    int Command_Offset=*(int*)&(header[60]);
    int EndOF = *(int*)&(header[64]);

    Model->Texture_Names = (struct Texture_Name *) calloc (Model->Number_Of_Textures,sizeof (struct Texture_Name));
    Model->Texture_Coordinates = (struct Texture_Coordinate *) calloc (Model->Number_Of_Texture_Vertices,sizeof (struct Texture_Coordinate));
    Model->Triangle_Indices = (struct Triangle *) calloc (Model->Number_Of_Triangles,sizeof (struct Triangle));
    Model->Frame_Data = (struct Frame *) calloc (Model->Number_Of_Frames,sizeof (struct Frame));
    Model->Command_Array = (int *)calloc (Model->Number_Of_Commands,sizeof (int));



  /* Read model data */
  fseek (file, Texture_Name_Offset, SEEK_SET);
  fread (Model->Texture_Names, sizeof (struct Texture_Name),
	 Model->Number_Of_Textures, file);

  fseek (file, Texture_Vertex_Offset, SEEK_SET);
  fread (Model->Texture_Coordinates, sizeof (struct Texture_Coordinate),
	 Model->Number_Of_Texture_Vertices, file);

  fseek (file, Triangle_Offset, SEEK_SET);
  fread (Model->Triangle_Indices, sizeof (struct Triangle),
	 Model->Number_Of_Triangles, file);

  fseek (file, Command_Offset, SEEK_SET);
  fread (Model->Command_Array, sizeof (int), Model->Number_Of_Commands, file);



  /* Read frames */
  fseek (file, Frame_Data_Offset, SEEK_SET);
  for (int j = 0; j < Model->Number_Of_Frames; j++)
    {
      /* Memory allocation for vertices of this frame */
      Model->Frame_Data[j].First_Vertex = (struct Vertex *)
	calloc (Model->Number_Of_Vertices,sizeof (struct Vertex));

      /* Read frame data */
      fread (Model->Frame_Data[j].Frame_Scale, sizeof (Vector), 1, file);
      fread (Model->Frame_Data[j].Frame_Translation_Vector, sizeof (Vector), 1, file);
      fread (Model->Frame_Data[j].Frame_Name, sizeof (char), 16, file);
      fread (Model->Frame_Data[j].First_Vertex, sizeof (struct Vertex),
	     Model->Number_Of_Vertices, file);
    }

  fclose (file);

    int Number_of_Vertices = Model->Number_Of_Triangles*3;

    //Model->glDraw_Vertices = (gl_vertex_t*)calloc(Number_of_Vertices, sizeof(gl_vertex_t));
    Model->Draw_Arrays = (struct _Draw_Array*)calloc(Model->Number_Of_Frames, sizeof(struct _Draw_Array));

    for(int n = 0; n < Model->Number_Of_Frames; n++) //For each sub model
    {
        Model->Draw_Arrays[n].Triangle_Coordinates = (float*)calloc(9*Model->Number_Of_Triangles,sizeof(float)); //Each vertex has 3 Coords!
        Model->Draw_Arrays[n].Texture_Triangle_Coordinates = (float*)calloc(6*Model->Number_Of_Triangles,sizeof(float));
        Model->Draw_Arrays[n].Triangle_Normal_Coordinates = (float*)calloc(3*Model->Number_Of_Triangles,sizeof(float));
    }
    Extract_All_Frames_To_Arrays(Model);
};

float Convert_Unsigned_Byte_To_Float(unsigned char Byte, float Scale, float Translate)
{

}

void Load_MD2_Texture(struct _MD2_Model* Image, const char* Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_texture.bmp", "rb");
    }

    unsigned char header[54];
    fread(header, 1, 54, file);
    int RGB_Image_Size = ((256) * (256) * 3);
    int RGBA_Image_Size = ((256) * (256) * 4);
    fseek(file, 54, SEEK_SET);
    Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size , sizeof(unsigned char));
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(Image->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);
    int in = 0;
    int r = 0;
    for(int j = 0; j < (RGBA_Image_Size/4); j++)
    {
        Image->RGBA_Canvas[r] = Image->RGB_Canvas[in+2];
        Image->RGBA_Canvas[r + 1] = Image->RGB_Canvas[in + 1];
        Image->RGBA_Canvas[r + 2] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 3] = 255;
        if(Image->RGBA_Canvas[r] == 200)
        {
            if(Image->RGBA_Canvas[r+1] == 200)
            {
                if(Image->RGBA_Canvas[r + 2] == 200)
                {
                    Image->RGBA_Canvas[r + 3] = 0;
                }
            }
        }
        in += 3;
        r += 4;
    }

    glGenTextures(1, &Image->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    glEnable(GL_TEXTURE_2D);
}

void Load_Transparent_MD2_Texture(struct _MD2_Model* Image, const char* Filename, const char* Transparency_Map_Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_texture.bmp", "rb");
    }

    unsigned char header[54];
    fread(header, 1, 54, file);
    int RGB_Image_Size = ((256) * (256) * 3);
    int RGBA_Image_Size = ((256) * (256) * 4);
    fseek(file, 54, SEEK_SET);
    Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size , sizeof(unsigned char));
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size , sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(Image->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);







    FILE * filetwo;
    filetwo = fopen(Transparency_Map_Filename,"rb");
    if(!filetwo)
    {
        filetwo = fopen("resources/artwork/placeholders/no_texture.bmp", "rb");
    }

    unsigned char headertwo[54];
    fread(headertwo, 1, 54, filetwo);
    fseek(filetwo, 54, SEEK_SET);
    unsigned char* Transparency_Buffer = (unsigned char*) calloc (RGB_Image_Size , sizeof(unsigned char));
    fread(Transparency_Buffer,1,RGB_Image_Size,filetwo);
    fclose(filetwo);

    int in = 0;
    int r = 0;
    for(int j = 0; j < (RGBA_Image_Size/4); j++)
    {
        Image->RGBA_Canvas[r] = Image->RGB_Canvas[in+2];
        Image->RGBA_Canvas[r + 1] = Image->RGB_Canvas[in + 1];
        Image->RGBA_Canvas[r + 2] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 3] = Transparency_Buffer[in];
        in += 3;
        r += 4;
    }

    glGenTextures(1, &Image->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clamp to edge to avoid issues with non-power-of-2 textures
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256,256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    glEnable(GL_TEXTURE_2D);
}


void Interpolate_MD2_Model(struct _MD2_Model* Model, Vector *vertlist )
{
    Vector  *curr_v;    // pointeur to current frame vertices
    Vector  *next_v;    // pointeur to next frame vertices

    // create current frame and next frame's vertex list
    // from the whole vertex list
    curr_v = &Model->Vertex_Array[ Model->Number_Of_Vertices * Model->Animation_State.Current_Frame_ID];
    next_v = &Model->Vertex_Array[ Model->Number_Of_Vertices * Model->Animation_State.Next_Frame_ID ];

    // interpolate and scale vertices to avoid ugly animation
    for( int i = 0; i < Model->Number_Of_Vertices ; i++ )
    {
        vertlist[i][0] = (curr_v[i][0] + Model->Animation_State.Interpolation_Percentage * (next_v[i][0] - curr_v[i][0])) * Model->Model_Scale;
        vertlist[i][1] = (curr_v[i][1] + Model->Animation_State.Interpolation_Percentage * (next_v[i][1] - curr_v[i][1])) * Model->Model_Scale;
        vertlist[i][2] = (curr_v[i][2] + Model->Animation_State.Interpolation_Percentage * (next_v[i][2] - curr_v[i][2])) * Model->Model_Scale;


    }
}

void Render_MD2_Model (struct _MD2_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B)
{
    float Color[3];
    Color[0] = R;
    Color[1] = G;
    Color[2] = B;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    int i, j;
    GLfloat s, t;
    Vector v;
    struct Frame *pframe;
    struct Vertex *pvert;
  /* Check if n is in a valid range */
    if ((n < 0) || (n > Model->Number_Of_Frames - 1))
    return;

  /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, Model->Texture_ID);

  /* Draw the model */
    glBegin (GL_TRIANGLES);
    /* Draw each triangle */
    for (int m = 0; m < Model->Number_Of_Triangles; m++)
    {
	/* Draw each vertex */
	for (int j = 0; j < 3; j++)
    {
        pframe = &Model->Frame_Data[n];
        pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[j]];
	    /* Compute texture coordinates */
	    t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].x / 256;
	    s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].y / 256;
	    /* Pass texture coordinates to OpenGL */
	    glTexCoord2f (s, t);
	    /* Nrmal vector */
	    glNormal3fv (Model->anorms_table[pvert->Normal_Index]);
	    /* Calculate vertex real position */
	    v[0] = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    v[1] = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    v[2] = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];
	    if(Vertex_Coloring)
	    {
           // glColor3f(Color[0],Color[1],Color[2]);
	    }
	    glVertex3fv (v);
	  }
	  //drawBillboard();
      }
  glEnd();
 // glPopMatrix();
}


void Render_MD2_Model_Dynamic_Texture(struct _MD2_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B, int Texture_ID)
{
    float Color[3];
    Color[0] = R;
    Color[1] = G;
    Color[2] = B;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    int i, j;
    GLfloat s, t;
    Vector v;
    struct Frame *pframe;
    struct Vertex *pvert;
  /* Check if n is in a valid range */
    if ((n < 0) || (n > Model->Number_Of_Frames - 1))
    return;

  /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, Texture_ID);

  /* Draw the model */
    glBegin (GL_TRIANGLES);
    /* Draw each triangle */
    for (int m = 0; m < Model->Number_Of_Triangles; m++)
    {
	/* Draw each vertex */
	for (int j = 0; j < 3; j++)
    {
        pframe = &Model->Frame_Data[n];
        pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[j]];
	    /* Compute texture coordinates */
	    t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].x / 256;
	    s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[j]].y / 256;
	    /* Pass texture coordinates to OpenGL */
	    glTexCoord2f (s, t);
	    /* Nrmal vector */
	    glNormal3fv (Model->anorms_table[pvert->Normal_Index]);
	    /* Calculate vertex real position */
	    v[0] = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    v[1] = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    v[2] = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];
	    if(Vertex_Coloring)
	    {
           // glColor3f(Color[0],Color[1],Color[2]);
	    }
	    glVertex3fv (v);
	  }
	  //drawBillboard();
      }
  glEnd();
 // glPopMatrix();
}


void Add_Animation(struct _Animation_File* A, unsigned char Index, char Name[32], unsigned char Keyframe_One, unsigned char Keyframe_Two, float Speed, bool Loop)
{
for(int j = 0; j <32; j++)
{
    A->Animations[Index].Name[j] = Name[j];
    }
     A->Animations[Index].Name[31] = '\0';

    A->Animations[Index].Keyframes[0] = Keyframe_One;
    A->Animations[Index].Keyframes[1] = Keyframe_Two;



    A->Animations[Index].Speed = Speed;
    A->Animations[Index].Loop = Loop;

}
void Create_Animation_File(const char* Filename)
{
    struct _Animation_File A;
    char Name[32];
    sprintf(Name, "BLUESPIKE");

    Add_Animation(&A, 0, Name, 0, 1, 1, true);

    FILE* f = fopen(Filename, "wb");
    fwrite(&A, 1, sizeof(struct _Animation_File), f);
    fclose(f);
}

void Load_MD2_Animation_Array(struct _MD2_Model* Model, const char* Filename)
{
    FILE* f = fopen(Filename, "rb");
    fread(Model->Animation_File, 1, sizeof(struct _Animation_File), f);
    fclose(f);
}


void Render_Wrapper(struct _MD2_Model* Model, unsigned char Index, unsigned char Keyframe_One, unsigned char Keyframe_Two, float Speed, bool Loop, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation) //for animations
{
    //Model->Current_Keyframe = Keyframe_One;


    //Render_MD2_Model_GLDraw(Model, Model->Current_Keyframe, x, y, z, x_Rotation, y_Rotation, z_Rotation,true,1,1,1);
    Model->Current_Keyframe += 1;
    if(Model->Current_Keyframe >= Model->Current_End_Keyframe)
    {
        Model->Current_Keyframe = Model->Current_Start_Keyframe;
    }
}


void Set_Model_Sequeance(struct _MD2_Model* Model, unsigned char Keyframe_One, unsigned char Keyframe_Two)
{
    Model->Current_Start_Keyframe = Keyframe_One;
    Model->Current_Keyframe = Keyframe_One;
    Model->Current_End_Keyframe = Keyframe_Two;
}


void Render_MD2_Model_GLDraw(struct _MD2_Model* Model, int n, float x, float y, float z, float x_Rotation, float y_Rotation, float z_Rotation, bool Vertex_Coloring, float R, float G, float B)
{
    glBindTexture (GL_TEXTURE_2D, Model->Texture_ID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(3, GL_FLOAT, 0, Model->Draw_Arrays[n].Triangle_Coordinates);
        // Normal Pointer
    glNormalPointer(GL_FLOAT, 0, Model->Draw_Arrays[n].Triangle_Normal_Coordinates);
        // Texture Coordinate Pointer
    glTexCoordPointer(2, GL_FLOAT, 0, Model->Draw_Arrays[n].Texture_Triangle_Coordinates);
        // RENDER using glDrawArrays
    glDrawArrays(GL_TRIANGLES, 0, Model->Number_Of_Vertices*9);
        // Disable client-side arrays after drawing
    glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
};

vec3_t calculate_frame_vertex_position(struct Frame *pframe, struct Vertex *pvert) {
    vec3_t pos;
    // Note: vertex->v[i] is now an unsigned short (2 bytes)
    pos.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
    pos.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
    pos.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

   // printf("Frame Vertex Positions: %f %f %f\n",pos.x,pos.y,pos.z);

    return pos;
}

void update_gl_vertices_for_frame(struct _MD2_Model* Model, int frame_index) {
    struct Frame *pframe;
    struct Vertex *pvert;
    struct Vertex *pvert2;
    struct Vertex *pvert3;

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    pframe = &Model->Frame_Data[frame_index];

    for (int i = 0; i < Model->Number_Of_Triangles; i++) {
        vec3_t p[3];

        pvert = &pframe->First_Vertex[Model->Triangle_Indices[i].Triangle_Vertex_IDs[0]];
        pvert2 = &pframe->First_Vertex[Model->Triangle_Indices[i].Triangle_Vertex_IDs[1]];
        pvert3 = &pframe->First_Vertex[Model->Triangle_Indices[i].Triangle_Vertex_IDs[2]];

        p[0] = calculate_frame_vertex_position(pframe, pvert);
        p[1] = calculate_frame_vertex_position(pframe, pvert2);
        p[2] = calculate_frame_vertex_position(pframe, pvert3);

        // 2. Calculate the face normal (N)
        vec3_t edge1 = vec3_t_sub(p[1], p[0]); // P2 - P1
        vec3_t edge2 = vec3_t_sub(p[2], p[0]); // P3 - P1
        vec3_t unnormalized_normal = vec3_t_cross(edge1, edge2);
        vec3_t normal = vec3_t_normalize(unnormalized_normal);

        // 3. Populate the gl_vertex_t array for all three vertices of the triangle
        for (int j = 0; j < 3; j++) {
            gl_vertex_t *v = &Model->glDraw_Vertices[i * 3 + j];

            // Position
            v->pos[0] = p[j].x;
            v->pos[1] = p[j].y;
            v->pos[2] = p[j].z;

            // Normal (using the single face normal for flat shading)
            v->normal[0] = normal.x;
            v->normal[1] = normal.y;
            v->normal[2] = normal.z;

            // UV Coordinates (normalized to 0.0-1.0 range)
            v->uv[0] = (float)Model->Texture_Coordinates[Model->Triangle_Indices[i].Triangle_Texture_Vertex_IDs[j]].x / 256;
            v->uv[1] = (float)Model->Texture_Coordinates[Model->Triangle_Indices[i].Triangle_Texture_Vertex_IDs[j]].y / 256;
        }
    }
}

void Extract_All_Frames_To_Arrays(struct _MD2_Model* Model)
{
    struct Frame *pframe;
    struct Vertex *pvert;
    for(int n = 0; n < Model->Number_Of_Frames; n++) //For each sub model
    {
        int Vertex_Counter = 0;
        int Normal_Counter = 0;
        int Texture_Vertex_Counter = 0;

        pframe = &Model->Frame_Data[n];
        for(int m = 0; m < Model->Number_Of_Triangles; m++)
        {
            vec3_t vOne;
            vec3_t vTwo;
            vec3_t vThree;

            GLfloat s, t;
            GLfloat sTwo, tTwo;
            GLfloat sThree, tThree;

            pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[0]];
            t = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[0]].x / 256;
            s = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[0]].y / 256;
            vOne.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
            vOne.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
            vOne.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

            pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[1]];
            tTwo = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[1]].x / 256;
            sTwo = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[1]].y / 256;
            vTwo.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
            vTwo.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
            vTwo.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

            pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[2]];
            tThree = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[2]].x / 256;
            sThree = (float)Model->Texture_Coordinates[Model->Triangle_Indices[m].Triangle_Texture_Vertex_IDs[2]].y / 256;
            vThree.x = (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
            vThree.y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
            vThree.z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];



            // 2. Calculate the face normal (N)
            vec3_t edge1 = vec3_t_sub(vTwo, vOne); // P2 - P1
            vec3_t edge2 = vec3_t_sub(vThree, vOne); // P3 - P1
            vec3_t unnormalized_normal = vec3_t_cross(edge1, edge2);
            vec3_t normal = vec3_t_normalize(unnormalized_normal);

            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter]=vOne.x;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+1]=vOne.y;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+2]=vOne.z;

            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+3]=vTwo.x;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+4]=vTwo.y;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+5]=vTwo.z;

            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+6]=vThree.x;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+7]=vThree.y;
            Model->Draw_Arrays[n].Triangle_Coordinates[Vertex_Counter+8]=vThree.z;


            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter]=s;
            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+1]=t;

            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+2]=sTwo;
            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+3]=tTwo;

            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+4]=sThree;
            Model->Draw_Arrays[n].Texture_Triangle_Coordinates[Texture_Vertex_Counter+5]=tThree;


            Model->Draw_Arrays[n].Triangle_Normal_Coordinates[Normal_Counter] = normal.x;
            Model->Draw_Arrays[n].Triangle_Normal_Coordinates[Normal_Counter+1] = normal.y;
            Model->Draw_Arrays[n].Triangle_Normal_Coordinates[Normal_Counter+2] = normal.z;

            Vertex_Counter += 9;
            Normal_Counter += 3;
            Texture_Vertex_Counter += 6;
	    }
    }
}

/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "image.h"
#include "string.h"








void drawBillboard(float x, float y, float z, float Width, float Height) {
    // 1. Get the current Modelview matrix

    // Billboard properties
float billboardWidth = Width;
float billboardHeight = Height;
float billboardPosition[3] = {x, y, z}; // Example position

// Camera position and orientation (for demonstration)
float cameraPosition[3] = {0.0f, 0.0f, 0.0f};
float cameraRotationY = 0.0f;

    float modelviewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);

    // 2. Extract the right (x) and up (y) vectors of the camera
    float rightVector[3] = {modelviewMatrix[0], modelviewMatrix[4], modelviewMatrix[8]};
    float upVector[3] = {modelviewMatrix[1], modelviewMatrix[5], modelviewMatrix[9]};

    // 3. Calculate the billboard's corners
    float halfWidth = billboardWidth / 2.0f;
    float halfHeight = billboardHeight / 2.0f;

    float corner1[3] = {
        billboardPosition[0] - rightVector[0] * halfWidth + upVector[0] * halfHeight,
        billboardPosition[1] - rightVector[1] * halfWidth + upVector[1] * halfHeight,
        billboardPosition[2] - rightVector[2] * halfWidth + upVector[2] * halfHeight
    };

    float corner2[3] = {
        billboardPosition[0] + rightVector[0] * halfWidth + upVector[0] * halfHeight,
        billboardPosition[1] + rightVector[1] * halfWidth + upVector[1] * halfHeight,
        billboardPosition[2] + rightVector[2] * halfWidth + upVector[2] * halfHeight
    };

    float corner3[3] = {
        billboardPosition[0] + rightVector[0] * halfWidth - upVector[0] * halfHeight,
        billboardPosition[1] + rightVector[1] * halfWidth - upVector[1] * halfHeight,
        billboardPosition[2] + rightVector[2] * halfWidth - upVector[2] * halfHeight
    };

    float corner4[3] = {
        billboardPosition[0] - rightVector[0] * halfWidth - upVector[0] * halfHeight,
        billboardPosition[1] - rightVector[1] * halfWidth - upVector[1] * halfHeight,
        billboardPosition[2] - rightVector[2] * halfWidth - upVector[2] * halfHeight
    };

    // 4. Render the billboard as a quad
    glBegin(GL_QUADS);
        // Assuming you have texture coordinates (uv) associated with your billboard
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(corner1);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(corner2);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(corner3);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(corner4);
    glEnd();
}

struct _Image* Create_Image()
{
    struct _Image* I = (struct _Image*) calloc(1,sizeof (struct _Image));

    I->x = 0;
    I->y = 0;
    I->Texture_Made = 0;
    return I;
}

void Destroy_Image(struct _Image* Image)
{
    //free(Image->RGB_Canvas);
    //free(Image->RGBA_Canvas);
    //free(Image);
}

void Load_Image(struct _Image* Image, const char* Filename)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_image.bmp", "rb");
        printf("Error: Could not load image ");
        printf(Filename);
        printf(".\n");
    }

    unsigned char header[54];

    fread(header, 1, 54, file);

    Image->Width = *(int*)&(header[0x12]);
    Image->Height = *(int*)&(header[0x16]);

    int RGB_Image_Size = ((Image->Width) * (Image->Height) * 3);

    int RGBA_Image_Size = ((Image->Width) * (Image->Height) * 4);

    fseek(file, 54, SEEK_SET);

    Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size,sizeof(unsigned char));



    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size,sizeof(unsigned char));
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(Image->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);
    //Image->RGBA_Canvas = Convert_RGB_To_RGBA(Image->RGB_Canvas,Image->Width, Image->Height);
    int in = 0;
    int r = 0;
    //Image->RGBA_Canvas[0] = Image->RGB_Canvas[0];
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

void Render_Image(struct _Image* Image, float _x, float _y, int Alpha_Present)
{
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);

    float Vertices[] = {_x, _y, _x + Image->Width, _y, _x + Image->Width, _y + Image->Height, _x, _y + Image->Height};
    float Texture_Vertices[] = {0, 0, 1, 0, 1, -1, 0, -1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(2, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}




void Render_Image_As_Billboard(struct _Image* Image, float _x, float _y, float _z, int Alpha_Present, float Scale)
{
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->Width, Image->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    //Image->Texture_Made = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_TEXTURE_2D);


    //glColor3f(0,0,255);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);


    float billboardWidth = Image->Width*Scale;
float billboardHeight = Image->Height*Scale;
float billboardPosition[3] = {_x, _y, _z}; // Example position

// Camera position and orientation (for demonstration)
float cameraPosition[3] = {0.0f, 0.0f, 0.0f};
float cameraRotationY = 0.0f;

    float modelviewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);

    // 2. Extract the right (x) and up (y) vectors of the camera
    float rightVector[3] = {modelviewMatrix[0], modelviewMatrix[4], modelviewMatrix[8]};
    float upVector[3] = {modelviewMatrix[1], modelviewMatrix[5], modelviewMatrix[9]};

    // 3. Calculate the billboard's corners
    float halfWidth = billboardWidth / 2.0f;
    float halfHeight = billboardHeight / 2.0f;

    float corner1[3] = {
        billboardPosition[0] - rightVector[0] * halfWidth + upVector[0] * halfHeight,
        billboardPosition[1] - rightVector[1] * halfWidth + upVector[1] * halfHeight,
        billboardPosition[2] - rightVector[2] * halfWidth + upVector[2] * halfHeight
    };

    float corner2[3] = {
        billboardPosition[0] + rightVector[0] * halfWidth + upVector[0] * halfHeight,
        billboardPosition[1] + rightVector[1] * halfWidth + upVector[1] * halfHeight,
        billboardPosition[2] + rightVector[2] * halfWidth + upVector[2] * halfHeight
    };

    float corner3[3] = {
        billboardPosition[0] + rightVector[0] * halfWidth - upVector[0] * halfHeight,
        billboardPosition[1] + rightVector[1] * halfWidth - upVector[1] * halfHeight,
        billboardPosition[2] + rightVector[2] * halfWidth - upVector[2] * halfHeight
    };

    float corner4[3] = {
        billboardPosition[0] - rightVector[0] * halfWidth - upVector[0] * halfHeight,
        billboardPosition[1] - rightVector[1] * halfWidth - upVector[1] * halfHeight,
        billboardPosition[2] - rightVector[2] * halfWidth - upVector[2] * halfHeight
    };

    float Vertices[] = {corner1[0],corner1[1],corner1[2], corner2[0],corner2[1],corner2[2], corner3[0],corner3[1],corner3[2], corner4[0],corner4[1],corner4[2],};
    float Texture_Vertices[] = {0, 1, 1, 1, 1, 0, 0, 0};

    glVertexPointer(3, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
}




void Render_Image_With_Vertex_Color(struct _Image* Image, float _x, float _y, int Alpha_Present, float R, float G, float B)
{
glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->Width, Image->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    //Image->Texture_Made = 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    float Vertices[] = {_x, _y, _x + Image->Width, _y, _x + Image->Width, _y + Image->Height, _x, _y + Image->Height};
    float Texture_Vertices[] = {0, 0, 1, 0, 1, -1, 0, -1};
    glColor3f(R,G,B);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(2, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Copy_Pixel(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y)
{
/* TIMES 3!??
 * COPY PIXELS
 * 1. Calculate Source and Destination array Sizes
 * 2. Calculate Source and Destination array locations
 * 3. Send Pixel
 */
    int Source_Array_Size;
    int Destination_Array_Size;
    int Source_Array_Location;
    int Destination_Array_Location;

    Source_Array_Size = Source_Width * Source_Height * Color_Depth;
    Destination_Array_Size = Destination_Width * Destination_Height * Color_Depth;

    Source_Array_Location = (Source_Y * ( Source_Width * Color_Depth)) + (Source_X * Color_Depth);
    Destination_Array_Location = (Destination_Y * ( Destination_Width * Color_Depth)) + (Destination_X * Color_Depth);

    if(Color_Depth == 1)
    {
        memcpy(&Destination[Destination_Array_Location], &Source[Source_Array_Location], 1);

        //Destination[Destination_Array_Location] = Source[Source_Array_Location];
    }

    else if(Color_Depth != 1)
    {
        memcpy(&Destination[Destination_Array_Location], &Source[Source_Array_Location], 1);
        memcpy(&Destination[Destination_Array_Location+1], &Source[Source_Array_Location+1], 1);
        memcpy(&Destination[Destination_Array_Location+2], &Source[Source_Array_Location+2], 1);
    }
}

void Copy_Pixel_Alpha(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y)
{
    int Source_Array_Size;
    int Destination_Array_Size;
    int Source_Array_Location;
    int Destination_Array_Location;

    Source_Array_Size = Source_Width * Source_Height * Color_Depth;
    Destination_Array_Size = Destination_Width * Destination_Height * Color_Depth;

    Source_Array_Location = (Source_Y * ( Source_Width * Color_Depth)) + (Source_X * Color_Depth);
    Destination_Array_Location = (Destination_Y * ( Destination_Width * Color_Depth)) + (Destination_X * Color_Depth);

    //printf("Source Array Location: %d\n",Source_Array_Location);
    //printf("Dest Array Location: %d\n",Destination_Array_Location);

    if(Color_Depth == 1)
    {
        memcpy(&Destination[Destination_Array_Location], &Source[Source_Array_Location], 1);

        //Destination[Destination_Array_Location] = Source[Source_Array_Location];
    }

    else if(Color_Depth != 1)
    {
        memcpy(&Destination[Destination_Array_Location], &Source[Source_Array_Location], 1);
        memcpy(&Destination[Destination_Array_Location+1], &Source[Source_Array_Location+1], 1);
        memcpy(&Destination[Destination_Array_Location+2], &Source[Source_Array_Location+2], 1);
        memcpy(&Destination[Destination_Array_Location+3], &Source[Source_Array_Location+3], 1);
    }
}

void Copy_Row(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y, int N_Pixels_To_Copy)
{
    int Line_Counter = 0;
    while(Line_Counter < N_Pixels_To_Copy)
    {
        if(Color_Depth == 4)
        {
        Copy_Pixel_Alpha(4,Source,Destination,Source_Width,Source_Height,Destination_Width,Destination_Height,Source_X+Line_Counter,Source_Y,Destination_X+Line_Counter,Destination_Y);
        Line_Counter += 1;
        }

        else if(Color_Depth == 3)
        {
        Copy_Pixel(3,Source,Destination,Source_Width,Source_Height,Destination_Width,Destination_Height,Source_X+Line_Counter,Source_Y,Destination_X+Line_Counter,Destination_Y);
        Line_Counter += 1;
        }

        else if(Color_Depth == 1)
        {
        Copy_Pixel(1,Source,Destination,Source_Width,Source_Height,Destination_Width,Destination_Height,Source_X+Line_Counter,Source_Y,Destination_X+Line_Counter,Destination_Y);
        Line_Counter += 1;
        }

    }
}

void Copy_Section(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y, int N_Pixels_To_Copy, int Rows_To_Copy)
{
    int Row_Counter = 0;
    while(Row_Counter < Rows_To_Copy)
    {
        if(Color_Depth == 4)
        {
        Copy_Row(4,Source,Destination,Source_Width,Source_Height,Destination_Width,Destination_Height,Source_X,Source_Y+Row_Counter,Destination_X,Destination_Y+Row_Counter,N_Pixels_To_Copy);
        Row_Counter += 1;
        }

        else if(Color_Depth == 3)
        {
        Copy_Row(3,Source,Destination,Source_Width,Source_Height,Destination_Width,Destination_Height,Source_X,Source_Y+Row_Counter,Destination_X,Destination_Y+Row_Counter,N_Pixels_To_Copy);
        Row_Counter += 1;
        }

        else if(Color_Depth == 1)
        {
        Copy_Row(1,Source,Destination,Source_Width,Source_Height,Destination_Width,Destination_Height,Source_X,Source_Y+Row_Counter,Destination_X,Destination_Y+Row_Counter,N_Pixels_To_Copy);
        Row_Counter += 1;
        }
    }
}

void Create_BIN()
{

}

void Load_BIN()
{

}

unsigned char* Convert_RGB_To_RGBA(unsigned char* Input, int Input_Width, int Input_Height)
{
    int Input_Size = (Input_Width+1) * (Input_Height+1);

    unsigned char* Result = (unsigned char*) calloc ((Input_Size * 4),sizeof(unsigned char));

    int in = 0;
    int r = 0;
    for(int j = 0; j < 64; j++)
    {
        Result[j] = 32;

    }
    return Result;
}


void Render_Billboard(struct _Image* Image, float _x, float _y, Vector Camera_Vector, int Alpha_Present)
{

};

void Render_Animated_Image(struct _Image* Image, float _x, float _y, int Alpha_Present, int Cell_Width, int Cell_Height, int Cell_Y, int Frame, int Total_Frames, float Rate)
{

};


void Write_TwoD_Sprite_Animation(const char* Filename)
{
    struct _TwoD_Sprite_Animation_Header Header;

    memset(Header.Sprite_Sheet_Filename, 0,256);
    memset(Header.Sprite_Sheet_Alpha_Channel_Filename, 0,256);
    sprintf(Header.Sprite_Sheet_Filename, "explosion_bubble.bmp");
    sprintf(Header.Sprite_Sheet_Alpha_Channel_Filename, "explosion_bubble.trm");
    Header.Sprite_Sheet_Filename[20] = '\0';
    Header.Sprite_Sheet_Alpha_Channel_Filename[20] = '\0';
    Header.Width = 640;
    Header.Height = 64;
    Header.Cel_X_Division = 0.1; //If 8 frames, this is 1/8 = 0.125
    Header.Cel_Y_Division = 0.1; //If 8 frames, this is 1/8 = 0.125
    Header.Number_Of_Animations = 1;
    Header.Offset_To_Animations = sizeof(struct _TwoD_Sprite_Animation_Header);

    struct _TwoD_Sprite_Animation_Parameters Animation;

    Animation.Is_A_Billboard = 1;
    Animation.Number_Of_Frames = 10;
    Animation.Cel_X_ID = 0; //If I want to start mid-animation.
    Animation.Cel_Y_ID = 0;
    Animation.Animation_Speed = 1;
    Animation.Loop = 0;
    Animation.Rotational_Velocity = 0;
    Animation.Rotational_Acceleration = 0;
    Animation.Billboard_Scale = 1;
    Animation.Scale_Velocity = 0;
    Animation.Scale_Acceleration = 0;

    Animation.x = 0;
    Animation.y = 0;
    Animation.z = 0;

    Animation.dx = 1;
    Animation.dy = 0;
    Animation.dz = 0;

    Animation.ddx = 0;
    Animation.ddy = 0;
    Animation.ddz = 0;

    Animation.Loop_Motion_Once_Too_Far = 1;
    Animation.Maximum_Distance_From_Origin = 30;

    Animation.Start_x = 0;
    Animation.Start_y = 0;
    Animation.Start_z = 0;

    Animation.Rotation=0;

    FILE* f;
    f = fopen(Filename, "wb");
    fwrite(&Header, 1, sizeof(struct _TwoD_Sprite_Animation_Header),f);
    rewind(f);

    fseek(f, Header.Offset_To_Animations, SEEK_SET);
    fwrite(&Animation, 1, sizeof(struct _TwoD_Sprite_Animation_Parameters),f);
    fclose(f);
};

void Load_TwoD_Sprite_Animation(struct _TwoD_Sprite_Object* TwoD_Sprite_Object, const char* Filename, unsigned int ID)
{
    FILE* f;
    f = fopen(Filename, "rb");
    long File_Size;
    fseek(f, 0, SEEK_END);
    File_Size = ftell(f);
    rewind(f);
    unsigned char Buffer[File_Size];
    fread(&Buffer, sizeof(unsigned char), File_Size, f);
    fclose(f);
    long Buffer_Location = 0;
    int Current_Buffer_Location =0;

    char Sprite_Filepath[256];
    char Transparency_Filepath[256];

    for(int j = 0; j < 256; j++)
    {
        Sprite_Filepath[j] = Buffer[j];
    }

    FILE * file;
    file = fopen(Sprite_Filepath,"rb");

    printf(Sprite_Filepath);

    unsigned char header[54];

    fread(header, 1, 54, file);

    TwoD_Sprite_Object->Width = *(int*)&(header[0x12]);
    TwoD_Sprite_Object->Height = *(int*)&(header[0x16]);

    int RGB_Image_Size = ((TwoD_Sprite_Object->Width) * (TwoD_Sprite_Object->Height) * 3);

    int RGBA_Image_Size = ((TwoD_Sprite_Object->Width) * (TwoD_Sprite_Object->Height) * 4);

    fseek(file, 54, SEEK_SET);

    TwoD_Sprite_Object->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size,sizeof(unsigned char));



    TwoD_Sprite_Object->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size,sizeof(unsigned char));
    memset(TwoD_Sprite_Object->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(TwoD_Sprite_Object->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);
    //Image->RGBA_Canvas = Convert_RGB_To_RGBA(Image->RGB_Canvas,Image->Width, Image->Height);
    int in = 0;
    int r = 0;
    //Image->RGBA_Canvas[0] = Image->RGB_Canvas[0];
    for(int j = 0; j < (RGBA_Image_Size/4); j++)
    {

        TwoD_Sprite_Object->RGBA_Canvas[r] = TwoD_Sprite_Object->RGB_Canvas[in+2];
        TwoD_Sprite_Object->RGBA_Canvas[r + 1] = TwoD_Sprite_Object->RGB_Canvas[in + 1];
        TwoD_Sprite_Object->RGBA_Canvas[r + 2] = TwoD_Sprite_Object->RGB_Canvas[in];
        TwoD_Sprite_Object->RGBA_Canvas[r + 3] = 255;
        if(TwoD_Sprite_Object->RGBA_Canvas[r] == 200)
        {
            if(TwoD_Sprite_Object->RGBA_Canvas[r+1] == 200)
            {
                if(TwoD_Sprite_Object->RGBA_Canvas[r + 2] == 200)
                {
                    TwoD_Sprite_Object->RGBA_Canvas[r + 3] = 0;
                }
            }
        }
        in += 3;
        r += 4;
    }

    // Generate a texture ID
    glGenTextures(1, &TwoD_Sprite_Object->Texture_ID);
    // Bind the texture to GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, TwoD_Sprite_Object->Texture_ID);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TwoD_Sprite_Object->Width, TwoD_Sprite_Object->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TwoD_Sprite_Object->RGBA_Canvas);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);


    TwoD_Sprite_Object->Width = *(int*)&(Buffer[256+256]);
    TwoD_Sprite_Object->Height = *(int*)&(Buffer[256+256+4]);
    TwoD_Sprite_Object->Cel_X_Division = *(float*)&(Buffer[256+256+4+4]);
    TwoD_Sprite_Object->Cel_Y_Division = *(float*)&(Buffer[256+256+4+4+4]);
    TwoD_Sprite_Object->Number_Of_Animations = *(int*)&(Buffer[256+256+4+4+4+4]);

    printf("Cel X Division: %f\n", TwoD_Sprite_Object->Cel_X_Division);



    TwoD_Sprite_Object->Animations = (struct _TwoD_Sprite_Animation_Parameters*)calloc(TwoD_Sprite_Object->Number_Of_Animations, sizeof(struct _TwoD_Sprite_Animation_Parameters));

    printf("Animations: %d\n", TwoD_Sprite_Object->Number_Of_Animations);

    for(int j = 0; j < TwoD_Sprite_Object->Number_Of_Animations; j++) //272
    {
        Current_Buffer_Location = (sizeof(struct _TwoD_Sprite_Animation_Header)) + (j*sizeof(struct _TwoD_Sprite_Animation_Parameters));

        TwoD_Sprite_Object->Animations[j].Is_A_Billboard = *(int*)&(Buffer[Current_Buffer_Location]);
        TwoD_Sprite_Object->Animations[j].Number_Of_Frames = *(int*)&(Buffer[Current_Buffer_Location+4]);
        TwoD_Sprite_Object->Animations[j].Cel_X_ID = *(int*)&(Buffer[Current_Buffer_Location+8]); //If I want to start mid-animation.
        TwoD_Sprite_Object->Animations[j].Cel_Y_ID = *(int*)&(Buffer[Current_Buffer_Location+12]);
        TwoD_Sprite_Object->Animations[j].Animation_Speed = *(float*)&(Buffer[Current_Buffer_Location+16]);
        TwoD_Sprite_Object->Animations[j].Loop = *(int*)&(Buffer[Current_Buffer_Location+20]);

        TwoD_Sprite_Object->Animations[j].Rotational_Velocity = *(float*)&(Buffer[Current_Buffer_Location+24]);
        TwoD_Sprite_Object->Animations[j].Rotational_Acceleration = *(float*)&(Buffer[Current_Buffer_Location+28]);
        TwoD_Sprite_Object->Animations[j].Billboard_Scale = *(float*)&(Buffer[Current_Buffer_Location+32]);
        TwoD_Sprite_Object->Animations[j].Scale_Velocity = *(float*)&(Buffer[Current_Buffer_Location+36]);
        TwoD_Sprite_Object->Animations[j].Scale_Acceleration = *(float*)&(Buffer[Current_Buffer_Location+40]);

        TwoD_Sprite_Object->Animations[j].x = *(float*)&(Buffer[Current_Buffer_Location+44]);
        TwoD_Sprite_Object->Animations[j].y = *(float*)&(Buffer[Current_Buffer_Location+48]);
        TwoD_Sprite_Object->Animations[j].z = *(float*)&(Buffer[Current_Buffer_Location+52]);

        TwoD_Sprite_Object->Animations[j].dx = *(float*)&(Buffer[Current_Buffer_Location+56]);
        TwoD_Sprite_Object->Animations[j].dy = *(float*)&(Buffer[Current_Buffer_Location+60]);
        TwoD_Sprite_Object->Animations[j].dz = *(float*)&(Buffer[Current_Buffer_Location+64]);

        TwoD_Sprite_Object->Animations[j].ddx = *(float*)&(Buffer[Current_Buffer_Location+68]);
        TwoD_Sprite_Object->Animations[j].ddy = *(float*)&(Buffer[Current_Buffer_Location+72]);
        TwoD_Sprite_Object->Animations[j].ddz = *(float*)&(Buffer[Current_Buffer_Location+76]);

        TwoD_Sprite_Object->Animations[j].Loop_Motion_Once_Too_Far = *(int*)&(Buffer[Current_Buffer_Location+80]);
        TwoD_Sprite_Object->Animations[j].Maximum_Distance_From_Origin = *(float*)&(Buffer[Current_Buffer_Location+84]);

        TwoD_Sprite_Object->Animations[j].Start_x = *(float*)&(Buffer[Current_Buffer_Location+88]);
        TwoD_Sprite_Object->Animations[j].Start_y = *(float*)&(Buffer[Current_Buffer_Location+92]);
        TwoD_Sprite_Object->Animations[j].Start_z = *(float*)&(Buffer[Current_Buffer_Location+96]);

        TwoD_Sprite_Object->Animations[j].Rotation = *(float*)&(Buffer[Current_Buffer_Location+100]);
    }

    TwoD_Sprite_Object->Current_Start_Cel = TwoD_Sprite_Object->Animations->Cel_X_ID;
    TwoD_Sprite_Object->Current_End_Cel = TwoD_Sprite_Object->Animations->Number_Of_Frames-1;
    TwoD_Sprite_Object->Current_Cel = TwoD_Sprite_Object->Current_Start_Cel;
};

void Render_TwoD_Sprite_Animation(struct _TwoD_Sprite_Object* TwoD_Sprite_Object, float x, float y)
{
    x = TwoD_Sprite_Object->Animations->x;
    y = TwoD_Sprite_Object->Animations->y;

    glBindTexture(GL_TEXTURE_2D, TwoD_Sprite_Object->Texture_ID);

    //glPushMatrix();
      //      glRotatef(23,0,0,1);



    float Vertices[] = {x, y, x + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Width*TwoD_Sprite_Object->Cel_X_Division), y, x + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Width*TwoD_Sprite_Object->Cel_X_Division), y + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Height), x, y + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Height)};

    float Texture_Vertex_X_One = TwoD_Sprite_Object->Cel_X_Division * TwoD_Sprite_Object->Current_Cel; //Top left corner
    float Texture_Vertex_X_Two = Texture_Vertex_X_One + TwoD_Sprite_Object->Cel_X_Division;

  //  printf("Texture Vertex X One: %f\n", Texture_Vertex_X_One);
   // printf("Texture Vertex X Two: %f\n", Texture_Vertex_X_Two);


    float Texture_Vertex_Y_One = TwoD_Sprite_Object->Cel_Y_Division * TwoD_Sprite_Object->Animations->Cel_Y_ID;
    float Texture_Vertex_Y_Two = Texture_Vertex_Y_One + TwoD_Sprite_Object->Cel_Y_Division;


    float Texture_Vertices[] = {Texture_Vertex_X_One, 0, Texture_Vertex_X_Two, 0, Texture_Vertex_X_Two, -1, Texture_Vertex_X_One, -1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(2, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

   // glPopMatrix();



    //float Vertices[] = {x, y, x + TwoD_Sprite_Object->Width*TwoD_Sprite_Object->Cel_X_Division, y, x + TwoD_Sprite_Object->Width*TwoD_Sprite_Object->Cel_X_Division, y + TwoD_Sprite_Object->Height*TwoD_Sprite_Object->Cel_X_Division, x, y + TwoD_Sprite_Object->Height*TwoD_Sprite_Object->Cel_X_Division};
    //float Texture_Vertices[] = {0, 0, 1, 0, 1, -1, 0, -1};
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    //glVertexPointer(2, GL_FLOAT, 0, Vertices);
    //glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    //glDrawArrays(GL_QUADS, 0, 4);
    //glDisableClientState(GL_VERTEX_ARRAY);
};

void Process_TwoD_Sprite_Animation(struct _TwoD_Sprite_Object* TwoD_Sprite_Object)
{
    TwoD_Sprite_Object->Animations->x += TwoD_Sprite_Object->Animations->dx;
    TwoD_Sprite_Object->Animations->y += TwoD_Sprite_Object->Animations->dy;
    TwoD_Sprite_Object->Animations->z += TwoD_Sprite_Object->Animations->dz;

    TwoD_Sprite_Object->Animations->dx += TwoD_Sprite_Object->Animations->ddx;
    TwoD_Sprite_Object->Animations->dy += TwoD_Sprite_Object->Animations->ddy;
    TwoD_Sprite_Object->Animations->dz += TwoD_Sprite_Object->Animations->ddz;

    TwoD_Sprite_Object->Animations->Rotation += TwoD_Sprite_Object->Animations->Rotational_Velocity;
    TwoD_Sprite_Object->Animations->Rotational_Velocity += TwoD_Sprite_Object->Animations->Rotational_Acceleration;
    TwoD_Sprite_Object->Animations->Billboard_Scale += TwoD_Sprite_Object->Animations->Scale_Velocity;
    TwoD_Sprite_Object->Animations->Scale_Velocity += TwoD_Sprite_Object->Animations->Scale_Acceleration;


    TwoD_Sprite_Object->Current_Cel += 1;
    if(TwoD_Sprite_Object->Current_Cel > TwoD_Sprite_Object->Current_End_Cel)
    {
        TwoD_Sprite_Object->Current_Start_Cel;
    }

    float Distance = Get_Distance(TwoD_Sprite_Object->Animations->x,TwoD_Sprite_Object->Animations->y,TwoD_Sprite_Object->Animations->z,TwoD_Sprite_Object->Animations->Start_x,TwoD_Sprite_Object->Animations->Start_y,TwoD_Sprite_Object->Animations->Start_z);

    //printf("Distance: %f\n", Distance);

    if(Distance >= TwoD_Sprite_Object->Animations->Maximum_Distance_From_Origin)
    {
        if(TwoD_Sprite_Object->Animations->Loop_Motion_Once_Too_Far == 1)
        {
            TwoD_Sprite_Object->Animations->x = TwoD_Sprite_Object->Animations->Start_x;
            TwoD_Sprite_Object->Animations->y = TwoD_Sprite_Object->Animations->Start_y;
            TwoD_Sprite_Object->Animations->z = TwoD_Sprite_Object->Animations->Start_z;
        }
    }

};



















void Render_TwoD_Sprite_Animation_As_Billboard(struct _TwoD_Sprite_Object* TwoD_Sprite_Object, float x, float y, float z,float Width, float Height)
{

      // 1. Get the current Modelview matrix

    // Billboard properties
float billboardWidth = Width;
float billboardHeight = Height;
float billboardPosition[3] = {x, y, z}; // Example position

// Camera position and orientation (for demonstration)
float cameraPosition[3] = {0.0f, 0.0f, 0.0f};
float cameraRotationY = 0.0f;

    float modelviewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);

    // 2. Extract the right (x) and up (y) vectors of the camera
    float rightVector[3] = {modelviewMatrix[0], modelviewMatrix[4], modelviewMatrix[8]};
    float upVector[3] = {modelviewMatrix[1], modelviewMatrix[5], modelviewMatrix[9]};

    // 3. Calculate the billboard's corners
    float halfWidth = billboardWidth / 2.0f;
    float halfHeight = billboardHeight / 2.0f;

    float corner1[3] = {
        billboardPosition[0] - rightVector[0] * halfWidth + upVector[0] * halfHeight,
        billboardPosition[1] - rightVector[1] * halfWidth + upVector[1] * halfHeight,
        billboardPosition[2] - rightVector[2] * halfWidth + upVector[2] * halfHeight
    };

    float corner2[3] = {
        billboardPosition[0] + rightVector[0] * halfWidth + upVector[0] * halfHeight,
        billboardPosition[1] + rightVector[1] * halfWidth + upVector[1] * halfHeight,
        billboardPosition[2] + rightVector[2] * halfWidth + upVector[2] * halfHeight
    };

    float corner3[3] = {
        billboardPosition[0] + rightVector[0] * halfWidth - upVector[0] * halfHeight,
        billboardPosition[1] + rightVector[1] * halfWidth - upVector[1] * halfHeight,
        billboardPosition[2] + rightVector[2] * halfWidth - upVector[2] * halfHeight
    };

    float corner4[3] = {
        billboardPosition[0] - rightVector[0] * halfWidth - upVector[0] * halfHeight,
        billboardPosition[1] - rightVector[1] * halfWidth - upVector[1] * halfHeight,
        billboardPosition[2] - rightVector[2] * halfWidth - upVector[2] * halfHeight
    };

    // 4. Render the billboard as a quad
    glBegin(GL_QUADS);
        // Assuming you have texture coordinates (uv) associated with your billboard
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(corner1);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(corner2);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(corner3);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(corner4);
    glEnd();





    x = TwoD_Sprite_Object->Animations->x;
    y = TwoD_Sprite_Object->Animations->y;

    glBindTexture(GL_TEXTURE_2D, TwoD_Sprite_Object->Texture_ID);

    float Vertices[] = {x, y, x + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Width*TwoD_Sprite_Object->Cel_X_Division), y, x + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Width*TwoD_Sprite_Object->Cel_X_Division), y + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Height), x, y + TwoD_Sprite_Object->Animations->Billboard_Scale*(TwoD_Sprite_Object->Height)};

    float Texture_Vertex_X_One = TwoD_Sprite_Object->Cel_X_Division * TwoD_Sprite_Object->Current_Cel; //Top left corner
    float Texture_Vertex_X_Two = Texture_Vertex_X_One + TwoD_Sprite_Object->Cel_X_Division;

   // printf("Texture Vertex X One: %f\n", Texture_Vertex_X_One);
   // printf("Texture Vertex X Two: %f\n", Texture_Vertex_X_Two);


    float Texture_Vertex_Y_One = TwoD_Sprite_Object->Cel_Y_Division * TwoD_Sprite_Object->Animations->Cel_Y_ID;
    float Texture_Vertex_Y_Two = Texture_Vertex_Y_One + TwoD_Sprite_Object->Cel_Y_Division;


    float Texture_Vertices[] = {Texture_Vertex_X_One, 0, Texture_Vertex_X_Two, 0, Texture_Vertex_X_Two, -1, Texture_Vertex_X_One, -1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
    glVertexPointer(2, GL_FLOAT, 0, Vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, Texture_Vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
};

void Load_Image_And_Convert_To_Bitmap(const char* Filename, const char* Output)
{
    FILE * file;
    file = fopen(Filename,"rb");
    unsigned char header[54];
    fread(header, 1, 54, file);
    int Width = *(int*)&(header[0x12]);
    int Height = *(int*)&(header[0x16]);

    int RGB_Image_Size = ((Width) * (Height) * 3);

    unsigned char* RGB_Canvas;

    fseek(file, 54, SEEK_SET);
    RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size,sizeof(unsigned char));
    memset(RGB_Canvas,0,RGB_Image_Size);
    fread(RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);

    unsigned char Output_Width = (unsigned char)Width;
    unsigned char Output_Height = (unsigned char)Height;
    unsigned char Color_Mode = Color_Mode;

    unsigned char Canvas[Output_Width * Output_Height];

    memset(Canvas,0,Output_Width * Output_Height);

    printf("Width: %d\n", Output_Width);


    int l = 0;
    int j = 0;

    while(l < (Output_Width * Output_Height))
    {
        Canvas[l] = RGB_Canvas[j];
        l+=1;
        j+=3;
    }


    FILE* fOut;
    fOut = fopen(Output, "wb");
    fwrite(&Output_Width,1,sizeof(unsigned char),fOut);
    fwrite(&Output_Height,1,sizeof(unsigned char),fOut);
    fwrite(&Color_Mode,1,sizeof(unsigned char),fOut);
    fwrite(Canvas,1,Output_Width * Output_Height,fOut);
    fclose(fOut);
};








void Load_Bitmap_And_Convert_To_Image(const char* Filename, struct _Image* Image)
{
    FILE * file;
    file = fopen(Filename,"rb");
    if(!file)
    {
        file = fopen("resources/artwork/placeholders/no_image.bmp", "rb");
    }

    unsigned char header[3];

    fread(header, 1, 3, file);

    Image->Width = *(unsigned char*)&(header[0x00]);
    Image->Height = *(unsigned char*)&(header[0x01]);

    unsigned char Color_Mode = *(unsigned char*)&(header[0x02]);

    printf("Loaded Image Width: %d\n", Image->Width);
    printf("Loaded Image Height: %d\n", Image->Height);

    int RGB_Image_Size = ((Image->Width) * (Image->Height));
    int RGBA_Image_Size = ((Image->Width) * (Image->Height) * 4);

    fseek(file, 3, SEEK_SET);
    Image->RGB_Canvas = (unsigned char*) calloc (RGB_Image_Size,sizeof(unsigned char));
    Image->RGBA_Canvas = (unsigned char*) calloc (RGBA_Image_Size,sizeof(unsigned char));

    memset(Image->RGB_Canvas, 0, RGB_Image_Size);
    memset(Image->RGBA_Canvas, 0, RGBA_Image_Size);
    fread(Image->RGB_Canvas,1,RGB_Image_Size,file);
    fclose(file);
    //Image->RGBA_Canvas = Convert_RGB_To_RGBA(Image->RGB_Canvas,Image->Width, Image->Height);



    int in = 0;
    int r = 0;
    //Image->RGBA_Canvas[0] = Image->RGB_Canvas[0];
    for(int j = 0; j < (RGBA_Image_Size/4); j++)
    {
        Image->RGBA_Canvas[r] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 1] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 2] = Image->RGB_Canvas[in];
        Image->RGBA_Canvas[r + 3] = 255;
        in += 1;
        r += 4;
    }
    glGenTextures(1, &Image->Texture_ID);
    glBindTexture(GL_TEXTURE_2D, Image->Texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->Width, Image->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->RGBA_Canvas);
    glEnable(GL_TEXTURE_2D);
};


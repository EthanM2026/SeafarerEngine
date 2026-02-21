/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   image.h
 * Author: drew
 *
 * Created on August 12, 2022, 6:01 PM
 */

#ifndef IMAGE_H
#define IMAGE_H
#include <stdlib.h>
#include <GL/glew.h>"
#include "stdio.h"
#include "../physics/vector.h"
#include "../physics/physics.h"

    struct _TwoD_Sprite_Object
    {
        GLuint Texture_ID;
        int RGBA_Image_Size;
        unsigned char Texture_Made;
        unsigned char* RGB_Canvas;
        unsigned char* RGBA_Canvas;
        float* Vertices;
        float* Texture_Vertices;

        int Width;
        int Height;
        float Cel_X_Division; //If 8 frames, this is 1/8 = 0.125
        float Cel_Y_Division; //If 8 frames, this is 1/8 = 0.125
        int Number_Of_Animations;

        int Current_Start_Cel;
        int Current_End_Cel;
        int Current_Cel;

        struct _TwoD_Sprite_Animation_Parameters* Animations;
    };

    struct _TwoD_Sprite_Animation_Header
    {
        char Sprite_Sheet_Filename[256];
        char Sprite_Sheet_Alpha_Channel_Filename[256];
        int Width;
        int Height;
        float Cel_X_Division; //If 8 frames, this is 1/8 = 0.125
        float Cel_Y_Division; //If 8 frames, this is 1/8 = 0.125
        int Number_Of_Animations;
        int Offset_To_Animations;
    };

    struct _TwoD_Sprite_Animation_Parameters
    {
        int Is_A_Billboard;
        int Number_Of_Frames;
        int Cel_X_ID; //If I want to start mid-animation.
        int Cel_Y_ID;
        float Animation_Speed;
        int Loop;
        float Rotational_Velocity;
        float Rotational_Acceleration;
        float Billboard_Scale;
        float Scale_Velocity;
        float Scale_Acceleration;

        float x;
        float y;
        float z;

        float dx;
        float dy;
        float dz;

        float ddx;
        float ddy;
        float ddz;

        int Loop_Motion_Once_Too_Far;
        float Maximum_Distance_From_Origin;

        float Start_x;
        float Start_y;
        float Start_z;

        float Rotation;
    };

    struct _Palette_Color
    {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char Transparency;
    };
    struct _Palette
    {
        struct _Palette_Color Colors[16];
    };

    struct _Palette_Binary_Output_Surface
    {
        int Width;
        int Height;
        unsigned char* Pixels;
        //w * h /2 rounded up
        // 4 bits per pixel
        // 1-3 = 8 colors from palette
        // 4 = is transparent?
    };
    struct _Image
    {
        GLuint Texture_ID;
        int RGBA_Image_Size;
        unsigned char Texture_Made;
        int x;
        int y;
        int Width;
        int Height;
        unsigned char* RGB_Canvas;
        unsigned char* RGBA_Canvas;
        float* Vertices;
        float* Texture_Vertices;
    };


    struct _Image* Create_Image();
    void Destroy_Image(struct _Image* Image);
    void Load_Image(struct _Image* Image, const char* Filename);
    void Initialize_Binary_Output_Surface(struct _Image* Image);
    void Palettize_Image();

    void Load_Binary_Output_Surface(struct _Image* Image, const char* Filename);
    void Render_Image(struct _Image* Image, float _x, float _y, int Alpha_Present);

    void Render_Animated_Image(struct _Image* Image, float _x, float _y, int Alpha_Present, int Cell_Width, int Cell_Height, int Cell_Y, int Frame, float Rate);

    void Create_BIN();
    void Load_BIN();

    unsigned char* Convert_RGB_To_RGBA(unsigned char* Input, int Input_Width, int Input_Height);

    void Copy_Pixel(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y);
    void Copy_Pixel_Alpha(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y);
    void Copy_Row(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y, int N_Pixels_To_Copy); //do Copy Pixel Width times
    void Copy_Section(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y, int N_Pixels_To_Copy, int Rows_To_Copy); //do Copy_Row Height times

    void Change_Three_Color_And_Alpha_Bit_Palette(unsigned char* RGB_Canvas, unsigned char New_Colors[8]);
void Change_Palette(struct _Image* Image, struct _Palette Old_Colors, struct _Palette New_Colors);

void Load_Palette(struct _Palette* P, const char* Filename);
void Write_Palette(const char* Filename);


void Render_Billboard(struct _Image* Image, float _x, float _y, Vector Camera_Vector, int Alpha_Present);

    void New_Copy_Section(int Color_Depth, unsigned char* Source, unsigned char* Destination, int Source_Width, int Source_Height, int Destination_Width, int Destination_Height, int Source_X, int Source_Y, int Destination_X, int Destination_Y, int N_Pixels_To_Copy, int Rows_To_Copy); //do Copy_Row Height

    void Render_Image_With_Vertex_Color(struct _Image* Image, float _x, float _y, int Alpha_Present, float R, float G, float B);

    void Render_Image_As_Billboard(struct _Image* Image, float _x, float _y, float _z, int Alpha_Present, float Scale);

void Write_TwoD_Sprite_Animation(const char* Filename);
void Load_TwoD_Sprite_Animation(struct _TwoD_Sprite_Object* TwoD_Sprite_Object, const char* Filename, unsigned int ID);
void Render_TwoD_Sprite_Animation(struct _TwoD_Sprite_Object* TwoD_Sprite_Object, float x, float y);
void Process_TwoD_Sprite_Animation(struct _TwoD_Sprite_Object* TwoD_Sprite_Object);
void Update_TwoD_Sprite_Animation(float New_Animation_Speed,
        int New_Loop,
        float New_Rotational_Velocity,
        float New_Rotational_Acceleration,
        float New_Billboard_Scale,
        float New_Scale_Velocity,
        float New_Scale_Acceleration,

        float New_x,
        float New_y,
        float New_z,

        float New_dx,
        float New_dy,
        float New_dz,

        float New_ddx,
        float New_ddy,
        float New_ddz);

void Load_Image_And_Convert_To_Bitmap(const char* Filename, const char* Output);
void Load_Bitmap_And_Convert_To_Image(const char* Filename, struct _Image* Image);

#endif /* IMAGE_H */




/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    double x, y, z;
} Vec3;

typedef struct {
    double x, y;
} Vec2;

typedef struct {
    Vec3 min; // Minimum corner
    Vec3 max; // Maximum corner
} AABB;

struct _Precalculated_Normal
{
    float Coordinates[3];
};

struct _Precalculated_Dot_Product
{
    float Coordinates[256];
};

struct _S_Vector
{
    float i;
    float j;
    float k;
};

typedef float Vector[3];


float Calculate_Shock_Factor(float Slant_Range, float TNT_Charge, float Depression_Angle);

double Dot_Product(double iOne, double iTwo, double jOne, double jTwo, double kOne, double kTwo);
struct _S_Vector* Unit_Vector(float i, float j, float k);
struct _S_Vector* Cross_Product(double iOne, double iTwo, double jOne, double jTwo, double kOne, double kTwo);
double Vector_Magnitude(double DotProduct);




#endif /* VECTOR_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   vector.h
 * Author: drew
 *
 * Created on August 7, 2023, 9:40 PM
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


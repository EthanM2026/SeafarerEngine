/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "vector.h"
#include "math.h"
float Calculate_Shock_Factor(float Slant_Range, float TNT_Charge, float Depression_Angle)
{
    float Result = (sqrt(TNT_Charge)/Slant_Range) * ((1+sin(Depression_Angle/180))/2);
    return Result;
}


double Dot_Product(double iOne, double iTwo, double jOne, double jTwo, double kOne, double kTwo)
{
    double Result = 0;

    Result += iOne * iTwo;
    Result += jOne * jTwo;
    Result += kOne * kTwo;

    return Result;
}
struct _S_Vector* Unit_Vector(float i, float j, float k)
{
    struct _S_Vector* Result = (struct _S_Vector*)calloc(1,sizeof(struct _S_Vector));
    //Step 1: Get the Dot Product of itself times itself
   // Step 2: Use the dot product to get the Vector magnitude
    //Step 3: Divide the

    double DotProd = Dot_Product(i,i,j,j,k,k);
    double Magnitude = Vector_Magnitude(DotProd);


    Result->i = i/Magnitude;
    Result->j = j/Magnitude;
    Result->k = k/Magnitude;

    return Result;
}
struct _S_Vector* Cross_Product(double iOne, double iTwo, double jOne, double jTwo, double kOne, double kTwo)
{
    struct _S_Vector* Result = (struct _S_Vector*)calloc(1,sizeof(struct _S_Vector));

    Result->i = (jOne * kTwo) - (kOne * jTwo);
    Result->j = (kOne * iTwo) - (iOne * kTwo);
    Result->k = (iOne * jTwo) - (jOne * iTwo);
    return Result;
}
double Vector_Magnitude(double DotProduct)
{
    double Result = 0;

    Result = sqrt(DotProduct * DotProduct);

    return Result;
}




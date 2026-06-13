#include "nn_train.h"

struct _Neural_Network_Training_State* Create_Neural_Network_Training_State()
{
    struct _Neural_Network_Training_State* NNTS = (struct _Neural_Network_Training_State*)calloc(1,sizeof(struct _Neural_Network_Training_State));
    return NNTS;
};

void Update_Short_Range_Sensors(struct _Engine* Engine)
{
    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[0] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Sensors[0] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[0] = 0;
        }

        l += 1;
    }
    //printf("Nose cone short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Sensors[0]);




    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[1] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Sensors[1] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[1] = 0;
        }
        l += 1;
    }
    //printf("Back cone short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Sensors[1]);















    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[2] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Sensors[2] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[2] = 0;
        }
        l += 1;
    }
    //printf("Left cone short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Sensors[2]);









    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[3] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Sensors[3] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[3] = 0;
        }
        l += 1;
    }
    //printf("Right cone short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Sensors[3]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[4] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Sensors[4] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[4] = 0;
        }
        l += 1;
    }
    //printf("Up cone short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Sensors[4]);







     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[5] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Sensors[5] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Sensors[5] = 0;
        }
        l += 1;
    }
    //printf("Down cone short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Sensors[5]);
}


void Update_Medium_Range_Sensors(struct _Engine* Engine)
{
    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[0] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[0] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[0] = 0;
        }

        l += 1;
    }
    //printf("Nose cone Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[0]);




    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[1] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[1] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[1] = 0;
        }
        l += 1;
    }
    //printf("Back cone Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[1]);















    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[2] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[2] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[2] = 0;
        }
        l += 1;
    }
    //printf("Left cone Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[2]);









    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[3] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[3] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[3] = 0;
        }
        l += 1;
    }
    //printf("Right cone Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[3]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[4] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[4] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[4] = 0;
        }
        l += 1;
    }
    //printf("Up cone Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[4]);







     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[5] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[5] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[5] = 0;
        }
        l += 1;
    }
    //printf("Down cone Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[5]);
}


void Update_Long_Range_Sensors(struct _Engine* Engine)
{
    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[0] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Sensors[0] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[0] = 0;
        }

        l += 1;
    }
    //printf("Nose cone Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Sensors[0]);




    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[1] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Sensors[1] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[1] = 0;
        }
        l += 1;
    }
    //printf("Back cone Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Sensors[1]);















    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[2] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Sensors[2] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[2] = 0;
        }
        l += 1;
    }
    //printf("Left cone Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Sensors[2]);









    l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[3] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Sensors[3] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Right_Vector.x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Right_Vector.y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Right_Vector.z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[3] = 0;
        }
        l += 1;
    }
    //printf("Right cone Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Sensors[3]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[4] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Sensors[4] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[4] = 0;
        }
        l += 1;
    }
    //printf("Up cone Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Sensors[4]);







     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[5] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Sensors[5] = 0;
            }
            l = 11;
        }

        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+Engine->Neural_Network_Training_State->Submarine->Up_Vector.x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+Engine->Neural_Network_Training_State->Submarine->Up_Vector.y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+Engine->Neural_Network_Training_State->Submarine->Up_Vector.z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Sensors[5] = 0;
        }
        l += 1;
    }
    //printf("Down cone Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Sensors[5]);
}



void Update_Short_Range_Diagonal_FR_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[0] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[0] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[0] = 0;
        }

        l += 1;
    }
    //printf("Right Forward short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[0]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[1] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[1] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[1] = 0;
        }

        l += 1;
    }
    //printf("Back Left short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[1]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[2] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[2] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[2] = 0;
        }

        l += 1;
    }
    //printf("Left Forward short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[2]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[3] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[3] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[3] = 0;
        }

        l += 1;
    }
    //printf("Back Right short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[3]);
}



void Update_Short_Range_Diagonal_UR_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[4] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[4] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[4] = 0;
        }

        l += 1;
    }
    //printf("Top Right short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[4]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[5] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[5] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[5] = 0;
        }

        l += 1;
    }
    //printf("Bottom Left short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[5]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[6] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[6] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[6] = 0;
        }

        l += 1;
    }
    //printf("Top Left short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[6]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[7] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[7] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[7] = 0;
        }

        l += 1;
    }
    //printf("Bottom Right short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[7]);
}



void Update_Short_Range_Diagonal_UF_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[8] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[8] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[8] = 0;
        }

        l += 1;
    }
    //printf("Top Forward short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[8]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[9] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[9] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[9] = 0;
        }

        l += 1;
    }
    //printf("Bottom Back short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[9]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[10] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[10] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[10] = 0;
        }

        l += 1;
    }
    //printf("Top Back short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[10]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[11] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[11] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->SRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[11] = 0;
        }

        l += 1;
    }
    //printf("Bottom Forward short distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[11]);
}




void Update_Medium_Range_Diagonal_FR_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[0] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[0] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[0] = 0;
        }

        l += 1;
    }
    //printf("Right Forward Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[0]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[1] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[1] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[1] = 0;
        }

        l += 1;
    }
    //printf("Back Left Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[1]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[2] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[2] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[2] = 0;
        }

        l += 1;
    }
    //printf("Left Forward Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[2]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[3] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[3] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[3] = 0;
        }

        l += 1;
    }
    //printf("Back Right Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[3]);
}



void Update_Medium_Range_Diagonal_UR_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[4] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[4] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[4] = 0;
        }

        l += 1;
    }
    //printf("Top Right Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[4]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[5] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[5] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[5] = 0;
        }

        l += 1;
    }
    //printf("Bottom Left Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[5]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[6] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[6] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[6] = 0;
        }

        l += 1;
    }
    //printf("Top Left Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[6]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[7] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[7] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[7] = 0;
        }

        l += 1;
    }
    //printf("Bottom Right Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[7]);
}



void Update_Medium_Range_Diagonal_UF_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[8] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[8] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[8] = 0;
        }

        l += 1;
    }
    //printf("Top Forward Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[8]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[9] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[9] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[9] = 0;
        }

        l += 1;
    }
    //printf("Bottom Back Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[9]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[10] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[10] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[10] = 0;
        }

        l += 1;
    }
    //printf("Top Back Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[10]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[11] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[11] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->MRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[11] = 0;
        }

        l += 1;
    }
    //printf("Bottom Forward Medium distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[11]);
}






void Update_Long_Range_Diagonal_FR_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[0] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[0] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[0] = 0;
        }

        l += 1;
    }
    //printf("Right Forward Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[0]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[1] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[1] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[1] = 0;
        }

        l += 1;
    }
    //printf("Back Left Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[1]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[2] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[2] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[2] = 0;
        }

        l += 1;
    }
    //printf("Left Forward Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[2]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[3] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[3] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[3] = 0;
        }

        l += 1;
    }
    //printf("Back Right Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[3]);
}



void Update_Long_Range_Diagonal_UR_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Right_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Right_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Right_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[4] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[4] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[4] = 0;
        }

        l += 1;
    }
    //printf("Top Right Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[4]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[5] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[5] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[5] = 0;
        }

        l += 1;
    }
    //printf("Bottom Left Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[5]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[6] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[6] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[6] = 0;
        }

        l += 1;
    }
    //printf("Top Left Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[6]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[7] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[7] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[7] = 0;
        }

        l += 1;
    }
    //printf("Bottom Right Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[7]);
}



void Update_Long_Range_Diagonal_UF_Sensors(struct _Engine* Engine)
{
    double FR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double FR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double FR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double FNR_Vector_x = Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double FNR_Vector_y = Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double FNR_Vector_z = Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double NFR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double NFR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double NFR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z + Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    double NFNR_Vector_x = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x;
    double NFNR_Vector_y = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y;
    double NFNR_Vector_z = -Engine->Neural_Network_Training_State->Submarine->Up_Vector.z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z;

    int l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[8] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[8] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[8] = 0;
        }

        l += 1;
    }
    //printf("Top Forward Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[8]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[9] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[9] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[9] = 0;
        }

        l += 1;
    }
    //printf("Bottom Back Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[9]);












     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[10] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[10] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l],Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[10] = 0;
        }

        l += 1;
    }
    //printf("Top Back Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[10]);











     l = 0;
    while(l < 11)
    {
        if(Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[11] = l;
            if(l <= 0)
            {
                Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[11] = 0;
            }
            l = 11;
        }
        else if(!Check_If_Ray_Sensor_Detects_Mesh(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->plane_x+FNR_Vector_x*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_y+FNR_Vector_y*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Submarine->plane_z+FNR_Vector_z*Engine->Neural_Network_Training_State->LRS[l]*-1,Engine->Neural_Network_Training_State->Mesh))
        {
            Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[11] = 0;
        }

        l += 1;
    }
    //printf("Bottom Forward Long distance sensor: %f\n",Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[11]);
}



double Make_Noise(bool Is_Training)
{
    int Value = rand() % 1000;
    double Result = Value*0.0001;
    printf("Value: %d\n", Value);
    printf("Noise: %lf\n", Result);
    if(!Is_Training)
    {
        Result = 0;
    }
    return Result;
}


void Initialize_Neural_Network_Training_State(struct _Engine* Engine)
{
    srand(time(NULL));

    Engine->Neural_Network_Training_State->Training = true;
    Engine->Neural_Network_Training_State->Manual_Control= true;

    Engine->Neural_Network_Training_State->Current_Timestamp = 0;
    Engine->Neural_Network_Training_State->Maximum_Timestamp = 9;


    //srand((unsigned int)time(NULL));

    // Structural Hyperparameters
    Engine->Neural_Network_Training_State->seq_len    = 10;
    Engine->Neural_Network_Training_State->input_dim  = 71;
    Engine->Neural_Network_Training_State->hidden1    = 128;
    Engine->Neural_Network_Training_State->hidden2    = 128;
    Engine->Neural_Network_Training_State->output_dim = 4;
    Engine->Neural_Network_Training_State->lr       = 0.02f;

    // Layer Object Allocations
    Engine->Neural_Network_Training_State->layer1 = lstm_create(Engine->Neural_Network_Training_State->input_dim, Engine->Neural_Network_Training_State->hidden1);
    Engine->Neural_Network_Training_State->layer2 = lstm_create(Engine->Neural_Network_Training_State->hidden1, Engine->Neural_Network_Training_State->hidden2);
    Engine->Neural_Network_Training_State->dense  = dense_create(Engine->Neural_Network_Training_State->hidden2, Engine->Neural_Network_Training_State->output_dim);

    // Context Sequence Memory History Structures
    Engine->Neural_Network_Training_State->hist1 = lstm_create_history(Engine->Neural_Network_Training_State->seq_len, Engine->Neural_Network_Training_State->input_dim, Engine->Neural_Network_Training_State->hidden1);
    Engine->Neural_Network_Training_State->hist2 = lstm_create_history(Engine->Neural_Network_Training_State->seq_len, Engine->Neural_Network_Training_State->hidden1, Engine->Neural_Network_Training_State->hidden2);

    // Variable Buffers
    Engine->Neural_Network_Training_State->raw_inputs    = (float*)calloc(Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->input_dim, sizeof(float));
    Engine->Neural_Network_Training_State->dense_outputs = (float*)calloc(Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->output_dim, sizeof(float));
    Engine->Neural_Network_Training_State->target_outs   = (float*)calloc(Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->output_dim, sizeof(float));

    // Recurrent Backward Gradient Trackers
    Engine->Neural_Network_Training_State->d_dense_out   = (float*)calloc(Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->output_dim, sizeof(float));
    Engine->Neural_Network_Training_State->dh_layer2     = (float*)calloc(Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->hidden2, sizeof(float));
    Engine->Neural_Network_Training_State->dh_layer1     = (float*)calloc(Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->hidden1, sizeof(float));

    // Populate mock training sequence data







    Engine->Neural_Network_Training_State->SRS[0] = 1;
    Engine->Neural_Network_Training_State->SRS[1] = METER_CONVERSION *1;
    Engine->Neural_Network_Training_State->SRS[2] = METER_CONVERSION *2;
    Engine->Neural_Network_Training_State->SRS[3] = METER_CONVERSION *3;
    Engine->Neural_Network_Training_State->SRS[4] = METER_CONVERSION *4;
    Engine->Neural_Network_Training_State->SRS[5] = METER_CONVERSION *5;
    Engine->Neural_Network_Training_State->SRS[6] = METER_CONVERSION *6;
    Engine->Neural_Network_Training_State->SRS[7] = METER_CONVERSION *7;
    Engine->Neural_Network_Training_State->SRS[8] = METER_CONVERSION *8;
    Engine->Neural_Network_Training_State->SRS[9] = METER_CONVERSION *9;
    Engine->Neural_Network_Training_State->SRS[10] = METER_CONVERSION *10;

    Engine->Neural_Network_Training_State->MRS[0] = 10;
    Engine->Neural_Network_Training_State->MRS[1] = METER_CONVERSION *10;
    Engine->Neural_Network_Training_State->MRS[2] = METER_CONVERSION *20;
    Engine->Neural_Network_Training_State->MRS[3] = METER_CONVERSION *30;
    Engine->Neural_Network_Training_State->MRS[4] = METER_CONVERSION *40;
    Engine->Neural_Network_Training_State->MRS[5] = METER_CONVERSION *50;
    Engine->Neural_Network_Training_State->MRS[6] = METER_CONVERSION *60;
    Engine->Neural_Network_Training_State->MRS[7] = METER_CONVERSION *70;
    Engine->Neural_Network_Training_State->MRS[8] = METER_CONVERSION *80;
    Engine->Neural_Network_Training_State->MRS[9] = METER_CONVERSION *90;
    Engine->Neural_Network_Training_State->MRS[10] = METER_CONVERSION * 100;

    Engine->Neural_Network_Training_State->LRS[0] = 100;
    Engine->Neural_Network_Training_State->LRS[1] = METER_CONVERSION *100;
    Engine->Neural_Network_Training_State->LRS[2] = METER_CONVERSION *200;
    Engine->Neural_Network_Training_State->LRS[3] = METER_CONVERSION *300;
    Engine->Neural_Network_Training_State->LRS[4] = METER_CONVERSION *400;
    Engine->Neural_Network_Training_State->LRS[5] = METER_CONVERSION *500;
    Engine->Neural_Network_Training_State->LRS[6] = METER_CONVERSION *600;
    Engine->Neural_Network_Training_State->LRS[7] = METER_CONVERSION *700;
    Engine->Neural_Network_Training_State->LRS[8] = METER_CONVERSION *800;
    Engine->Neural_Network_Training_State->LRS[9] = METER_CONVERSION *900;
    Engine->Neural_Network_Training_State->LRS[10] = METER_CONVERSION * 1000;

    memset(Engine->Neural_Network_Training_State->My_Long_Range_Sensors,0,6);
    memset(Engine->Neural_Network_Training_State->My_Medium_Range_Sensors,0,6);
    memset(Engine->Neural_Network_Training_State->My_Short_Range_Sensors,0,6);

    memset(Engine->Neural_Network_Training_State->My_Current_Output_Policy,0,4);

    //srand(time(NULL));

    // Define Network: 3 Inputs, 2 Hidden NNLayers (8 neurons each), 2 Outputs
    // Inputs: [Pitch angle, Roll angle, Altitude error]
    // Outputs: [Elevator command, Aileron command]
    int inputs = 54;
    int hidden_NNLayers = 2;
    int hidden_size = 10;
    int outputs = 8;

    printf("Creating network...\n");
    Engine->Neural_Network_Training_State->autopilot = create_network(inputs, hidden_NNLayers, hidden_size, outputs);




    Engine->Neural_Network_Training_State->Snapshot_Timer = Create_Timer();
    Set_Timer(Engine->Neural_Network_Training_State->Snapshot_Timer);

    Engine->Neural_Network_Training_State->Current_IO_Example = 0;
    Engine->Neural_Network_Training_State->Current_IO_Capture = 0;
    Engine->Neural_Network_Training_State->Snapshot_Time = 0;

    Engine->Neural_Network_Training_State->Mesh = Create_Preloaded_Collision_Mesh("resources/sphere.se3");
    Engine->Neural_Network_Training_State->Manual_Control = true;
    Engine->Neural_Network_Training_State->Mesh->z = 0;

    Engine->Neural_Network_Training_State->Skybox = Create_SE3_Model();
    Load_SE3_Model(Engine->Neural_Network_Training_State->Skybox,"resources/models/model_skybox.se3");
    Load_SE3_Texture(Engine->Neural_Network_Training_State->Skybox,"resources/models/title_sonichu_skybox_light.sei");

    Engine->Neural_Network_Training_State->Test_Sub = Create_SE3_Model();
    Load_SE3_Model(Engine->Neural_Network_Training_State->Test_Sub,"resources/sphere.se3");
    Load_SE3_Texture(Engine->Neural_Network_Training_State->Test_Sub,"car.sei");

    glColor3f(1,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH); // Enable smooth shading (Gouraud shading)
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40,40,40,0,0,20,0,0,1);

    // Somewhere in the initialization part of your program…
    // Enable depth testing and lighting
    // --- Enable Depth Test and Smooth Shading ---

glEnable(GL_DEPTH_TEST);

glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
    // --- LIGHT 1: GENERAL LIGHT (Dim & verywhere) ---
    GLfloat gn_ambient[] = { 1, 1, 1, 1.0 }; // Base light so shadows aren't pitch black
    GLfloat gn_diffuse[] = { 1, 1, 1, 1.0 }; // Dim diffuse light
    GLfloat gn_specular[] = { 1,1,1, 1.0 }; // Very little shine

    glLightfv(GL_LIGHT2, GL_AMBIENT, gn_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, gn_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, gn_specular);

    // Position the general light
    // IMPORTANT: The 0.0 at the end makes this a DIRECTIONAL light (like the sun)
    // coming from the top-right-front, rather than a positional point light.
    GLfloat gn_position[] = { 1, 1, 1, 0.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, gn_position);



     glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
        glCullFace(GL_BACK); // Temporarily flip culling


    Engine->Neural_Network_Training_State->Submarine = Create_Submarine_Object();
    Initialize_Submarine_Object(Engine,0);

};


void Process_NN_Plane(struct _Engine* Engine)
{
    Engine->Neural_Network_Training_State->Submarine->plane_x += Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x * Engine->Neural_Network_Training_State->Submarine->Throttle;
    Engine->Neural_Network_Training_State->Submarine->plane_y += Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y * Engine->Neural_Network_Training_State->Submarine->Throttle;
    Engine->Neural_Network_Training_State->Submarine->plane_z += Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z * Engine->Neural_Network_Training_State->Submarine->Throttle;

    Update_Short_Range_Sensors(Engine);
    Update_Medium_Range_Sensors(Engine);
    Update_Long_Range_Sensors(Engine);

    Update_Short_Range_Diagonal_FR_Sensors(Engine);
    Update_Short_Range_Diagonal_UR_Sensors(Engine);
    Update_Short_Range_Diagonal_UF_Sensors(Engine);

    Update_Medium_Range_Diagonal_FR_Sensors(Engine);
    Update_Medium_Range_Diagonal_UR_Sensors(Engine);
    Update_Medium_Range_Diagonal_UF_Sensors(Engine);

    Update_Long_Range_Diagonal_FR_Sensors(Engine);
    Update_Long_Range_Diagonal_UR_Sensors(Engine);
    Update_Long_Range_Diagonal_UF_Sensors(Engine);

    Engine->Neural_Network_Training_State->My_Velocity = Engine->Neural_Network_Training_State->Submarine->Throttle/MAX_THROTTLE;
    Engine->Neural_Network_Training_State->My_Roll = Engine->Neural_Network_Training_State->Submarine->plane_roll/360;
    Engine->Neural_Network_Training_State->My_Pitch = Engine->Neural_Network_Training_State->Submarine->plane_pitch/360;
    Engine->Neural_Network_Training_State->My_Yaw = Engine->Neural_Network_Training_State->Submarine->plane_yaw/360;
    Engine->Neural_Network_Training_State->My_dRoll = Engine->Neural_Network_Training_State->Delta_Roll;
    Engine->Neural_Network_Training_State->My_dPitch = Engine->Neural_Network_Training_State->Delta_Pitch;
    Engine->Neural_Network_Training_State->My_dYaw = Engine->Neural_Network_Training_State->Delta_Yaw;
    Engine->Neural_Network_Training_State->My_dThrottle = Engine->Neural_Network_Training_State->Delta_Throttle;

    Engine->Neural_Network_Training_State->Target_Proximity = Get_Distance(Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,0,0,0);
    Engine->Neural_Network_Training_State->Target_Velocity = 0;
    Engine->Neural_Network_Training_State->Target_Roll = 0;
    Engine->Neural_Network_Training_State->Target_Pitch = 0;
    Engine->Neural_Network_Training_State->Target_Yaw = 0;

    Engine->Neural_Network_Training_State->Altitude = Engine->Neural_Network_Training_State->Submarine->plane_z;
    Engine->Neural_Network_Training_State->Alert_Status = 0;
    Engine->Neural_Network_Training_State->My_Damage = 0;
    Engine->Neural_Network_Training_State->My_Comms = 0;

    //Now, If a snapshot occurs, we record these in a time sequence.

    Engine->Neural_Network_Training_State->My_Current_Output_Policy[0] = Engine->Neural_Network_Training_State->Submarine->plane_pitch/360;
    Engine->Neural_Network_Training_State->My_Current_Output_Policy[1] = Engine->Neural_Network_Training_State->Submarine->plane_roll/360;
    Engine->Neural_Network_Training_State->My_Current_Output_Policy[2] = Engine->Neural_Network_Training_State->Submarine->plane_yaw/360;
    Engine->Neural_Network_Training_State->My_Current_Output_Policy[3] = Engine->Neural_Network_Training_State->Submarine->Throttle/MAX_THROTTLE;


    int Output_Offset = Engine->Neural_Network_Training_State->Current_Timestamp * 4;
    printf("Output Offset: %d\n", Output_Offset);
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Output_Vector[Output_Offset+0] = Engine->Neural_Network_Training_State->My_Current_Output_Policy[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Output_Vector[Output_Offset+1] = Engine->Neural_Network_Training_State->My_Current_Output_Policy[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Output_Vector[Output_Offset+2] = Engine->Neural_Network_Training_State->My_Current_Output_Policy[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Output_Vector[Output_Offset+3] = Engine->Neural_Network_Training_State->My_Current_Output_Policy[3];

    int Input_Offset = Engine->Neural_Network_Training_State->Current_Timestamp * 71;
    printf("Input Offset: %d\n", Input_Offset);

    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+0] = Engine->Neural_Network_Training_State->My_Long_Range_Sensors[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+1] = Engine->Neural_Network_Training_State->My_Long_Range_Sensors[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+2] = Engine->Neural_Network_Training_State->My_Long_Range_Sensors[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+3] = Engine->Neural_Network_Training_State->My_Long_Range_Sensors[3];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+4] = Engine->Neural_Network_Training_State->My_Long_Range_Sensors[4];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+5] = Engine->Neural_Network_Training_State->My_Long_Range_Sensors[5];

    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+6] = Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+7] = Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+8] = Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+9] = Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[3];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+10] = Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[4];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+11] = Engine->Neural_Network_Training_State->My_Medium_Range_Sensors[5];

    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+12] = Engine->Neural_Network_Training_State->My_Short_Range_Sensors[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+13] = Engine->Neural_Network_Training_State->My_Short_Range_Sensors[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+14] = Engine->Neural_Network_Training_State->My_Short_Range_Sensors[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+15] = Engine->Neural_Network_Training_State->My_Short_Range_Sensors[3];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+16] = Engine->Neural_Network_Training_State->My_Short_Range_Sensors[4];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+17] = Engine->Neural_Network_Training_State->My_Short_Range_Sensors[5];


    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+18] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+19] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+20] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+21] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[3];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+22] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[4];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+23] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[5];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+24] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[6];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+25] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[7];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+26] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[8];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+27] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[9];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+28] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[10];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+29] = Engine->Neural_Network_Training_State->My_Long_Range_Diagonal_Sensors[11];

    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+30] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+31] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+32] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+33] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[3];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+34] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[4];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+35] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[5];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+36] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[6];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+37] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[7];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+38] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[8];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+39] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[9];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+40] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[10];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+41] = Engine->Neural_Network_Training_State->My_Medium_Range_Diagonal_Sensors[11];

    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+42] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[0];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+43] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[1];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+44] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[2];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+45] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[3];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+46] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[4];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+47] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[5];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+48] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[6];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+49] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[7];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+50] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[8];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+51] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[9];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+52] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[10];
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+53] = Engine->Neural_Network_Training_State->My_Short_Range_Diagonal_Sensors[11];



    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+54] = Engine->Neural_Network_Training_State->My_Velocity;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+55] = Engine->Neural_Network_Training_State->My_Roll;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+56] = Engine->Neural_Network_Training_State->My_Pitch;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+57] = Engine->Neural_Network_Training_State->My_Yaw;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+58] = Engine->Neural_Network_Training_State->My_dRoll;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+59] = Engine->Neural_Network_Training_State->My_dPitch;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+60] = Engine->Neural_Network_Training_State->My_dYaw;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+61] = Engine->Neural_Network_Training_State->My_dThrottle;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+62] = Engine->Neural_Network_Training_State->Target_Proximity;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+63] = Engine->Neural_Network_Training_State->Target_Velocity;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+64] = Engine->Neural_Network_Training_State->Target_Roll;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+65] = Engine->Neural_Network_Training_State->Target_Pitch;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+66] = Engine->Neural_Network_Training_State->Target_Yaw;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+67] = Engine->Neural_Network_Training_State->Altitude;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+68] = Engine->Neural_Network_Training_State->Alert_Status;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+69] = Engine->Neural_Network_Training_State->My_Damage;
    Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[Input_Offset+70] = Engine->Neural_Network_Training_State->My_Comms;

    for (int i = 0; i < Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->input_dim; i++)  Engine->Neural_Network_Training_State->raw_inputs[i] = Engine->Neural_Network_Training_State->My_NEW_LSTM_Input_Vector[i];
    for (int i = 0; i < Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->output_dim; i++) Engine->Neural_Network_Training_State->target_outs[i] = Engine->Neural_Network_Training_State->My_NEW_LSTM_Output_Vector[i];
        // ----------------------------------------------------
        // A. FORWARD PIPELINE
        // ----------------------------------------------------
        lstm_forward(Engine->Neural_Network_Training_State->layer1, Engine->Neural_Network_Training_State->hist1, Engine->Neural_Network_Training_State->raw_inputs);
        lstm_forward(Engine->Neural_Network_Training_State->layer2, Engine->Neural_Network_Training_State->hist2, Engine->Neural_Network_Training_State->hist1->h);

        // Project Output from Hidden Layer 2 through Dense Weights
        for (int t = 0; t < Engine->Neural_Network_Training_State->seq_len; t++) {
            for (int i = 0; i < Engine->Neural_Network_Training_State->output_dim; i++) {
                float net = Engine->Neural_Network_Training_State->dense->b[i];
                for (int j = 0; j < Engine->Neural_Network_Training_State->hidden2; j++) {
                    net += Engine->Neural_Network_Training_State->dense->W[i * Engine->Neural_Network_Training_State->hidden2 + j] * Engine->Neural_Network_Training_State->hist2->h[t * Engine->Neural_Network_Training_State->hidden2 + j];
                }
                Engine->Neural_Network_Training_State->dense_outputs[t * Engine->Neural_Network_Training_State->output_dim + i] = net;
            }
        }

        if(Engine->Neural_Network_Training_State->Training)
        {

        // Calculate loss (MSE Evaluation)
        float total_loss = 0.0f;
        for (int i = 0; i < Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->output_dim; i++) {
            float diff = Engine->Neural_Network_Training_State->dense_outputs[i] - Engine->Neural_Network_Training_State->target_outs[i];
            total_loss += 0.5f * diff * diff;
            Engine->Neural_Network_Training_State->d_dense_out[i] = diff; // Backprop seeding derivative
        }

            printf(" Aggregated Sequence MSE Loss: %f\n", total_loss / Engine->Neural_Network_Training_State->seq_len);


        // ----------------------------------------------------
        // B. BACKPROPAGATION THROUGH TIME (BPTT) CHAIN
        // ----------------------------------------------------

        // Step 1: Initialize Dense Gradients
        memset(Engine->Neural_Network_Training_State->dense->db, 0, Engine->Neural_Network_Training_State->output_dim * sizeof(float));
        memset(Engine->Neural_Network_Training_State->dense->dW, 0, Engine->Neural_Network_Training_State->output_dim * Engine->Neural_Network_Training_State->hidden2 * sizeof(float));
        memset(Engine->Neural_Network_Training_State->dh_layer2, 0, Engine->Neural_Network_Training_State->seq_len * Engine->Neural_Network_Training_State->hidden2 * sizeof(float));

        // Step 2: Clear and push dense deviations back down to hidden states
        for (int t = 0; t < Engine->Neural_Network_Training_State->seq_len; t++) {
            for (int i = 0; i < Engine->Neural_Network_Training_State->output_dim; i++) {
                float d_out = Engine->Neural_Network_Training_State->d_dense_out[t * Engine->Neural_Network_Training_State->output_dim + i];
                Engine->Neural_Network_Training_State->dense->db[i] += d_out;
                for (int j = 0; j < Engine->Neural_Network_Training_State->hidden2; j++) {
                    Engine->Neural_Network_Training_State->dense->dW[i * Engine->Neural_Network_Training_State->hidden2 + j] += d_out * Engine->Neural_Network_Training_State->hist2->h[t * Engine->Neural_Network_Training_State->hidden2 + j];
                    Engine->Neural_Network_Training_State->dh_layer2[t * Engine->Neural_Network_Training_State->hidden2 + j] += d_out * Engine->Neural_Network_Training_State->dense->W[i * Engine->Neural_Network_Training_State->hidden2 + j];
                }
            }
        }

        // Step 3: Recurrent step extraction back through Layer 2
        lstm_backward(Engine->Neural_Network_Training_State->layer2, Engine->Neural_Network_Training_State->hist2, Engine->Neural_Network_Training_State->dh_layer2, Engine->Neural_Network_Training_State->dh_layer1);

        // Step 4: Recurrent step extraction back through Layer 1
        lstm_backward(Engine->Neural_Network_Training_State->layer1, Engine->Neural_Network_Training_State->hist1, Engine->Neural_Network_Training_State->dh_layer1, NULL);

        // ----------------------------------------------------
        // C. DESCENT WEIGHT PARAMETER OPTIMIZATION
        // ----------------------------------------------------
        lstm_update(Engine->Neural_Network_Training_State->layer1, Engine->Neural_Network_Training_State->lr);
        lstm_update(Engine->Neural_Network_Training_State->layer2, Engine->Neural_Network_Training_State->lr);

        for (int i = 0; i < Engine->Neural_Network_Training_State->output_dim * Engine->Neural_Network_Training_State->hidden2; i++) Engine->Neural_Network_Training_State->dense->W[i] -= Engine->Neural_Network_Training_State->lr * Engine->Neural_Network_Training_State->dense->dW[i];
        for (int i = 0; i < Engine->Neural_Network_Training_State->output_dim; i++)           Engine->Neural_Network_Training_State->dense->b[i] -= Engine->Neural_Network_Training_State->lr * Engine->Neural_Network_Training_State->dense->db[i];
        }


        int last_t = Engine->Neural_Network_Training_State->seq_len - 1;
        float *final_output_step = &Engine->Neural_Network_Training_State->dense_outputs[last_t * Engine->Neural_Network_Training_State->output_dim];

        for (int i = 0; i < Engine->Neural_Network_Training_State->output_dim; i++) {
            Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[i] = final_output_step[i];
        }

        printf("Current time: %d\n", Engine->Neural_Network_Training_State->Current_Timestamp);


        double x = Get_Ticks(Engine->Neural_Network_Training_State->Snapshot_Timer);

        if(x > Engine->Neural_Network_Training_State->Snapshot_Time + 0.001) //Capture at 1 FPS
        {
            Engine->Neural_Network_Training_State->Current_Timestamp += 1;
            Engine->Neural_Network_Training_State->Snapshot_Time = x;
        }

            if(Engine->Neural_Network_Training_State->Current_Timestamp > 9)
            {
                Engine->Neural_Network_Training_State->Current_Timestamp = 0;
            }

}


void Render_Neural_Network_Training_State(struct _Engine* Engine)
{
    glPushMatrix();
    glTranslatef(0,0,0);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Skybox,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,Engine->Neural_Network_Training_State->Mesh->z);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Test_Sub,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(Engine->Neural_Network_Training_State->Submarine->plane_x, Engine->Neural_Network_Training_State->Submarine->plane_y, Engine->Neural_Network_Training_State->Submarine->plane_z);
    // Apply the orientation matrix
    float mat[16] = {
        (float)Engine->Neural_Network_Training_State->Submarine->Right_Vector.x, (float)Engine->Neural_Network_Training_State->Submarine->Right_Vector.y, (float)Engine->Neural_Network_Training_State->Submarine->Right_Vector.z, 0,
        (float)Engine->Neural_Network_Training_State->Submarine->Up_Vector.x,    (float)Engine->Neural_Network_Training_State->Submarine->Up_Vector.y,    (float)Engine->Neural_Network_Training_State->Submarine->Up_Vector.z,    0,
        -(float)Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x,  -(float)Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y,  -(float)Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Submarine->Models[0],0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
};

void Handle_Neural_Network_Training_State_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Engine->Neural_Network_Training_State->Manual_Control)
    {
    if(!Keypad.Keyboard_A || Keypad.Keyboard_B)
    {
        Engine->Neural_Network_Training_State->Delta_Throttle = 0;
    }

    if(!Keypad.Keyboard_A || Keypad.Keyboard_B)
    {
        //Engine->Neural_Network_Training_State->Delta_Roll = 0;
    }


    if(!Keypad.Keyboard_Up || Keypad.Keyboard_Down)
    {
        Engine->Neural_Network_Training_State->Delta_Pitch = 0;
    }


    if(!Keypad.Keyboard_Left || Keypad.Keyboard_Right)
    {
        Engine->Neural_Network_Training_State->Delta_Yaw = 0;
    }

        if(Keypad.Keyboard_A && Engine->Neural_Network_Training_State->Submarine->Throttle + 1 <= MAX_THROTTLE)
        {
            Engine->Neural_Network_Training_State->Submarine->Throttle += 1;
            Engine->Neural_Network_Training_State->Delta_Throttle = 1;
        }

        if(Keypad.Keyboard_B && Engine->Neural_Network_Training_State->Submarine->Throttle -1 >= 0)
        {
            Engine->Neural_Network_Training_State->Submarine->Throttle -= 1;
            Engine->Neural_Network_Training_State->Delta_Throttle = -1;
        // Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_C)
        {
            Engine->Neural_Network_Training_State->Submarine->Throttle = 0;
        //  Process_NN_Plane(Engine);
        }


        if(Keypad.Keyboard_Up)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Pitch = 1;
        // Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_Down)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Pitch = -1;
        //Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_Left)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Yaw = 1;
       //Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_Right)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Yaw = -1;
       // Process_NN_Plane(Engine);
        }
    }

    else if(!Engine->Neural_Network_Training_State->Manual_Control)
    {
        double Intended_Pitch = Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[0] * 360;
        double Intended_Roll = Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[1] * 360;
        double Intended_Yaw = Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[2] * 360;
        double Intended_Throttle = Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[3] * MAX_THROTTLE;

        if(Intended_Pitch < 0)
        {
            Intended_Pitch += 360;
        }

        if(Intended_Roll < 0)
        {
            Intended_Roll += 360;
        }

        if(Intended_Yaw < 0)
        {
            Intended_Yaw += 360;
        }

        if(Intended_Throttle < 0)
        {
            Intended_Throttle = 0;
        }


        printf("Intended PRY: %f %f %f\n", Intended_Pitch, Intended_Roll, Intended_Yaw);

        if(Engine->Neural_Network_Training_State->Submarine->plane_pitch > Intended_Pitch)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
        }

        else if(Engine->Neural_Network_Training_State->Submarine->plane_pitch < Intended_Pitch)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);

        }

        if(Engine->Neural_Network_Training_State->Submarine->plane_yaw < Intended_Yaw)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
        }

        else if(Engine->Neural_Network_Training_State->Submarine->plane_yaw > Intended_Yaw)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
        }

        if(Engine->Neural_Network_Training_State->Submarine->Throttle < Intended_Throttle)
        {
            if(Engine->Neural_Network_Training_State->Submarine->Throttle + 1 < MAX_THROTTLE)
            {
            Engine->Neural_Network_Training_State->Submarine->Throttle += 1;
            }
        }

        if(Engine->Neural_Network_Training_State->Submarine->Throttle > Intended_Throttle)
        {
            if(Engine->Neural_Network_Training_State->Submarine->Throttle - 1 > 0)
            {
            Engine->Neural_Network_Training_State->Submarine->Throttle -= 1;
            }
        }
        //double Intended_Throttle =


        printf("Neuropilot pitch: %f roll: %f yaw: %f\n", Engine->Neural_Network_Training_State->Submarine->plane_pitch,Engine->Neural_Network_Training_State->Submarine->plane_roll,Engine->Neural_Network_Training_State->Submarine->plane_yaw,Engine->Neural_Network_Training_State->Submarine->Throttle);
        printf("Neuropilot throttle: %f\n", Engine->Neural_Network_Training_State->Submarine->Throttle);
    }

    if(Keypad.Keyboard_D)
    {
        Engine->Neural_Network_Training_State->Manual_Control = true;
        printf("Manual Control is on.\n");
    }

    if(Keypad.Keyboard_E)
    {
        Engine->Neural_Network_Training_State->Manual_Control = false;
        printf("Manual Control is off.\n");
    }


    if(Keypad.Keyboard_F)
    {
        Engine->Neural_Network_Training_State->Training= false;
        printf("Training is off.\n");
        Engine->Neural_Network_Training_State->Submarine->plane_x = rand() % 1000;
        Engine->Neural_Network_Training_State->Submarine->plane_y = rand() % 1000;
        Engine->Neural_Network_Training_State->Submarine->plane_z = rand() % 1000;
    }

    if(Keypad.Keyboard_G)
    {
        Engine->Neural_Network_Training_State->Training = true;
        printf("Training is on.\n");
    }

    if(Engine->Neural_Network_Training_State->Manual_Control)
    {
        printf("Manual Control is ON!\n");
    }

    if(Engine->Neural_Network_Training_State->Training)
    {
       printf("Training is ON!\n");
    }


    if(!Engine->Neural_Network_Training_State->Manual_Control)
    {
        printf("Manual Control isnt ON!\n");
    }

    if(!Engine->Neural_Network_Training_State->Training)
    {
       printf("Training isnt ON!\n");
    }

    if(Keypad.Keyboard_Start)
    {
        printf("Third-Person View!\n");
        Engine->Neural_Network_Training_State->Third_Person = true;
    }

};

void Process_Neural_Network_Training_State(struct _Engine* Engine)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Vec3 cam;
    cam.x = Engine->Neural_Network_Training_State->Submarine->plane_x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x * 1200.0f + Engine->Neural_Network_Training_State->Submarine->Up_Vector.x * 3.0f;
    cam.y = Engine->Neural_Network_Training_State->Submarine->plane_y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y * 1200.0f + Engine->Neural_Network_Training_State->Submarine->Up_Vector.y * 3.0f;
    cam.z = Engine->Neural_Network_Training_State->Submarine->plane_z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z * 1200.0f + Engine->Neural_Network_Training_State->Submarine->Up_Vector.z * 3.0f;
    Process_NN_Plane(Engine);

    if(!Engine->Neural_Network_Training_State->Third_Person)
    {
    gluLookAt(cam.x, cam.y, cam.z, Engine->Neural_Network_Training_State->Submarine->plane_x, Engine->Neural_Network_Training_State->Submarine->plane_y, Engine->Neural_Network_Training_State->Submarine->plane_z, Engine->Neural_Network_Training_State->Submarine->Up_Vector.x, Engine->Neural_Network_Training_State->Submarine->Up_Vector.y, Engine->Neural_Network_Training_State->Submarine->Up_Vector.z);
    }

    else if(Engine->Neural_Network_Training_State->Third_Person)
    {
    gluLookAt(0,0,0, Engine->Neural_Network_Training_State->Submarine->plane_x, Engine->Neural_Network_Training_State->Submarine->plane_y, Engine->Neural_Network_Training_State->Submarine->plane_z, 0,0,1);
    }

};

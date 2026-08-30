#ifndef UID_H_INCLUDED
#define UID_H_INCLUDED

#pragma pack(push, 1)

struct _Universal_ID
{
    char Name[256];
    unsigned long Category;
    unsigned long Subcategory;
    unsigned long Variant;
    unsigned long ID;
    unsigned short Global_X_Coordinate; //4096 pixels / circumference of 40040 km = 10 km diameter per pixel

    //So, draw distance needs to be 5km.

    unsigned short Global_Y_Coordinate;
};

#pragma pack(pop)

#endif // UID_H_INCLUDED

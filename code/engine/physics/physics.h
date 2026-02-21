/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   physics.h
 * Author: drew
 *
 * Created on September 28, 2023, 12:01 PM
 */

#ifndef PHYSICS_H
#define PHYSICS_H
#include <iostream>
#include <cmath>
#include <limits>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../graphics/new_model.h"
#include "../physics/vector.h"
#define GRAVITY 2
#define METER_CONVERSION 23.2240437158

// Define vector structure





typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

// Define triangle structure
typedef struct {
    Vec3 v[3];
} _Triangle;

// Define capsule structure
typedef struct {
    Vec3 a, b;
    double radius;
} Capsule;

struct _Point_Collider
{
    bool Collider_May_Be_Overwritten; //If the character is gone, overwrite this.
    int For_This_Character;
    float x;
    float y;
    float z;
};


struct _Sphere_Collider
{
    bool Collider_May_Be_Overwritten; //If the character is gone, overwrite this.
    int For_This_Character;
    float x;
    float y;
    float z;
    float Radius;
};


struct _Capsule_Collider
{
    bool Collider_May_Be_Overwritten; //If the character is gone, overwrite this.
    int For_This_Character;
    float x1;
    float y1;
    float z1;

    float x2;
    float y2;
    float z2;
    float Radius;
};





struct _AABB_Collider
{
    bool Collider_May_Be_Overwritten; //If the character is gone, overwrite this.
    int For_This_Character;
};

struct _Triangle_Collider
{

};

struct _Mesh_Collider_Triangle
{
    float x1;
    float x2;
    float x3;
};

struct _Mesh_Collider
{
    bool Collider_May_Be_Overwritten; //If the character is gone, overwrite this.
    int For_This_Character;
    float x_Offset;
    float y_Offset;
    float z_Offset;
    struct _MD2_Model* Collision_Mesh;
};




struct _Collision_Registry
{

};

struct _Preloaded_Collision_Mesh //Just the vertices and triangles
{
    int Number_Of_Maximum_Zs; //USED TO TEST FEET
    int Number_Of_Minimum_Zs; //USED TO TEST HEAD

    float Maximum_Z;
    float Minimum_Z;

    float x;
    float y;
    float z;
    int Number_Of_Triangles;
    _Triangle* Triangles;
};


// Structure to represent an Axis-Aligned Bounding Box (AABB)

Vec3 cross_product(Vec3 a, Vec3 b);

void Add_Point_Collider(int For_This_Character);
void Add_Sphere_Collider(int For_This_Character, float Radius);
void Add_Capsule_Collider(int For_This_Character, float Radius, float x1, float y1, float z1, float x2, float y2, float z2);
void Add_AABB_Collider(int For_This_Character);
struct _Mesh_Collider* Add_Mesh_Collider(int For_This_Character, const char* Mesh_Collider_Filepath);

bool Test_Capsule_And_Mesh_Collision(struct _Capsule_Collider* Capsule_Collider, struct _Preloaded_Collision_Mesh* Mesh, float x_Offset, float y_Offset, float z_Offset);


struct _Preloaded_Collision_Mesh* Create_Preloaded_Collision_Mesh(const char* Filename);

void Draw_Capsule_Collision_Geometry(struct _Capsule_Collider* Capsule_Collider);

bool Test_Mesh_And_Mesh_Collision(struct _Preloaded_Collision_Mesh* Mesh,struct _Preloaded_Collision_Mesh* MeshTwo);

void Update_Mesh_Location(struct _Preloaded_Collision_Mesh* Mesh, float x, float y, float z);


bool Check_If_Ray_Sensor_Detects_Mesh(float Ray_x1, float Ray_y1, float Ray_z1, float Ray_x2, float Ray_y2, float Ray_z2, struct _Preloaded_Collision_Mesh* Mesh);

bool ray_triangle_intersect(Ray ray, _Triangle triangle, double *t, double *u, double *v);


bool Check_If_AABB_Detects_Mesh(AABB aabb,struct _Preloaded_Collision_Mesh* Mesh, float Offset_x, float Offset_y, float Offset_z);

bool checkAABBTriangleCollision(AABB aabb, Vec3 v1, Vec3 v2, Vec3 v3);


bool checkAABBCollision(AABB box1, AABB box2, float Offset_x, float Offset_y, float Offset_z);

bool Check_If_Ray_Sensor_Detects_AABB(AABB box1, float Ray_x1, float Ray_y1, float Ray_z1, float Ray_x2, float Ray_y2, float Ray_z2, float Offset_x, float Offset_y, float Offset_z);

int Check_If_Mouse_Clicked_On_Button(struct _Keypad Keypad, double Mouse_X, double Mouse_Y, float box_x1, float box_x2, float box_y1, float box_y2);

float Get_Distance(float x1, float y1, float z1, float x2, float y2, float z2);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_cross(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, double scalar);
vec3_t vec3_t_sub(const vec3_t a, const vec3_t b);
vec3_t vec3_t_cross(const vec3_t a, const vec3_t b);
vec3_t vec3_t_normalize(vec3_t v);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
double vec3_dot(Vec3 a, Vec3 b);

#endif /* PHYSICS_H */
























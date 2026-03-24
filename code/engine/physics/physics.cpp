/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "physics.h"
#include "math.h"
#define EPSILON 1e-2

bool Box_Collision_Occurred(float x1, float length_1, float y1, float height_1,float x2, float length_2, float y2, float height_2)
{
    if(y1 + height_1 <= y2)
    {
        return false;
    }

    if(y1 >= y2+height_2)
    {
        return false;
    }

    if(x1+length_1 <= x2)
    {
        return false;
    }

    if(x1 >= x2 + length_2)
    {
        return false;
    }


    return true;
}



bool Collision_Occurred(float x, float x2, float y, float y2, float z, float z2, float Threshold)
{
    float Result = sqrt(((x2-x)*(x2-x))*((y2-y)*(y2-y))*((z2-z)*(z2-z)));

    if(Result > Threshold)
    {
        return false;
    }
    else if(Result <= Threshold)
    {
        return true;
    }
}


bool Sphere_Sphere_Collision_Occurred(float x1, float y1, float z1, float x2, float y2, float z2, float Sphere_One_Radius, float Sphere_Two_Radius)
{
    float Distance = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

    if(Distance < Sphere_One_Radius + Sphere_Two_Radius)
    {
        return true;
    }

    return false;
};

float Get_Distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float Result;
    Result = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
    //printf("Distance in meters: %f\n", Result/METER_CONVERSION);

    //42.5 units = 1.83m
    //1 unit = 0.04305882352m
    //1 meter = 23.2240437158units
    return Result;
}




void Add_Point_Collider(int For_This_Character)
{

};
void Add_Sphere_Collider(int For_This_Character, float Radius)
{

};
void Add_Capsule_Collider(int For_This_Character, float Radius, float x1, float y1, float z1, float x2, float y2, float z2)
{

};
void Add_AABB_Collider(int For_This_Character)
{

};
struct _Mesh_Collider* Add_Mesh_Collider(int For_This_Character, const char* Mesh_Collider_Filepath)
{

struct _Mesh_Collider* Mesh = (struct _Mesh_Collider*) calloc(1,sizeof(struct _Mesh_Collider));

Mesh->Collision_Mesh = Create_SE3_Model();
Load_SE3_Model(Mesh->Collision_Mesh,Mesh_Collider_Filepath);
return Mesh;

};

// Function to subtract two vectors
Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

// Function to add two vectors
Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 vec3_scale(Vec3 v, double scalar) {
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    return v;
}

// Function for cross product of two vectors
Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// Function for dot product of two vectors
double vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Function to get the squared magnitude of a vector
double vec3_sq_mag(Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// Function to get the magnitude (length) of a vector
double vec3_mag(Vec3 v) {
    return sqrt(vec3_sq_mag(v));
}

// Function to normalize a vector
Vec3 vec3_normalize(Vec3 v) {
    double mag = vec3_mag(v);
    if (mag > 1e-6) { // Avoid division by zero
        return (Vec3){v.x / mag, v.y / mag, v.z / mag};
    }
    return (Vec3){0.0, 0.0, 0.0};
}

// Function to project a point onto a line segment
Vec3 project_point_on_segment(Vec3 p, Vec3 a, Vec3 b) {
    Vec3 ab = vec3_sub(b, a);
    double t = vec3_dot(vec3_sub(p, a), ab) / vec3_sq_mag(ab);
    t = fmax(0.0, fmin(1.0, t)); // Clamp t to [0, 1]
    return vec3_add(a, (Vec3){ab.x * t, ab.y * t, ab.z * t});
}

// Function to calculate the squared distance between two points
double point_sq_dist(Vec3 a, Vec3 b) {
    return vec3_sq_mag(vec3_sub(a, b));
}

// Structure for a triangle


// Structure for a capsule (defined by two end points and a radius)


// Function to project a triangle onto an axis and get the min/max extents
void project_triangle(_Triangle t, Vec3 axis, double *min, double *max) {
    *min = *max = vec3_dot(t.v[0], axis);
    for (int i = 1; i < 3; ++i) {
        double projection = vec3_dot(t.v[i], axis);
        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

// Function to project a capsule onto an axis and get the min/max extents
void project_capsule(Capsule c, Vec3 axis, double *min, double *max) {
    Vec3 ab = vec3_sub(c.b, c.a);
    Vec3 normalized_ab = vec3_normalize(ab);
    double half_length = vec3_mag(ab) / 2.0;
    Vec3 center = vec3_add(c.a, (Vec3){ab.x * 0.5, ab.y * 0.5, ab.z * 0.5});

    double projection_center = vec3_dot(center, axis);
    double projection_half_length = half_length * fabs(vec3_dot(normalized_ab, axis));

    *min = projection_center - projection_half_length - c.radius;
    *max = projection_center + projection_half_length + c.radius;
}

// Function to check for overlap between two 1D intervals
bool overlap1D(double minA, double maxA, double minB, double maxB) {
    return maxA >= minB && maxB >= minA;
}

// Function to perform SAT test between a triangle and a capsule
bool triangle_capsule_sat(_Triangle t, Capsule c) {
    // 1. Triangle normal
    Vec3 edge1 = vec3_sub(t.v[1], t.v[0]);
    Vec3 edge2 = vec3_sub(t.v[2], t.v[0]);
    Vec3 triangle_normal = vec3_normalize(vec3_cross(edge1, edge2));

    // 2. Capsule segment direction
    Vec3 capsule_segment = vec3_sub(c.b, c.a);
    Vec3 capsule_direction = vec3_normalize(capsule_segment);

    // 3. Edge cross products (triangle edges with capsule direction)
    Vec3 axis1 = vec3_cross(edge1, capsule_direction);
    Vec3 axis2 = vec3_cross(edge2, capsule_direction);
    Vec3 axis3 = vec3_cross(vec3_sub(t.v[2], t.v[1]), capsule_direction);

    // Array of separating axes to test
    Vec3 axes[] = {
        triangle_normal,
        capsule_direction,
        axis1,
        axis2,
        axis3
    };
    int num_axes = sizeof(axes) / sizeof(Vec3);

    for (int i = 0; i < num_axes; ++i) {
        if (vec3_sq_mag(axes[i]) < 1e-9) continue; // Skip near-zero axes

        double min_tri, max_tri, min_cap, max_cap;
        project_triangle(t, axes[i], &min_tri, &max_tri);
        project_capsule(c, axes[i], &min_cap, &max_cap);

        if (!overlap1D(min_tri, max_tri, min_cap, max_cap)) {
            return false; // Separating axis found, no collision
        }
    }

    // If no separating axis found, there might be a collision.
    // We also need to consider the closest point on the capsule segment to the triangle.

    // Check if any triangle vertex is inside the capsule
    for (int i = 0; i < 3; ++i) {
        Vec3 closest_point = project_point_on_segment(t.v[i], c.a, c.b);
        if (point_sq_dist(t.v[i], closest_point) <= c.radius * c.radius) {
            return true;
        }
    }

    // Check if any capsule end point is inside the triangle (or close to it)
    // This requires a point-in-triangle test, which can be done using barycentric coordinates
    // or by checking if the point is on the correct side of each edge.
    // For simplicity, we'll omit the full point-in-triangle test here and rely more on SAT.
    // A more robust implementation would include this for completeness.

    return true; // No separating axis found, likely a collision
}

bool Test_Capsule_And_Mesh_Collision(struct _Capsule_Collider* Capsule_Collider, struct _Preloaded_Collision_Mesh* Mesh, float x_Offset, float y_Offset, float z_Offset)
{

//Vector3 capsuleStart = {0.5f, -0.5f, 0.0f};
//    Vector3 capsuleEnd = {0.5f, 2.0f, 0.0f};

    Capsule cap;

        cap.a.x = Capsule_Collider->x1 + x_Offset;
        cap.a.y = Capsule_Collider->z1 + z_Offset;
        cap.a.z = Capsule_Collider->y1 + y_Offset;


        cap.b.x = Capsule_Collider->x2 + x_Offset;
        cap.b.y = Capsule_Collider->z2 + z_Offset;
        cap.b.z = Capsule_Collider->y2 + y_Offset;

        cap.radius = Capsule_Collider->Radius;

        //printf("CAPSULE: x1 %f\n y1 %f\n z1 %f\n x2 %f\n y2 %f\n z2 %f\n Radius %f\n",cap.a.x, cap.a.y, cap.a.z,cap.b.x, cap.b.y, cap.b.z, cap.r);

    for(int j = 0; j < Mesh->Number_Of_Triangles; j++)
    {
        if(triangle_capsule_sat(Mesh->Triangles[j], cap))
        {
            return true;
        }
    }
    return false;
};



struct _Preloaded_Collision_Mesh* Create_Preloaded_Collision_Mesh(const char* Filename)
{
    struct _SE3_Model* Model = Create_SE3_Model();
    Load_SE3_Model(Model,Filename);



    int i, j;
  GLfloat s, t;
  Vector v;
  struct Frame *pframe;
  struct Vertex *pvert;

  struct _Preloaded_Collision_Mesh* Mesh = (struct _Preloaded_Collision_Mesh*)calloc(1,sizeof(struct _Preloaded_Collision_Mesh));

  Mesh->x = 0;
  Mesh->y = 0;
  Mesh->z = 0;

  Mesh->Number_Of_Triangles = Model->Number_Of_Triangles;
  Mesh->Triangles = (_Triangle*)calloc(1,Mesh->Number_Of_Triangles*sizeof(_Triangle));

    Mesh->Maximum_Z = 0;
    Mesh->Minimum_Z = 0;

    for (int m = 0; m < Model->Number_Of_Triangles; m++)
      {
	/* Draw each vertex */

	    pframe = &Model->Frame_Data[0];
	    pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[0]];
	    Mesh->Triangles[m].v[0].x= (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    Mesh->Triangles[m].v[0].y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    Mesh->Triangles[m].v[0].z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

        pframe = &Model->Frame_Data[0];
	    pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[1]];
	    Mesh->Triangles[m].v[1].x= (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    Mesh->Triangles[m].v[1].y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    Mesh->Triangles[m].v[1].z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

        pframe = &Model->Frame_Data[0];
	    pvert = &pframe->First_Vertex[Model->Triangle_Indices[m].Triangle_Vertex_IDs[2]];
	    Mesh->Triangles[m].v[2].x= (pframe->Frame_Scale[0] * pvert->Coordinates[0]) + pframe->Frame_Translation_Vector[0];
	    Mesh->Triangles[m].v[2].y = (pframe->Frame_Scale[1] * pvert->Coordinates[1]) + pframe->Frame_Translation_Vector[1];
	    Mesh->Triangles[m].v[2].z = (pframe->Frame_Scale[2] * pvert->Coordinates[2]) + pframe->Frame_Translation_Vector[2];

	    if(Mesh->Triangles[m].v[0].z > Mesh->Maximum_Z)
	    {
            Mesh->Maximum_Z = Mesh->Triangles[m].v[0].z;
	    }
	    if(Mesh->Triangles[m].v[1].z > Mesh->Maximum_Z)
	    {
            Mesh->Maximum_Z = Mesh->Triangles[m].v[1].z;
	    }
	    if(Mesh->Triangles[m].v[2].z > Mesh->Maximum_Z)
	    {
            Mesh->Maximum_Z = Mesh->Triangles[m].v[2].z;
	    }

	    if(Mesh->Triangles[m].v[0].z < Mesh->Minimum_Z)
	    {
            Mesh->Minimum_Z = Mesh->Triangles[m].v[0].z;
	    }
	    if(Mesh->Triangles[m].v[1].z < Mesh->Minimum_Z)
	    {
            Mesh->Minimum_Z = Mesh->Triangles[m].v[1].z;
	    }
	    if(Mesh->Triangles[m].v[2].z < Mesh->Minimum_Z)
	    {
            Mesh->Minimum_Z = Mesh->Triangles[m].v[2].z;
	    }
      }
      printf("Maximum Z: %f\n",Mesh->Maximum_Z);
      return Mesh;
};


void Draw_Capsule_Collision_Geometry(struct _Capsule_Collider* Capsule_Collider)
{

}



// Helper function to calculate the cross product of two vectors
Vec3 cross_product(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

// Helper function to calculate the dot product of two vectors
double dot_product(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Helper function to subtract two vectors (a - b)
Vec3 subtract_vectors(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

// Helper function to project a triangle onto an axis and get the min/max projection values
void project_triangle(const _Triangle* triangle, Vec3 axis, double* min, double* max) {
    *min = INFINITY;
    *max = -INFINITY;
    for (int i = 0; i < 3; ++i) {
        double projection = dot_product(triangle->v[i], axis);
        if (projection < *min) *min = projection;
        if (projection > *max) *max = projection;
    }
}

// Helper function to check if two intervals overlap
bool overlap(double minA, double maxA, double minB, double maxB) {
    return (minA <= maxB && maxA >= minB);
}

// Function to get the face normal of a triangle
Vec3 get_face_normal(const _Triangle* triangle) {
    Vec3 edge1 = subtract_vectors(triangle->v[1], triangle->v[0]);
    Vec3 edge2 = subtract_vectors(triangle->v[2], triangle->v[0]);
    return cross_product(edge1, edge2);
}

// Function to get the edge normals of a triangle (perpendicular to the edges in the plane of the triangle)
void get_edge_normals(const _Triangle* triangle, Vec3 normals[3]) {
    Vec3 v0 = triangle->v[0];
    Vec3 v1 = triangle->v[1];
    Vec3 v2 = triangle->v[2];

    Vec3 edge1 = subtract_vectors(v1, v0);
    Vec3 edge2 = subtract_vectors(v2, v1);
    Vec3 edge3 = subtract_vectors(v0, v2);

    Vec3 face_normal = get_face_normal(triangle);

    normals[0] = cross_product(face_normal, edge1);
    normals[1] = cross_product(face_normal, edge2);
    normals[2] = cross_product(face_normal, edge3);
}

// Separating Axis Theorem (SAT) test for triangle-triangle collision
bool collide_triangles(const _Triangle* t1, const _Triangle* t2) {
    // Get the face normals of both triangles
    Vec3 normal1 = get_face_normal(t1);
    Vec3 normal2 = get_face_normal(t2);

    // Get the edge normals of both triangles
    Vec3 edge_normals1[3];
    get_edge_normals(t1, edge_normals1);
    Vec3 edge_normals2[3];
    get_edge_normals(t2, edge_normals2);

    // Array of all potential separating axes (face normals and cross products of edges)
    Vec3 axes[13];
    axes[0] = normal1;
    axes[1] = normal2;

    int axis_index = 2;
    for (int i = 0; i < 3; ++i) {
        axes[axis_index++] = edge_normals1[i];
    }
    for (int i = 0; i < 3; ++i) {
        axes[axis_index++] = edge_normals2[i];
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            axes[axis_index++] = cross_product(subtract_vectors(t1->v[i], t1->v[(i + 1) % 3]),
                                               subtract_vectors(t2->v[j], t2->v[(j + 1) % 3]));
        }
    }

    // Test for separation along each axis
    for (int i = 0; i < 13; ++i) {
        // Normalize the axis to avoid issues with zero vectors
        double magnitude_sq = dot_product(axes[i], axes[i]);
        if (magnitude_sq < 1e-9) continue; // Skip near-zero axes

        Vec3 axis = axes[i];
        double inv_magnitude = 1.0 / sqrt(magnitude_sq);
        axis.x *= inv_magnitude;
        axis.y *= inv_magnitude;
        axis.z *= inv_magnitude;

        double minA, maxA, minB, maxB;
        project_triangle(t1, axis, &minA, &maxA);
        project_triangle(t2, axis, &minB, &maxB);

        // If the projections do not overlap, the triangles are separated
        if (!overlap(minA, maxA, minB, maxB)) {
            return false; // No collision
        }
    }

    // If no separating axis was found, the triangles are colliding
    return true;
}









bool Test_Mesh_And_Mesh_Collision(struct _Preloaded_Collision_Mesh* Mesh,struct _Preloaded_Collision_Mesh* MeshTwo)
{

    for(int j = 0; j < Mesh->Number_Of_Triangles; j++)
    {
        for(int m = 0; m < MeshTwo->Number_Of_Triangles; m++)
        {
        if(collide_triangles(&Mesh->Triangles[j], &MeshTwo->Triangles[m]))
        {
            return true;
        }
        }
    }
    return false;
};


void Update_Mesh_Location(struct _Preloaded_Collision_Mesh* Mesh, float x, float y, float z)
{
    for (int m = 0; m < Mesh->Number_Of_Triangles; m++)
      {

	    Mesh->Triangles[m].v[0].x += x;
	    Mesh->Triangles[m].v[0].y += y;
	    Mesh->Triangles[m].v[0].z += z;

	    Mesh->Triangles[m].v[1].x += x;
	    Mesh->Triangles[m].v[1].y += y;
	    Mesh->Triangles[m].v[1].z += z;

	    Mesh->Triangles[m].v[2].x += x;
	    Mesh->Triangles[m].v[2].y += y;
	    Mesh->Triangles[m].v[2].z += z;

      }
}


bool Check_If_Ray_Sensor_Detects_Mesh(float Ray_x1, float Ray_y1, float Ray_z1, float Ray_x2, float Ray_y2, float Ray_z2,struct _Preloaded_Collision_Mesh* Mesh)
{
    Ray ray;
    ray.direction.x = Ray_x2;
    ray.direction.y = Ray_y2;
    ray.direction.z = Ray_z2;

    ray.origin.x = Ray_x1;
    ray.origin.y = Ray_y1;
    ray.origin.z = Ray_z1;

    double t,u,v;

    for(int j = 0; j < Mesh->Number_Of_Triangles; j++)
    {
        if(ray_triangle_intersect(ray, Mesh->Triangles[j], &t, &u, &v))
        {
            return true;
        }
    }
    return false;
}



// Vector subtraction: result = a - b
Vec3 subtract(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}



// Function to detect ray-triangle intersection using the Möller-Trumbore algorithm
bool ray_triangle_intersect(Ray ray, _Triangle triangle, double *t, double *u, double *v) {
    Vec3 edge1 = subtract(triangle.v[1], triangle.v[0]);
    Vec3 edge2 = subtract(triangle.v[2], triangle.v[0]);
    Vec3 h = cross_product(ray.direction, edge2);
    double a = dot_product(edge1, h);

    if (a > -EPSILON && a < EPSILON) {
        return false; // Ray is parallel to the triangle
    }

    double f = 1.0 / a;
    Vec3 s = subtract(ray.origin, triangle.v[0]);
    *u = f * dot_product(s, h);

    if (*u < 0.0 || *u > 1.0) {
        return false;
    }

    Vec3 q = cross_product(s, edge1);
    *v = f * dot_product(ray.direction, q);

    if (*v < 0.0 || (*u + *v) > 1.0) {
        return false;
    }

    *t = f * dot_product(edge2, q);

    if (*t > EPSILON) {
        return true; // Intersection at parameter t
    } else {
        return false; // No intersection (or intersection behind the ray origin)
    }
}











































// Function to find the minimum and maximum of two floats
void minmax(float a, float b, float *min, float *max) {
    *min = (a < b) ? a : b;
    *max = (a > b) ? a : b;
}

// Function to check if a point is inside an AABB
bool isPointInAABB(Vec3 point, AABB aabb) {
    return (point.x >= aabb.min.x && point.x <= aabb.max.x &&
            point.y >= aabb.min.y && point.y <= aabb.max.y &&
            point.z >= aabb.min.z && point.z <= aabb.max.z);
}

// Function to calculate the dot product of two vectors
float dotProduct(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Function to calculate the cross product of two vectors
Vec3 crossProduct(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

// Function to subtract two vectors
Vec3 subtractVectors(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

// Function to check for overlap in 1D
bool overlap1D(float min1, float max1, float min2, float max2) {
    return (max1 >= min2 && max2 >= min1);
}

// Function to project a triangle onto an axis
void projectTriangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 axis, float *minProj, float *maxProj) {
    float proj1 = dotProduct(v1, axis);
    float proj2 = dotProduct(v2, axis);
    float proj3 = dotProduct(v3, axis);

    *minProj = fminf(proj1, fminf(proj2, proj3));
    *maxProj = fmaxf(proj1, fmaxf(proj2, proj3));
}

// Function to project an AABB onto an axis
void projectAABB(AABB aabb, Vec3 axis, float *minProj, float *maxProj) {
    Vec3 corners[8];
    corners[0] = aabb.min;
    corners[1] = {aabb.max.x, aabb.min.y, aabb.min.z};
    corners[2] = {aabb.min.x, aabb.max.y, aabb.min.z};
    corners[3] = {aabb.min.x, aabb.min.y, aabb.max.z};
    corners[4] = {aabb.max.x, aabb.max.y, aabb.min.z};
    corners[5] = {aabb.max.x, aabb.min.y, aabb.max.z};
    corners[6] = {aabb.min.x, aabb.max.y, aabb.max.z};
    corners[7] = aabb.max;

    *minProj = dotProduct(corners[0], axis);
    *maxProj = *minProj;

    for (int i = 1; i < 8; ++i) {
        float projection = dotProduct(corners[i], axis);
        if (projection < *minProj) *minProj = projection;
        if (projection > *maxProj) *maxProj = projection;
    }
}



bool Check_If_AABB_Detects_Off_Center_Mesh(AABB aabb,struct _Preloaded_Collision_Mesh* Mesh, float Offset_x, float Offset_y, float Offset_z, float Mesh_Offset_X, float Mesh_Offset_Y, float Mesh_Offset_Z)
{
    AABB newAABB;
    newAABB.min.x = aabb.min.x + Offset_x;
    newAABB.min.y = aabb.min.y + Offset_y;
    newAABB.min.z = aabb.min.z + Offset_z;

    newAABB.max.x = aabb.max.x + Offset_x;
    newAABB.max.y = aabb.max.y + Offset_y;
    newAABB.max.z = aabb.max.z + Offset_z;

    for(int j = 0; j < Mesh->Number_Of_Triangles; j++)
    {
        Vec3 vec1;
        Vec3 vec2;
        Vec3 vec3;

        vec1.x = Mesh->Triangles[j].v[0].x + Mesh_Offset_X;
        vec1.y = Mesh->Triangles[j].v[0].y + Mesh_Offset_Y;
        vec1.z = Mesh->Triangles[j].v[0].z + Mesh_Offset_Z;

        vec2.x = Mesh->Triangles[j].v[1].x + Mesh_Offset_X;
        vec2.y = Mesh->Triangles[j].v[1].y + Mesh_Offset_Y;
        vec2.z = Mesh->Triangles[j].v[1].z + Mesh_Offset_Z;

        vec3.x = Mesh->Triangles[j].v[2].x + Mesh_Offset_X;
        vec3.y = Mesh->Triangles[j].v[2].y + Mesh_Offset_Y;
        vec3.z = Mesh->Triangles[j].v[2].z + Mesh_Offset_Z;

        if(checkAABBTriangleCollision(newAABB, vec1, vec2, vec3))
        {
            return true;
        }
    }
    return false;
}




bool Check_If_AABB_Detects_Mesh(AABB aabb,struct _Preloaded_Collision_Mesh* Mesh, float Offset_x, float Offset_y, float Offset_z)
{
    AABB newAABB;
    newAABB.min.x = aabb.min.x + Offset_x;
    newAABB.min.y = aabb.min.y + Offset_y;
    newAABB.min.z = aabb.min.z + Offset_z;

    newAABB.max.x = aabb.max.x + Offset_x;
    newAABB.max.y = aabb.max.y + Offset_y;
    newAABB.max.z = aabb.max.z + Offset_z;

    for(int j = 0; j < Mesh->Number_Of_Triangles; j++)
    {
        Vec3 vec1;
        Vec3 vec2;
        Vec3 vec3;

        vec1.x = Mesh->Triangles[j].v[0].x;
        vec1.y = Mesh->Triangles[j].v[0].y;
        vec1.z = Mesh->Triangles[j].v[0].z;

        vec2.x = Mesh->Triangles[j].v[1].x;
        vec2.y = Mesh->Triangles[j].v[1].y;
        vec2.z = Mesh->Triangles[j].v[1].z;

        vec3.x = Mesh->Triangles[j].v[2].x;
        vec3.y = Mesh->Triangles[j].v[2].y;
        vec3.z = Mesh->Triangles[j].v[2].z;

        if(checkAABBTriangleCollision(newAABB, vec1, vec2, vec3))
        {
            return true;
        }
    }
    return false;
}

// Function to detect collision between an AABB and a triangle
bool checkAABBTriangleCollision(AABB aabb, Vec3 v1, Vec3 v2, Vec3 v3) {
    // 1. Check if any of the triangle's vertices are inside the AABB
    if (isPointInAABB(v1, aabb) || isPointInAABB(v2, aabb) || isPointInAABB(v3, aabb)) {
        return true;
    }

    // 2. Check if any of the AABB's vertices are inside the triangle (more complex, often omitted for efficiency)
    //    This would involve plane equations and barycentric coordinates.

    // 3. Use the Separating Axis Theorem (SAT)
    // Define the axes to test:
    //    - The 3 face normals of the AABB (aligned with the x, y, and z axes)
    //    - The normal of the triangle's plane
    //    - The cross products of the triangle's edges with the AABB's face normals

    Vec3 aabb_axes[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    Vec3 edge1 = subtractVectors(v2, v1);
    Vec3 edge2 = subtractVectors(v3, v2);
    Vec3 edge3 = subtractVectors(v1, v3);

    Vec3 triangle_normal = crossProduct(edge1, edge2);

    Vec3 triangle_edge_aabb_axes[9];
    triangle_edge_aabb_axes[0] = crossProduct(edge1, aabb_axes[0]);
    triangle_edge_aabb_axes[1] = crossProduct(edge1, aabb_axes[1]);
    triangle_edge_aabb_axes[2] = crossProduct(edge1, aabb_axes[2]);
    triangle_edge_aabb_axes[3] = crossProduct(edge2, aabb_axes[0]);
    triangle_edge_aabb_axes[4] = crossProduct(edge2, aabb_axes[1]);
    triangle_edge_aabb_axes[5] = crossProduct(edge2, aabb_axes[2]);
    triangle_edge_aabb_axes[6] = crossProduct(edge3, aabb_axes[0]);
    triangle_edge_aabb_axes[7] = crossProduct(edge3, aabb_axes[1]);
    triangle_edge_aabb_axes[8] = crossProduct(edge3, aabb_axes[2]);

    // Normalize the triangle normal (optional but recommended for numerical stability)
    float normal_magnitude = sqrtf(dotProduct(triangle_normal, triangle_normal));
    if (normal_magnitude > 1e-6) { // Avoid division by zero
        triangle_normal.x /= normal_magnitude;
        triangle_normal.y /= normal_magnitude;
        triangle_normal.z /= normal_magnitude;
    }

    // Test projection on AABB face normals
    for (int i = 0; i < 3; ++i) {
        float minA, maxA, minB, maxB;
        projectAABB(aabb, aabb_axes[i], &minA, &maxA);
        projectTriangle(v1, v2, v3, aabb_axes[i], &minB, &maxB);
        if (!overlap1D(minA, maxA, minB, maxB)) {
            return false; // Separating axis found
        }
    }

    // Test projection on the triangle's normal
    float minA, maxA, minB, maxB;
    projectAABB(aabb, triangle_normal, &minA, &maxA);
    projectTriangle(v1, v2, v3, triangle_normal, &minB, &maxB);
    if (!overlap1D(minA, maxA, minB, maxB)) {
        return false; // Separating axis found
    }

    // Test projection on the cross products of edges and AABB normals
    for (int i = 0; i < 9; ++i) {
        float minA, maxA, minB, maxB;
        // Normalize the axis before projection (important for SAT)
        float axis_magnitude = sqrtf(dotProduct(triangle_edge_aabb_axes[i], triangle_edge_aabb_axes[i]));
        if (axis_magnitude > 1e-6) {
            Vec3 normalized_axis = {
                triangle_edge_aabb_axes[i].x / axis_magnitude,
                triangle_edge_aabb_axes[i].y / axis_magnitude,
                triangle_edge_aabb_axes[i].z / axis_magnitude
            };
            projectAABB(aabb, normalized_axis, &minA, &maxA);
            projectTriangle(v1, v2, v3, normalized_axis, &minB, &maxB);
            if (!overlap1D(minA, maxA, minB, maxB)) {
                return false; // Separating axis found
            }
        }
    }

    // If no separating axis was found, the AABB and triangle are colliding
    return true;
}



bool checkAABBCollision(AABB box1, AABB box2, float Offset_x, float Offset_y, float Offset_z) {
    // Check for overlap on the x-axis

    if (box1.max.x < box2.min.x || box2.max.x < box1.min.x) {
        return false; // No overlap on x-axis, so no collision
    }

    // Check for overlap on the y-axis
    if (box1.max.y < box2.min.y || box2.max.y < box1.min.y) {
        return false; // No overlap on y-axis, so no collision
    }

    // Check for overlap on the z-axis
    if (box1.max.z < box2.min.z || box2.max.z < box1.min.z) {
        return false; // No overlap on z-axis, so no collision
    }

    // If there is overlap on all three axes, then the boxes collide
    return true;
}

int Check_If_Mouse_Clicked_On_Button(struct _Keypad Keypad, double Mouse_X, double Mouse_Y, float box_x1, float box_x2, float box_y1, float box_y2)
{
    //1 = Mouse over;
    //2 = Mouse Clicked;
    //0 = Mouse not over
   // printf("%f\n", Keypad.Mouse_X);
    //printf("%f\n", Keypad.Mouse_Y);

   // printf("Box x1 %f\n", box_x1);
   // printf("Box y1 %f\n", box_y1);

   // printf("Box x2 %f\n", box_x2);
   // printf("Box y2 %f\n", box_y2);

    int Result;
    if(Keypad.Mouse_X > box_x1 && Keypad.Mouse_X < box_x2)
    {
     //   printf("X Matching\n");
        if(Keypad.Mouse_Y > box_y1 && Keypad.Mouse_Y < box_y2)
        {
        //    printf("Y Matching\n");
         //   printf("Mouse over!");
            if(Keypad.Mouse_Is_Clicked)
            {
            return 2;
            }
            else if(!Keypad.Mouse_Is_Clicked)
            {
            return 1;
            }
        }
    }
   // printf("Mouse not over!");
    return 0;
}


bool Check_If_Ray_Sensor_Detects_AABB(AABB box1, float Ray_x1, float Ray_y1, float Ray_z1, float Ray_x2, float Ray_y2, float Ray_z2, float Offset_x, float Offset_y, float Offset_z) {
    Ray_x1 += Offset_x;
    Ray_y1 += Offset_y;
    Ray_z1 += Offset_z;

    Ray_x2 += Offset_x;
    Ray_y2 += Offset_y;
    Ray_z2 += Offset_z;


    // Check for overlap on the x-axis
    if (box1.max.x < Ray_x2 || Ray_x1 < box1.min.x) {
        return false; // No overlap on x-axis, so no collision
    }

    // Check for overlap on the y-axis
    if (box1.max.y < Ray_y2 || Ray_y1 < box1.min.y) {
        return false; // No overlap on y-axis, so no collision
    }

    // Check for overlap on the z-axis
    if (box1.max.z < Ray_z2 || Ray_z1 < box1.min.z) {
        return false; // No overlap on z-axis, so no collision
    }

    // If there is overlap on all three axes, then the boxes collide
    return true;
}

vec3_t vec3_t_sub(const vec3_t a, const vec3_t b) {
    vec3_t out;
    out.x = a.x - b.x; out.y = a.y - b.y; out.z = a.z - b.z;
    return out;
}


vec3_t vec3_t_cross(const vec3_t a, const vec3_t b) {
    vec3_t out;
    out.x = (a.y * b.z) - (a.z * b.y);
    out.y = (a.z * b.x) - (a.x * b.z);
    out.z = (a.x * b.y) - (a.y * b.x);
    return out;
}

double vec3_t_sq_mag(vec3_t v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// Function to get the magnitude (length) of a vector
double vec3_t_mag(vec3_t v) {
    return sqrt(vec3_t_sq_mag(v));
}

vec3_t vec3_t_normalize(vec3_t v) {
    float mag = vec3_t_mag(v);
    if (mag > 1e-6) { // Avoid division by zero
        return (vec3_t){v.x / mag, v.y / mag, v.z / mag};
    }
    return (vec3_t){0.0, 0.0, 0.0};
}

Vec3 V3_scale(Vec3 v, float s) {
    return (Vec3){v.x * s, v.y * s, v.z * s};
}

double Get_Theta_From_XY(double x, double y, double xtwo, double ytwo)
{
    double Theta = 0;
    if(xtwo-x != 0)
    {
    Theta = atan((ytwo-y)/(xtwo-x));
    }
    return Theta;
}

Vec3 Get_Cylindrical_Onboard_Coordinates(double Ship_Center_x, double Ship_Center_y, double Ship_Center_z, double Ship_Rotation, double Onboard_Object_x,double Onboard_Object_y,double Onboard_Object_z)
{
    double Object_Old_x = Onboard_Object_x;
    double Object_Old_y = Onboard_Object_y;
    double Object_Old_z = Onboard_Object_z;

    double Object_Old_Distance_From_Ship_Center = Get_Distance(Object_Old_x,Object_Old_y,0,Ship_Center_x,Ship_Center_y,0);
    double Object_Old_Theta_Coordinate = Get_Theta_From_XY(Object_Old_x,Object_Old_y, Ship_Center_x, Ship_Center_y);

    double Object_New_Theta_Coordinate = Ship_Rotation*0.01745329251; //radians

    double Object_New_x = Ship_Center_x + Object_Old_Distance_From_Ship_Center * cos(Object_New_Theta_Coordinate);
    double Object_New_y = Ship_Center_y + Object_Old_Distance_From_Ship_Center * sin(Object_New_Theta_Coordinate);
    double Object_New_z = Object_Old_z;

    Vec3 Result;
    Result.x = Object_New_x;
    Result.y = Object_New_y;
    Result.z = Object_New_z;

    return Result;
}

double Get_Cylindrical_Onboard_Rotation(double Ship_Rotation, double Object_Internal_Rotation)
{
    //When a ship rotates, convert everything to cylindrical coodrds and back.
}

double Get_Radius_From_XY(double x, double y)
{
    double Radius = sqrt((x*x) + (y*y));
    return Radius;
}





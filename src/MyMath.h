#pragma once
#include <math.h>

# define M_PI 3.14159265358979323846

float radian(float deg)
{
    float r = deg*M_PI/180.0f;
    return r;
}


struct vec3
{
    float x,y,z;
};

vec3 operator+(vec3 &left, vec3 &right)
{
    return {left.x+right.x, left.y+right.y, left.z + right.z};
}

vec3 operator-(vec3 &vec)
{
    return {-vec.x, -vec.y, -vec.z};
}

vec3 operator-(vec3 &left, vec3 &right)
{
    return {left.x-right.x, left.y-right.y, left.z - right.z};
}

float Dot(vec3 &left, vec3 &right)
{
    return (left.x*right.x + left.y*right.y + left.z+right.z);
}

float operator*(vec3 &left, vec3 &right)
{
    return Dot(left, right);
}

vec3 Cross(vec3 &left, vec3 &right)
{
    return {left.y*right.z-left.z*right.y, left.z*right.x-left.x*right.z, left.x*right.y-left.y*right.x};
}

vec3 operator^(vec3 &left, vec3 &right)
{
    return Cross(left, right);
}

float len(vec3 &vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

std::ostream& operator<<(std::ostream &o, const vec3& vec)
{
    return o << "vec3 { " << vec.x << " , " << vec.y << " , " << vec.z << " }";
}

float distanceTo(vec3 &a, vec3 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz); 
}

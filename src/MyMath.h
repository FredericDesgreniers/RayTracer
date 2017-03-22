#pragma once
#include <math.h>

# define M_PI 3.14159265358979323846

float radian(float deg)
{
    float r = deg*M_PI/180.0f;
    return r;
}

float Max(float a, float b)
{
    return (a > b)?a:b;
}

float Min(float a, float b)
{
    return (a < b)?a:b;
}




struct vec3
{
    union {
        struct{
            float x,y,z;
        };
        struct{
            float r,g,b;
        };
    };
};

vec3 Mult(vec3 &left, vec3 &right)
{
    return {
        left.x*right.x, left.y*right.y, left.z*right.z
    };
}

vec3 operator+(vec3 &left, vec3 &right)
{
    return {left.x+right.x, left.y+right.y, left.z + right.z};
}

vec3 operator-(vec3 &vec)
{
    return {-vec.x, -vec.y, -vec.z};
}

vec3 operator*(float d, vec3 &vec)
{
    return {d*vec.x, d*vec.y, d*vec.z};
}

vec3 operator*(vec3 &vec,float d)
{
    return {d*vec.x, d*vec.y, d*vec.z};
}


vec3 operator/(vec3 &vec,float d)
{
    return {vec.x/d, vec.y/d, vec.z/d};
}


vec3 operator-(vec3 &left, vec3 &right)
{
    return {left.x-right.x, left.y-right.y, left.z - right.z};
}

float Dot(vec3 &left, vec3 &right)
{
    return (left.x*right.x + left.y*right.y + left.z*right.z);
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

float Len(vec3 &vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

vec3 Norm(vec3 &vec)
{
    float length = abs(Len(vec));
    return {vec.x/length, vec.y/length, vec.z/length};
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

bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1) 
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
        -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    
    return true;
}

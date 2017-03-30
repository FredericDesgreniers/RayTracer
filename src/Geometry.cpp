#define EPSILON 0.000001

#define PLANE 0
#define  SPHERE 1
#define TRIANGLE 2

//scene geometry
struct Object
{
    
};

struct Surface
{
    vec3 amb;
    vec3 dif;
    vec3 spe;
    float shi;
    
    int type;
};

//camera object
struct Camera
{
    vec3 pos;
    float fov; //Field of view
    float f; //focal length
    float a; //aspect ratio
};

std::ostream& operator<<(std::ostream &o, const Camera& cam)
{
    return o << "Camera { pos:" << cam.pos  << " , fov:" << cam.fov << " , f:" << cam.f  << " , a:" << cam.a <<  " }"; 
}


struct Light:public Object
{
    vec3 pos;
    vec3 col;
};

std::ostream& operator<<(std::ostream &o, const Light& light)
{
    return o << "Light { pos:" << light.pos  << " , color:"<< light.col <<  " }"; 
}


struct Plane:public Surface
{
    vec3 nor;
    vec3 pos;
};
std::ostream& operator<<(std::ostream &o, const Plane& plane)
{
    return o << "Plane { nor: " << plane.nor << "pos: " << plane.pos <<  " }"; 
}


struct Triangle:public Surface
{
    vec3 v1;
    vec3 v2;
    vec3 v3;
};

struct Sphere:public Surface
{
    vec3 pos;
    float rad;
};


struct Ray
{
    vec3 pos;
    vec3 dir;
};

struct Intersection
{
    bool valid;
    int solNum;
    float distance1;
    float distance2;
};

//Intersection of ray and sphere
Intersection Intersects(Ray &ray, Sphere &sphere)
{
    
    vec3 dOrigin = ray.pos - sphere.pos;
    
    float a = (ray.dir * (dOrigin));
    
    float sqrtPart = a*a - Len(dOrigin)*Len(dOrigin) + (sphere.rad * sphere.rad);
    
    Intersection inter = {0};
    
    if(sqrtPart < 0) //No solution
    {
        inter.valid = false;
        return inter;
    }
    
    inter.valid = true;
    
    if(sqrtPart == 0) //one solution
    {
        inter.solNum = 1;
        inter.distance1 = -a;
    }
    else //two solutions
    {
        inter.solNum = 2;
        
        inter.distance1 = -a + sqrt(sqrtPart);
        inter.distance2 = -a - sqrt(sqrtPart);
        
    }
    
    return inter;
    /**
    
    Intersection inter;
    
    float t0, t1;
    vec3 dir = ray.dir;;
    vec3 L = ray.pos-sphere.pos;
    float a = dir * dir;
    float b = 2 * (dir * L);
    float c = (L * L)  - pow(sphere.rad,2);
    if (!solveQuadratic(a, b, c, t0, t1)) 
    {
        inter.valid = false;
        return inter;
    }
    
    inter.valid = true;
    inter.solNum = 2;
    inter.distance1 = t0;
    inter.distance2 = t1;
    return inter;
    **/
}

//Intersection of Ray and Plane
Intersection Intersects(Ray &ray, Plane &plane)
{
    float bottomPart = ray.dir * plane.nor;
    float upperPart = (plane.pos - ray.pos) * plane.nor;
    
    Intersection inter = {0};
    
    if(bottomPart==0)
    {
        inter.valid = false;
        
        return inter;
    }
    
    //one solution
    inter.valid = true;
    inter.solNum = 1;
    inter.distance1 = upperPart / bottomPart;
    
    return inter;
}

//Intersection of Ray and Triangle
Intersection Intersects(Ray &ray, Triangle &triangle)
{
    //edges
    vec3  e1,e2;
    
    vec3 P, Q, T;
    
    float det, inv_det, u, v;
    
    //final distance
    float t;
    
    //calculate edgtes
    e1 = triangle.v2 - triangle.v1;
    e2 = triangle.v3 - triangle.v1;
    
    P = Norm(ray.dir) ^ e2;
    
    det = e1 * P;
    
    Intersection inter = {0};
    
    if(det > -EPSILON && det < EPSILON)
    {
        inter.valid = false;
        return inter;
    }
    
    inv_det = 1.0f/det;
    
    T = ray.pos - triangle.v1;
    
    u = (T*P)* inv_det;
    
    if(u < 0.0f || u > 1.0f)
    {
        inter.valid = false;
        return inter;
    }
    
    Q = T ^ e1;
    
    v = (ray.dir*Q) * inv_det;
    
    if(v < 0.0f || (u+v) > 1.0f)
    {
        inter.valid = false;
        return inter;
    }
    
    t = (e2 * Q) * inv_det;
    if(t > EPSILON)
    {
        inter.valid = true;
        inter.solNum = 1;
        inter.distance1 = t;
        return inter;
    }
    
    inter.valid = false;
    return inter;
}


Intersection IntersectsSurface(Ray &ray, Surface &surface)
{
    switch(surface.type)
    {
        case PLANE:
        return Intersects(ray, *(static_cast<Plane *>(&surface)));
        
        case TRIANGLE:
        return Intersects(ray, *(static_cast<Triangle *>(&surface)));
        
        case SPHERE:
        return Intersects(ray, *(static_cast<Sphere *>(&surface)));
        
    }
    
    Intersection inter = {0};
    inter.valid = false;
    return inter;
}

vec3 GetNormal(Surface* surface, Ray &ray)
{
    vec3 normal;
    switch(surface->type)
    {
        case PLANE:
        {
            normal = Norm((static_cast<Plane *>(surface))->nor);
            break;
        }
        case TRIANGLE:
        {
            Triangle *triangle = (static_cast<Triangle *>(surface));
            normal = Norm(((triangle->v2)-triangle->v1) ^ (triangle->v3-triangle->v1));
            break;
        }
        case SPHERE:
        {
            Sphere *sphere= (static_cast<Sphere *>(surface));
            
            normal = Norm(ray.pos-sphere->pos);
            
            
            break;
        }
    }
    return normal;
    
}
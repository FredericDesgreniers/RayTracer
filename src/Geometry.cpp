
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
    
    float sqrtPart = a*a - len(dOrigin) + (sphere.rad * sphere.rad);
    
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
        inter.solNum = 1;
        
        inter.distance1 = -a + sqrt(sqrtPart);
        inter.distance2 = -a - sqrt(sqrtPart);
        
    }
    
    
    return inter;
}

//Intersection of Ray and Plane
Intersection Intersects(Ray &ray, Plane &plane)
{
    float bottomPart = plane.nor * ray.dir;
    float upperPart = (plane.pos - ray.pos) * plane.nor;
    
    Intersection inter = {0};
    
    if(bottomPart==0)
    {
        if(upperPart == 0) //infinite solutions
        {
            inter.valid = true;
            inter.solNum = -1;
        }
        else //no solution
        {
            inter.valid = false;
        }
        
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
    //TODO calculate intersection of ray and triangle
    
    Intersection inter = {0};
    return inter;
}
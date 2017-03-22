#include "SceneCreator.h"
#define accuracy 0.00001

Surface* ShootRay(Ray &ray, Scene &scene, float &d)
{
    
    float bd = -1;
    
    Surface *bestSurface = nullptr;
    
    for(int objectIndex=0; objectIndex < scene.surfaces.size(); objectIndex++)
    {
        Surface *surface = scene.surfaces.at(objectIndex);
        
        Intersection inter = IntersectsSurface(ray, *surface);
        
        if(inter.valid)
        {
            //std::cout << "Pixel " << pixelPoint << " intersects at " << inter.distance1 << std::endl;
            float distance1 = inter.distance1;
            
            if(distance1 > accuracy && (distance1 <= bd || bd < 0))
            {
                bd = distance1;
                bestSurface = surface;
            }
            
            if(inter.solNum > 1)
            {
                float distance2 = inter.distance2;
                
                if(distance2 > accuracy && (distance2 <= bd || bd < 0))
                {
                    bd = distance2;
                    bestSurface= surface;
                }
                
            }
        }
        else
        {
            //std::cout << "Pixel " << pixelPoint << " does NOT intersects " << std::endl;
            
        }
        
    }
    d = bd;
    return bestSurface;
    
    
}

void GetReflectionColor(Ray &ray, Scene &scene, float level,vec3 &color)
{
    float distance;
    Surface *rSurface = ShootRay(ray, scene, distance);
    
    if(rSurface == nullptr)
    {
        
        return;
    }
    else
    {
        //std::cout << rSurface->type << " < ";
        
        vec3 normal = GetNormal(rSurface, ray);
        
        
        vec3 R =//-Norm(ray.dir - 2 * normal *(ray.dir * normal)); 
        Norm(2 * ((ray.dir * normal) * normal) - ray.dir);
        
        
        vec3 SpectralColor = rSurface->spe;
        
        color = color + level * ( SpectralColor + rSurface->amb + rSurface->dif);
        
        Ray newRay = {0};
        
        newRay.pos = ray.pos + (ray.dir * distance);
        
        newRay.dir = R;
        
        GetReflectionColor(newRay, scene,  level/2, color);
    }
}

vec3 GetPixelColorAt(vec3 position, Scene &scene)
{
    vec3 pixelPoint = {position.x - scene.width/2 ,position.y - scene.height/2, -scene.camera.f };
    
    pixelPoint = pixelPoint + scene.camera.pos;
    
    Ray ray = {0};
    
    ray.dir = Norm({pixelPoint - scene.camera.pos});
    ray.pos = scene.camera.pos;
    
    float d;
    
    Surface *surface = ShootRay(ray, scene, d);
    
    float acc = 0.0001;
    
    if(surface != nullptr){
        
        vec3 color = {0, 0, 0};
        
        for(int LightIndex=0; LightIndex < scene.lights.size(); LightIndex++)
        {
            Light *light = scene.lights.at(LightIndex);
            Ray LightRay = {0};
            LightRay.pos = ray.pos + (d * ray.dir);
            LightRay.dir = Norm(light->pos- LightRay.pos);
            
            //LightRay.pos = LightRay.pos + (0.001f * LightRay.dir);
            
            vec3 normal = GetNormal(surface, LightRay);
            
            float nL = (Norm(normal) * Norm(LightRay.dir));
            
            if(nL > 1)
            {
                nL = 1;
            }
            
            if(nL < accuracy){
                
            }else
            {
                float d1;
                Surface* lightSurface = ShootRay(LightRay, scene, d1);
                
                if((lightSurface == nullptr || lightSurface == surface) && d1 <= Len(light->pos - LightRay.pos))
                {
                    vec3 DiffuseColor = {
                        (surface->dif.r * Max(0, nL)),
                        (surface->dif.g * Max(0, nL)), 
                        (surface->dif.b * Max(0, nL))
                    };
                    
                    vec3 R = //-Norm(LightRay.dir - 2 * normal *(LightRay.dir * normal)); 
                    Norm(2 * ((LightRay.dir * normal) * normal) - LightRay.dir);
                    
                    Ray rRay = {0};
                    rRay.pos = LightRay.pos;
                    rRay.dir = R;
                    
                    float rD;
                    
                    float RV = R * LightRay.dir;
                    
                    if(RV > 1)
                        RV = 1;
                    
                    vec3 SpectralColor = {0};
                    
                    if(RV >= accuracy)
                    {
                        float MaxRV = pow(Max(0, RV), surface->shi );
                        
                        vec3 ReflectionColor=  {0};
                        
                        //std::cout << surface->type << " < ";
                        
                        GetReflectionColor(rRay, scene,0.5f, ReflectionColor);
                        
                        if(ReflectionColor.r > 1)
                            ReflectionColor.r = 1;
                        
                        if(ReflectionColor.g > 1)
                            ReflectionColor.g = 1;
                        
                        if(ReflectionColor.b > 1)
                            ReflectionColor.b = 1;
                        
                        
                        //std::cout << std::endl;
                        int refs;
                        if(ReflectionColor.x != 0)
                        {
                            refs++;
                            //std::cout << ReflectionColor << std::endl;
                        }
                        
                        SpectralColor =  MaxRV*Mult((vec3{1,1,1}-ReflectionColor), surface->spe);
                        
                    }
                    
                    
                    
                    vec3 addition = DiffuseColor + SpectralColor;
                    
                    color.r = color.r + light->col.r*(addition.r);
                    color.g = color.g + light->col.g*(addition.g);
                    color.b = color.b + light->col.b*(addition.b);
                    
                }
                
                
            }
            
        }
        /**
        color = {color.x*surface->amb.x,color.y*surface->amb.y,color.z*surface->amb.z};
        */
        color = color + (surface->amb * 0.5f);
        color.r = Min(color.r, 1);
        color.g = Min(color.g, 1);
        color.b = Min(color.b, 1);
        
        return color;
    }
    else 
    {
        return {0,0,0};
    }
    
}

int main()
{
    vec3 cam_dir = {0, 0, -1};
    vec3 cam_down = {0, -1, 0};
    vec3 cam_right = {1, 0, 0};
    
    Scene scene = {};
    
    ReadSceneFile("scene.txt", scene);
    
    std::cout << "Scene has " << scene.objectNum  << " objects" <<std::endl;
    
    scene.height = (round(tan(radian(scene.camera.fov)/2) * 2.0f * scene.camera.f));
    scene.width = scene.height * scene.camera.a;
    
    std::cout << scene.width << "x" << scene.height << std::endl;
    
    std::vector<vec3> picData;
    
    cimg_library::CImg<unsigned char> image(scene.width, scene.height, 1,3,0);
    
    
    
    vec3 Cam_ray_origin = scene.camera.pos;
    float xamt, yamt;
    
    int AntiAliasing = 2;
    
    for(int x = 0; x < scene.width; x++)
    {
        for(int y=0; y < scene.height; y++)
        {
            std::vector<vec3> colors;
            
            vec3 CenterColor = GetPixelColorAt({(float)x,(float)y,0.0f}, scene);
            
            colors.push_back(CenterColor);
            
            for(int i=0; i < AntiAliasing; i++)
            {
                for(int j=0; j < AntiAliasing; j++)
                {
                    vec3 color = GetPixelColorAt({(float)x-3+i*3/AntiAliasing, (float)y-3+j*3/AntiAliasing, 0.0f}, scene);
                    colors.push_back({color.r, color.g, color.b});
                    
                }
            }
            
            vec3 FinalColor = {0};
            for(int i=0; i < colors.size();i ++)
            {
                FinalColor = FinalColor + colors.at(i);
            }
            FinalColor = FinalColor / colors.size();
            
            
            const unsigned char colorData[] = {FinalColor.r*255, FinalColor.g*255, FinalColor.b*255};
            image.draw_point(x,scene.height-y-1,colorData);
            
        }
    }
    
    image.save("render.bmp");
    
    cimg_library::CImgDisplay main_disp(image,"Render");
    while (!main_disp.is_closed())
        main_disp.wait();
    
    return 0;
}

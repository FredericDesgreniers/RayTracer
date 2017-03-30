#include "SceneCreator.h"
#define accuracy 0.00001

Surface* ShootRay(Ray ray, Scene &scene, float &d)
{
    ray.pos = ray.pos + ray.dir*0.1;
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


vec3 GetRayColorAt(Ray ray,bool softshadows, bool rr,  Scene &scene, int recursions)
{
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
            
            
            if(nL > 0)
            {
                float d1;
                
                float intensity;
                
                int numRays = 15;
                int lightRays = 0;
                if(softshadows)
                {
                    for(int i=0; i < numRays; i++)
                    {
                        float jitter = 0.01f;
                        
                        float offsetX = float(rand()%10)/10.0f*(jitter*2)-jitter;
                        
                        float offsetY = float(rand()%10)/10.0f*(jitter*2)-jitter;
                        
                        float offsetZ = float(rand()%10)/10.0f*(jitter*2)-jitter;
                        
                        vec3 offset = {offsetX, offsetY, offsetZ};
                        
                        //std::cout << LightRay.dir << " : " << offset << std::endl;
                        
                        
                        Ray offLightRay = {LightRay.pos, LightRay.dir};
                        
                        float offDistance;
                        
                        Surface* lightSurface = ShootRay(offLightRay, scene, offDistance);
                        
                        if(lightSurface == nullptr && offDistance <= Len(light->pos - LightRay.pos)+1)
                        {
                            lightRays++;
                        }
                        
                    }
                    intensity = (float)lightRays / (float)numRays;
                }
                else
                {
                    float offDistance;
                    Surface* lightSurface = ShootRay(LightRay, scene, offDistance);
                    
                    if(lightSurface == nullptr && offDistance <= Len(light->pos - LightRay.pos))
                    {
                        intensity = 1.0f;
                    }
                }
                
                
                
                
                
                
                
                float MaxnL = Max(0, nL);
                
                vec3 DiffuseColor = surface->dif * MaxnL;
                
                
                vec3 R = //-Norm(LightRay.dir - 2 * normal *(LightRay.dir * normal)); 
                Norm(2 * ((LightRay.dir * normal) * normal) - LightRay.dir);
                
                Ray rRay = {0};
                rRay.pos = LightRay.pos;
                rRay.dir = R;
                
                float rD;
                
                float RV = R * -ray.dir;
                
                vec3 SpectralColor = {0};
                
                
                if(RV >= 0.0001f)
                {
                    float MaxRV = Max(0, pow(RV,  surface->shi) );
                    
                    
                    
                    //std::cout << surface->type << " < ";
                    
                    /**
                    if(rr)
                        GetReflectionColor(rRay, scene,0.5f, ReflectionColor);
                    **/
                    
                    
                    //SpectralColor =  MaxRV*Mult((vec3{1,1,1}-ReflectionColor), surface->spe);
                    SpectralColor =  MaxRV*(surface->spe );
                    
                }
                
                
                
                vec3 addition = intensity * (DiffuseColor + SpectralColor);
                
                color = color + Mult(light->col, addition);
                
            }
            
            
            
            
        }
        
        vec3 ReflectionColor=  {0};
        
        
        if(recursions < 10)
        {
            Ray ray3 = {ray.pos+ray.dir*d, ray.dir};
            
            vec3 normal = Norm(GetNormal(surface, ray3));
            
            vec3 reflection_ray_origin =  (ray.pos+(ray.dir*d));
            
            vec3 R2 = //-Norm(LightRay.dir - 2 * normal *(LightRay.dir * normal)); 
            //Norm(2 * ((ray.dir * normal) * normal) - ray.dir);
            ray.dir - 2*(normal*ray.dir)*normal;
            
            ReflectionColor = GetRayColorAt({reflection_ray_origin, R2}, true ,true  , scene, recursions+1);
            
            
            
            //std::cout << ReflectionColor << std::endl;
            
        }
        
        
        /**
        color = {color.x*surface->amb.x,color.y*surface->amb.y,color.z*surface->amb.z};
        */
        color = color + (surface->amb * 0.5f)+ 0.25f*ReflectionColor;
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

vec3 GetPixelColorAt(bool softshadows, bool rr, vec3 position, Scene &scene)
{
    vec3 pixelPoint = {position.x - scene.width/2 ,position.y - scene.height/2, -scene.camera.f };
    
    pixelPoint = pixelPoint + scene.camera.pos;
    
    Ray ray = {0};
    
    ray.dir = Norm({pixelPoint - scene.camera.pos});
    ray.pos = scene.camera.pos;
    
    return GetRayColorAt(ray,softshadows, rr, scene, 0);
}

Scene scene;
cimg_library::CImg<unsigned char> image;

void calculateImageData(int AntiAliasing, bool softshadows, bool rr,  int minX, int maxX, int minY, int maxY)
{
    for(int x = minX; x < maxX; x++)
    {
        for(int y=minY; y < maxY; y++)
        {
            std::vector<vec3> colors;
            
            vec3 CenterColor = GetPixelColorAt(softshadows, rr, {(float)x,(float)y,0.0f}, scene);
            
            colors.push_back(CenterColor);
            
            for(int i=0; i < AntiAliasing; i++)
            {
                for(int j=0; j < AntiAliasing; j++)
                {
                    float xOffset = float(rand()%3)-1.5f;
                    float yOffset = float(rand()%3)-1.5f;
                    
                    vec3 color = GetPixelColorAt(softshadows, rr, {(float)x+xOffset, (float)y+yOffset, 0.0f}, scene);
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
}

//main method
int main()
{
    
    ReadSceneFile("scene.txt", scene);
    
    std::cout << "Scene has " << scene.objectNum  << " objects" <<std::endl;
    
    scene.height = (round(tan(radian(scene.camera.fov)/2) * 2.0f * scene.camera.f));
    scene.width = scene.height * scene.camera.a;
    
    std::cout << scene.width << "x" << scene.height << std::endl;
    
    std::vector<vec3> picData;
    
    image = cimg_library::CImg<unsigned char>(scene.width, scene.height, 1,3,0);
    
    std::vector<std::thread> threads;
    
    int numThreads = 4;
    int stepX = scene.width/numThreads;
    int stepY = scene.height/numThreads;
    
    
    for(int i=0; i < numThreads; i++)
    {
        int minX = i * stepX;
        int maxX = minX + stepX;
        
        for(int j=0; j < numThreads; j++)
        {
            int minY = j * stepY;
            int maxY = minY + stepY;
            
            threads.push_back(std::thread(calculateImageData, 2, true, true , minX, maxX, minY, maxY));
        }
    }
    
    
    vec3 Cam_ray_origin = scene.camera.pos;
    float xamt, yamt;
    
    
    
    
    cimg_library::CImgDisplay main_disp(image,"Render");
    while (!main_disp.is_closed())
    {
        //main_disp.wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        image.display(main_disp);
    }
    
    image.save("render.bmp");
    
    
    //std::terminate();
    return 0;
}

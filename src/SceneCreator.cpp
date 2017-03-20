#include "SceneCreator.h"
#define accuracy 0.0001

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
    
    int reflectionNum = 0;
    
    vec3 Cam_ray_origin = scene.camera.pos;
    float xamt, yamt;
    
    for(int x = 0; x < scene.width; x++)
    {
        for(int y=0; y < scene.height; y++)
        {
            
            vec3 pixelPoint = {(float)x - scene.width/2 ,(float)y - scene.height/2, -scene.camera.f };
            
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
                    
                    float d1;
                    Surface* lightSurface = ShootRay(LightRay, scene, d1);
                    
                    if(lightSurface == nullptr || lightSurface == surface)
                    {
                        vec3 normal;
                        switch(surface->type)
                        {
                            case PLANE:
                            {
                                normal = (static_cast<Plane *>(surface))->nor;
                                break;
                            }
                            case TRIANGLE:
                            {
                                Triangle *triangle = (static_cast<Triangle *>(surface));
                                normal = ((triangle->v3)-triangle->v1) ^ (triangle->v2-triangle->v1);
                                break;
                            }
                            case SPHERE:
                            {
                                Sphere *sphere= (static_cast<Sphere *>(surface));
                                
                                normal = LightRay.pos-sphere->pos;
                                
                                
                                break;
                            }
                        }
                        
                        
                        float nL = (Norm(normal) * Norm(LightRay.dir));
                        
                        if(nL < accuracy){
                            
                        }else
                        {
                            color.x = color.x + (light->col.x * (surface->dif.x * Max(0, nL)));
                            color.y = color.y + (light->col.y * (surface->dif.y * Max(0, nL)));
                            color.z = color.z + (light->col.z * (surface->dif.z * Max(0, nL)));
                            
                            reflectionNum++;
                        }
                    }
                    
                }
                /**
                color = {color.x*surface->amb.x,color.y*surface->amb.y,color.z*surface->amb.z};
                */
                color = color + (surface->amb*0.5f);
                const unsigned char colors[] = {color.x*255,color.y*255,color.z*255};
                image.draw_point(x,scene.height-y-1,colors);
            }
            else 
            {
                const unsigned char color[] = {0,0,0};
                image.draw_point(x,scene.height-y-1,color);
                //std::cout << x << " " << y << std::endl;
            }
            
        }
    }
    std::cout << reflectionNum << std::endl;
    image.save("render.bmp");
    
    cimg_library::CImgDisplay main_disp(image,"Render");
    while (!main_disp.is_closed())
        main_disp.wait();
    
    return 0;
}

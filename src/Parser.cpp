//parse vec3 from file in format x y z
vec3 ReadVec3(std::ifstream &file)
{
    vec3 vec;
    file >> vec.x;
    file >> vec.y;
    file >> vec.z;
    
    
    return vec;
};

//parse surface from file
void ReadSurface(Surface *surface, std::ifstream &file)
{
    std::string line;
    
    file >> line;
    surface->amb = ReadVec3(file);
    
    file >> line;
    surface->dif = ReadVec3(file);
    
    file >> line;
    surface->spe = ReadVec3(file);
    
    file >> line >> surface->shi;
}

//parse a scene from file
void ReadSceneFile(std::string fileName, Scene &scene)
{
    std::ifstream file;
    file.open(fileName);
    
    int objectNum;
    file >> objectNum;
    
    scene.objectNum = objectNum + 1;
    
    for(int i=0; i < objectNum + 1; i++)
    {
        std::string line;
        file >> line;
        std::cout << "Finding " << line << std::endl;
        if(line == "camera")
        {
            Camera *camera = &scene.camera;
            
            std::cout << "Camera object" << std::endl;
            
            file >> line;
            camera->pos = ReadVec3(file);
            
            file >> line >> camera->fov;
            
            file >> line  >> camera->f;
            
            file >> line  >> camera->a;
            
            std::cout << *camera << std::endl;
            
        }
        else if(line == "light")
        {
            Light *light = new Light();
            file >> line;
            light->pos = ReadVec3(file);
            file >> line;
            light->col = ReadVec3(file);
            
            scene.lights.push_back(light);
            
            std::cout << *light << std::endl;
        }
        else if(line == "plane")
        {
            std::cout << "Plane object" << std::endl;
            Plane *plane = new Plane();
            
            plane->type = PLANE;
            
            file >> line;
            plane->nor = ReadVec3(file);
            file >> line;
            plane->pos = ReadVec3(file);
            
            ReadSurface(plane, file);
            
            scene.surfaces.push_back(plane);
        }
        else if(line == "triangle")
        {
            std::cout << "Triangle object" << std::endl;
            Triangle *triangle = new Triangle();
            
            triangle->type = TRIANGLE;
            
            file >> line;
            triangle->v1 = ReadVec3(file);
            
            file >> line;
            triangle->v2 = ReadVec3(file);
            
            file >> line;
            triangle->v3 = ReadVec3(file);
            
            ReadSurface(triangle, file);
            
            scene.surfaces.push_back(triangle);
            
        }
        else if(line == "sphere")
        {
            std::cout << "Sphere  object" << std::endl;
            
            Sphere *sphere = new Sphere();
            
            sphere->type = SPHERE;
            
            file >> line;
            sphere->pos = ReadVec3(file);
            
            file >> line >> sphere->rad;
            
            ReadSurface(sphere, file);
            
            scene.surfaces.push_back(sphere);
            
        }
        
    }
    
    file.close();
}

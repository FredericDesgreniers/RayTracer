
//render scene
struct Scene
{
    int objectNum;
    Camera camera;
    std::vector<Surface *> surfaces;
    std::vector<Light *> lights;
    
    int width;
    int height;
};
          
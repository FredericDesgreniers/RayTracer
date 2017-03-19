#include "SceneCreator.h"

int main()
{
    Scene scene = {};
    
    ReadSceneFile("scene.txt", scene);
    
    std::cout << "Scene has " << scene.objectNum  << " objects" <<std::endl;
    
    scene.height = round(tan(radian(scene.camera.fov)/2) * 2.0f * scene.camera.f);
    scene.width = scene.height * scene.camera.a;
    
    std::cout << scene.width << "x" << scene.height << std::endl;
    
    std::cin.get();
    return 0;
}

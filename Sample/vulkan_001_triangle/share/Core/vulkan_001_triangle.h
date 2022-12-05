// author: LostPeter
// time:   2022-11-05

#ifndef _VULKAN_001_TRIANGLE_H_
#define _VULKAN_001_TRIANGLE_H_

#include "vulkanwindow.h"
using namespace LostPeter;

class Vulkan_001_Triangle : public VulkanWindow
{
public:
    Vulkan_001_Triangle(int width, int height, std::string name);

public:
    std::vector<Vertex_Pos2Color4> vertices;

public:


protected:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Custom();
            
public:
    
};


#endif
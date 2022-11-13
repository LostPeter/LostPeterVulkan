// author: LostPeter
// time:   2022-11-08

#ifndef _VULKAN_002_IMGUI_H_
#define _VULKAN_002_IMGUI_H_

#include "vulkanwindow.h"
using namespace LibUtil; 

class Vulkan_002_Imgui : public VulkanWindow
{
public:
    Vulkan_002_Imgui(int width, int height, std::string name);

public:
    std::vector<Vertex_Pos2Color3> vertices;

public:


protected:
    //Create Pipeline

    //Load Assets

        //Geometry
            virtual void createVertexIndexDataByUser();

    //Render/Update
        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

public:
    
};


#endif
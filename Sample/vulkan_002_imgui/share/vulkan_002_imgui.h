/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-11-08
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_002_IMGUI_H_
#define _VULKAN_002_IMGUI_H_

#include "VulkanWindow.h"
using namespace LostPeter; 

class Vulkan_002_Imgui : public VulkanWindow
{
public:
    Vulkan_002_Imgui(int width, int height, std::string name);

public:
    std::vector<Vertex_Pos2Color4> vertices;

public:


protected:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Custom();

    //Render/Update
        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

public:
    
};


#endif
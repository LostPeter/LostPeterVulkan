/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_001_TRIANGLE_H_
#define _VULKAN_001_TRIANGLE_H_

#include "VulkanWindow.h"
using namespace LostPeterVulkan;

class Vulkan_001_Triangle : public VulkanWindow
{
public:
    Vulkan_001_Triangle(int width, int height, String name);

public:
    std::vector<FVertex_Pos2Color4> vertices;

public:


protected:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Custom();
            
public:
    
};


#endif
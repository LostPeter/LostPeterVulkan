/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_003_TEXTURE_H_
#define _VULKAN_003_TEXTURE_H_

#include "VulkanWindow.h"
using namespace LostPeterVulkan; 

class Vulkan_003_Texture : public VulkanWindow
{
public:
    Vulkan_003_Texture(int width, int height, String name);

public:

protected:
    std::vector<FVertex_Pos2Color4Tex2> vertices;

public:
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
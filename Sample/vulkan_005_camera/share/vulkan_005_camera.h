/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_005_CAMERA_H_
#define _VULKAN_005_CAMERA_H_

#include "VulkanWindow.h"
using namespace LostPeterVulkan; 

class Vulkan_005_Camera : public VulkanWindow
{
public:
    Vulkan_005_Camera(int width, int height, String name);

public:
    std::vector<FVertex_Pos3Color4Tex2> vertices;
    std::vector<uint32_t> indices;

protected:
    //Create Pipeline
    
    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry
        virtual void loadModel_Default();

    //Render/Update
        virtual bool beginRenderImgui();
            virtual void modelConfig();
            
        virtual void endRenderImgui();

private:
    void resetSetting(int index);
    void changeModel(int index);
};


#endif
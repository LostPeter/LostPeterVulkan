/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_004_MODEL_H_
#define _VULKAN_004_MODEL_H_

#include "VulkanWindow.h"
using namespace LostPeter; 

class Vulkan_004_Model : public VulkanWindow
{
public:
    Vulkan_004_Model(int width, int height, String name);

public:
    std::vector<FVertex_Pos3Color4Tex2> vertices;
    std::vector<uint32_t> indices;

protected:
    //Create Pipeline
    
    //Load Assets
        //Geometry
        virtual void loadModel_Assimp();

    //Render/Update
        virtual bool beginRenderImgui();
            virtual void modelConfig();
            
        virtual void endRenderImgui();

private:
    void resetSetting(int index);
    void changeModel(int index);


};


#endif
/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipeline.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    VKPipeline::VKPipeline(const String& namePipeline)
        : Base(namePipeline)
        , vkPipeline(VK_NULL_HANDLE)
    {

    }
    VKPipeline::~VKPipeline()
    {
        Destroy();
    }
    
    void VKPipeline::Destroy()
    {
        if (this->vkPipeline != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->vkPipeline);
        }
        this->vkPipeline = VK_NULL_HANDLE;
    }

    bool VKPipeline::LoadPipeline()
    {   
        if (this->name.empty())
        {
            F_LogError("*********************** VKPipeline::LoadPipeline: Pipeline name is empty !");
            return false;
        }

        Destroy();

        

        return true;
    }

}; //LostPeter
/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineLayout.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    VKPipelineLayout::VKPipelineLayout(uint32 _group,
                                       const String& _namePipelineLayout)
        : Base(_group, _namePipelineLayout)
    {

    }
    VKPipelineLayout::~VKPipelineLayout()
    {

    }
    
    void VKPipelineLayout::Destroy()
    {
        
    }

    bool VKPipelineLayout::LoadPipelineLayout()
    {

        return true;
    }

}; //LostPeter
/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanPipelineGraphics.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
        : RHIPipelineGraphics(createInfo)
    {

    }

    RHIVulkanPipelineGraphics::~RHIVulkanPipelineGraphics()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineGraphics::Destroy()
    {

    }

}; //LostPeterPluginRHIVulkan
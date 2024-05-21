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
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics(RHIVulkanDevice* pVulkanDevice, const RHIPipelineGraphicsCreateInfo& createInfo)
        : RHIPipelineGraphics(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics")

    }

    RHIVulkanPipelineGraphics::~RHIVulkanPipelineGraphics()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineGraphics::Destroy()
    {

    }

}; //LostPeterPluginRHIVulkan
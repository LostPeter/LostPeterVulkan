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

#include "../include/RHIVulkanPipelineCompute.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineCompute::RHIVulkanPipelineCompute(RHIVulkanDevice* pVulkanDevice, const RHIPipelineComputeCreateInfo& createInfo)
        : RHIPipelineCompute(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineCompute::RHIVulkanPipelineCompute")

    }

    RHIVulkanPipelineCompute::~RHIVulkanPipelineCompute()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineCompute::Destroy()
    {

    }

}; //LostPeterPluginRHIVulkan
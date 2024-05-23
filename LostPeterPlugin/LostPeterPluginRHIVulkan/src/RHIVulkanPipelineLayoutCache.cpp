/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanPipelineLayoutCache.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineLayoutCache::RHIVulkanPipelineLayoutCache(RHIVulkanDevice* pVulkanDevice, const RHIPipelineLayoutCacheCreateInfo& createInfo)
        : RHIPipelineLayoutCache(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)

        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineLayoutCache::RHIVulkanPipelineLayoutCache")

    }

    RHIVulkanPipelineLayoutCache::~RHIVulkanPipelineLayoutCache()
    {
        Destroy();
    }

    void RHIVulkanPipelineLayoutCache::Destroy()
    {
        
    }
    
    

}; //LostPeterPluginRHIVulkan
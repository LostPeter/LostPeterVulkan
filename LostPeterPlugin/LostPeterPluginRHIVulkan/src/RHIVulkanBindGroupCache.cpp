/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanBindGroupCache.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroupCache::RHIVulkanBindGroupCache(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupCacheCreateInfo& createInfo)
        : RHIBindGroupCache(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBindGroupCache::RHIVulkanBindGroupCache")

    }

    RHIVulkanBindGroupCache::~RHIVulkanBindGroupCache()
    {
        Destroy();
    }

    void RHIVulkanBindGroupCache::Destroy()
    {

    }
    
 

}; //LostPeterPluginRHIVulkan
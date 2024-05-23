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

#include "../include/RHIVulkanBindGroupLayoutCache.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroupLayoutCache::RHIVulkanBindGroupLayoutCache(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupLayoutCacheCreateInfo& createInfo)
        : RHIBindGroupLayoutCache(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBindGroupLayoutCache::RHIVulkanBindGroupLayoutCache")

    }   

    RHIVulkanBindGroupLayoutCache::~RHIVulkanBindGroupLayoutCache()
    {
        Destroy();
    }

    void RHIVulkanBindGroupLayoutCache::Destroy()
    {
        
    }
    

}; //LostPeterPluginRHIVulkan
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

#include "../include/RHIVulkanPipelineCache.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
     RHIVulkanPipelineCache::RHIVulkanPipelineCache(RHIVulkanDevice* pVulkanDevice, const RHIPipelineCacheCreateInfo& createInfo)
        : RHIPipelineCache(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkPipelineCache(VK_NULL_HANDLE)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineCache::RHIVulkanPipelineCache")

        createVkPipelineCache();
    }

    RHIVulkanPipelineCache::~RHIVulkanPipelineCache()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineCache::Destroy()
    {
        if (m_vkPipelineCache != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkPipelineCache(m_vkPipelineCache);
        }
        m_vkPipelineCache = VK_NULL_HANDLE;
    }
    
    void RHIVulkanPipelineCache::createVkPipelineCache()
    {   
        if (!m_pVulkanDevice->CreateVkPipelineCache(m_vkPipelineCache))
        {
            F_LogError("*********************** RHIVulkanPipelineCache::createVkPipelineCache: CreateVkPipelineCache failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE_CACHE, reinterpret_cast<uint64_t>(m_vkPipelineCache), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanPipelineCache::createVkPipelineCache: Create VkPipelineCache success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan
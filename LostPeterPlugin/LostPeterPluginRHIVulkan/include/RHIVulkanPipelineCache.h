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

#ifndef _RHI_VULKAN_PIPELINE_CACHE_H_
#define _RHI_VULKAN_PIPELINE_CACHE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineCache : public RHIPipelineCache
    {
    public:
        RHIVulkanPipelineCache(RHIVulkanDevice* pVulkanDevice, const RHIPipelineCacheCreateInfo& createInfo);
        virtual ~RHIVulkanPipelineCache();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkPipelineCache m_vkPipelineCache;

        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkPipelineCache& GetVkPipelineCache() { return m_vkPipelineCache; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkPipelineCache();
    };
    

}; //LostPeterPluginRHIVulkan

#endif
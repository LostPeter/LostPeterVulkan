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
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineCache : public RHIPipelineCache
                                                 , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanPipelineCache(RHIVulkanDevice* pVulkanDevice, const RHIPipelineCacheCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanPipelineCache();

    public:
    protected:
        VkPipelineCache m_vkPipelineCache;

        String m_strDebugName;

    public:
        F_FORCEINLINE VkPipelineCache& GetVkPipelineCache() { return m_vkPipelineCache; }

        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkPipelineCache();
    };
    

}; //LostPeterPluginRHIVulkan

#endif
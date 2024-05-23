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

#ifndef _RHI_VULKAN_PIPELINE_LAYOUT_CACHE_H_
#define _RHI_VULKAN_PIPELINE_LAYOUT_CACHE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineLayoutCache : public RHIPipelineLayoutCache
                                                       , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanPipelineLayoutCache(RHIVulkanDevice* pVulkanDevice, const RHIPipelineLayoutCacheCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanPipelineLayoutCache();

    public:
    protected:


        String m_strDebugName;

    public:

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:

    };
    
}; //LostPeterPluginRHIVulkan

#endif
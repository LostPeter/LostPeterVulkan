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

#ifndef _RHI_VULKAN_BIND_GROUP_CACHE_H_
#define _RHI_VULKAN_BIND_GROUP_CACHE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBindGroupCache : public RHIBindGroupCache
                                                  , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanBindGroupCache(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupCacheCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanBindGroupCache();

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
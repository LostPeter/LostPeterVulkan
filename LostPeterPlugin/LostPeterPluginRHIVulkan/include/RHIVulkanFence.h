/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_FENCE_H_
#define _RHI_VULKAN_FENCE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanFence : public RHIFence
                                         , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanFence(RHIVulkanDevice* pVulkanDevice, const RHIFenceCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanFence();

    public:
    protected:
        VkFence m_vkFence;

        bool m_bIsSignaled;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkFence& GetVkFence() { return m_vkFence; }

        F_FORCEINLINE bool IsSignaled() { return m_bIsSignaled; }
        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

        virtual RHIFenceStatusType GetStatus();
        virtual void Reset();
        virtual void Wait();

    protected:
        void createVkFence();
    };
    
}; //LostPeterPluginRHIVulkan

#endif
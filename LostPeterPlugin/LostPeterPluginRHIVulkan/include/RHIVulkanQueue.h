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

#ifndef _RHI_VULKAN_QUEUE_H_
#define _RHI_VULKAN_QUEUE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanQueue : public RHIQueue
                                         , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanQueue(RHIVulkanDevice* pVulkanDevice, const RHIQueueCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanQueue();

    public:
    protected:
        VkQueue m_vkQueue;
        uint32 m_nFamilyIndex;

        String m_strDebugName;

    public:
        F_FORCEINLINE VkQueue& GetVkQueue() { return m_vkQueue; }
        F_FORCEINLINE uint32 GetFamilyIndex() const { return m_nFamilyIndex; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

        virtual void Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal);
        virtual void Wait(RHIFence* pFenceToSignal);

    protected:
        void createVkQueue();
    };

}; //LostPeterPluginRHIVulkan

#endif
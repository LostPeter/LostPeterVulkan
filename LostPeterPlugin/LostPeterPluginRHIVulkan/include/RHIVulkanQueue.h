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

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanQueue : public RHIQueue
    {
    public:
        RHIVulkanQueue(RHIVulkanDevice* pDevice, uint32 nFamilyIndex);
        virtual ~RHIVulkanQueue();

    public:
    protected:
        RHIVulkanDevice* m_pDevice;
        uint32 m_nFamilyIndex;
        VkQueue m_vkQueue;

    public:
        F_FORCEINLINE VkQueue& GetVkQueue() { return m_vkQueue; }
        F_FORCEINLINE uint32 GetFamilyIndex() const { return m_nFamilyIndex; }

    public:
        virtual void Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal);
        virtual void Wait(RHIFence* pFenceToSignal);

    public:
        void Destroy();
        bool Init();
    };

}; //LostPeterPluginRHIVulkan

#endif
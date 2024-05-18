/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_SEMAPHORE_H_
#define _RHI_VULKAN_SEMAPHORE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanSemaphore
    {
    public:
        RHIVulkanSemaphore(RHIVulkanDevice* pVulkanDevice);
        virtual ~RHIVulkanSemaphore();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkSemaphore m_vkSemaphore;
        
    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkSemaphore& GetVkSemaphore() { return m_vkSemaphore; }

    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIVulkan

#endif
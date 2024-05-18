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

#ifndef _RHI_VULKAN_COMMAND_POOL_H_
#define _RHI_VULKAN_COMMAND_POOL_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanCommandPool : public RHICommandPool
    {
    public:
        RHIVulkanCommandPool(RHIVulkanDevice* pVulkanDevice);
        virtual ~RHIVulkanCommandPool();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        uint32 m_nFamilyIndex;
        VkCommandPool m_vkCommandPool;
        
    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkCommandPool& GetVkCommandPool() { return m_vkCommandPool; }

    public:
        virtual void Destroy();
        bool Init(uint32 nFamilyIndex);

    };

}; //LostPeterPluginRHIVulkan

#endif
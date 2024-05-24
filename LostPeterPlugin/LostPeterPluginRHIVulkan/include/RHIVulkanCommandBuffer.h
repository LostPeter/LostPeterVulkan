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

#ifndef _RHI_VULKAN_COMMAND_BUFFER_H_
#define _RHI_VULKAN_COMMAND_BUFFER_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanCommandBuffer : public RHICommandBuffer
                                                 , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanCommandBuffer(RHIVulkanDevice* pVulkanDevice, RHIVulkanCommandPool* pVulkanCommandPool, const RHICommandBufferCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanCommandBuffer();

    public:
    protected:
        RHIVulkanCommandPool* m_pVulkanCommandPool;
        VkCommandBuffer m_vkCommandBuffer;



    public:
        F_FORCEINLINE RHIVulkanCommandPool* GetVulkanCommandPool() const { return m_pVulkanCommandPool; }
        F_FORCEINLINE VkCommandBuffer& GetVkCommandBuffer() { return m_vkCommandBuffer; }


    public:
        virtual void Destroy();

        virtual RHICommandEncoder* Begin();

    public:


    protected:
        void createVkCommandBuffer();
    };

}; //LostPeterPluginRHIVulkan

#endif
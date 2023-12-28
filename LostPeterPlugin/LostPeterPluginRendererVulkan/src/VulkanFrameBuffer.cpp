/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanFrameBuffer.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanFrameBuffer::VulkanFrameBuffer(const String& nameFrameBuffer, VulkanDevice* pDevice)
        : Base(nameFrameBuffer)
        , m_pDevice(pDevice)
        , m_vkFrameBuffer(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanFrameBuffer::VulkanFrameBuffer")
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        Destroy();
    }

    void VulkanFrameBuffer::Destroy()
    {
        if (this->m_vkFrameBuffer != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkFramebuffer(this->m_vkFrameBuffer);
        }
        this->m_vkFrameBuffer = VK_NULL_HANDLE;
    }

    bool VulkanFrameBuffer::Init(const VkImageViewVector& aImageView, 
                                 VkRenderPass& vkRenderPass,
                                 VkFramebufferCreateFlags flags,
                                 uint32_t width,
                                 uint32_t height,
                                 uint32_t layers)
    {
        Destroy();
        const String& nameFrameBuffer = GetName();
        if (!m_pDevice->CreateVkFramebuffer(nameFrameBuffer,
                                            aImageView,
                                            vkRenderPass,
                                            flags,
                                            width,
                                            height,
                                            layers,
                                            this->m_vkFrameBuffer))
        {
            F_LogError("*********************** VulkanFrameBuffer::Init: Failed to CreateVkFramebuffer, nameFrameBuffer: [%s] !", nameFrameBuffer.c_str());
            return false;
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan
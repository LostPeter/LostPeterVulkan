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

#ifndef _VULKAN_FRAME_BUFFER_H_
#define _VULKAN_FRAME_BUFFER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanFrameBuffer : public Base
    {
    public:
        VulkanFrameBuffer(const String& nameFrameBuffer, VulkanDevice* pDevice);
        ~VulkanFrameBuffer();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkFramebuffer m_vkFrameBuffer;

    public:
        F_FORCEINLINE VkFramebuffer GetVkFramebuffer() const { return m_vkFrameBuffer; }
        

    public:
        void Destroy();
        bool Init(const VkImageViewVector& aImageView, 
                  VkRenderPass& vkRenderPass,
                  VkFramebufferCreateFlags flags,
                  uint32_t width,
                  uint32_t height,
                  uint32_t layers);

    public:

    };

}; //LostPeterPluginRendererVulkan

#endif
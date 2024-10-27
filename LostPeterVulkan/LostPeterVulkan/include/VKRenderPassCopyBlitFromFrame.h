/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_RENDER_PASS_COPY_BLIT_FROM_FRAME_H_
#define _VK_RENDER_PASS_COPY_BLIT_FROM_FRAME_H_

#include "Base.h"
#include "VKFrameBufferAttachment.h"

namespace LostPeterVulkan
{
    class vulkanExport VKRenderPassCopyBlitFromFrame : public Base
    {
    public:
        VKRenderPassCopyBlitFromFrame(const String& nameRenderPass);
        virtual ~VKRenderPassCopyBlitFromFrame();

    public:
        //Common
        VkOffset2D offset;
        VkExtent2D extent;
        VkClearValueVector aClearValue;
        VkViewport viewPort;
        VkRect2D rtScissor;

        //Image
        VkImage poImage;
        VkDeviceMemory poImageMemory;
        VkImageView poImageView;
        VkSampler poSampler;
        VkDescriptorImageInfo imageInfo;

        //RenderPass
        VkRenderPass poRenderPass;

        //FrameBuffer
        VkFramebuffer poFrameBuffer;

    public:
        void Destroy();

        virtual bool Init(uint32_t width, 
                          uint32_t height,
                          VkFormat format,
                          bool isDepth);

    public:
        void CleanupSwapChain();
        void RecreateSwapChain();
    };

}; //LostPeterVulkan

#endif
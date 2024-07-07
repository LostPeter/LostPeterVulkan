/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-06-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_RENDER_PASS_SHADOW_MAP_H_
#define _VK_RENDER_PASS_SHADOW_MAP_H_

#include "Base.h"
#include "VKFrameBufferAttachment.h"

namespace LostPeterVulkan
{
    class vulkanExport VKRenderPassShadowMap : public Base
    {
    public:
        VKRenderPassShadowMap(const String& _nameRenderPass);
        virtual ~VKRenderPassShadowMap();

    public:
        //Common
        VkOffset2D offset;
        VkExtent2D extent;
        VkClearValueVector aClearValue;
        VkViewport viewPort;
        VkRect2D rtScissor;

        //Depth
        VkImage poDepthImage;
        VkDeviceMemory poDepthImageMemory;
        VkImageView poDepthImageView;
        VkSampler sampler;
        VkDescriptorImageInfo imageInfo;
        
        //RenderPass
        VkRenderPass poRenderPass;

        //FrameBuffer
        VkFramebuffer poFrameBuffer;

    public:
        void Destroy();

        virtual void Init(uint32_t width, 
                          uint32_t height,
                          VkFormat format);
    
    public:
        void CleanupSwapChain();
        void RecreateSwapChain();
    };

}; //LostPeterVulkan

#endif
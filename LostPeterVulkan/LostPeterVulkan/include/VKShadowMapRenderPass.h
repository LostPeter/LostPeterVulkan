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

#ifndef _VK_SHADOW_MAP_RENDER_PASS_H_
#define _VK_SHADOW_MAP_RENDER_PASS_H_

#include "Base.h"
#include "VKFrameBufferAttachment.h"

namespace LostPeterVulkan
{
    class vulkanExport VKShadowMapRenderPass : public Base
    {
    public:
        VKShadowMapRenderPass(const String& _nameRenderPass);
        virtual ~VKShadowMapRenderPass();

    public:
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
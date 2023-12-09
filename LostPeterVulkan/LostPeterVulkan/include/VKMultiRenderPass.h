/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_MULTI_RENDER_PASS_H_
#define _VK_MULTI_RENDER_PASS_H_

#include "Base.h"
#include "VKFrameBufferAttachment.h"

namespace LostPeterVulkan
{
    class vulkanExport VKMultiRenderPass : public Base
    {
    public:
        VKMultiRenderPass(const String& _nameRenderPass,
                          bool _isUseDefault,
                          bool _isMultiView2);
        virtual ~VKMultiRenderPass();

    public:
        //Common
        bool isUseDefault;
        bool isMultiView2;

        //Attachment
        VKFrameBufferAttachment framebufferColor;
        VKFrameBufferAttachment framebufferDepth;
        VkSampler sampler;
        VkDescriptorImageInfo imageInfo;
        
        //RenderPass
        VkRenderPass poRenderPass;

        //FrameBuffer
        VkFramebuffer poFrameBuffer;

    public:
        void Destroy();

        virtual void Init(uint32_t width, uint32_t height);
    
    public:
        void CleanupSwapChain();
        void RecreateSwapChain();
    };

}; //LostPeterVulkan

#endif
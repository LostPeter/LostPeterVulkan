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

#ifndef _MULTI_RENDER_PASS_H_
#define _MULTI_RENDER_PASS_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport FrameBufferAttachment
    {
    public:
        FrameBufferAttachment();
        virtual ~FrameBufferAttachment();

    public:
        bool isDepth;
        bool isImageArray;

        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;

    public:
        void Destroy();
        virtual void Init(uint32_t width, 
                          uint32_t height, 
                          bool _isDepth,
                          bool _isImageArray);
    };

    class utilExport MultiRenderPass : public Base
    {
    public:
        MultiRenderPass(const String& _nameRenderPass,
                        bool _isUseDefault,
                        bool _isMultiView2);
        virtual ~MultiRenderPass();

    public:
        //Common
        bool isUseDefault;
        bool isMultiView2;

        //Attachment
        FrameBufferAttachment framebufferColor;
        FrameBufferAttachment framebufferDepth;
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

}; //LostPeter

#endif
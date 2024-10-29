/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_RENDER_PASS_CULL_H_
#define _VK_RENDER_PASS_CULL_H_

#include "Base.h"
#include "VKFrameBufferAttachment.h"

namespace LostPeterVulkan
{
    class vulkanExport VKRenderPassCull : public Base
    {
    public:
        VKRenderPassCull(const String& nameRenderPass);
        virtual ~VKRenderPassCull();

    public:
        static int s_nHizDepthWidth;

    public:
        //HizDepth
        VkFormat poFormat;
        VkImage poHizDepthImage;
        VkDeviceMemory poHizDepthImageMemory;
        VkImageView poHizDepthImageView_Main;
        VkImageView poHizDepthImageView_0;
        VkImageViewVector aHizDepthImageView_Mipmap;
        VkSampler poHizDepthSampler;
        VkDescriptorImageInfo poHizDepthImageInfo_Sampler;

        int nHizDepthWidth;
        int nHizDepthHeight;
        int nHizDepthMinmapCount; 

        //HizDepthConstants
        HizDepthConstants hizDepthCB;
        VkBuffer poBuffer_HizDepthCB;  
        VkDeviceMemory poBufferMemory_HizDepthCB;

        //RenderPass/FrameBuffer
        VkOffset2D offset;
        VkExtent2D extent;
        VkClearValueVector aClearValue;
        VkViewport viewPort;
        VkRect2D rtScissor;

        VkRenderPass poRenderPass;
        VkFramebuffer poFrameBuffer;

    public:
        void Destroy();
        virtual bool Init();

        virtual void GetDescriptorImageInfo(int mipmap, VkDescriptorImageInfo& imageInfo);

        virtual void UpdateHizDepthBuffer_Render();
        virtual void UpdateHizDepthBuffer_Compute(float w, float h);

    protected:
        virtual void destroyBufferHizDepth();

        virtual bool createBufferHizDepth();
        virtual bool createCullTexture();
        virtual bool createRenderPassFrameBuffer();

    public: 
        void CleanupSwapChain();
        void RecreateSwapChain();

    protected:
        void updateHizDepthRTSize();
        void updateHizDepthBuffer();
        void getHizDepthRTSizeFromScreen(int screenWidth, int& w, int& mip);
    };

}; //LostPeterVulkan

#endif
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
        VKRenderPassCull(const String& _nameRenderPass);
        virtual ~VKRenderPassCull();

    public:
        static int s_nHizDepthWidth;
        static int s_nHizDepthHeight;

    public:
        ////////////////////////// Texture //////////////////////////////////
        //HizDepth
        VkImage poHizDepthImage;
        VkDeviceMemory poHizDepthImageMemory;
        VkImageView poHizDepthImageView;
        VkSampler poHizDepthSampler;
        VkDescriptorImageInfo poHizDepthImageInfo_NoSampler;
        VkDescriptorImageInfo poHizDepthImageInfo_Sampler;


    public:
        void Destroy();
        virtual bool Init();

    protected:
        virtual void setupCullTexture();


    public: 
        void CleanupSwapChain();
        void RecreateSwapChain();

    protected:
        
    };

}; //LostPeterVulkan

#endif
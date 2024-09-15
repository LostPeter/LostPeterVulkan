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

#include "../include/VKRenderPassCull.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    int VKRenderPassCull::s_nHizDepthWidth = 1024;
    int VKRenderPassCull::s_nHizDepthHeight = 512;

    VKRenderPassCull::VKRenderPassCull(const String& _nameRenderPass)
        //Window
        : Base(_nameRenderPass)

        //HizDepth
        , poHizDepthImage(VK_NULL_HANDLE)
        , poHizDepthImageMemory(VK_NULL_HANDLE)
        , poHizDepthImageView(VK_NULL_HANDLE)
        , poHizDepthSampler(VK_NULL_HANDLE)

    {

    }
    VKRenderPassCull::~VKRenderPassCull()
    {
        Destroy();
    }   

    void VKRenderPassCull::Destroy()
    {
        //HizDepth
        if (this->poHizDepthImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poHizDepthImage, this->poHizDepthImageMemory, this->poHizDepthImageView);
        }
        this->poHizDepthImage = VK_NULL_HANDLE;
        this->poHizDepthImageMemory = VK_NULL_HANDLE;
        this->poHizDepthImageView = VK_NULL_HANDLE;
        if (this->poHizDepthSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poHizDepthSampler);
        }
        this->poHizDepthSampler = VK_NULL_HANDLE;
    } 

    bool VKRenderPassCull::Init()
    {
        setupCullTexture();
        
        return true;
    }
        void VKRenderPassCull::setupCullTexture()
        {
            //HizDepth
            {
                Base::GetWindowPtr()->createTextureRenderTarget2D(nullptr,
                                                                  s_nHizDepthWidth,
                                                                  s_nHizDepthHeight,
                                                                  true,
                                                                  9,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_FORMAT_D32_SFLOAT,
                                                                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                                                                  VK_IMAGE_LAYOUT_GENERAL,
                                                                  true,
                                                                  this->poHizDepthImage,
                                                                  this->poHizDepthImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poHizDepthImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D, 
                                                        VK_FORMAT_D32_SFLOAT, 
                                                        VK_IMAGE_ASPECT_DEPTH_BIT, 
                                                        1, 
                                                        1, 
                                                        this->poHizDepthImageView);
                Base::GetWindowPtr()->createVkSampler(F_TextureFilter_Bilinear, 
                                                      F_TextureAddressing_Clamp,
                                                      F_TextureBorderColor_OpaqueBlack,
                                                      true,
                                                      Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                      0.0f,
                                                      1.0f,
                                                      0.0f,
                                                      this->poHizDepthSampler);


                this->poHizDepthImageInfo_NoSampler = {};
                this->poHizDepthImageInfo_NoSampler.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                this->poHizDepthImageInfo_NoSampler.imageView = this->poHizDepthImageView;
                this->poHizDepthImageInfo_NoSampler.sampler = nullptr;
                this->poHizDepthImageInfo_Sampler = {};
                this->poHizDepthImageInfo_Sampler.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                this->poHizDepthImageInfo_Sampler.imageView = this->poHizDepthImageView;
                this->poHizDepthImageInfo_Sampler.sampler = this->poHizDepthSampler;

                F_LogInfo("VKRenderPassCull::setupCullTexture: Compute: Create render texture [HizDepth] - [%d, %d] success !", s_nHizDepthWidth, s_nHizDepthHeight);
            }
        }

    void VKRenderPassCull::CleanupSwapChain()
    {   
        Destroy();
    }

    void VKRenderPassCull::RecreateSwapChain()
    {

    }

}; //LostPeterVulkan
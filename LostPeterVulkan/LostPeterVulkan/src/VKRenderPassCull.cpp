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
        if (!setupCullTexture())
        {
            F_LogError("VKRenderPassCull::Init: Failed to setup cull texture !");
            return false;
        }
        
        return true;
    }
        bool VKRenderPassCull::setupCullTexture()
        {
            F_LogInfo("VKRenderPassCull::setupCullTexture: Start setup cull texture !");

            //HizDepth
            {
                updateHizDepthRTSize();

                String nameTexture = "Texture-HizDepth";
                Base::GetWindowPtr()->createTextureRenderTarget2D(nameTexture,
                                                                  nullptr,
                                                                  (uint32_t)this->nHizDepthWidth,
                                                                  (uint32_t)this->nHizDepthHeight,
                                                                  true,
                                                                  (uint32_t)this->nHizDepthMinmapCount,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_FORMAT_R32_SFLOAT,
                                                                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                                                                  VK_IMAGE_LAYOUT_GENERAL,
                                                                  true,
                                                                  this->poHizDepthImage,
                                                                  this->poHizDepthImageMemory);
                F_LogInfo("VKRenderPassCull::setupCullTexture: createTextureRenderTarget2D !");
                Base::GetWindowPtr()->createVkImageView(nameTexture,
                                                        this->poHizDepthImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D, 
                                                        VK_FORMAT_R32_SFLOAT, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                                        1, 
                                                        1, 
                                                        this->poHizDepthImageView);
                F_LogInfo("VKRenderPassCull::setupCullTexture: createVkImageView !");
                Base::GetWindowPtr()->createVkSampler(nameTexture,
                                                      F_TextureFilter_None, 
                                                      F_TextureAddressing_Clamp,
                                                      F_TextureBorderColor_OpaqueBlack,
                                                      true,
                                                      Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                      0.0f,
                                                      1.0f,
                                                      0.0f,
                                                      this->poHizDepthSampler);
                F_LogInfo("VKRenderPassCull::setupCullTexture: createVkSampler !");

                this->poHizDepthImageInfo_NoSampler = {};
                this->poHizDepthImageInfo_NoSampler.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                this->poHizDepthImageInfo_NoSampler.imageView = this->poHizDepthImageView;
                this->poHizDepthImageInfo_NoSampler.sampler = nullptr;
                this->poHizDepthImageInfo_Sampler = {};
                this->poHizDepthImageInfo_Sampler.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                this->poHizDepthImageInfo_Sampler.imageView = this->poHizDepthImageView;
                this->poHizDepthImageInfo_Sampler.sampler = this->poHizDepthSampler;

                F_LogInfo("VKRenderPassCull::setupCullTexture: Compute: Create render texture: [%s] - (w,h): [%d, %d], mip: [%d] success !", 
                          nameTexture.c_str(), this->nHizDepthWidth, this->nHizDepthHeight, this->nHizDepthMinmapCount);
            }

            F_LogInfo("VKRenderPassCull::setupCullTexture: End setup cull texture !");
            return true;
        }

    void VKRenderPassCull::CleanupSwapChain()
    {   
        Destroy();
    }

    void VKRenderPassCull::RecreateSwapChain()
    {

    }

    void VKRenderPassCull::updateHizDepthRTSize()
    {
        int nScreenWith = (int)Base::GetWindowPtr()->poSwapChainExtent.width;

        this->nHizDepthWidth = s_nHizDepthWidth;
        this->nHizDepthMinmapCount = 9;
        getHizDepthRTSizeFromScreen(nScreenWith, this->nHizDepthWidth, this->nHizDepthMinmapCount);
        this->nHizDepthHeight = this->nHizDepthWidth / 2;
    }
    void VKRenderPassCull::getHizDepthRTSizeFromScreen(int screenWidth, int& w, int& mip)
    {
        if (screenWidth >= 2048)
        {
            w = 2048;
            mip = 10;
            return;
        }
        else if (screenWidth >= 1024)
        {
            w = 1024;
            mip = 9;
            return;
        }

        w = 512;
        mip = 8;
    }

}; //LostPeterVulkan
/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanTexture.h"
#include "../include/VulkanTextureManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanTexture::VulkanTexture(VulkanDevice* pDevice,
                                 ResourceManager* pResourceManager,
                                 uint32 nGroup, 
                                 const String& strName,
                                 const String& strGroupName,
                                 ResourceHandle nHandle,
                                 bool bIsManualLoad /*= false*/,
                                 ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Texture(pResourceManager,
                  nGroup, 
                  strName,
                  strGroupName,
                  nHandle,
                  bIsManualLoad,
                  pResourceManualLoader)
        , m_pDevice(pDevice)
        , m_eVkImage(VK_IMAGE_TYPE_2D)
        , m_eVkImageView(VK_IMAGE_VIEW_TYPE_2D)
        , m_eVkFormat(VK_FORMAT_UNDEFINED)
        , m_eVkImageAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
        , m_eVkSamplesCountFlagBits(VK_SAMPLE_COUNT_1_BIT)
        , m_vkRTImageUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT)
        , m_vkImage(VK_NULL_HANDLE)
        , m_vkImageMemory(VK_NULL_HANDLE)
        , m_vkImageView(VK_NULL_HANDLE)
        , m_vkSampler(VK_NULL_HANDLE)
        , m_vkImageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
        , m_vkBufferStaging(VK_NULL_HANDLE)
        , m_vkBufferMemoryStaging(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanTexture::VulkanTexture")

        m_eVkComponentMapping.r = VK_COMPONENT_SWIZZLE_R;
		m_eVkComponentMapping.g = VK_COMPONENT_SWIZZLE_G;
		m_eVkComponentMapping.b = VK_COMPONENT_SWIZZLE_B;
		m_eVkComponentMapping.a = VK_COMPONENT_SWIZZLE_A;
    }

    VulkanTexture::~VulkanTexture()
    {
        Destroy();
    }

    void VulkanTexture::Destroy()
    {
        Unload();
        Texture::Destroy();
    }

    StreamTexture* VulkanTexture::GetTextureStream(uint32 nFace /*= 0*/, uint32 nMipmap /*= 0*/)
    {
        
        return nullptr;
    }

    void VulkanTexture::addParameterInherit()
    {

    }

    void VulkanTexture::destroyInternalResourcesImpl()
    {
        if (this->m_vkBufferStaging != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkBuffer(this->m_vkBufferStaging, this->m_vkBufferMemoryStaging);
        }
        this->m_vkBufferStaging = VK_NULL_HANDLE;
        this->m_vkBufferMemoryStaging = VK_NULL_HANDLE;

        if (this->m_vkImage != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkImage(this->m_vkImage, this->m_vkImageMemory, this->m_vkImageView);
        }
        this->m_vkImage = VK_NULL_HANDLE;
        this->m_vkImageMemory = VK_NULL_HANDLE;
        this->m_vkImageView = VK_NULL_HANDLE;

        if (this->m_vkSampler != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkSampler(this->m_vkSampler);
        }
        this->m_vkSampler = VK_NULL_HANDLE;
    }

    void VulkanTexture::createInternalResourcesImpl()
    {
        m_eVkImage = VulkanConverter::Transform2VkImageType(m_eTexture);
        m_eVkImageView = VulkanConverter::Transform2VkImageViewType(m_eTexture);
        m_eVkFormat = VulkanConverter::Transform2VkFormat(m_ePixelFormat);
        m_eVkComponentMapping = VulkanConverter::Transform2VkComponentMapping(m_ePixelFormat);
        m_eVkImageAspectFlags = VulkanConverter::Transform2VkImageAspectFlags(m_ePixelFormat);
        m_eVkSamplesCountFlagBits = VulkanConverter::Transform2VkSampleCountFlagBits(m_eMSAASampleCount);
        bool bIsAutoMipMap = IsUsage_AutoMipMap() && IsMipMapsHardwareGenerated();
        bool bIsStagingBuffer = IsUsage_StagingBuffer();
        uint32 nWidth = GetWidth();
        uint32 nHeight = GetHeight();
        uint32 nDepth = GetDepth();

        if (!IsUsage_RenderTarget())
        {
            this->m_vkImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            if (this->m_eTexture == F_Texture_1D ||
                this->m_eTexture == F_Texture_2D)
            {
                if (bIsStagingBuffer)
                {
                    m_pDevice->CreateTexture2D(this->m_aPath[0], 
                                               this->m_eVkImage,
                                               this->m_eVkSamplesCountFlagBits,
                                               this->m_eVkFormat,
                                               bIsAutoMipMap,
                                               this->m_nMipMapsCount, 
                                               this->m_vkImage, 
                                               this->m_vkImageMemory,
                                               this->m_vkBufferStaging,
                                               this->m_vkBufferMemoryStaging);
                }
                else
                {
                    m_pDevice->CreateTexture2D(this->m_aPath[0], 
                                               this->m_eVkImage,
                                               this->m_eVkSamplesCountFlagBits,
                                               this->m_eVkFormat,
                                               bIsAutoMipMap,
                                               this->m_nMipMapsCount, 
                                               this->m_vkImage, 
                                               this->m_vkImageMemory);
                }
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             1, 
                                             this->m_vkImageView);
            }
            else if (this->m_eTexture == F_Texture_2DArray)
            {
                if (bIsStagingBuffer)
                {
                    m_pDevice->CreateTexture2DArray(this->m_aPath, 
                                                    this->m_eVkImage,
                                                    this->m_eVkSamplesCountFlagBits,
                                                    this->m_eVkFormat,
                                                    bIsAutoMipMap,
                                                    this->m_nMipMapsCount, 
                                                    this->m_vkImage, 
                                                    this->m_vkImageMemory,
                                                    this->m_vkBufferStaging,
                                                    this->m_vkBufferMemoryStaging);
                }
                else
                {
                    m_pDevice->CreateTexture2DArray(this->m_aPath, 
                                                    this->m_eVkImage,
                                                    this->m_eVkSamplesCountFlagBits,
                                                    this->m_eVkFormat,
                                                    bIsAutoMipMap,
                                                    this->m_nMipMapsCount, 
                                                    this->m_vkImage, 
                                                    this->m_vkImageMemory);
                }
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             (uint32_t)this->m_aPath.size(), 
                                             this->m_vkImageView);
            }
            else if (this->m_eTexture == F_Texture_3D)
            {
                uint32_t nSize = nWidth * nHeight * nDepth;
                this->m_pDataRGBA = new uint8[nSize];
                memset(this->m_pDataRGBA, 0, (size_t)nSize);
                m_pDevice->CreateTexture3D(this->m_eVkFormat, 
                                           this->m_pDataRGBA, 
                                           nSize, 
                                           nWidth, 
                                           nHeight, 
                                           nDepth, 
                                           this->m_vkImage, 
                                           this->m_vkImageMemory, 
                                           this->m_vkBufferStaging, 
                                           this->m_vkBufferMemoryStaging);
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount,
                                             1, 
                                             this->m_vkImageView);
            }
            else if (this->m_eTexture == F_Texture_CubeMap)
            {
                if (bIsStagingBuffer)
                {
                    m_pDevice->CreateTextureCubeMap(this->m_aPath, 
                                                    this->m_eVkSamplesCountFlagBits,
                                                    this->m_eVkFormat,
                                                    bIsAutoMipMap,
                                                    this->m_nMipMapsCount, 
                                                    this->m_vkImage, 
                                                    this->m_vkImageMemory,
                                                    this->m_vkBufferStaging,
                                                    this->m_vkBufferMemoryStaging);
                }
                else
                {
                    m_pDevice->CreateTextureCubeMap(this->m_aPath, 
                                                    this->m_eVkSamplesCountFlagBits,
                                                    this->m_eVkFormat,
                                                    bIsAutoMipMap,
                                                    this->m_nMipMapsCount, 
                                                    this->m_vkImage, 
                                                    this->m_vkImageMemory);
                }
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             (uint32_t)this->m_aPath.size(),
                                             this->m_vkImageView);
            }
            else
            {
                F_LogError("*********************** VulkanTexture::createInternalResourcesImpl: Wrong texture type, create render target !");
                return;
            }
        }
        else
        {
            this->m_vkImageLayout = VK_IMAGE_LAYOUT_GENERAL;

            if (this->m_eTexture == F_Texture_1D)
            {
                m_pDevice->CreateTextureRenderTarget1D(this->m_colorRT, 
                                                       this->m_bIsRTSetColor, 
                                                       nWidth, 
                                                       this->m_nMipMapsCount, 
                                                       this->m_eVkSamplesCountFlagBits,
                                                       this->m_eVkFormat,
                                                       this->m_vkRTImageUsage,
                                                       this->m_bIsGraphicsComputeShared,
                                                       this->m_vkImage, 
                                                       this->m_vkImageMemory,
                                                       this->m_vkBufferStaging,
                                                       this->m_vkBufferMemoryStaging);
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             1, 
                                             this->m_vkImageView);
            }
            else if (this->m_eTexture == F_Texture_2D)
            {
                m_pDevice->CreateTextureRenderTarget2D(this->m_colorRT, 
                                                       this->m_bIsRTSetColor, 
                                                       nWidth, 
                                                       nHeight,
                                                       this->m_nMipMapsCount, 
                                                       this->m_eVkImage,
                                                       this->m_eVkSamplesCountFlagBits,
                                                       this->m_eVkFormat, 
                                                       this->m_vkRTImageUsage,
                                                       this->m_bIsGraphicsComputeShared,
                                                       this->m_vkImage, 
                                                       this->m_vkImageMemory,
                                                       this->m_vkBufferStaging,
                                                       this->m_vkBufferMemoryStaging);
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             1, 
                                             this->m_vkImageView);
            }
            else if (this->m_eTexture == F_Texture_2DArray)
            {
                m_pDevice->CreateTextureRenderTarget2DArray(this->m_colorRT, 
                                                            this->m_bIsRTSetColor, 
                                                            nWidth, 
                                                            nHeight,
                                                            nDepth,
                                                            this->m_nMipMapsCount, 
                                                            this->m_eVkImage,
                                                            this->m_eVkSamplesCountFlagBits,
                                                            this->m_eVkFormat, 
                                                            this->m_vkRTImageUsage,
                                                            this->m_bIsGraphicsComputeShared,
                                                            this->m_vkImage, 
                                                            this->m_vkImageMemory,
                                                            this->m_vkBufferStaging,
                                                            this->m_vkBufferMemoryStaging);
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             nDepth, 
                                             this->m_vkImageView);
            }   
            else if (this->m_eTexture == F_Texture_3D)
            {
                m_pDevice->CreateTextureRenderTarget3D(this->m_colorRT, 
                                                       this->m_bIsRTSetColor, 
                                                       nWidth, 
                                                       nHeight,
                                                       nDepth,
                                                       this->m_nMipMapsCount, 
                                                       this->m_eVkSamplesCountFlagBits,
                                                       this->m_eVkFormat, 
                                                       this->m_vkRTImageUsage,
                                                       this->m_bIsGraphicsComputeShared,
                                                       this->m_vkImage, 
                                                       this->m_vkImageMemory,
                                                       this->m_vkBufferStaging,
                                                       this->m_vkBufferMemoryStaging);
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             1, 
                                             this->m_vkImageView);
            }
            else if (this->m_eTexture == F_Texture_CubeMap)
            {
                m_pDevice->CreateTextureRenderTargetCubeMap(nWidth, 
                                                            nHeight,
                                                            this->m_nMipMapsCount, 
                                                            this->m_eVkSamplesCountFlagBits,
                                                            this->m_eVkFormat, 
                                                            this->m_vkRTImageUsage,
                                                            this->m_bIsGraphicsComputeShared,
                                                            this->m_vkImage, 
                                                            this->m_vkImageMemory,
                                                            this->m_vkBufferStaging,
                                                            this->m_vkBufferMemoryStaging);
                m_pDevice->CreateVkImageView(this->m_vkImage, 
                                             this->m_eVkImageView, 
                                             this->m_eVkFormat, 
                                             this->m_eVkComponentMapping,
                                             this->m_eVkImageAspectFlags, 
                                             this->m_nMipMapsCount, 
                                             6, 
                                             this->m_vkImageView);
            }
            else
            {
                F_LogError("*********************** VulkanTexture::createInternalResourcesImpl: Wrong texture type, create texture !");
                return;
            }
        }

        m_pDevice->CreateVkSampler(this->m_eTextureFilter, 
                                   this->m_eTextureAddressing,
                                   this->m_eTextureBorderColor,
                                   true,
                                   m_pDevice->GetVkPhysicalDeviceLimits().maxSamplerAnisotropy,
                                   0.0f,
                                   static_cast<float>(this->m_nMipMapsCount),
                                   0.0f,
                                   this->m_vkSampler);

        this->m_vkDescriptorImageInfo = {};
        this->m_vkDescriptorImageInfo.imageLayout = this->m_vkImageLayout;
        this->m_vkDescriptorImageInfo.imageView = this->m_vkImageView;
        this->m_vkDescriptorImageInfo.sampler = this->m_vkSampler;

    }

}; //LostPeterPluginRendererVulkan
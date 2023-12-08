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

namespace LostPeter
{
    VulkanTexture::VulkanTexture(uint32 nGroup, const String& strName)
        : Texture(nGroup, strName)
        , m_vkLayoutCurrent(VK_IMAGE_LAYOUT_UNDEFINED)
        , m_vkLayoutNext(VK_IMAGE_LAYOUT_UNDEFINED)
        , m_vkImageDefault(VK_NULL_HANDLE)
        , m_vkImageViewDefaultSrv(VK_NULL_HANDLE)
        , m_vkImageFinal(VK_NULL_HANDLE)
        , m_vkDeviceMemoryFinal(VK_NULL_HANDLE)
        , m_vkImageMSAA(VK_NULL_HANDLE)
        , m_vkDeviceMemoryMSAA(VK_NULL_HANDLE)
    {

    }

    VulkanTexture::~VulkanTexture()
    {
        Unload();
    }

    VkImageView VulkanTexture::CreateImageView() const
    {
        F_Assert(m_vkImageViewDefaultSrv && "VulkanTexture::CreateImageView: Either the texture wasn't properly loaded or wasn't called when it should have been !")
        return m_vkImageViewDefaultSrv;
    }

    VkImageView VulkanTexture::CreateImageView(uint8 mipLevel, 
                                               uint8 numMipMaps, 
                                               uint16 arraySlice, 
                                               uint32 numSlices /*= 0u*/, 
                                               VkImage imageOverride /*= nullptr*/) const
    {
        VkImageViewType texType = GetInternalVulkanTextureViewType();
        if (numSlices == 1u && m_eTexture == F_Texture_CubeMap)
        {
            texType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        }

        if (!numMipMaps)
        {
            numMipMaps = m_nNumMipMaps - mipLevel + 1;
        }
        F_Assert(numMipMaps <= (m_nNumMipMaps - mipLevel + 1) && "VulkanTexture::CreateImageView: Asking for more mipmaps than the texture has !")

        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice* pDevice = pTextureManager->GetDevice();

        VkImageViewCreateInfo imageViewCi = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        imageViewCi.image = imageOverride ? imageOverride : m_vkImageFinal;
        imageViewCi.viewType = texType;
        imageViewCi.format = VulkanConverter::Transform2VkFormat(m_ePixelFormat);

        if (FPixelFormat::IsLuminance(m_ePixelFormat) && !FPixelFormat::IsDepth(m_ePixelFormat))
        {
            if (FPixelFormat::GetPixelFormatComponentCount(m_ePixelFormat) == 2)
            {
                imageViewCi.components = 
                { 
                    VK_COMPONENT_SWIZZLE_R, 
                    VK_COMPONENT_SWIZZLE_R, 
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G
                };
            }
            else
            {
                imageViewCi.components = 
                {
                    VK_COMPONENT_SWIZZLE_R, 
                    VK_COMPONENT_SWIZZLE_R, 
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_ONE
                };
            }
        }
        else if (m_ePixelFormat == F_PixelFormat_BYTE_A8_UNORM)
        {
            imageViewCi.components = 
            {
                VK_COMPONENT_SWIZZLE_ONE, 
                VK_COMPONENT_SWIZZLE_ONE, 
                VK_COMPONENT_SWIZZLE_ONE,
                VK_COMPONENT_SWIZZLE_R
            };
        }

        imageViewCi.subresourceRange.aspectMask = VulkanConverter::Transform2VkImageAspectFlags(m_ePixelFormat, imageOverride == 0);
        imageViewCi.subresourceRange.baseMipLevel = mipLevel;
        imageViewCi.subresourceRange.levelCount = numMipMaps;
        imageViewCi.subresourceRange.baseArrayLayer = arraySlice;
        if (numSlices == 0u)
            imageViewCi.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
        else
            imageViewCi.subresourceRange.layerCount = numSlices;

        VkImageViewUsageCreateInfo flagRestriction = { VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO };
        if (pTextureManager->CanRestrictImageViewUsage() && IsUAV())
        {
            imageViewCi.pNext = &flagRestriction;
            flagRestriction.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            flagRestriction.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
            if (m_nUsage & E_TextureUsage_RenderTarget)
            {
                flagRestriction.usage |= FPixelFormat::IsDepth(m_ePixelFormat) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }
        }

        VkImageView imageView;
        E_VK_CHECK(vkCreateImageView(pDevice->GetVkDevice(), &imageViewCi, 0, &imageView));
        return imageView;
    }

    void VulkanTexture::DestroyImageView(VkImageView vkImageView)
    {

    }

    void VulkanTexture::CopyTo(Texture* pDst, 
                               const FPixelBox& dstBox, 
                               uint8 dstMipLevel,
                               const FPixelBox& srcBox, 
                               uint8 srcMipLevel,
                               bool keepResolvedTexSynced /*= true*/,
                               VulkanResourceAccessType issueBarriers /*= Vulkan_ResourceAccess_ReadWrite*/)
    {
        F_Assert(dynamic_cast<VulkanTexture*>(pDst) != nullptr && "VulkanTexture::CopyTo")

        VulkanTexture* pTextureDst = static_cast<VulkanTexture*>(pDst);
        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice* pDevice = pTextureManager->GetDevice();

        // if (issueBarriers & Vulkan_ResourceAccess_Read)
        //     pDevice->m_vulkanQueueGraphics.GetCopyEncoder(nullptr, this, true);
        // else
        // {
        //     pDevice->m_vulkanQueueGraphics.GetCopyEncoder(nullptr, 0, true);
        // }

        // if (issueBarriers & Vulkan_ResourceAccess_Write)
        //     pDevice->m_vulkanQueueGraphics.GetCopyEncoder(nullptr, pTextureDst, false);

        VkImageCopy region;

        const uint32 sourceSlice = srcBox.m_nFront;
        const uint32 destinationSlice = dstBox.m_nFront;
        const uint32 numSlices = dstBox.GetDepth() != 0 ? dstBox.GetDepth() : pTextureDst->GetDepth();

        region.srcSubresource.aspectMask = VulkanConverter::Transform2VkImageAspectFlags(GetPixelFormat());
        region.srcSubresource.mipLevel = srcMipLevel;
        region.srcSubresource.baseArrayLayer = sourceSlice;
        region.srcSubresource.layerCount = numSlices;

        region.srcOffset.x = static_cast<int32_t>(srcBox.m_nLeft);
        region.srcOffset.y = static_cast<int32_t>(srcBox.m_nTop);
        region.srcOffset.z = static_cast<int32_t>(srcBox.m_nFront);

        region.dstSubresource.aspectMask = VulkanConverter::Transform2VkImageAspectFlags(pDst->GetPixelFormat());
        region.dstSubresource.mipLevel = dstMipLevel;
        region.dstSubresource.baseArrayLayer = destinationSlice;
        region.dstSubresource.layerCount = numSlices;

        region.dstOffset.x = dstBox.m_nLeft;
        region.dstOffset.y = dstBox.m_nTop;
        region.dstOffset.z = dstBox.m_nFront;

        region.extent.width = srcBox.GetWidth();
        region.extent.height = srcBox.GetHeight();
        region.extent.depth = srcBox.GetDepth();

        VkImage srcTextureName = this->m_vkImageFinal;
        VkImage dstTextureName = pTextureDst->m_vkImageFinal;

        if (IsMultisample() && !HasMSAAExplicitResolves())
            srcTextureName = this->m_vkImageMSAA;
        if (pTextureDst->IsMultisample() && !pTextureDst->HasMSAAExplicitResolves())
            dstTextureName = pTextureDst->m_vkImageMSAA;

        // vkCmdCopyImage(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent, 
        //                srcTextureName, 
        //                m_vkLayoutCurrent,
        //                dstTextureName, 
        //                pTextureDst->m_vkLayoutCurrent, 
        //                1u, 
        //                &region);

        if (pTextureDst->IsMultisample() && 
            !pTextureDst->HasMSAAExplicitResolves() &&
            keepResolvedTexSynced)
        {
            VkImageResolve resolve = { };
            resolve.srcSubresource = region.dstSubresource;
            resolve.dstSubresource = region.dstSubresource;
            resolve.extent.width = GetWidth();
            resolve.extent.height = GetHeight();
            resolve.extent.depth = GetDepth();

            // vkCmdResolveImage(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent,
            //                    pTextureDst->m_vkImageMSAA, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            //                    pTextureDst->m_vkImageFinal, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u,
            //                    &resolve);
        }
    }

    void VulkanTexture::AutogenerateMipmaps(bool bUseBarrierSolver /*= false*/)
    {
        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice* pDevice = pTextureManager->GetDevice();

        const bool callerIsCompositor = m_vkLayoutCurrent == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

        // if (callerIsCompositor)
        //     pDevice->m_vulkanQueueGraphics.GetCopyEncoder(nullptr, 0, true);
        // else
        // {
        //     pDevice->m_vulkanQueueGraphics.GetCopyEncoder(nullptr, this, true);
        // }

        const uint32 numSlices = GetNumLayers();
        VkImageMemoryBarrier imageBarrier = GetImageMemoryBarrier();
        imageBarrier.subresourceRange.levelCount = 1u;
        const uint32 internalWidth = GetWidth();
        const uint32 internalHeight = GetHeight();
        for (size_t i = 1u; i <= m_nNumMipMaps; ++i)
        {
            imageBarrier.subresourceRange.baseMipLevel = static_cast<uint32_t>(i);
            imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageBarrier.srcAccessMask = 0;
            imageBarrier.dstAccessMask = 0;
            // vkCmdPipelineBarrier(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent,
            //                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            //                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            //                      0, 
            //                      0u, 
            //                      nullptr, 
            //                      0u, 
            //                      nullptr, 
            //                      1u, 
            //                      &imageBarrier);

            VkImageBlit region;
            region.srcSubresource.aspectMask = VulkanConverter::Transform2VkImageAspectFlags(GetPixelFormat());
            region.srcSubresource.mipLevel = static_cast<uint32_t>(i - 1u);
            region.srcSubresource.baseArrayLayer = 0u;
            region.srcSubresource.layerCount = numSlices;

            region.srcOffsets[0].x = 0;
            region.srcOffsets[0].y = 0;
            region.srcOffsets[0].z = 0;

            region.srcOffsets[1].x = static_cast<int32_t>(FMath::Max((uint32)(internalWidth >> (i - 1u)), (uint32)1u));
            region.srcOffsets[1].y = static_cast<int32_t>(FMath::Max((uint32)(internalHeight >> (i - 1u)), (uint32)1u));
            region.srcOffsets[1].z = static_cast<int32_t>(FMath::Max((uint32)(GetDepth() >> (i - 1u)), (uint32)1u));

            region.dstSubresource.aspectMask = region.srcSubresource.aspectMask;
            region.dstSubresource.mipLevel = static_cast<uint32_t>( i );
            region.dstSubresource.baseArrayLayer = 0u;
            region.dstSubresource.layerCount = numSlices;

            region.dstOffsets[0].x = 0;
            region.dstOffsets[0].y = 0;
            region.dstOffsets[0].z = 0;

            region.dstOffsets[1].x = static_cast<int32_t>(FMath::Max((uint32)(internalWidth >> i), (uint32)1u));
            region.dstOffsets[1].y = static_cast<int32_t>(FMath::Max((uint32)(internalHeight >> i), (uint32)1u));
            region.dstOffsets[1].z = static_cast<int32_t>(FMath::Max((uint32)(GetDepth() >> i), (uint32)1u));

            if (m_eTexture == F_Texture_2DArray)
            {
                region.srcOffsets[1].z = 1;
                region.dstOffsets[1].z = 1;
            }

            // vkCmdBlitImage(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent, 
            //                m_vkImageFinal, 
            //                m_vkLayoutCurrent,
            //                m_vkImageFinal, 
            //                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            //                1u, 
            //                &region,
            //                VK_FILTER_LINEAR);

            imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            // vkCmdPipelineBarrier(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent,
            //                      VK_PIPELINE_STAGE_TRANSFER_BIT, 
            //                      VK_PIPELINE_STAGE_TRANSFER_BIT, 
            //                      0, 
            //                      0u,
            //                      nullptr, 
            //                      0u, 
            //                      nullptr, 
            //                      1u, 
            //                      &imageBarrier);
        }
    }

    VkImageType VulkanTexture::GetVulkanTextureType() const
    {
        switch ((int32)m_eTexture)
        {
            case F_Texture_1D:              return VK_IMAGE_TYPE_1D;
            case F_Texture_2D:              return VK_IMAGE_TYPE_2D;
            case F_Texture_2DArray:         return VK_IMAGE_TYPE_2D;
            case F_Texture_CubeMap:         return VK_IMAGE_TYPE_2D;
            case F_Texture_3D:              return VK_IMAGE_TYPE_3D;
            //case F_Texture_ExternalOS:  break;
        }
        return VK_IMAGE_TYPE_2D;
    }

    VkImageViewType VulkanTexture::GetInternalVulkanTextureViewType() const
    {
        switch((int32)m_eTexture)
        {
            case F_Texture_1D:              return VK_IMAGE_VIEW_TYPE_1D;
            case F_Texture_2D:              return VK_IMAGE_VIEW_TYPE_2D;
            case F_Texture_2DArray:         return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            case F_Texture_CubeMap:         return VK_IMAGE_VIEW_TYPE_CUBE;
            case F_Texture_3D:              return VK_IMAGE_VIEW_TYPE_3D;
            //case F_Texture_ExternalOS:  break;
        }
        return VK_IMAGE_VIEW_TYPE_2D;
    }

    VkImageMemoryBarrier VulkanTexture::GetImageMemoryBarrier() const
    {
        VkImageMemoryBarrier imageMemBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        imageMemBarrier.image = m_vkImageFinal;
        imageMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemBarrier.subresourceRange.aspectMask = VulkanConverter::Transform2VkImageAspectFlags(m_ePixelFormat);
        imageMemBarrier.subresourceRange.baseMipLevel = 0u;
        imageMemBarrier.subresourceRange.levelCount = m_nNumMipMaps + 1;
        imageMemBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemBarrier.subresourceRange.layerCount = GetNumLayers();
        return imageMemBarrier;
    }

    void VulkanTexture::createMSAASurface()
    {
        VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
        imageInfo.imageType = GetVulkanTextureType();
        imageInfo.extent.width = GetWidth();
        imageInfo.extent.height = GetHeight();
        imageInfo.extent.depth = GetDepth();
        imageInfo.mipLevels = 1u;
        imageInfo.arrayLayers = 1u;
        imageInfo.format = VulkanConverter::Transform2VkFormat(m_ePixelFormat);
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VkSampleCountFlagBits(m_nFSAA);
        imageInfo.flags = 0;
        imageInfo.usage |= FPixelFormat::IsDepth(m_ePixelFormat) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        String textureName = GetName() + "/MSAAImplicit";

        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice* pDevice = pTextureManager->GetDevice();

        E_VK_CHECK(vkCreateImage(pDevice->GetVkDevice(), &imageInfo, 0, &m_vkImageMSAA));

        // VulkanUtil::Util_SetObjectName(pDevice->GetVkDevice(), (uint64_t)m_vkImageMSAA, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
        //                                  textureName.c_str());

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(pDevice->GetVkDevice(), m_vkImageMSAA, &memRequirements);

        VkMemoryAllocateInfo memAllocInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        memAllocInfo.allocationSize = memRequirements.size;
        memAllocInfo.memoryTypeIndex = 0;

        // const VkPhysicalDeviceMemoryProperties& memProperties = pDevice->m_vkPhysicalDeviceMemoryProperties;
        // for (; memAllocInfo.memoryTypeIndex < memProperties.memoryTypeCount; memAllocInfo.memoryTypeIndex++)
        // {
        //     if ((memProperties.memoryTypes[memAllocInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
        //     {
        //         break;
        //     }
        // }

        E_VK_CHECK(vkAllocateMemory(pDevice->GetVkDevice(), &memAllocInfo, nullptr, &m_vkDeviceMemoryMSAA));
        E_VK_CHECK(vkBindImageMemory(pDevice->GetVkDevice(), m_vkImageMSAA, m_vkDeviceMemoryMSAA, 0));

        // Immediately transition to its only state
        VkImageMemoryBarrier imageBarrier = this->GetImageMemoryBarrier();
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        if (FPixelFormat::IsDepth(m_ePixelFormat))
            imageBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        imageBarrier.image = m_vkImageMSAA;
        // vkCmdPipelineBarrier(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent,
        //                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0,
        //                      0u, nullptr, 0u, nullptr, 1u, &imageBarrier);
    }

    void VulkanTexture::destroyMSAASurface()
    {
        if (!m_vkImageMSAA)
            return;

        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice *pDevice = pTextureManager->GetDevice();

        vkDestroyImage(pDevice->GetVkDevice(), m_vkImageMSAA, nullptr);
        vkFreeMemory(pDevice->GetVkDevice(), m_vkDeviceMemoryMSAA, nullptr);
    }

    StreamTexture* VulkanTexture::GetTextureStream(size_t nFace /*= 0*/, size_t nMipmap /*= 0*/)
    {
        
        return nullptr;
    }

    bool VulkanTexture::Load()
    {

        return true;
    }

    bool VulkanTexture::Unload()
    {

        return true;
    }

    void VulkanTexture::createInternalResourcesImpl()
    {
        if (m_ePixelFormat == F_PixelFormat_Unknown)
            return;

        m_ePixelFormat = TextureManager::GetSingleton().GetNativeFormat(m_eTexture, m_ePixelFormat, m_nUsage);
        size_t bitSet = FBitwise::MostSignificantBitSet(FMath::Max(FMath::Max(m_nWidth, m_nHeight), m_nDepth));                                                
        m_nNumMipMaps = FMath::Min(m_nNumMipMaps, bitSet);

        VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
        imageInfo.imageType = GetVulkanTextureType();
        imageInfo.extent.width = GetWidth();
        imageInfo.extent.height = GetHeight();
        imageInfo.extent.depth = GetDepth();
        imageInfo.mipLevels = m_nNumMipMaps + 1;
        imageInfo.arrayLayers = GetNumFaces();
        imageInfo.flags = 0;
        imageInfo.format = VulkanConverter::Transform2VkFormat(m_ePixelFormat);
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (m_eTexture == F_Texture_2DArray)
            std::swap(imageInfo.extent.depth, imageInfo.arrayLayers);

        if (HasMSAAExplicitResolves())
        {
            imageInfo.samples = VkSampleCountFlagBits(FMath::Max(m_nFSAA, 1u));
        }
        else
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

        if (m_eTexture == F_Texture_CubeMap)
            imageInfo.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

        imageInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

        if (FPixelFormat::IsDepth(m_ePixelFormat))
            imageInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        else if(m_nUsage & E_TextureUsage_RenderTarget)
            imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (IsUAV())
            imageInfo.usage |= VK_IMAGE_USAGE_STORAGE_BIT;

        String textureName = GetName();

        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice* pDevice = pTextureManager->GetDevice();

        E_VK_CHECK(vkCreateImage(pDevice->GetVkDevice(), &imageInfo, nullptr, &m_vkImageFinal));

        // VulkanUtil::Util_SetObjectName(pDevice->GetVkDevice(), (uint64_t)m_vkImageFinal,
        //                                VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, textureName.c_str());

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(pDevice->GetVkDevice(), m_vkImageFinal, &memRequirements);

        VkMemoryAllocateInfo memAllocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
        memAllocInfo.allocationSize = memRequirements.size;
        memAllocInfo.memoryTypeIndex = 0;

        // const VkPhysicalDeviceMemoryProperties& memProperties = pDevice->m_vkPhysicalDeviceMemoryProperties;
        // for(; memAllocInfo.memoryTypeIndex < memProperties.memoryTypeCount; memAllocInfo.memoryTypeIndex++)
        // {
        //     if ((memProperties.memoryTypes[memAllocInfo.memoryTypeIndex].propertyFlags &
        //          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
        //     {
        //         break;
        //     }
        // }
        E_VK_CHECK(vkAllocateMemory(pDevice->GetVkDevice(), &memAllocInfo, nullptr, &m_vkDeviceMemoryFinal));
        E_VK_CHECK(vkBindImageMemory(pDevice->GetVkDevice(), m_vkImageFinal, m_vkDeviceMemoryFinal, 0));

        //F_Assert(pDevice->m_vulkanQueueGraphics.GetEncoderState() != DFT_EncoderState_GraphicsOpen && "VulkanTexture::createInternalResourcesImpl:: Interrupting RenderPass not supported !")
        //pDevice->m_vulkanQueueGraphics.EndAllEncoders();

        VkImageMemoryBarrier imageBarrier = this->GetImageMemoryBarrier();
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        if (FPixelFormat::IsDepth(m_ePixelFormat))
            imageBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // vkCmdPipelineBarrier(pDevice->m_vulkanQueueGraphics.m_vkCommandBufferCurrent,
        //                      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        //                      0, 0u, nullptr, 0u, nullptr, 1u, &imageBarrier);

        m_vkLayoutCurrent = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        m_vkLayoutNext = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        uint32 depth = m_nDepth;
        for (uint8 face = 0; face < GetNumFaces(); face++)
        {
            uint32 width = m_nWidth;
            uint32 height = m_nHeight;

            for (uint32 mip = 0; mip <= GetNumMipMaps(); mip++)
            {
                if (width > 1)
                    width = width / 2;
                if (height > 1)
                    height = height / 2;
                if (depth > 1 && m_eTexture != F_Texture_2DArray)
                    depth = depth / 2;
            }
        }

        m_vkImageViewDefaultSrv = CreateImageView(0, 0, 0, GetNumLayers());

        if (m_nFSAA > 1 && !HasMSAAExplicitResolves())
            createMSAASurface();
    }

    void VulkanTexture::freeInternalResourcesImpl()
    {
        VulkanTextureManager* pTextureManager = static_cast<VulkanTextureManager*>(TextureManager::GetSingletonPtr());
        VulkanDevice* pDevice = pTextureManager->GetDevice();
        //pDevice->m_vulkanQueueGraphics.NotifyTextureDestroyed(this);

        vkDestroyImageView(pDevice->GetVkDevice(), m_vkImageViewDefaultSrv, nullptr);
        m_vkImageViewDefaultSrv = 0;

        vkDestroyImage(pDevice->GetVkDevice(), m_vkImageFinal, nullptr);
        vkFreeMemory(pDevice->GetVkDevice(), m_vkDeviceMemoryFinal, nullptr);

        destroyMSAASurface();

        m_vkLayoutCurrent = VK_IMAGE_LAYOUT_UNDEFINED;
        m_vkLayoutNext = VK_IMAGE_LAYOUT_UNDEFINED;
    }

}; //LostPeter
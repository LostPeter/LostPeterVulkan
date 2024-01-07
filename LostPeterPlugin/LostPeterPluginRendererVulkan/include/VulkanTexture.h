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

#ifndef _VULKAN_TEXTURE_H_
#define _VULKAN_TEXTURE_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanTexture : public Texture
    {
    public:
        VulkanTexture(ResourceManager* pResourceManager,
                      uint32 nGroup, 
                      const String& strName,
                      const String& strGroupName,
                      ResourceHandle nHandle,
                      bool bIsManualLoad = false,
                      ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~VulkanTexture();

    public:
        VkImageLayout m_vkLayoutCurrent;
        VkImageLayout m_vkLayoutNext;

    protected:
        VkImage m_vkImageDefault;
        VkImageView m_vkImageViewDefaultSrv;

        VkImage m_vkImageFinal;
        VkDeviceMemory m_vkDeviceMemoryFinal;

        VkImage m_vkImageMSAA;
        VkDeviceMemory m_vkDeviceMemoryMSAA;

    public:
        F_FORCEINLINE uint32 GetLayersCount() const { return m_eTexture == F_Texture_2DArray ? (uint32)m_nDepth : (uint32)GetFacesCount(); }
        F_FORCEINLINE bool HasMSAAExplicitResolves() const { return false; }
        F_FORCEINLINE bool IsUAV() const { return false; }
        F_FORCEINLINE bool IsMultisample() const { return m_nFSAA > 1; }

        F_FORCEINLINE VkImageLayout GetLayoutCurrent() const { return m_vkLayoutCurrent; }
        F_FORCEINLINE VkImageLayout GetLayoutNext() const { return m_vkLayoutNext; }
        F_FORCEINLINE VkImage GetImageDefault() const { return m_vkImageDefault; }
        F_FORCEINLINE VkImageView GetImageViewDefaultSrv( void ) const { return m_vkImageViewDefaultSrv; }
        F_FORCEINLINE VkImage GetImageFinal() const { return m_vkImageFinal; }
        F_FORCEINLINE VkDeviceMemory GetDeviceMemoryFinal() const { return m_vkDeviceMemoryFinal; }
        F_FORCEINLINE VkImage GetImageMSAA() const { return m_vkImageMSAA; }
        F_FORCEINLINE VkDeviceMemory GetDeviceMemoryMSAA() const { return m_vkDeviceMemoryMSAA; }

    public:
        virtual bool IsRenderWindowSpecific() const { return false; }

    public:
        VkImageView CreateImageView() const;
        VkImageView CreateImageView(uint8 mipLevel, 
                                    uint8 mipMapsCount, 
                                    uint16 arraySlice, 
                                    uint32 slicesCount = 0u, 
                                    VkImage imageOverride = nullptr) const;
        void DestroyImageView(VkImageView vkImageView);

        virtual void CopyTo(Texture* pDst, 
                            const FPixelBox& dstBox, 
                            uint8 dstMipLevel,
                            const FPixelBox& srcBox, 
                            uint8 srcMipLevel,
                            bool keepResolvedTexSynced = true,
                            VulkanResourceAccessType issueBarriers = Vulkan_ResourceAccess_ReadWrite);

        void AutogenerateMipmaps(bool bUseBarrierSolver = false);

        VkImageType GetVulkanTextureType() const;
        VkImageViewType GetInternalVulkanTextureViewType() const;
        VkImageMemoryBarrier GetImageMemoryBarrier() const;
        
    protected:
        virtual void createMSAASurface();
        virtual void destroyMSAASurface();

    ////Texture
    public:
        virtual StreamTexture* GetTextureStream(uint32 nFace = 0, uint32 nMipmap = 0);

    protected:
		virtual void createInternalResourcesImpl();
		virtual void freeInternalResourcesImpl();

    ////Resource
    protected:
        virtual void loadImpl();
		virtual void unloadImpl();
    };

}; //LostPeterPluginRendererVulkan

#endif
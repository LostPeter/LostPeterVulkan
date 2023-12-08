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

namespace LostPeter
{
    class VulkanTexture : public Texture
    {
    public:
        VulkanTexture(uint32 nGroup, const String& strName);
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
        E_FORCEINLINE uint32 GetNumLayers() const { return m_eTexture == F_Texture_2DArray ? m_nDepth : GetNumFaces(); }
        E_FORCEINLINE bool HasMSAAExplicitResolves() const { return false; }
        E_FORCEINLINE bool IsUAV() const { return false; }
        E_FORCEINLINE bool IsMultisample() const { return m_nFSAA > 1; }

        E_FORCEINLINE VkImageLayout GetLayoutCurrent() const { return m_vkLayoutCurrent; }
        E_FORCEINLINE VkImageLayout GetLayoutNext() const { return m_vkLayoutNext; }
        E_FORCEINLINE VkImage GetImageDefault() const { return m_vkImageDefault; }
        E_FORCEINLINE VkImageView GetImageViewDefaultSrv( void ) const { return m_vkImageViewDefaultSrv; }
        E_FORCEINLINE VkImage GetImageFinal() const { return m_vkImageFinal; }
        E_FORCEINLINE VkDeviceMemory GetDeviceMemoryFinal() const { return m_vkDeviceMemoryFinal; }
        E_FORCEINLINE VkImage GetImageMSAA() const { return m_vkImageMSAA; }
        E_FORCEINLINE VkDeviceMemory GetDeviceMemoryMSAA() const { return m_vkDeviceMemoryMSAA; }

    public:
        virtual bool IsRenderWindowSpecific() const { return false; }

    public:
        VkImageView CreateImageView() const;
        VkImageView CreateImageView(uint8 mipLevel, 
                                    uint8 numMipMaps, 
                                    uint16 arraySlice, 
                                    uint32 numSlices = 0u, 
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
        virtual StreamTexture* GetTextureStream(size_t nFace = 0, size_t nMipmap = 0);

		virtual bool Load();
		virtual bool Unload();

    protected:
		virtual void createInternalResourcesImpl();
		virtual void freeInternalResourcesImpl();
    };

}; //LostPeter

#endif
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
        VulkanTexture(VulkanDevice* pDevice,
                      ResourceManager* pResourceManager,
                      uint32 nGroup, 
                      const String& strName,
                      const String& strGroupName,
                      ResourceHandle nHandle,
                      bool bIsManualLoad = false,
                      ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~VulkanTexture();

    public:
    protected:
        VulkanDevice* m_pDevice;

        VkFormat m_eFormat; 
        VkImage m_vkImage;
        VkDeviceMemory m_vkImageMemory;
        VkImageView m_vkImageView;
        VkSampler m_vkSampler;
        VkImageLayout m_vkImageLayout;
        VkDescriptorImageInfo m_vkDescriptorImageInfo;

        VkBuffer m_vkBufferStaging;
        VkDeviceMemory m_vkBufferMemoryStaging;


    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

        F_FORCEINLINE VkFormat GetVkFormat() const { return m_eFormat; }
        F_FORCEINLINE VkImage GetVkImage() const { return m_vkImage; }
        F_FORCEINLINE VkDeviceMemory GetVkImageDeviceMemory() const { return m_vkImageMemory; }
        F_FORCEINLINE VkImageView GetVkImageView() const { return m_vkImageView; }
        F_FORCEINLINE VkSampler GetVkSampler() const { return m_vkSampler; }
        F_FORCEINLINE VkImageLayout GetVkImageLayout() const { return m_vkImageLayout; }
        F_FORCEINLINE const VkDescriptorImageInfo& GetVkDescriptorImageInfo() const { return m_vkDescriptorImageInfo; }

        F_FORCEINLINE VkBuffer GetVkBufferStaging() const { return m_vkBufferStaging; }
        F_FORCEINLINE VkDeviceMemory GetVkBufferMemoryStaging() const { return m_vkBufferMemoryStaging; }

        F_FORCEINLINE uint32 GetLayersCount() const { return m_eTexture == F_Texture_2DArray ? m_nDepth : GetFacesCount(); }
        F_FORCEINLINE bool HasMSAAExplicitResolves() const { return false; }
        F_FORCEINLINE bool IsUAV() const { return false; }
        F_FORCEINLINE bool IsMultisample() const { return m_eMSAASampleCount != F_MSAASampleCount_1_Bit; }

    public:
        virtual void Destroy();

    public:
        

       
        
    protected:
    

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
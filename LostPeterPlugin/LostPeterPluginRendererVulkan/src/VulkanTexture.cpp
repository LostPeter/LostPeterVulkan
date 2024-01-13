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
        , m_eFormat(VK_FORMAT_UNDEFINED)
        , m_vkImage(VK_NULL_HANDLE)
        , m_vkImageMemory(VK_NULL_HANDLE)
        , m_vkImageView(VK_NULL_HANDLE)
        , m_vkSampler(VK_NULL_HANDLE)
        , m_vkImageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
        , m_vkBufferStaging(VK_NULL_HANDLE)
        , m_vkBufferMemoryStaging(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanTexture::VulkanTexture")
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
        
    }

}; //LostPeterPluginRendererVulkan
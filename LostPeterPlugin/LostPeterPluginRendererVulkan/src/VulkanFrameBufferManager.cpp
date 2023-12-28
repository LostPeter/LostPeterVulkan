/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanFrameBufferManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanFrameBuffer.h"

template<> LostPeterPluginRendererVulkan::VulkanFrameBufferManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanFrameBufferManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanFrameBufferManager* VulkanFrameBufferManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanFrameBufferManager& VulkanFrameBufferManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanFrameBufferManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanFrameBufferManager::VulkanFrameBufferManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanFrameBufferManager::VulkanFrameBufferManager")
    }   

    VulkanFrameBufferManager::~VulkanFrameBufferManager()
    {
        Destroy();
    }

    void VulkanFrameBufferManager::Destroy()
    {
        DeleteFrameBufferAll();
    }

    bool VulkanFrameBufferManager::HasFrameBuffer(const String& strName)
    {
        return GetFrameBuffer(strName) != nullptr;
    }
    VulkanFrameBuffer* VulkanFrameBufferManager::GetFrameBuffer(const String& strName)
    {
        VulkanFrameBufferPtrMap::iterator itFind = m_mapFrameBuffer.find(strName);
        if (itFind == m_mapFrameBuffer.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanFrameBufferManager::AddFrameBuffer(VulkanFrameBuffer* pFrameBuffer)
    {
        const String& strName = pFrameBuffer->GetName();
        VulkanFrameBufferPtrMap::iterator itFind = m_mapFrameBuffer.find(strName);
        if (itFind != m_mapFrameBuffer.end())
        {
            F_LogError("*********************** VulkanFrameBufferManager::AddFrameBuffer: FrameBuffer name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapFrameBuffer.insert(VulkanFrameBufferPtrMap::value_type(strName, pFrameBuffer));
        m_aFrameBuffer.push_back(pFrameBuffer);
        return true;
    }
    bool VulkanFrameBufferManager::CreateFrameBuffer(const String& nameFrameBuffer,
                                                     const VkImageViewVector& aImageView, 
                                                     VkRenderPass& vkRenderPass,
                                                     VkFramebufferCreateFlags flags,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint32_t layers)
    {
        VulkanFrameBuffer* pFrameBuffer = GetFrameBuffer(nameFrameBuffer);
        if (pFrameBuffer != nullptr)
            return true;
        
        pFrameBuffer = new VulkanFrameBuffer(nameFrameBuffer, this->m_pDevice);
        if (!pFrameBuffer->Init(aImageView,
                                vkRenderPass,
                                flags,
                                width,
                                height,
                                layers))
        {
            return false;
        }
        AddFrameBuffer(pFrameBuffer);
        return pFrameBuffer;
    }
    void VulkanFrameBufferManager::DeleteFrameBuffer(const String& strName)
    {
        VulkanFrameBufferPtrMap::iterator itFind = m_mapFrameBuffer.find(strName);
        if (itFind == m_mapFrameBuffer.end())
        {
            return;
        }

        VulkanFrameBufferPtrVector::iterator itFindA = std::find(m_aFrameBuffer.begin(), m_aFrameBuffer.end(), itFind->second);
        if (itFindA != m_aFrameBuffer.end())
            m_aFrameBuffer.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapFrameBuffer.erase(itFind);
    }
    void VulkanFrameBufferManager::DeleteFrameBufferAll()
    {
        for (VulkanFrameBufferPtrVector::iterator it = m_aFrameBuffer.begin();
             it != m_aFrameBuffer.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aFrameBuffer.clear();
        m_mapFrameBuffer.clear();
    }

}; //LostPeterPluginRendererVulkan
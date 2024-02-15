/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanPipelineCache.h"
#include "../include/VulkanDevice.h"

template<> LostPeterPluginRendererVulkan::VulkanPipelineCache* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanPipelineCache>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanPipelineCache* VulkanPipelineCache::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanPipelineCache& VulkanPipelineCache::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanPipelineCache::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanPipelineCache::VulkanPipelineCache(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
        , m_vkPipelineCache(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanPipelineCache::VulkanPipelineCache")
        Init();
    }   

    VulkanPipelineCache::~VulkanPipelineCache()
    {
        Destroy();
    }

    void VulkanPipelineCache::Destroy()
    {
        if (this->m_vkPipelineCache != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkPipelineCache(this->m_vkPipelineCache);
        }
        this->m_vkPipelineCache = VK_NULL_HANDLE;
    }

    bool VulkanPipelineCache::Init()
    {
        Destroy();
        if (!m_pDevice->CreateVkPipelineCache(this->m_vkPipelineCache))
        {
            F_LogError("*********************** VulkanPipelineCache::Init: Failed to CreateVkPipelineCache !");
            return false;
        }
        return true;
    }

}; //LostPeterPluginRendererVulkan
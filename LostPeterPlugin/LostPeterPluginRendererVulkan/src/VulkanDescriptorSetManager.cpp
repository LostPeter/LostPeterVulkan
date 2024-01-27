/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanDescriptorSetManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanDescriptorSet.h"

template<> LostPeterPluginRendererVulkan::VulkanDescriptorSetManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanDescriptorSetManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanDescriptorSetManager* VulkanDescriptorSetManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanDescriptorSetManager& VulkanDescriptorSetManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanDescriptorSetManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanDescriptorSetManager::VulkanDescriptorSetManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanDescriptorSetManager::VulkanDescriptorSetManager")
    }   

    VulkanDescriptorSetManager::~VulkanDescriptorSetManager()
    {
        Destroy();
    }

    void VulkanDescriptorSetManager::Destroy()
    {
        
    }

}; //LostPeterPluginRendererVulkan
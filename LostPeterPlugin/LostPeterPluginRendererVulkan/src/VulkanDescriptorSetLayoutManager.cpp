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

#include "../include/VulkanDescriptorSetLayoutManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanDescriptorSetLayout.h"

template<> LostPeterPluginRendererVulkan::VulkanDescriptorSetLayoutManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanDescriptorSetLayoutManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanDescriptorSetLayoutManager* VulkanDescriptorSetLayoutManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanDescriptorSetLayoutManager& VulkanDescriptorSetLayoutManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanDescriptorSetLayoutManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanDescriptorSetLayoutManager::VulkanDescriptorSetLayoutManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanDescriptorSetLayoutManager::VulkanDescriptorSetLayoutManager")
    }   

    VulkanDescriptorSetLayoutManager::~VulkanDescriptorSetLayoutManager()
    {
        Destroy();
    }

    void VulkanDescriptorSetLayoutManager::Destroy()
    {
        
    }

}; //LostPeterPluginRendererVulkan
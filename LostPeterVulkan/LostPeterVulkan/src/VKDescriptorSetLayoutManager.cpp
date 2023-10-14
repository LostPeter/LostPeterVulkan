/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKDescriptorSetLayoutManager.h"
#include "../include/VulkanWindow.h"
#include "../include/VKDescriptorSetLayoutSerializer.h"
#include "../include/VKDescriptorSetLayout.h"

template<> LostPeter::VKDescriptorSetLayoutManager* LostPeterFoundation::FSingleton<LostPeter::VKDescriptorSetLayoutManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    VKDescriptorSetLayoutManager* VKDescriptorSetLayoutManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKDescriptorSetLayoutManager& VKDescriptorSetLayoutManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKDescriptorSetLayoutManager::GetSingleton")
		return (*ms_Singleton);  
	}

    VKDescriptorSetLayoutManager::VKDescriptorSetLayoutManager()
        : Base("VKDescriptorSetLayoutManager")
        , m_pVKDescriptorSetLayoutSerializer(nullptr)
    {

    }
    VKDescriptorSetLayoutManager::~VKDescriptorSetLayoutManager()
    {
        Destroy();
    }
    
    void VKDescriptorSetLayoutManager::Destroy()
    {
        F_DELETE(m_pVKDescriptorSetLayoutSerializer)

    }
    bool VKDescriptorSetLayoutManager::Init(uint nGroup, const String& strNameCfg)
    {
        
        return true;
    }
    

}; //LostPeter
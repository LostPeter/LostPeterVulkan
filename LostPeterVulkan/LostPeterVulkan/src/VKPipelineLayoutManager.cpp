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

#include "../include/VKPipelineLayoutManager.h"
#include "../include/VulkanWindow.h"
#include "../include/VKPipelineLayoutSerializer.h"
#include "../include/VKPipelineLayout.h"

template<> LostPeter::VKPipelineLayoutManager* LostPeterFoundation::FSingleton<LostPeter::VKPipelineLayoutManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    VKPipelineLayoutManager* VKPipelineLayoutManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKPipelineLayoutManager& VKPipelineLayoutManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKPipelineLayoutManager::GetSingleton")
		return (*ms_Singleton);  
	}

    VKPipelineLayoutManager::VKPipelineLayoutManager()
        : Base("VKPipelineLayoutManager")
        , m_pVKPipelineLayoutSerializer(nullptr)
    {

    }
    VKPipelineLayoutManager::~VKPipelineLayoutManager()
    {
        Destroy();
    }
    
    void VKPipelineLayoutManager::Destroy()
    {
        F_DELETE(m_pVKPipelineLayoutSerializer)

    }
    bool VKPipelineLayoutManager::Init(uint nGroup, const String& strNameCfg)
    {
        
        return true;
    }

    

}; //LostPeter
/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MaterialDataManager.h"
#include "../include/VulkanWindow.h"

template<> LostPeter::MaterialDataManager* LostPeterFoundation::FSingleton<LostPeter::MaterialDataManager>::ms_Singleton = nullptr;

namespace LostPeter
{
	MaterialDataManager* MaterialDataManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	MaterialDataManager& MaterialDataManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "MaterialDataManager::GetSingleton")
		return (*ms_Singleton);     
	}

    MaterialDataManager::MaterialDataManager()
        : Base("MaterialDataManager")
    {
        
    }
    MaterialDataManager::~MaterialDataManager()
    {

    }
    
}; //LostPeter
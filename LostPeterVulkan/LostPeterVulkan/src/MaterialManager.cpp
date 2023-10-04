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

#include "../include/MaterialManager.h"
#include "../include/VulkanWindow.h"

template<> LostPeter::MaterialManager* LostPeterFoundation::FSingleton<LostPeter::MaterialManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    MaterialManager* MaterialManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	MaterialManager& MaterialManager::GetSingleton()
	{  
		assert(ms_Singleton && "MaterialManager::GetSingleton");
		return (*ms_Singleton);     
	}

    MaterialManager::MaterialManager()
        : Base("MaterialManager")
    {

    }
    MaterialManager::~MaterialManager()
    {

    }
    
}; //LostPeter
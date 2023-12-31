/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-31
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ResourceGroupManager.h"

template<> LostPeterEngine::ResourceGroupManager* LostPeterFoundation::FSingleton<LostPeterEngine::ResourceGroupManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ResourceGroupManager* ResourceGroupManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ResourceGroupManager& ResourceGroupManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ResourceGroupManager::GetSingleton")
		return (*ms_Singleton);     
	}

    ResourceGroupManager::ResourceGroupManager()
        : Base("ResourceGroupManager")
    {

    }
    ResourceGroupManager::~ResourceGroupManager()
    {
		Destroy();
    }

	void ResourceGroupManager::Destroy()
	{
		
	}

}; //LostPeterEngine
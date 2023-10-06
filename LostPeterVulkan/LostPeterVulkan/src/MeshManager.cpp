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

#include "../include/MeshManager.h"
#include "../include/VulkanWindow.h"

template<> LostPeter::MeshManager* LostPeterFoundation::FSingleton<LostPeter::MeshManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    MeshManager* MeshManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	MeshManager& MeshManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "MeshManager::GetSingleton")
		return (*ms_Singleton);     
	}

    MeshManager::MeshManager()
        : Base("MeshManager")
    {

    }
    MeshManager::~MeshManager()
    {

    }
    
}; //LostPeter
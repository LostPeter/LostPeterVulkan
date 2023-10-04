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

#include "../include/ShaderManager.h"
#include "../include/VulkanWindow.h"

template<> LostPeter::ShaderManager* LostPeterFoundation::FSingleton<LostPeter::ShaderManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    ShaderManager* ShaderManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ShaderManager& ShaderManager::GetSingleton()
	{  
		assert(ms_Singleton && "ShaderManager::GetSingleton");
		return (*ms_Singleton);     
	}

    ShaderManager::ShaderManager()
        : Base("ShaderManager")
    {

    }
    ShaderManager::~ShaderManager()
    {

    }

}; //LostPeter
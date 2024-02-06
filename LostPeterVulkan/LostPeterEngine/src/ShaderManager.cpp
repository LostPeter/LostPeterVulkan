/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderManager.h"
#include "../include/Shader.h"

template<> LostPeterEngine::ShaderManager* LostPeterFoundation::FSingleton<LostPeterEngine::ShaderManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ShaderManager* ShaderManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ShaderManager& ShaderManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ShaderManager::GetSingleton")
		return (*ms_Singleton);     
	}

    ShaderManager::ShaderManager()
        : Base("ShaderManager")
    {

    }
    ShaderManager::~ShaderManager()
    {

    }

    void ShaderManager::Destroy()
    {

    }
   

}; //LostPeterEngine
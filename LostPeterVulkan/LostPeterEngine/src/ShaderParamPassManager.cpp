/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderParamPassManager.h"
#include "../include/ShaderParamPass.h"

template<> LostPeterEngine::ShaderParamPassManager* LostPeterFoundation::FSingleton<LostPeterEngine::ShaderParamPassManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ShaderParamPassManager* ShaderParamPassManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ShaderParamPassManager& ShaderParamPassManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ShaderParamPassManager::GetSingleton")
		return (*ms_Singleton);     
	}


    ShaderParamPassManager::ShaderParamPassManager()
    {

    }
    
    ShaderParamPassManager::~ShaderParamPassManager()
    {

    }

    void ShaderParamPassManager::Destroy()
    {

    }

    bool ShaderParamPassManager::Init()
    {

        return true;
    }

}; //LostPeterEngine
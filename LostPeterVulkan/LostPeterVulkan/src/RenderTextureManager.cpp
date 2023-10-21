/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderTextureManager.h"
#include "../include/VulkanWindow.h"
#include "../include/RenderTexture.h"

template<> LostPeter::RenderTextureManager* LostPeterFoundation::FSingleton<LostPeter::RenderTextureManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    RenderTextureManager* RenderTextureManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderTextureManager& RenderTextureManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderTextureManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderTextureManager::RenderTextureManager()
        : Base("RenderTextureManager")
    {

    }
    RenderTextureManager::~RenderTextureManager()
    {

    }

    
    

}; //LostPeter
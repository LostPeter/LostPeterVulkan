/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderPassManager.h"
#include "../include/VulkanWindow.h"
#include "../include/RenderPass.h"

template<> LostPeter::RenderPassManager* LostPeterFoundation::FSingleton<LostPeter::RenderPassManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    RenderPassManager* RenderPassManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderPassManager& RenderPassManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderPassManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderPassManager::RenderPassManager()
        : Base("RenderPassManager")
    {

    }
    RenderPassManager::~RenderPassManager()
    {

    }

    
    

}; //LostPeter
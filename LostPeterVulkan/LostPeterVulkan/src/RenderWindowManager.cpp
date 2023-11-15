/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderWindowManager.h"
#include "../include/VulkanWindow.h"
#include "../include/RenderWindow.h"

template<> LostPeter::RenderWindowManager* LostPeterFoundation::FSingleton<LostPeter::RenderWindowManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    RenderWindowManager* RenderWindowManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderWindowManager& RenderWindowManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderWindowManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderWindowManager::RenderWindowManager()
        : Base("RenderWindowManager")
    {

    }
    RenderWindowManager::~RenderWindowManager()
    {

    }

    
    

}; //LostPeter
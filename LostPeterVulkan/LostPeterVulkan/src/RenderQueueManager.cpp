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

#include "../include/RenderQueueManager.h"
#include "../include/VulkanWindow.h"
#include "../include/RenderQueue.h"

template<> LostPeter::RenderQueueManager* LostPeterFoundation::FSingleton<LostPeter::RenderQueueManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    RenderQueueManager* RenderQueueManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderQueueManager& RenderQueueManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderQueueManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderQueueManager::RenderQueueManager()
        : Base("RenderQueueManager")
    {

    }
    RenderQueueManager::~RenderQueueManager()
    {

    }
    
    

}; //LostPeter
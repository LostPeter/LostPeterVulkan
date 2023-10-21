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

#include "../include/RenderPipelineManager.h"
#include "../include/VulkanWindow.h"
#include "../include/RenderPipeline.h"

template<> LostPeter::RenderPipelineManager* LostPeterFoundation::FSingleton<LostPeter::RenderPipelineManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    RenderPipelineManager* RenderPipelineManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderPipelineManager& RenderPipelineManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderPipelineManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderPipelineManager::RenderPipelineManager()
        : Base("RenderPipelineManager")
    {

    }
    RenderPipelineManager::~RenderPipelineManager()
    {

    }

    
    

}; //LostPeter
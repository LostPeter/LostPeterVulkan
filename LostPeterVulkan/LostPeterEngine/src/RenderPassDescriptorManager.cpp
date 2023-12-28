/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderPassDescriptorManager.h"
#include "../include/RenderPassDescriptor.h"

template<> LostPeterEngine::RenderPassDescriptorManager* LostPeterFoundation::FSingleton<LostPeterEngine::RenderPassDescriptorManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    RenderPassDescriptorManager* RenderPassDescriptorManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderPassDescriptorManager& RenderPassDescriptorManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderPassDescriptorManager::GetSingleton")
		return (*ms_Singleton);     
	}


    RenderPassDescriptorManager::RenderPassDescriptorManager()
        : Base("RenderPassDescriptorManager")
    {

    }

    RenderPassDescriptorManager::~RenderPassDescriptorManager()
    {

    }

    void RenderPassDescriptorManager::Destroy()
    {

    }
    
    bool RenderPassDescriptorManager::Init()
    {

        return true;
    }

}; //LostPeterEngine
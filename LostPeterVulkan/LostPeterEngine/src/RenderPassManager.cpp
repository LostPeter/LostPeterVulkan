/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderPassManager.h"
#include "../include/RenderPass.h"

template<> LostPeterEngine::RenderPassManager* LostPeterFoundation::FSingleton<LostPeterEngine::RenderPassManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
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

    void RenderPassManager::Destroy()
    {
        DestroyRenderPassAll();
    }

    bool RenderPassManager::Init()
    {
        

        return true;
    }

    void RenderPassManager::DestroyRenderPassAll()
    {
        int count = (int)m_aRenderPass.size();
        for (int i = 0; i < count; i++)
        {
            RenderPass* pRenderPass = m_aRenderPass[i];
            F_DELETE(pRenderPass)
        }
        m_aRenderPass.clear();
        m_mapRenderPass.clear();
    }

}; //LostPeterEngine
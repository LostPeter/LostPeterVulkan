/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderTargetManager.h"
#include "../include/RenderTarget.h"
#include "../include/RenderTexture.h"
#include "../include/RenderWindow.h"
#include "../include/Renderer.h"

template<> LostPeterEngine::RenderTargetManager* LostPeterFoundation::FSingleton<LostPeterEngine::RenderTargetManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    RenderTargetManager* RenderTargetManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderTargetManager& RenderTargetManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderTargetManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderTargetManager::RenderTargetManager(Renderer* pRenderer)
        : Base("RenderTargetManager")
        , m_pRenderer(pRenderer)
    {

    }   

    RenderTargetManager::~RenderTargetManager()
    {
        Destroy();
    }

    void RenderTargetManager::Destroy()
    {

    }

    bool RenderTargetManager::Init()
    {

        return true;
    }

    RenderTarget* RenderTargetManager::GetRenderTarget(const String& strName)
    {
        RenderTargetPtrMap::iterator itFind = m_mapRenderTarget.find(strName);
		if (itFind != m_mapRenderTarget.end())
			return itFind->second;
		return nullptr;
    }
    void RenderTargetManager::AttachRenderTarget(RenderTarget* pRenderTarget)
    {
        m_mapRenderTarget.insert(RenderTargetPtrMap::value_type(pRenderTarget->GetName(), pRenderTarget));
		m_mapPriorityRenderTarget.insert(RenderTargetPtrPriorityMap::value_type(pRenderTarget->GetPriority(), pRenderTarget));
    }
    RenderTarget* RenderTargetManager::DetachRenderTarget(const String& strName)
    {
        RenderTargetPtrMap::iterator itFind = m_mapRenderTarget.find(strName);
		RenderTarget* pRenderTarget = nullptr;
		if (itFind != m_mapRenderTarget.end())
		{
			pRenderTarget = itFind->second;

			for (RenderTargetPtrPriorityMap::iterator it = m_mapPriorityRenderTarget.begin();
				 it != m_mapPriorityRenderTarget.end(); ++it)
			{
				if(it->second == pRenderTarget) 
				{
					m_mapPriorityRenderTarget.erase(it);
					break;
				}
			}
			m_mapRenderTarget.erase(itFind);
		}

		if(pRenderTarget == m_pRenderer->GetActiveRenderTarget())
		{
            m_pRenderer->SetActiveRenderTarget(nullptr);
        }

		return pRenderTarget;
    }
    void RenderTargetManager::DestroyRenderTarget(const String& strName)
    {   
        RenderTarget* pRenderTarget = DetachRenderTarget(strName);
        F_DELETE(pRenderTarget)
    }
    void RenderTargetManager::DestroyRenderTargetAll()
    {
        RenderTarget* pRenderTargetPrimary = nullptr;
		for (RenderTargetPtrMap::iterator it = m_mapRenderTarget.begin(); 
			 it != m_mapRenderTarget.end(); ++it)
		{
			if (!pRenderTargetPrimary && it->second->IsPrimary())
				pRenderTargetPrimary = it->second;
			else
				delete it->second;
		}
		F_DELETE(pRenderTargetPrimary)
		m_mapRenderTarget.clear();
		m_mapPriorityRenderTarget.clear();
    }

    void RenderTargetManager::DestroyRenderWindow(const String& strName)
    {
        DestroyRenderTarget(strName);
    }

    void RenderTargetManager::DestroyRenderTexture(const String& strName)
    {
        DestroyRenderTarget(strName);
    }

}; //LostPeterEngine
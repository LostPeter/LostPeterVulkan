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

#include "../include/RenderQueueManager.h"
#include "../include/RenderQueue.h"

template<> LostPeterEngine::RenderQueueManager* LostPeterFoundation::FSingleton<LostPeterEngine::RenderQueueManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
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
        Destroy();
    }
    
    void RenderQueueManager::Destroy()
    {
        DeleteRenderQueueAll();
    }

    bool RenderQueueManager::HasRenderQueue(const String& strName)
    {
        return GetRenderQueue(strName) != nullptr;
    }
    RenderQueue* RenderQueueManager::GetRenderQueue(const String& strName)
    {
        RenderQueuePtrMap::iterator itFind = m_mapRenderQueue.find(strName);
        if (itFind == m_mapRenderQueue.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool RenderQueueManager::AddRenderQueue(RenderQueue* pRenderQueue)
    {
        const String& strName = pRenderQueue->GetName();
        RenderQueuePtrMap::iterator itFind = m_mapRenderQueue.find(strName);
        if (itFind != m_mapRenderQueue.end())
        {
            F_LogError("*********************** RenderQueueManager::AddRenderQueue: FrameBuffer name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapRenderQueue.insert(RenderQueuePtrMap::value_type(strName, pRenderQueue));
        m_aRenderQueue.push_back(pRenderQueue);
        return true;
    }
    RenderQueue* RenderQueueManager::CreateRenderQueue(const String& strName)
	{
        RenderQueue* pRenderQueue = GetRenderQueue(strName);
		if (pRenderQueue != nullptr)
		{
			pRenderQueue->AddRef();
            return pRenderQueue;
		}

		pRenderQueue = new RenderQueue(strName);
        if (!pRenderQueue->Init())
        {
            F_LogError("*********************** RenderQueueManager::CreateRenderQueue: Failed to init RenderQueue, name: [%s] !", strName.c_str());
            F_DELETE(pRenderQueue)
            return nullptr;
        }
        AddRenderQueue(pRenderQueue);
        pRenderQueue->AddRef();
		return pRenderQueue;
	}
    void RenderQueueManager::DeleteRenderQueue(const String& strName)
    {
        RenderQueuePtrMap::iterator itFind = m_mapRenderQueue.find(strName);
        if (itFind == m_mapRenderQueue.end())
        {
            return;
        }

        RenderQueuePtrVector::iterator itFindA = std::find(m_aRenderQueue.begin(), m_aRenderQueue.end(), itFind->second);
        if (itFindA != m_aRenderQueue.end())
            m_aRenderQueue.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapRenderQueue.erase(itFind);
    }
    void RenderQueueManager::DeleteRenderQueue(RenderQueue* pRenderQueue)
    {
        if (!pRenderQueue)
            return;
        DeleteRenderQueue(pRenderQueue->GetName());
    }
    void RenderQueueManager::DeleteRenderQueueAll()
    {
        for (RenderQueuePtrVector::iterator it = m_aRenderQueue.begin();
             it != m_aRenderQueue.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aRenderQueue.clear();
        m_mapRenderQueue.clear();
    }

}; //LostPeterEngine
/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/StreamVertexBindingManager.h"
#include "../include/StreamVertexBinding.h"

template<> LostPeterEngine::StreamVertexBindingManager* LostPeterFoundation::FSingleton<LostPeterEngine::StreamVertexBindingManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    StreamVertexBindingManager* StreamVertexBindingManager::GetSingletonPtr()
    {
        return ms_Singleton;
    }
    StreamVertexBindingManager& StreamVertexBindingManager::GetSingleton()
    {  
        F_Assert(ms_Singleton && "StreamVertexBindingManager::GetSingleton")
        return (*ms_Singleton);  
    }

    StreamVertexBindingManager::StreamVertexBindingManager()
        : Base("StreamVertexBindingManager")
        , m_nCount(0)
    {
        m_vsbActiveHeadNode.m_pNext = &m_vsbActiveTailNode;
		m_vsbActiveTailNode.m_pPrev = &m_vsbActiveHeadNode;
		m_vsbFreeHeadNode.m_pNext = &m_vsbFreeTailNode;
		m_vsbFreeTailNode.m_pPrev = &m_vsbFreeHeadNode;
    }

    StreamVertexBindingManager::~StreamVertexBindingManager()
    {
		Destroy();
    }

	void StreamVertexBindingManager::Destroy()
	{
		DestroyVertexStreamBindingAll();
	}

    StreamVertexBinding* StreamVertexBindingManager::CreateVertexStreamBinding()
	{
		//ENGINE_LOCK_MUTEX(m_mutexVertexBindings)
		StreamVertexBinding* pBinding = nullptr;
		if (m_vsbFreeHeadNode.m_pNext == &m_vsbFreeTailNode)
		{		
			VertexStreamBindingPtrListNode* pNode = new VertexStreamBindingPtrListNode;
			pNode->m_pPointer = createVertexStreamBindingImpl();	
			pNode->m_pPointer->SetCreator(pNode);
			pNode->m_pNext = &m_vsbActiveTailNode;
			pNode->m_pPrev = m_vsbActiveTailNode.m_pPrev;
			m_vsbActiveTailNode.m_pPrev->m_pNext = pNode;
			m_vsbActiveTailNode.m_pPrev = pNode;
			++m_nCount;
			pBinding = pNode->m_pPointer;
		}
		else
		{
			VertexStreamBindingPtrListNode* pNode = m_vsbFreeHeadNode.m_pNext;
			m_vsbFreeHeadNode.m_pNext = pNode->m_pNext;
			pNode->m_pNext->m_pPrev = &m_vsbFreeHeadNode;
			pNode->m_pNext = &m_vsbActiveTailNode;
			pNode->m_pPrev = m_vsbActiveTailNode.m_pPrev;
			m_vsbActiveTailNode.m_pPrev->m_pNext = pNode;
			m_vsbActiveTailNode.m_pPrev = pNode;
			pBinding = pNode->m_pPointer;
		}
		return pBinding;
	}

	void StreamVertexBindingManager::DestroyVertexStreamBinding(StreamVertexBinding* pStreamVertexBinding)
	{
		//ENGINE_LOCK_MUTEX(m_mutexVertexBindings)
		VertexStreamBindingPtrListNode* pNode = static_cast<VertexStreamBindingPtrListNode*>(pStreamVertexBinding->GetCreator());
		pStreamVertexBinding->Destroy();
		VertexStreamBindingPtrListNode* pPrev = pNode->m_pPrev;
		VertexStreamBindingPtrListNode* pNext = pNode->m_pNext;
		pPrev->m_pNext = pNext;
		pNext->m_pPrev = pPrev;
		m_vsbFreeTailNode.m_pPrev->m_pNext = pNode;
		pNode->m_pPrev = m_vsbFreeTailNode.m_pPrev;
		pNode->m_pNext = &m_vsbFreeTailNode;
		m_vsbFreeTailNode.m_pPrev = pNode;
	}

	void StreamVertexBindingManager::DestroyVertexStreamBindingAll()
	{
		VertexStreamBindingPtrListNode* it = m_vsbActiveHeadNode.m_pNext;
		VertexStreamBindingPtrListNode* pNode = nullptr;
		while (it != &m_vsbActiveTailNode)
		{
			if (it->m_pPointer)
			{
				destroyVertexStreamBindingImpl(it->m_pPointer);
			}
			pNode = it;
			it = it->m_pNext;

			F_DELETE(pNode)
		}
		it = m_vsbFreeHeadNode.m_pNext;
		while (it != &m_vsbFreeTailNode)
		{
			if (it->m_pPointer)
			{
				destroyVertexStreamBindingImpl(it->m_pPointer);
			}
			pNode = it;
			it = it->m_pNext;

			F_DELETE(pNode)
		}
		m_vsbActiveHeadNode.m_pNext = &m_vsbActiveTailNode;
		m_vsbActiveTailNode.m_pPrev = &m_vsbActiveHeadNode;
		m_vsbFreeHeadNode.m_pNext = &m_vsbFreeTailNode;
		m_vsbFreeTailNode.m_pPrev = &m_vsbFreeHeadNode;
	}

	StreamVertexBinding* StreamVertexBindingManager::createVertexStreamBindingImpl()
	{
		return new StreamVertexBinding;
	}

	void StreamVertexBindingManager::destroyVertexStreamBindingImpl(StreamVertexBinding* pStreamVertexBinding)
	{
		F_DELETE(pStreamVertexBinding)
	}

}; //LostPeterEngine